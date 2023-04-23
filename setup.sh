#!/bin/bash

mkdir .libs && cd .libs

git clone https://github.com/oatpp/oatpp.git
cd oatpp
mkdir build && cd build
cmake ..
make install