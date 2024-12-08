#!/usr/bin/env bash

# Check if in repo root 
if [ ! -f CMakeLists.txt ]; then
    echo "[ERROR]: ./CMakeLists.txt not found (run from of repo's root)"
    exit 1
fi

# Select build type
BIN_PATH=""
if [ ${1:0:1} == "d" ]; then
    BIN_PATH="Debug/bin"
    cmake -B ./Debug -S. && cmake --build Debug
elif [ ${1:0:1} == "p" ]; then
    # Build with profiling enabled
    BIN_PATH="Profile/bin"
    cmake -DCMAKE_C_FLAGS=-pg -B./Profile -S. && cmake --build ./Profile
else
    echo "[ERROR]: '$1' Unsupported build type"
    exit 1
fi

echo $BIN_PATH

# Run with specified ROM to generate gmon.out
if [ -f "$2" ]; then
    "./$BIN_PATH/CHIP-8" "$2"
else
    echo "[ERROR]: '$2' inalid path to ROM"
    exit 1
fi

# If "profile" output profiler graph to png
if [ ${1:0:1} == "p" ]; then
    if [ -f gmon.out ]; then
        gprof "$BIN_PATH/CHIP-8" gmon.out | gprof2dot | dot -Tpng -Gdpi=100 -o prof.png
    else
        echo "[ERROR]: gmon.out missing"
        exit 1
    fi
fi
