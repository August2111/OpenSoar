#!/usr/bin/env python3

import os, sys
from CMakeXCSoar import create_xcsoar
class ComputerDirectories(object):
    def __init__(self, name, directories):
        self.name = name
        self.project_dir = directories["project_dir"]
        self.third_party_dir = directories["third_party_dir"]
        self.binary_dir = directories["binary_dir"]
        self.program_dir = directories["program_dir"]
        self.link_libs = directories["link_libs"]


if sys.platform.startswith('win'):
    toolchain = 'mgw73'
    # toolchain = 'ninja'
    # toolchain = 'clang10'
    toolchain = 'msvc2019'
else:
    toolchain = 'unix'

arguments = []
arguments.append('XCSoarAug')   # project_name
arguments.append('master')      # branch
arguments.append(toolchain)     # build-toolchain
# arguments.append('Arg2')
# arguments.append('Arg3')
# arguments.append('Arg4')

print('Jetzt gehts los: ', arguments)
create_xcsoar(arguments)


## batch: set SOURCE_DIR=%CD%
## batch: set BINARY_DIR=D:\Projects\Binaries
## batch: set TARGET_PLATFORM=64
## batch: set THIRD_PARTY=D:/Projects/3rd_Party
## batch: set COMPILER=VS2019
## batch: set PROGRAM_DIR=D:\Programs
## batch: set Boost_ROOT=%THIRD_PARTY%\boost\boost_1_72_0\vc142
## batch: set QT_ROOT=%THIRD_PARTY%/qt
## batch: call CMakeXCSoar.cmd
