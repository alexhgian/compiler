#!/bin/sh

# Run the goodness
rm -r .tmp
mkdir .tmp
cp -rf ./**/*.glsl ./**/*.dat .tmp/
./outfile.sh `(ls .tmp/  | sed -ne 's/\.glsl$//p')`
python test-all.py
