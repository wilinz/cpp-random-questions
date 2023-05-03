#!/bin/bash

odb -o generated/ --std c++14 -d mysql --generate-query --generate-schema --show-sloc models/*.hpp