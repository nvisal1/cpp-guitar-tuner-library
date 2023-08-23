#!/bin/bash

cmake . \
  -DCMAKE_BUILD_TYPE=Debug \
  -DCMAKE_CXX_FLAGS_DEBUG="-g -fprofile-arcs -ftest-coverage" \
  -DCMAKE_C_FLAGS_DEBUG="-g -fprofile-arcs -ftest-coverage"
