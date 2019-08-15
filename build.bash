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

function usage() {
cat <<EOF
usage: bash build.bash [-h | --help] [-f | --force] [-i | --install]
EOF
}

function error() {
  echo "*** error: $*" >&2
  exit 1
}

while [ $# -gt 0 ]
do
  case "$1" in
    '-f'|'--force'   ) FLAG_FORCE=1; shift;;
    '-i'|'--install' ) FLAG_INSTALL=1; shift;;
    '-h'|'--help'    ) usage; exit;;
    * ) error "unknown option $1.";;
  esac 
done

cd "$( dirname "$BASH_SOURCE" )"

INSTALL_PATH=
FULL_PRODUCT_NAME=
while read line
do
  if [[ $line =~ ([[:alnum:]_]+)\ =\ (.+) ]]; then
    case "${BASH_REMATCH[1]}" in
      "FULL_PRODUCT_NAME" ) FULL_PRODUCT_NAME=${BASH_REMATCH[2]};;
      "INSTALL_PATH"      ) INSTALL_PATH=${BASH_REMATCH[2]};;
    esac
  fi
done < <( xcodebuild -showBuildSettings )

[ "${FLAG_INSTALL:-0}" -eq 0 ] && INSTALL_PATH="$( pwd )"

[ -n "$INSTALL_PATH"         ] || error "failed to get INSTALL_PATH from XCode build settings."
[ -n "$FULL_PRODUCT_NAME"    ] || error "failed to get FULL_PRODUCT_NAME from XCode build settings."

if [ -e "$INSTALL_PATH/$FULL_PRODUCT_NAME" ]; then
  if [ "${FLAG_FORCE:-0}" -eq 1 ]; then
    rm -rf "$INSTALL_PATH/$FULL_PRODUCT_NAME"
  else
    error "$FULL_PRODUCT_NAME already exists in $INSTALL_PATH, please remove it first or use '--force' option."
  fi
fi

bash cmark-gfm/build.bash

[ -e build ] && rm -rf build

xattr -rc qlcmark-gfm

if [ "${FLAG_INSTALL:-0}" -eq 1 ]; then
  xcodebuild install SYMROOT="$( pwd )/build" DSTROOT=/ 
  qlmanage -r
else
  xcodebuild install SYMROOT="$( pwd )/build" DSTROOT=/ INSTALL_PATH="$( pwd )"
fi
