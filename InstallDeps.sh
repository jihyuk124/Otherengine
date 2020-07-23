#!/bin/bash
set -e
work_dir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
tools_dir=$work_dir/Tools

# cmake
if ! command -v cmake &> /dev/null; then
    cmake_install=true
    echo "cmake not found"
else
    cmake_ver="$(cmake --version | head -n1 | cut -d" " -f3)"
    cmake_reqver=3.14
    if [ "$(printf '%s\n' "$cmake_reqver" "$cmake_ver" | sort -V | head -n1)" = "$cmake_reqver" ]; then
        echo "cmake $cmake_ver found, suitable for required version $cmake_reqver"
    else
        cmake_install=true
        echo "cmake $cmake_ver found, but not compatible with required version $cmake_reqver"
    fi
fi
if [ "$cmake_install" = true ]; then
    if [ -f "$tools_dir/cmake-install/bin/cmake" ]; then
        echo "Downloaded cmake found"
    else
        mkdir -p $tools_dir && cd $tools_dir
        wget -nc https://github.com/Kitware/CMake/releases/download/v3.18.0/cmake-3.18.0-Linux-x86_64.tar.gz
        tar -xvf cmake-3.18.0-Linux-x86_64.tar.gz > /dev/null
        mv cmake-3.18.0-Linux-x86_64 cmake-install
    fi
    PATH=$tools_dir/cmake-install/bin:$PATH
fi

# sdl2 glew
sudo apt install libsdl2-dev libglew-dev