
#!/bin/bash

set -eu

echo "=================================="
echo " OS SKILL 22 - AUTOMATED TESTS"
echo "=================================="

gcc -Wall -Wextra -g memory_fixed.c -o memory_fixed

EXPECTED="OS Skill 22: Memory Debugging
Memory released successfully"

ACTUAL=$(./memory_fixed)

echo "$ACTUAL"

if [ "$ACTUAL" = "$EXPECTED" ]; then
    echo "TEST 1: Program output - PASS"
else
    echo "TEST 1: Program output - FAIL"
    exit 1
fi

echo "Running Valgrind memory test..."

if valgrind --leak-check=full --error-exitcode=1 \
    ./memory_fixed > valgrind_report.txt 2>&1; then

    if grep -q "ERROR SUMMARY: 0 errors" valgrind_report.txt; then
        echo "TEST 2: Memory errors - PASS"
    else
        echo "TEST 2: Memory errors - FAIL"
        exit 1
    fi
else
    echo "TEST 2: Memory errors - FAIL"
    exit 1
fi

if grep -q "in use at exit: 0 bytes in 0 blocks" \
    valgrind_report.txt; then
    echo "TEST 3: Memory cleanup - PASS"
else
    echo "TEST 3: Memory cleanup - FAIL"
    exit 1
fi

{
    echo "OS SKILL 22 - TEST REPORT"
    echo "-------------------------"
    echo "Test 1: Program output - PASS"
    echo "Test 2: Memory errors - PASS"
    echo "Test 3: Memory cleanup - PASS"
    echo "Overall Result: ALL TESTS PASSED"
} > test_report.txt

echo "=================================="
echo "ALL TESTS PASSED"
echo "Report saved to test_report.txt"
echo "Valgrind details saved to valgrind_report.txt"

