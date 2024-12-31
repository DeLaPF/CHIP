#!/usr/bin/env bash

BIN_NAME="CHIP-8"

# Check if in repo root
if [ ! -f CMakeLists.txt ]; then
    echo "[ERROR]: ./CMakeLists.txt not found (run from of repo's root)"
    exit 1
fi

# Parse flags
build_type="default"
run=false
while getopts 'b:r' flag; do
    case "${flag}" in
        b) build_type="${OPTARG}" ;;
        r) run=true ;;
        *) print_usage
           exit 1 ;;
    esac
done

# Select build type
BIN_PATH=""
if [ ${build_type:0:1} == "d" ]; then
    BIN_PATH="builds/linux/default/bin"
    cmake -B ./builds/linux/default -S. && \
        cmake --build builds/linux/default
elif [ ${build_type:0:1} == "p" ]; then
    # Profiling enabled
    BIN_PATH="builds/linux/profile/bin"
    cmake -DCMAKE_C_FLAGS=-pg -B./builds/linux/profile -S. && \
        cmake --build ./builds/linux/profile
else
    echo "[ERROR]: '$1' Unsupported build type"
    exit 1
fi

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
