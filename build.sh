#!/bin/bash
mkdir -p build 
qmake -project -o BibliothecaProcurator.pro -after "QT += core gui widgets"
cd build
make clean
qmake ../BibliothecaProcurator.pro
make
./BibliothecaProcurator
