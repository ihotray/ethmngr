#!/bin/bash
set -e
echo "preparation script"

pwd

make unit-test -C ./

# report part
# GitLab-CI output
gcovr -r .
# Artefact
gcovr -r . --xml -o ./unit-test-coverage.xml
date +%s > timestamp.log

