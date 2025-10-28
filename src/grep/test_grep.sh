#!/bin/bash

  S21=./grep
  SYS=grep
  TMP=tmp
  mkdir -p $TMP

  GREEN='\033[0;32m'
  RED='\033[0;31m'
  NC='\033[0m'

  FILES=(inputs/input_01.txt inputs/input_02.txt inputs/input_03.txt inputs/input_04.txt)
  FLAGS=("e" "i" "v" "c" "l" "n" "h" "s" "o" "f")

  COMBOS=()
  for f1 in "${FLAGS[@]}"; do
    COMBOS+=("-$f1")
    for f2 in "${FLAGS[@]}"; do
      if [[ $f1 != $f2 ]]; then
        COMBOS+=("-$f1 -$f2")
      fi
    done
  done

  SUCCESS=0
  FAIL=0

  for file in "${FILES[@]}"; do
    for flags in "${COMBOS[@]}"; do
      if [[ $flags == *"-f"* ]]; then
        CMD="$flags patterns.txt $file"
      else
        CMD="$flags test $file"
      fi

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