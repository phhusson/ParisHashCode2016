#!/bin/bash

set -x
./main "$@" > out.v 2> out2.v

cat out2.v out.v > out
rm -f out2.v out.v
