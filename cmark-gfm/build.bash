#!/bin/bash

# Copyright (c) 2019 rokudogobu
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

set -e

url=https://github.com/github/cmark-gfm/archive/0.29.0.gfm.11.zip

[ -n "$CMAKE" ] || CMAKE="$( which cmake || echo )"
[ -x "$CMAKE" ] || { echo "*** error: 'cmake' not found. you must install it first, and then run this script again." >&2; exit 1; }

file_zip="${url##*/}"
version=${file_zip%.zip}
dir_src="cmark-gfm-$version"
dir_build="build"

cd "$( dirname "$BASH_SOURCE" )"

is_installed=1
[ -f include/cmark-gfm_export.h            ] || is_installed=0
[ -f include/cmark-gfm_version.h           ] || is_installed=0
[ -f include/cmark-gfm-core-extensions.h   ] || is_installed=0
[ -f include/cmark-gfm-extension_api.h     ] || is_installed=0
[ -f include/cmark-gfm-extensions_export.h ] || is_installed=0
[ -f include/cmark-gfm.h                   ] || is_installed=0
[ -f lib/libcmark-gfm-extensions.a         ] || is_installed=0
[ -f lib/libcmark-gfm.a                    ] || is_installed=0

[ $is_installed -eq 1 ] && { echo "*** skip: libcmark-gfm is already built." >&2; exit 0; }

[ -f "$file_zip" ] || curl -LO "$url"
unzip -u "$file_zip"

[ -e "$dir_build" ] || mkdir "$dir_build"

"$CMAKE" \
  -S "$dir_src" \
  -B "$dir_build" \
  -DCMAKE_INSTALL_PREFIX="." \
  -DCMARK_TESTS=OFF \
  -DCMARK_SHARED=OFF

make -C "$dir_build" && make -C "$dir_build" install
