#!/bin/bash
idx=0
for i in split*;
do
  mv $i list_$idx.txt
  idx=$((idx +1))
done
