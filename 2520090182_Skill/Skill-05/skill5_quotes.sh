#!/bin/bash

echo "===== SKILL 5: SHELL QUOTING ====="

name="Bhardwaj"
message="Operating System Practical"

echo
echo "--- Single Quotes ---"
echo '$name'
echo '$message'
echo 'This is a single quoted string with $name'

echo
echo "--- Double Quotes ---"
echo "$name"
echo "$message"
echo "Hello $name"
echo "The topic is: $message"

echo
echo "--- Preserving Spaces ---"
text="Linux Shell Programming"
echo "$text"

echo
echo "--- Quoted Command ---"
command="echo Hello from Linux"
echo "Command stored as: $command"
eval "$command"

echo
echo "--- Nested Tokens ---"
value="Bhardwaj OS"
echo "Value: $value"

echo
echo "===== END OF SKILL 5 ====="
