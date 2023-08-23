#!/bin/bash

emcmake cmake \
  -DBUILD_SHARED_LIBS=OFF \
  -DKISSFFT_TEST:BOOL=OFF \
  -DKISSFFT_TOOLS:BOOL=OFF \
  -DKISSFFT_STATIC:BOOL=OFF \
  .

make wasm_tuner

mv ./wasm_tuner.js ./wasm_public
mv ./wasm_tuner.wasm ./wasm_public

rm ./Makefile
rm ./CMakeCache.txt


