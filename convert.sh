#!/bin/bash
# Uses Sox to convert a file to 16 bit integer wave
# Usage: convert.sh <input file> <output file>

# NOTE: or unsigned-integer?
ENCODING=signed-integer
# ENCODING=unsigned-integer
BITS=16
CHANS=2
TARGET_FILETYPE=wav

sox "$1" -r 44100 -e ${ENCODING} -b ${BITS} -t ${TARGET_FILETYPE} -c ${CHANS} "$2"
