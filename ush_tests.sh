#!/bin/bash

SUCCESS_COUNT=0
FAILED_COUNT=0
RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m' # No color
YELLOW='\033[33;1m'

runner="/bin/bash"
if [ $1 ]; then
    if [ $1 = "-z" ]; then
        runner="/bin/zsh"
    elif [ $1 = "-u" ]; then
        runner="../Ucode-ush/ush"
    fi
fi

run_test() {
    command="$1"
    echo -e "Testing command: $command"
    echo $command | $runner &> temp_file1
    echo $command | ./ush &> temp_file2
    DIFF=$(diff temp_file1 temp_file2)
    if ["$DIFF" != ""] 
    then
        ((SUCCESS_COUNT++))
    else
        # echo -e $DIFF
        ((FAILED_COUNT++)) 
    fi
}

echo -e "Running tests with $runner"
run_test "pwd";
# run_test "ls -la src";
run_test "pwd; cd ..; pwd"
run_test "pwd; cd ..;cd ushe;"
run_test "export"

rm temp_file1 temp_file2
echo -e "Tests completed. ${GREEN}${SUCCESS_COUNT} successful tests.${NC} ${RED}${FAILED_COUNT} failed tests.${NC}"

