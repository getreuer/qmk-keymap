# Achordion tests

End-to-end tests for `features/achordion.c`, run against real QMK with QMK's
[unit test framework](https://docs.qmk.fm/unit_testing). They exercise Achordion
together with QMK's Combos, layer-tap, and mod-tap keys, and cover the
combo-on-a-layer-tap's-layer bug from
[issue #93](https://github.com/getreuer/qmk-keymap/issues/93).

## Running

Copy this directory into a `qmk_firmware` checkout, then point the build at this
repo's `features` directory:

```sh
cp -R tests/achordion /path/to/qmk_firmware/tests/
cd /path/to/qmk_firmware
make test:achordion ACHORDION_PATH="$OLDPWD/features"
```

QMK discovers tests by searching `tests/*/test.mk` with `find`, which does not
follow symlinked directories, so this directory has to be copied rather than
linked. `ACHORDION_PATH` keeps the sources under test in this repo, so there is
no second copy of `achordion.c` to keep in sync.
