#!/bin/zsh

IN_DIR=$1
OUT_DIR=$2

# run the convert.sh script on all files in the directory. add _convered suffix
for file in ${IN_DIR}/*.wav; do
  OUT_FILE="${OUT_DIR}/$(basename $file .wav)_converted.wav"
  echo "Converting $file to $OUT_FILE"
    ./convert.sh "$file" "$OUT_FILE"
done
