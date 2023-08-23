#!/bin/bash

rm -rf ./_deps \
  ./CMakeFiles \
  ./coverage \
  ./generated-includes &> /dev/null

rm ./CMakeCache.txt \
  ./coverage.info \
  ./coverage_filtered.info \
  ./Makefile \
  ./test \
  ./result-junit.xml \
  ./acceptance_test \
  ./unit_test &> /dev/null
