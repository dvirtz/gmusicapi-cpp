#!/bin/sh

pushd gmusicapi
python setup.py sdist --formats=zip -k
find ./dist -iname "*.zip" -print0 | xargs -0 pip install
popd