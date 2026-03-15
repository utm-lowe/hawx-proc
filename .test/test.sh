#!/bin/bash
expected=4

.test/run-hawx > .test/hawx.out
#passed=$(grep PASSED .test/hawx.out | wc -l)

cat .test/hawx.out

#echo
#echo
#echo "Passed $passed out of $expected tests."

#if [ $passed -ne $expected ]; then exit 1; fi
