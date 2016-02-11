#!/bin/bash

./main "$@" > out.v 2> out2.v

cat out2.v out.v |grep -vE '^D:' > out
cat out

grep -E '^D:' out.v
rm -f out2.v out.v
