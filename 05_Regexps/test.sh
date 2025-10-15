#!/bin/bash

GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m'

test_count=0
passed_count=0

run_test() {
    local input="$1"
    local pattern="$2"
    local replacement="$3"
    local description="$4"
    
    test_count=$((test_count + 1))
    
    esub_output=$(./esub "$input" "$pattern" "$replacement" 2>/dev/null)
    esub_exit=$?
    
    sed_output=$(echo "$input" | sed -E "s/$pattern/$replacement/" 2>/dev/null)
    sed_exit=$?
    
    if [ "$esub_output" = "$sed_output" ] && [ $esub_exit -eq $sed_exit ]; then
        echo -e "${GREEN}✓${NC} Test $test_count passed: $description"
        passed_count=$((passed_count + 1))
    else
        echo -e "${RED}✗${NC} Test $test_count failed: $description"
        echo "  Input: '$input'"
        echo "  Pattern: '$pattern'"
        echo "  Replacement: '$replacement'"
        echo "  Expected (sed): '$sed_output'"
        echo "  Got (esub): '$esub_output'"
    fi
}

run_test "hello world" "world" "universe" "Simple replacement"
run_test "123-456-789" "[0-9]+" "X" "Replace first number group"
run_test "abc123def456" "([a-z]+)([0-9]+)" "\\2-\\1" "Backreferences"
run_test "test@example.com" "(.*)@(.*)" "\\1 at \\2" "Email parsing"
run_test "foo bar baz" "\\b\\w{3}\\b" "XXX" "Word boundary"
run_test "aaabbbccc" "(.)\\1+" "\\1" "Repeated characters"
run_test "  spaces  " "^\\s+" "" "Leading spaces"
run_test "  spaces  " "\\s+$" "" "Trailing spaces"
run_test "CamelCase" "([A-Z])" "_\\1" "CamelCase - first capital"
run_test "a1b2c3" "([a-z])([0-9])" "\\2\\1" "Swap first letter and digit"

run_test "test test test" "test" "OK" "Multiple occurrences - replace first"
run_test "no match here" "xyz" "replaced" "No match"
run_test "" ".*" "empty" "Empty string"
run_test "abc" "(.*)" "[\\1]" "Whole string capture"

echo ""
echo "================================"
echo "Tests passed: $passed_count/$test_count"
if [ $passed_count -eq $test_count ]; then
    echo -e "${GREEN}All tests passed!${NC}"
    exit 0
else
    echo -e "${RED}Some tests failed!${NC}"
    exit 1
fi
