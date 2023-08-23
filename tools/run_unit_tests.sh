#!/usr/bin/env bash

set -e
set -o pipefail
set -u

function check_cmake_version() {
  if [[ $(cmake --version) ]]
  then
    echo "cmake was found"
  else
    echo "cmake is not installed"
    exit 1
  fi
}

function build_unit_test_executable() {
  cmake --build . --target unit_test
}

function run_unit_test_executable() {
  ./unit_test \
    --reporter JUnit::out=result-junit.xml \
    --reporter console::out=-::colour-mode=ansi \
    --durations yes \
    --verbosity high

  make coverage
}

function clean_up_gcda_files() {
  cd "$(dirname "${0}")/../../tuner/CMakeFiles/test.dir/tuner"
  echo "$PWD"
  rm ./*.gcda
}

check_cmake_version
build_unit_test_executable
run_unit_test_executable
clean_up_gcda_files