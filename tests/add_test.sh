#!/bin/bash

TEST_NAME="$1"
URL="$2"

mkdir "$TEST_NAME"

wget "$URL" -O "$TEST_NAME/index.html"
cp test_template.cc "$TEST_NAME/test.cc"
