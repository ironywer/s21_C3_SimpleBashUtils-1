#!/bin/bash

GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m' # No Color


flag=( 'e'
  'v'
  'v'
  'c'
  'l'
  'n'
  'h'
  's'
  'o'
)
ALL=0
COMPLETE=0
#Одиночные флаги
function check_result {
  if [ $? -eq 0 ]; then
    echo -e "${GREEN}Test passed${NC}\n"
    ALL=$((ALL+1))
    COMPLETE=$((COMPLETE+1))
  else
    echo -e "${RED}Test failed${NC}\n"
    echo "result.txt"
    cat result.txt 
    echo "expected.txt"
    ALL=$((ALL+1))
    cat expected.txt
  fi
}

# leaks -atExit --  
# valgrind --tool=memcheck --leak-check=yes --track-origins=yes -q

for i in ${flag[*]}; do
  echo -e "Test -${i} flag" 
  ./s21_grep -$i "pattern" file1.txt file2.txt > result.txt
  grep -$i "pattern" file1.txt file2.txt > expected.txt
  cmp result.txt expected.txt
  check_result
done 

echo -e "Test -f flag"
./s21_grep -f pattern.txt file1.txt file2.txt > result.txt
grep -Ef pattern.txt file1.txt file2.txt > expected.txt
cmp result.txt expected.txt
check_result

# Парные комбинации флагов
COUNTER1=$(( 0 ))
for i in ${flag[*]}; do
  COUNTER1=$(( $COUNTER1 + 1 ))
  COUNTER2=$(( 0 ))
  for j in ${flag[*]}; do
    COUNTER2=$(( $COUNTER2 + 1 ))
    # if [ $COUNTER2 -gt $COUNTER1 ]; then
    echo -e "Test -${i}${j} flags"
    ./s21_grep -$i$j "pattern" file1.txt file2.txt > result.txt
    grep -$i$j "pattern" file1.txt file2.txt > expected.txt
    cmp result.txt expected.txt
    check_result
    # fi
  done
done
# echo -e "Test -iv flags"
# ./s21_grep -iv pattern file1.txt file2.txt > result.txt
# grep -iv "pattern" file1.txt file2.txt > expected.txt
# cmp result.txt expected.txt
# check_result

# echo -e "Test -in flags"
# ./s21_grep -in pattern file1.txt file2.txt > result.txt
# grep -in "pattern" file1.txt file2.txt > expected.txt
# cmp result.txt expected.txt
# check_result

# echo -e "Test -ls flags"
# ./s21_grep -ls pattern file1.txt file2.txt NOTEXIST.txt > result.txt
# grep -ls "pattern" file1.txt file2.txt NOTEXIST.txt > expected.txt
# cmp result.txt expected.txt
# check_result

# echo -e "Test -ch flags"
# ./s21_grep -ch pattern file1.txt file2.txt > result.txt
# grep -ch "pattern" file1.txt file2.txt > expected.txt
# cmp result.txt expected.txt
# check_result

# echo -e "Test -cv flags"
# ./s21_grep -cv pattern file1.txt file2.txt > result.txt 
# grep -cv "pattern" file1.txt file2.txt > expected.txt
# cmp result.txt expected.txt
# check_result

# echo -e "Test -lv flags"
# ./s21_grep -lv pattern file1.txt file2.txt > result.txt
# grep -lv "pattern" file1.txt file2.txt > expected.txt
# cmp result.txt expected.txt
# check_result

# echo -e "Test -ho flags"
# ./s21_grep -ho pattern file1.txt file2.txt > result.txt
# grep -ho "pattern" file1.txt file2.txt > expected.txt
# cmp result.txt expected.txt
# check_result

for i in ${flag[*]}; do
  echo -e "Test -f${i} flags"
  ./s21_grep -f$i "pattern" file1.txt file2.txt > result.txt
  grep -f$i "pattern" file1.txt file2.txt > expected.txt
  cmp result.txt expected.txt
  check_result
done

# echo -e "Test -nf flags"
# ./s21_grep -nf pattern.txt file2.txt > result.txt
# grep -nf pattern.txt file2.txt > expected.txt
# cmp result.txt expected.txt
# check_result

echo -e "Total tests passed: ${COMPLETE}/${ALL}"

rm result.txt expected.txt


clang-format -n *.c

cppcheck *.c q