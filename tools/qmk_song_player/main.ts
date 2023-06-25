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

import { SongPlayer, Song } from "./song.js";

function updatePageUrl(query: string) {
  let newUrl = window.location.href.split("?")[0] + query;
  window.history.replaceState("", "", newUrl);
}

function showToast(icon: string, message: string) {
  const toast = document.getElementById("toast") as HTMLDivElement;
  const toastIcon = document.getElementById("toast-icon") as HTMLSpanElement;
  const toastMessage =
    document.getElementById("toast-message") as HTMLSpanElement;

  if (toast == null || toastIcon == null || toastMessage == null) {
    return;
  }

  toastIcon.innerText = icon;
  toastMessage.innerText = message;
  toast.className = "show";
  setTimeout(() => { toast.className = ""; }, 3000);
}

function copyToClipboard(content: string): Promise<void> {
  if (navigator.clipboard) {
    return navigator.clipboard.writeText(content);
  }

  if (!window.getSelection) {
    return Promise.resolve();
  }
  const selection = window.getSelection();
  if (selection == null) {
    return Promise.reject();
  }

  const dummy = document.createElement('p');
  dummy.textContent = content;
  document.body.appendChild(dummy);

  const range = document.createRange();
  range.setStartBefore(dummy);
  range.setEndAfter(dummy);
  selection.removeAllRanges();
  selection.addRange(range);

  const success = document.execCommand("copy");
  document.body.removeChild(dummy);
  return success ? Promise.resolve() : Promise.reject();
}

function onShareClick() {
  const url = window.location.href;
  copyToClipboard(url)
    .then(() => showToast("share", "URL copied to clipboard"))
    .catch(() => showToast("error", "Please copy URL from location bar"));
}

function init() {
  const songCode = document.getElementById("song-code") as HTMLInputElement;
  const songTitle = document.getElementById("song-title") as HTMLHeadingElement;
  const songInfo = document.getElementById("song-info") as HTMLSpanElement;
  const playButton = document.getElementById("play-button") as HTMLInputElement;
  const playButtonIcon =
    document.getElementById("play-button-icon") as HTMLSpanElement;
  const shareButton =
    document.getElementById("share-button") as HTMLInputElement;
  if (songCode == null || songTitle == null || songInfo == null ||
    playButton == null || playButtonIcon == null || shareButton == null) {
    return;
  }

  let songPlayer: SongPlayer | null = null;
  let song: Song | null = null;
  let tempo = 120;

  function updateSong(s: string) {
    songCode.value = s;

    try {
      song = new Song(s);
      playButton.disabled = false;
    } catch (error) {
      if (error instanceof Error) {
        songInfo.innerHTML = `<span class="code-error">${error.message}</span>`;
      }
      song = null;
      playButton.disabled = true;
      return;
    }

    const bpmToSeconds = 60.0 / (64 * tempo);
    const songDuration = song.duration * bpmToSeconds;
    songInfo.innerHTML = `Duration: ${songDuration.toFixed(1)} seconds<br>Firmware size: ${song.notes.length * 8} bytes`;

    const title = ((song.name) ? song.name + " \u2013 " : "")
      + "QMK song player \u266b";
    songTitle.innerText = title;
    document.title = title;
    updatePageUrl("?" + encodeURI(btoa(song.binary())));
  }

  shareButton.onclick = onShareClick;

  const queryString = window.location.search;
  if (queryString.length > 2) {
    let s = queryString.slice(1);
    updateSong(Song.songCodeFromBinary(atob(decodeURI(s))));
  } else {
    updateSong(songCode.value);
  }

  function updatePlaybackActive() {
    const active = (songPlayer != null && songPlayer.playbackActive);
    playButtonIcon.innerText = active ? "stop" : "play_arrow";
    songCode.readOnly = active;

    if (!active) {
      songCode.setSelectionRange(0, 0);
    }
  }

  songCode.oninput = () => updateSong(songCode.value);
  playButton.onclick = () => {
    if (songPlayer == null) {
      songPlayer = new SongPlayer();
    }

    if (songPlayer.playbackActive || song == null) {
      songPlayer.stop();
    } else {
      songPlayer.play(song, (note) => {
        songCode.focus();
        if (note == undefined) {
          updatePlaybackActive();
        } else if (note.codeSlice != null) {
          songCode.setSelectionRange(note.codeSlice.start, note.codeSlice.end);
        }
      });
    }

    updatePlaybackActive();
  };
}
init();
