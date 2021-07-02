#/bin/bash

find ./ -name ".settings"  -type d | xargs rm -rf
find ./ -name ".cxx"       -type d | xargs rm -rf
find ./ -name ".project"   -type f | xargs rm -rf
find ./ -name ".classpath" -type f | xargs rm -rf

