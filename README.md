# qlcmark-gfm

QuickLook Plugin for [CommonMark](https://commonmark.org) documents by using [cmark-gfm](https://github.com/github/cmark-gfm).

![screenshot](https://github.com/rokudogobu/qlcmark-gfm/blob/images/darklight.png?raw=true)

## Supported Features

- [GitHub Flavored Markdown extensions](https://github.github.com/gfm/)
- Dark Mode
- Unicode encoding (UTF-8, UTF-16BE, UTF-16LE, UTF-32BE and UTF-32LE) detection by BOM
- Japanese encoding (Shift-JIS, EUC-JP and ISO-2022-JP) detection by Brute forcing

## Installation

Via Homebrew

    $ # installed to ~/Library/QuickLook
    $ brew cask install https://raw.githubusercontent.com/rokudogobu/qlcmark-gfm/master/qlcmark-gfm.rb
    $ xattr -r -d com.apple.quarantine ~/Library/QuickLook/qlcmark-gfm.qlgenerator

Optionally, clear QuickLook client's generator cache

    $ qlmanage -r

If you want to build from source, please make sure `cmake` is installed to your mac, and then follow the steps below.

    $ git clone https://github.com/rokudogobu/qlcmark-gfm.git
    $ cd qlcmark-gfm
    $ bash build.bash

Please build at a directory which is NOT synced with iCloud, otherwise a codesign process may fail.

## Setting Preferences

You can change color mode and font-size by executing following command:

    $ defaults write io.github.rokudogobu.qlcmark-gfm <key> -string <value>

The keys and values are listed below:

| key | default | possible values |
|:---:|:---:|:---:|
| mode | `auto` | `light`, `dark` or others ( interpreted as `auto` ) |
| font-size | 14px | any CSS values for `font-size` property |

## License

Copyright (c) 2019 rokudogobu.  
Licensed under the Apache License, Version 2.0.  
See LICENSE for details.
