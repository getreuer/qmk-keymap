# Copyright 2021-2022 Google LLC
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     https://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

"""Program to count character frequencies."""
import collections
import sys
from typing import Dict, List, Set

HELP_TEXT = """Count character frequencies.
Use: python3 count_chars.py [options] file [file2 ...]

Reads the specified files and counts how often each character occurs.

Options:
  --chars   Which chars to display results for:
            --chars=symbols          Only symbols !"#$%& etc.
            --chars=digits           Only digits 0123456789
            --chars=letters          Only letters A-Z,a-z
            --chars=symbols+digits   Symbols and digits (default)
            --chars=all              All characters
"""


def count_chars(input_file_names: List[str]) -> Dict[str, int]:
  """Counts how often each char occurs in `input_file_names`."""
  hist = collections.defaultdict(int)
  for file_name in input_file_names:
    for line in open(file_name, 'rt'):
      for char in line.lower():
        hist[char] += 1

  return dict(hist)


def print_char_count_table(hist: Dict[str, int], chars: str) -> None:
  """Prints results table of char counts, filtered according to `chars`."""
  if chars == 'all':
    chars = set(hist.keys())
  else:
    chars = parse_chars_option(chars)

  ranked_chars = sorted(sorted(chars), key=lambda char: -hist.get(char, 0))
  total_chars = sum(hist.values())
  print('Rank  char    count        %')
  for i, char in enumerate(ranked_chars):
    count = hist.get(char, 0)
    if count:
      percent = (100.0 / total_chars) * count
      print(f'#{(i + 1):<3} {repr(char):>5} {count:8} {percent:8.3f}')

  print(f'\ntotal chars: {total_chars}\n')


def parse_chars_option(value: str) -> Set[str]:
  """Parses the `--chars` command line option."""
  char_sets = {
    'symbols': '!"#$%&\'()*+,-./:;<=>?@[\\]^_`{|}~',
    'digits': '0123456789',
    'letters': 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz',
  }

  chars = ''
  for name in value.split('+'):
    try:
      chars += char_sets[name.lower()]
    except KeyError:
      print(f'Invalid char set: {name}')
      sys.exit(1)

  return set(chars)


def main(argv):
  chars = 'symbols+digits'  # Show counts for symbols and digits by default.
  input_file_names = []

  for arg in argv[1:]:
    if arg.startswith('--'):  # Parse command line options.
      option, value = arg.split('=', 1)
      if option == '--chars':
        chars = value
      else:
        print(f'Invalid option: {arg}')
        sys.exit(1)

    else:
      input_file_names.append(arg)

  if not input_file_names:  # No input given; show help text and exit.
    print(HELP_TEXT)
    sys.exit(1)

  hist = count_chars(input_file_names)
  print_char_count_table(hist, chars)


if __name__ == '__main__':
  main(sys.argv)

