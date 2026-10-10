
# OS Skill-22: Memory Debugging and Automated Testing

## Objective
To detect and fix memory leaks using Valgrind, release allocated
resources, verify cleanup, automate test execution, compare outputs,
and generate test reports.

## Tools Used
- Ubuntu Linux
- GCC compiler
- C programming language
- Valgrind
- Bash scripting

## Programs
- memory_bug.c: Demonstrates an intentional memory leak.
- memory_fixed.c: Releases allocated memory using free().
- test_script.sh: Automates output and memory checks.

## Test Cases

| Test Case | Expected Result |
|---|---|
| Program output verification | PASS |
| Memory error verification | PASS |
| Memory cleanup verification | PASS |

## Findings
Valgrind identified a memory leak in the initial program.
The issue was fixed by releasing allocated memory with free().
The corrected program passed the automated output, memory-error,
and cleanup tests.

## Conclusion
Memory debugging and automated testing were performed successfully.
The corrected program completed execution without memory leaks
or reported memory errors.
