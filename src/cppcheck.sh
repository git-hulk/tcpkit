#!/bin/bash
CHECK_TYPES="warning,performance,portability,information,missingInclude"
STANDARD=c89
ERROR_EXITCODE=1
LANG=c
# Not having the system headers is expected and is not a finding, but with
# `information` enabled it is still reported and trips --error-exitcode.
SUPPRESS="--suppress=missingIncludeSystem --suppress=checkersReport"
FILES=$(ls *.h *.c|grep -v cJSON|awk '{printf $0" "}')
cppcheck --enable=${CHECK_TYPES} ${SUPPRESS} -U__GNUC__ -x ${LANG} ${FILES} --std=${STANDARD} --error-exitcode=${ERROR_EXITCODE}
