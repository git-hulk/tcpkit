#!/bin/bash
CHECK_TYPES="warning,performance,portability,information,missingInclude"
STANDARD=c89
ERROR_EXITCODE=1
LANG=c
FILES=$(ls *.h *.c|grep -v cJSON|awk '{printf $0" "}')
cppcheck --enable=${CHECK_TYPES} -U__GNUC__ -x ${LANG} ${FILES} --std=${STANDARD} --error-exitcode=${ERROR_EXITCODE}
