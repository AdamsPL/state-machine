#!/bin/bash

rm -rvf build
CC=gcc CXX=g++ meson build/gcc
CC=clang CXX=clang++ meson build/clang
