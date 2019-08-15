# qlcmark-gfm

QuickLook Plugin for [CommonMark](https://commonmark.org) documents by using [cmark-gfm](https://github.com/github/cmark-gfm).

![screenshot](https://github.com/rokudogobu/QLCommonMark/blob/images/darklight.png?raw=true)

## Supported Features

- [GitHub Flavored Markdown extensions](https://github.github.com/gfm/)
- Dark Mode
- Unicode encoding (UTF-8, UTF-16BE, UTF-16LE, UTF-32BE and UTF-32LE) detection by BOM
- Japanese encoding (Shift-JIS, EUC-JP and ISO-2022-JP) detection by Brute forcing

## Installation

To install qlcmark-gfm, please copy or move `qlcmark-gfm.qlgenerator` to `~/Library/QuickLook` and execute following command in Terminal.app.
    
    $ qlmanage -r

If you want to build from source, please make sure `cmake` is installed to your mac, and then follow the steps below.

    $ git clone https://github.com/rokudogobu/qlcmark-gfm.git
    $ cd qlcmark-gfm
    $ bash build.bash

Please build at a directory which is NOT synced with iCloud, otherwise a codesign process may fail.

## License

Copyright (c) 2019 rokudogobu.  
Licensed under the Apache License, Version 2.0.  
See LICENSE for details.
