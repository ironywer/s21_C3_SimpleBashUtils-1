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

./s21_cat -b ${INPUTFILE} > result.txt
cat -b ${INPUTFILE} > expected.txt
echo -e "Test -b flag"
cmp result.txt expected.txt
check_result

echo -e "\n-------------\n"

./s21_cat -e ${INPUTFILE} > result.txt
cat -e ${INPUTFILE} > expected.txt
echo -e "Test -e flag"
cmp result.txt expected.txt
check_result

echo -e "\n-------------\n"

./s21_cat -n ${INPUTFILE} > result.txt
cat -n ${INPUTFILE} > expected.txt
echo -e "Test -n flag"
cmp result.txt expected.txt
check_result

echo -e "\n-------------\n"

./s21_cat -s ${INPUTFILE} > result.txt
cat -s ${INPUTFILE} > expected.txt
echo -e "Test -s flag"
cmp result.txt expected.txt
check_result

echo -e "\n-------------\n"

./s21_cat -t ${INPUTFILE} > result.txt
cat -t ${INPUTFILE} > expected.txt
echo -e "Test -t flag"
cmp result.txt expected.txt
check_result

echo -e "\n-------------\n"

./s21_cat -E ${INPUTFILE} > result.txt
cat -E ${INPUTFILE} > expected.txt
echo -e "Test -E flag"
cmp result.txt expected.txt
check_result

echo -e "\n-------------\n"

./s21_cat -T ${INPUTFILE} > result.txt
cat -T ${INPUTFILE} > expected.txt
echo -e "Test -T flag"
cmp result.txt expected.txt
check_result

echo -e "\n-------------\n"

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