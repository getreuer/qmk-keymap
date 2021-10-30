# Pascal Getreuer's QMK keymap

(This is not an officially supported Google product.)

![Dactyl Ergodox](doc/dactyl_ergodox.jpg)

This is my [Quantum Mechanical Keyboard (QMK)](https://docs.qmk.fm) keymap for
the [Dactyl
Ergodox](https://ohkeycaps.com/products/built-to-order-dactyl-manuform-keyboard).
Who knew a keyboard could do so much?

### Topics

* [Autocorrection with QMK](https://getreuer.info/posts/keyboards/autocorrection/index.html) &ndash; run rudimentary
  autocorrection on your keyboard

* [Caps Word](https://getreuer.info/posts/keyboards/caps-word/index.html) &ndash; modern alternative to Caps Lock

* [Custom shift keys](https://getreuer.info/posts/keyboards/custom-shift-keys/index.html) &ndash; they're
  surprisingly tricky to get right; here is my approach

* [Macro buttons](https://getreuer.info/posts/keyboards/macros/index.html) &ndash; an assortment of practical QMK
  macros

* [Designing a symbol layer](https://getreuer.info/posts/keyboards/symbol-layer/index.html) &ndash; ergonomic and
  character frequency considerations


## My keymap

Here is a [KLE](http://www.keyboard-layout-editor.com/) visualization of my
keymap. See the [keymap.c](keymap.c) itself for full details.

**Base layer** (Dvorak)

![Base layer](doc/layout_base.png)

**QWERTY layer**

![QWERTY layer](doc/layout_qwerty.png)

**Symbol layer**

![Symbol layer](doc/layout_symbol.png)

**Unicode layer**

![Unicode layer](doc/layout_unicode.png)


## License

This code uses the Apache License 2.0. See the [LICENSE file](LICENSE.txt) for
details.


## Installation

Clone the [QMK firmware](https://github.com/qmk/qmk_firmware) and place this
repo in `qmk_firmware/keyboards/handwired/dactyl_promicro/keymaps/getreuer`.


