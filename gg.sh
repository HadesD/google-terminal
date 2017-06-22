#!/bin/sh
g++ -o gg -std=c++11 \
  main.cpp \
  -lcurl

./gg
