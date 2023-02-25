#!/bin/bash

echo "Functional API Tests"
pwd

source ./gitlab-ci/shared.sh

make coverage -C ./
supervisorctl restart all
supervisorctl update
ubus wait_for ethernet
supervisorctl status all

# run API validation
ubus-api-validator -d ./test/api/json/ > ./api-result.log
check_ret $?
tap-junit --input ./api-result.log --output report

supervisorctl status all
supervisorctl stop all
supervisorctl status

# report part
date +%s > timestamp.log
gcovr -r . --xml -o ./api-test-coverage.xml
gcovr -r .

echo "Checking memory leaks..."
grep -q "Leak" memory-report.xml
error_on_zero $?

echo "Functional ubus API test :: PASS"

