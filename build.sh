#!/bin/bash
mkdir -p build 
cd build
make clean
qmake ../BibliothecaProcurator.pro
make
./BibliothecaProcurator
