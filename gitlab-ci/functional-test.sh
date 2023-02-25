#!/bin/bash
set -e
echo "Functional Test"
pwd

supervisorctl status all
supervisorctl update
sleep 3
supervisorctl status all

make functional-test -C ./

supervisorctl stop all
supervisorctl status

# report part
# GitLab-CI output
gcovr -r .
# Artefact
gcovr -r . --xml -o ./functional-test-coverage.xml
date +%s > timestamp.log
