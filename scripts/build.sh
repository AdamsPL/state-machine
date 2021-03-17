#!/bin/bash
for B in build/*;
do
    ninja -C ${B} $*
done


