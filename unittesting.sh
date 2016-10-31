#!/usr/bin/env bash
cd test
rm -rf build
mkdir build
cd build
cmake ..
make
chmod u+x SeedCup2016_Test
./SeedCup2016_Test