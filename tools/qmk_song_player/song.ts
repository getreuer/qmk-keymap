// Copyright 2023 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     https://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/**
 * QMK song player.
 *
 * The Quantum Mechanical Keyboard (QMK) firmware has a feature for playing
 * simple songs (https://docs.qmk.fm/features/audio#songs). This library
 * reads song in QMK's song code syntax and plays them using Web Audio.
 */

/** Library version. */
const VERSION = 0;

interface ReadonlyStringToNumberArray {
  readonly [index: string]: number;
}
interface ReadonlyNumberToStringArray {
  readonly [index: number]: string;
}
type Slice = {
  start: number;
  end: number;
}

/**
 * `SongPlayer` plays a `Song` using WebAudio.
 *
 * The Web Audio graph is
 *
 *     oscillator -> noteEnvelope -> overallGain -> lowpass -> (speaker)
 *
 * The oscillator produces the current note pitch frequency with a square wave.
 * The noteEnvelope (a gain node) shapes the onset, duration, and release of the
 * notes. The overallGain (a second gain node) controls the overall gain to
 * start and stop playback. The lowpass filter makes the square wave sound less
 * harsh and better approximates how the keyboard sounds.
 */
export class SongPlayer {
  playbackActive: boolean = false;
  tempo: number = SongPlayer.DEFAULT_TEMPO_BPM;
  gain: number = 0.25;
  private context: AudioContext = new AudioContext();
  private oscillator: OscillatorNode = this.context.createOscillator();
  private noteEnvelope: GainNode = this.context.createGain();
  private overallGain: GainNode = this.context.createGain();
  private lowpass: BiquadFilterNode = this.context.createBiquadFilter();
  private trackTimeout: ReturnType<typeof setTimeout> | null = null;

  constructor() {
    this.oscillator.type = "square";
    this.oscillator.connect(this.noteEnvelope);
    this.noteEnvelope.gain.value = 0.0;
    this.noteEnvelope.connect(this.overallGain);
    this.overallGain.gain.value = 0.0;
    this.overallGain.connect(this.lowpass);
    this.lowpass.type = "lowpass";
    this.lowpass.frequency.value = 2000.0;
    this.lowpass.Q.value = 0.7;
    this.lowpass.connect(this.context.destination);

    this.oscillator.start();
  }

  play(song: Song, noteFun?: (note?: Note) => void): void {
    if (song.notes.length == 0) { return; }

    this.stopTrackTimeout();
    this.playbackActive = true;
    // Schedule playback to begin 20 ms from now.
    let t = this.context.currentTime + 0.02;
    this.overallGain.gain.cancelScheduledValues(t);
    this.noteEnvelope.gain.cancelScheduledValues(t);
    this.oscillator.frequency.cancelScheduledValues(t);
    this.noteEnvelope.gain.setValueAtTime(0.0, t);
    this.overallGain.gain.linearRampToValueAtTime(0.0, t);
    this.overallGain.gain.linearRampToValueAtTime(this.gain, t + 0.005);

    let noteStartTimes: number[] = [t];
    const bpmToSeconds = 60.0 / (64 * this.tempo);

    for (let i = 0; i < song.notes.length; i++) {
      // Schedule playback of the ith note.
      const f = song.notes[i].frequency;
      const d = bpmToSeconds * song.notes[i].duration;
      this.noteEnvelope.gain.setValueAtTime(0.0, t);

      if (f > 0.0) {
        this.oscillator.frequency.setValueAtTime(f, t);
        this.noteEnvelope.gain.linearRampToValueAtTime(
          1.0, t + SongPlayer.NOTE_ATTACK_S);
        this.noteEnvelope.gain.setValueAtTime(
          1.0, t + d - SongPlayer.NOTE_RELEASE_S - SongPlayer.NOTE_GAP_S);
        this.noteEnvelope.gain.linearRampToValueAtTime(
          0.0, t + d - SongPlayer.NOTE_GAP_S);
      }

      t += d;
      noteStartTimes.push(t);
    }
    this.overallGain.gain.setValueAtTime(0.0, t);

    let noteIndex = 0;
    this.trackTimeout = setInterval(() => {
      const t = this.context.currentTime + 0.005;
      if (t < noteStartTimes[noteIndex]) {
        return;
      }

      while (noteIndex < song.notes.length &&
             t >= noteStartTimes[noteIndex + 1]) {
        noteIndex++;
      }

      if (noteIndex < song.notes.length) {
        if (noteFun != undefined) {
          noteFun(song.notes[noteIndex]);
        }
      } else {
        this.stopTrackTimeout();
        this.playbackActive = false;
        if (noteFun != undefined) {
          noteFun();
        }
      }
    }, 15);
  }

  stop(): void {
    if (!this.playbackActive) { return; }

    this.stopTrackTimeout();
    this.playbackActive = false;

    let t = this.context.currentTime;
    this.overallGain.gain.cancelScheduledValues(t);
    this.overallGain.gain.setValueAtTime(this.overallGain.gain.value, t + 0.02);
    // Ramp to zero to silence playback.
    this.overallGain.gain.linearRampToValueAtTime(0.0, t + 0.1);
  }

  private stopTrackTimeout(): void {
    if (this.trackTimeout != null) {
      clearTimeout(this.trackTimeout);
      this.trackTimeout = null;
    }
  }

  static readonly DEFAULT_TEMPO_BPM = 120;
  private static readonly NOTE_ATTACK_S = 0.003;
  private static readonly NOTE_RELEASE_S = 0.003;
  private static readonly NOTE_GAP_S = 0.0018;
}

function isSpaceChar(c: string): boolean {
  return " \t\n\\".indexOf(c) != -1;
}

/** Finds the next non-space char in `s`, beginning from `start`. */
function findNextNonSpaceChar(s: string, start: number): number {
  for (let i = start; i < s.length; i++) {
    if (!isSpaceChar(s.charAt(i))) {
      return i;
    }
  }
  return s.length;
}


/** `Song` represents a song with an array of `Notes`. */
export class Song {
  notes: Note[] = [];
  duration: number = 0;
  name: string = "";

  /** Parses and constructs a `Song` from a code string. */
  constructor(s: string) {
    let start = 0;
    let parensLevel = 0;

    s = s.toUpperCase();  // Make parsing ignore the case.
    start = findNextNonSpaceChar(s, start);

    // If the string begins with "#define", parse the song name.
    if (s.slice(start,  start + 8) === "#DEFINE ") {
      start = findNextNonSpaceChar(s, start + 8);
      let i = start;
      for (; i < s.length; i++) {
        const c = s.charAt(i);
        if (isSpaceChar(c)) {
          break;
        } else if (Song.SONG_NAME_CHARS.indexOf(c) == -1) {
          throw new Error(
            `Invalid character in song name: "${s.slice(start, i + 1)}"`);
        } else if (i - start > Song.MAX_SONG_NAME_LEN) {
          throw new Error("Song name is too long.");
        }
      }
      if (i == start) {
        throw new Error("Error parsing song name.");
      }
      this.name = s.slice(start, i);
      start = i;
    }

    for (let i = start; i < s.length; i++) {
      const c = s.charAt(i);
      if (c === "\\") {  // Ignore backslash line continuation chars.
        continue;
      } else if (c === "(") {
        parensLevel++;  // Track the parentheses level.
        continue;
      } else if (c === ")") {
        parensLevel--;
      } else if (Song.CODE_CHARS.indexOf(c) == -1) {
        throw new Error(`Invalid character: '${c}'`);
      }

      if (parensLevel > 1) {
        throw new Error("')' expected.");
      } else if (parensLevel < 0) {
        throw new Error("Unbalanced ( ).");
      }

      // Split the string on "," characters where not enclosed in ( ).
      if (parensLevel == 0 && (c === "," || i == s.length - 1)) {
        // Find the start and end character indices of the token.
        let end = (c === ",") ? i : i + 1;
        start = findNextNonSpaceChar(s, start);
        while (end > start && isSpaceChar(s.charAt(end - 1))) {
          end--;
        }
        if (end == s.length && end == start) {
          continue;
        }

        // Parse a Note from the token [start, end).
        let note = Note.parse(s.slice(start, end));
        note.codeSlice = {start: start, end: end};
        this.duration += note.duration;
        this.notes.push(note);
        start = i + 1;  // One past the "," character.
      }
    }
    if (parensLevel > 0) {
      throw new Error("')' expected.");
    }
  }

  /** Serializes the Song to a binary string representation. */
  binary(): string {
    let bytes = [];
    // Prepend library VERSION, useful in case encoding changes in the future.
    bytes.push(VERSION);

    // Serialize the song name.
    const nameBytes = Song.encodeName(this.name);
    bytes.push(nameBytes.length);
    bytes.push(...nameBytes);

    for (let i = 0; i < this.notes.length; i++) {  // For each note.
      // Convert and write the note's frequency as an index in the range 0-108.
      bytes.push(
        Math.min(Math.max(Note.hzToIndex(this.notes[i].frequency), 0), 255));
      // Directly write the note's duration in 1/64th beats as a byte.
      bytes.push(
        Math.min(Math.max(Math.round(this.notes[i].duration), 0), 255));
    }

    // Append Fletcher-16 checksum. This enables simple error detection on the
    // deserialization side, e.g. if the string was accidentally truncated.
    bytes.push(...fletcher16(bytes));

    let s = "";
    for (let i = 0; i < bytes.length; i++) {
      s += String.fromCharCode(bytes[i]);
    }
    return s;
  }

  /** Deserialize song code from binary string. */
  static songCodeFromBinary(binary: string): string {
    if (binary.length < 4) {
      throw new Error("Song data is too short.");
    }

    let bytes = [];
    for (let i = 0; i < binary.length; i++) {
      bytes.push(binary.charCodeAt(i));
    }

    // Check the VERSION.
    if (bytes[0] > VERSION) {
      throw new Error("Song has unsupported encoding.");
    }
    // Check the Fletcher-16 checksum.
    let checksum = fletcher16(bytes.slice(0, -2));
    if (bytes[bytes.length - 2] != checksum[0] ||
      bytes[bytes.length - 1] != checksum[1]) {
      throw new Error("Song has bad checksum.");
    }

    let s = "";

    // Deserialize the song name.
    let numNameBytes = bytes[1];
    if (numNameBytes > 0) {
      s += "#define " + Song.decodeName(bytes.slice(2, 2 + numNameBytes)) + " ";
    }

    // Deserialize the song notes.
    bytes = bytes.slice(2 + numNameBytes, -2);
    let numNotes = bytes.length / 2;
    for (let i = 0; i < numNotes; i++) {
      let f = Note.indexToHz(bytes[2 * i]);
      let d = bytes[2 * i + 1];
      if (i > 0) { s += ", "; }
      s += new Note(f, d).stringify();
    }

    return s;
  }

  /** Encodes a song name to bytes. */
  private static encodeName(s: string): number[] {
    if (s.length > Song.MAX_SONG_NAME_LEN) {
      throw new Error("Name is too long.");
    }

    let bytes = [];
    for (let i = 0; i < s.length; i += 4) {
      const numChars = Math.min(s.length - i, 4);
      const numBytes = (numChars == 4) ? 3 : numChars;
      let pack24 = 0;
      for (let j = 0; j < numChars; j++) {
        pack24 |=
          (Song.SONG_NAME_CHARS.indexOf(s.charAt(i + j)) + 1) << (6 * j);
      }
      for (let j = 0; j < numBytes; j++) {
        bytes.push((pack24 >> (8 * j)) & 255);
      }
    }
    return bytes;
  }

  /** Inverse of above, decodes a song name from bytes. */
  private static decodeName(bytes: number[]): string {
    if (bytes.length > (Song.MAX_SONG_NAME_LEN * 6 + 7) / 8) {
      throw new Error("Name is too long.");
    }

    let name = "";
    for (let i = 0; i < bytes.length; i += 3) {
      const numBytes = Math.min(bytes.length - i, 3);
      const numChars = (numBytes == 3) ? 4 : numBytes;
      let pack24 = 0;
      for (let j = 0; j < numBytes; j++) {
        pack24 |= bytes[i + j] << (8 * j);
      }
      for (let j = 0; j < numChars; j++) {
        const code = (pack24 >> (6 * j)) & 63;
        if (code > 0) {
          name += Song.SONG_NAME_CHARS.charAt(code - 1);
        }
      }
    }
    return name;
  }

  /** Characters that are allowed in the song name. */
  private static readonly SONG_NAME_CHARS =
    "0123456789_ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
  /** Characters allowed in the song code. */
  private static readonly CODE_CHARS = Song.SONG_NAME_CHARS + " \t\n\\,()#";
  /** Max allowed length for the song's name. */
  private static readonly MAX_SONG_NAME_LEN = 32;
}

/** `Note` represents a single note, having a frequency and a duration. */
class Note {
  /** Pitch frequency in Hz. */
  frequency: number;
  /** Duration in 1/64ths of a beat. */
  duration: number;
  /** The range where this Note appears in the song code. */
  codeSlice: Slice | null = null;

  constructor(frequency: number, duration: number) {
    this.frequency = frequency;
    this.duration = duration;
  }

  /** Parses a Note from a string like "Q__NOTE(_E4)". */
  static parse(s: string): Note {
    s = s.trim();

    if (!(12 <= s.length && s.length <= 18) ||
        s.slice(2, 8) !== "_NOTE(" || s.slice(-1) !== ")") {
      const syntax = (s.length <= 20) ? s : s.slice(0, 18) + "...";
      const closeParen = s.indexOf(")");
      if (0 <= closeParen && closeParen <= s.length - 2 &&
          s[closeParen + 1] !== ",") {
        throw new Error(`',' expected: "${syntax}"`);
      } else {
        throw new Error(`Invalid note: "${syntax}"`);
      }
    }

    const args = s.slice(8, -1).split(",");
    let duration = 0;

    if (s.slice(0, 2) === "M_") {
      if (args.length != 2) {
        throw new Error(`Invalid note: "${s}"`);
      }

      duration = parseFloat(args[1]);
      if (!(0.0 <= duration && duration <= 1000.0)) {
        throw new Error(`Invalid duration: "${s}"`);
      }
    } else {
      const key = s.slice(0, 2);
      if (!(key in Note.NAME_TO_DURATION) || args.length != 1) {
        throw new Error(`Invalid note: "${s}"`);
      }

      duration = Note.NAME_TO_DURATION[key];
    }

    s = args[0].trim();
    let frequency = 0;

    if (s !== "_REST") {
      const octave = parseInt(s.slice(-1), 10);
      const key = s.slice(1, -1);

      if (!(key in Note.NAME_TO_SEMITONE) || s.charAt(0) !== "_" ||
          !(0 <= octave && octave <= 8)) {
        throw new Error(`Invalid note: "${s}"`);
      }

      frequency = Note.indexToHz(12 * octave + Note.NAME_TO_SEMITONE[key]);
    }

    return new Note(frequency, duration);
  }

  /** Represents a Note as a string. */
  stringify(): string {
    let s = "";
    if (this.frequency > 0.0) {
      const i = Note.hzToIndex(this.frequency);
      const semitone = Note.SEMITONE_NAMES[i % 12];
      const octave = Math.floor(i / 12);
      s = `_${semitone}${octave}`;
    } else {
      s = "_REST";
    }

    if (this.duration in Note.DURATION_NAMES) {
      s = `${Note.DURATION_NAMES[this.duration]}_NOTE(${s})`;
    } else {
      s = `M__NOTE(${s}, ${Math.round(this.duration)})`;
    }

    return s;
  }

  /** Converts a note index to frequency in Hz. */
  static indexToHz(noteIndex: number): number {
    if (noteIndex >= 108) { return 0.0; }
    return Note.C0_FREQUENCY_HZ * Math.exp(noteIndex / (12.0 * Math.LOG2E));
  }

  /** Inverse of above; converts frequency in Hz to note index. */
  static hzToIndex(frequencyHz: number): number {
    if (frequencyHz == 0.0) { return 108; }
    return Math.round((12.0 * Math.LOG2E) * Math.log(frequencyHz / Note.C0_FREQUENCY_HZ));
  }

  /** Frequency of C0, the lowest key, in Hz. */
  private static readonly C0_FREQUENCY_HZ = 16.35159783;
  /** Mapping of name to duration (in 1/6th beats), used in parsing. */
  private static readonly NAME_TO_DURATION: ReadonlyStringToNumberArray = {
    "B_": 128, "W_": 64, "H_": 32, "Q_": 16, "E_": 8, "S_": 4, "T_": 2,
    "BD": 128 + 64, "WD": 64 + 32, "HD": 32 + 16, "QD": 16 + 8, "ED": 8 + 4,
    "SD": 4 + 2, "TD": 2 + 1
  };
  /** Inverse of above, mapping duration to name. */
  private static readonly DURATION_NAMES: ReadonlyNumberToStringArray = {2: "T_", 3: "TD", 4: "S_",
    6: "SD", 8: "E_", 12: "ED", 16: "Q_", 24: "QD", 32: "H_", 48: "HD",
    64: "W_", 96: "WD", 128: "B_", 192: "BD"};
  /** Mapping of semitone name to index from 0 to 11. */
  private static readonly NAME_TO_SEMITONE: ReadonlyStringToNumberArray = {
    "C": 0, "CS": 1, "DF": 1, "D": 2, "DS": 3, "EF": 3, "E": 4, "F": 5, "FS": 6,
    "GF": 6, "G": 7, "GS": 8, "AF": 8, "A": 9, "AS": 10, "BF": 10, "B": 11
  };
  /** Array of semitone names. */
  private static readonly SEMITONE_NAMES = [
    "C", "CS", "D", "DS", "E", "F", "FS", "G", "GS", "A", "AS", "B"
  ];
}

/** Computes the Fletcher-16 checksum of `bytes`. */
function fletcher16(bytes: number[]): number[] {
  let sum1 = 1;
  let sum2 = 0;
  for (let i = 0; i < bytes.length; i++) {
    sum1 = (sum1 + bytes[i]) % 255;
    sum2 = (sum2 + sum1) % 255;
  }
  return [sum1 % 255, sum2 % 255];
}

