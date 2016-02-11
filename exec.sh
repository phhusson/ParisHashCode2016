#!/bin/bash

./main "$@" > out.v 2> out2.v

grep -E '^D:' out.v

cat out2.v out.v |grep -vE '^D:' > out
rm -f out2.v out.v
cat out
