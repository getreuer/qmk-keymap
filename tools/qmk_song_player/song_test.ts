import { Song } from "./song";

const TEST_SONG_CODE = (
  "#define TEST_SONG Q__NOTE(_C0), W__NOTE(_REST), M__NOTE(_CS3, 13)"
);

// Test that TEST_SONG_CODE parses as expected.
test("parse song", () => {
  const song = new Song(TEST_SONG_CODE);

  expect(song.name).toEqual("TEST_SONG");
  expect(song.notes.length).toBe(3);
  expect(song.notes[0].frequency).toBeCloseTo(16.35);
  expect(song.notes[0].duration).toBe(16);
  expect(song.notes[0].codeSlice).toEqual({start: 18, end: 30});
  expect(song.notes[1].frequency).toBeCloseTo(0.0);
  expect(song.notes[1].duration).toBe(64);
  expect(song.notes[1].codeSlice).toEqual({start: 32, end: 46});
  expect(song.notes[2].frequency).toBeCloseTo(138.59);
  expect(song.notes[2].duration).toBe(13);
  expect(song.notes[2].codeSlice).toEqual({start: 48, end: 65});
  expect(song.duration).toBe(16 + 64 + 13);
});

// Test a round trip of encoding to and from binary representation.
test("song binary round trip", () => {
  const song = new Song(TEST_SONG_CODE);
  const binary = song.binary();
  const recoveredCode = Song.songCodeFromBinary(binary);

  expect(recoveredCode).toEqual(TEST_SONG_CODE);
});

