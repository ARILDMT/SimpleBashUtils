#!/bin/bash

S21=./cat
SYS=cat
TMP=tmp
mkdir -p $TMP

GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m'

FILES=(input_01.txt input_02.txt input_03.txt)
FLAGS=("b" "e" "n" "s" "t" "v")

SUCCESS=0
FAIL=0

for file in "${FILES[@]}"; do
  for flag in "${FLAGS[@]}"; do
    CMD="-$flag $file"

    $S21 $CMD > $TMP/s21.txt 2>/dev/null
    $SYS $CMD > $TMP/sys.txt 2>/dev/null

    if diff -q $TMP/s21.txt $TMP/sys.txt > /dev/null; then
      echo -e "${GREEN}[PASS]${NC} $CMD"
      ((SUCCESS++))
    else
      echo -e "${RED}[FAIL]${NC} $CMD"
      ((FAIL++))
    fi
  done
done

echo -e "${GREEN} PASSED:${NC} $SUCCESS"
echo -e "${RED} FAILED:${NC} $FAIL"