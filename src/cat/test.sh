#!/bin/bash

GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m' # No Color
if [ $# -eq 0 ]; then
  echo "No arguments supplied"
  exit 1
fi
if [ $1 == 1 ]; then 
  INPUTFILE="file1.txt"
else if [ $1 == 2 ]; then
  INPUTFILE="file2.txt"
else
  INPUTFILE=$1
fi fi
COUNT=0

function check_result {
  if [ $? -eq 0 ]; then
    echo -e "${GREEN}Test passed${NC}"
    COUNT=$((COUNT+1))
  else
    echo -e "${RED}Test failed${NC}"
  fi
}

flag=(
  '-b'
  '-e'
  '-n'
  '-s'
  '-t'
  '-E'
  '-T'
)

for i in ${flag[*]}; do
  ./s21_cat ${i} ${INPUTFILE} > result.txt
  cat ${i} ${INPUTFILE} > expected.txt
  echo -e "Test ${i} flag"
  cmp result.txt expected.txt
  check_result

  echo -e "\n-------------\n"
done 

./s21_cat --number-nonblank ${INPUTFILE} > result.txt
cat -b ${INPUTFILE} > expected.txt
echo -e "Test --number-nonblank flag"
cmp result.txt expected.txt
check_result

echo -e "\n-------------\n"

./s21_cat --number ${INPUTFILE} > result.txt
cat -n ${INPUTFILE} > expected.txt
echo -e "Test --number flag"
cmp result.txt expected.txt
check_result

echo -e "\n-------------\n"

./s21_cat --squeeze-blank ${INPUTFILE} > result.txt
cat -s ${INPUTFILE} > expected.txt
echo -e "Test --squeeze-blank flag"
cmp result.txt expected.txt
check_result

echo -e "\n-------------\n"

./s21_cat -ne ${INPUTFILE} > result.txt
cat -ne ${INPUTFILE} > expected.txt
echo -e "Test -ne flags"
cmp result.txt expected.txt
check_result

echo -e "\n-------------\n"

./s21_cat -tbs ${INPUTFILE} > result.txt
cat -tbs ${INPUTFILE} > expected.txt
echo -e "Test -tbs flags"
cmp result.txt expected.txt
check_result

echo -e "\n-------------\n"

rm result.txt expected.txt

echo -e "Total tests passed: ${COUNT}/12"

clang-format -n *.c

cppcheck --enable=all *.c