#!/bin/sh

# Output path (relative to build sh file)
OutputPath="./build/arm32"

cd $OutputPath
gdb ./keep_them_alive
