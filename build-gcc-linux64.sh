#!/bin/sh

# Output path (relative to build sh file)
OutputPath="./build/linux64"

# Assets path (relative to output folder)
AssetsPath="../../assets"

# Include path (relative to output folder)
IncludePath="../../deps/include"

# Lib path (relative to output folder)
LibPath="../../deps/lib/linux64"

# Compiler flags
# ProfilerFlags="-pg"
# OptimizeFlags="-O2"
# AssemblyFlags="-g -Wa,-ahl"
# DebugFlags="-g -D __DEBUG__"
# Allow the war1 binary to run via clicking in file browser="-no-pie" https://bugzilla.gnome.org/show_bug.cgi?id=737849
CommonCompilerFlags="-std=c99 -Wall -x c $ProfilerFlags $OptimizeFlags $AssemblyFlags $DebugFlags -I $IncludePath"
CommonLinkerFlags="-L $LibPath -lglfw -lpthread -lm -ldl -no-pie"


# Create output path if doesn't exists
mkdir -p $OutputPath
cd $OutputPath

# Empty the build folder
rm -f *

# Compile the project
if gcc $CommonCompilerFlags ../../src/main.c ../../deps/include/glad/glad.c -o keep_them_alive $CommonLinkerFlags; then

# Copy assets

cp $AssetsPath/* ./

# Generate ctags
cd ../../
echo "Generating ctags - Start";
ctags --tag-relative=yes --extras=f --language-force=C --recurse=yes
echo "Generating ctags - Done";
fi
