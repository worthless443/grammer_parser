#!/bin/bash
g++ parser_prot.cc eval.cc stack.cpp TableGen.cc -I./ -std=c++20 -lfmt -o parser 

