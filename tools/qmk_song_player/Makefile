# Copyright 2023 Google LLC
#
# Licensed under the Apache License, Version 2.0 (the "License"); you may not
# use this file except in compliance with the License. You may obtain a copy of
# the License at
#
#     https://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
# WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
# License for the specific language governing permissions and limitations under
# the License.

.PHONY: test clean
.SUFFIXES: .ts .js

main.min.js: main.js song.js header-comment.js
	google-closure-compiler -O ADVANCED --language_in ECMASCRIPT_2016 --language_out ECMASCRIPT_2016 $^ --js_output_file temp.js
	cat header-comment.js temp.js > $@
	$(RM) temp.js

.ts.js:
	tsc

check:
	npm test

clean:
	$(RM) main.js song.js main.min.js
