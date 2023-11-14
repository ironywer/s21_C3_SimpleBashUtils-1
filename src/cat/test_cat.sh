#!/bin/bash

GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m' # No Color
ALL=$((0))
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
  ALL=$((ALL+1))
  if [ $? -eq 0 ]; then
    echo -e "${GREEN}Test passed${NC}"
    COUNT=$((COUNT+1))
  else
    echo -e "${RED}Test failed${NC}"
  fi
}

flag=(
  'b'
  'e'
  'n'
  's'
  't'
)

for i in ${flag[*]}; do
  ./s21_cat -${i} ${INPUTFILE} > result.txt
  cat -${i} ${INPUTFILE} > expected.txt
  echo -e "Test -${i} flag"
  cmp result.txt expected.txt
  check_result

  echo -e "\n-------------\n"
done 
# leaks -atExit --  
# valgrind --tool=memcheck --leak-check=yes --track-origins=yes -q

COUNTER1=$((0))
for i in ${flag[*]}; do
  COUNTER1=$(( $COUNTER1 + 1))
  COUNTER2=$((0))
  for j in ${flag[*]}; do
    COUNTER2=$(( $COUNTER2 + 1))
    COUNTER3=$((0))
    # if [ $COUNTER2 -gt $COUNTER1 ]; then
      for k in ${flag[*]}; do
        COUNTER3=$(( $COUNTER3 + 1))
        # if [ $COUNTER3 -gt $COUNTER2 ]; then
          ./s21_cat -$i$j$k ${INPUTFILE} > result.txt
          cat -$i$j$k  ${INPUTFILE} > expected.txt
          echo -e "Test -${i}${j}${k}"
          cmp result.txt expected.txt
          check_result
        # fi
      done 
    # fi
  done 
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

echo -e "Total tests passed: ${COUNT}/${ALL}"

clang-format -n *.c

cppcheck *.c