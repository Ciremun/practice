#!/usr/bin/env python3

import subprocess
import sys
import os

SOURCES = os.listdir('.')
CSOURCES = [f for f in SOURCES if f.endswith('.c')]
CC = None

if len(sys.argv) > 1:
    CC = sys.argv[1]

if sys.platform == 'win32':
    if CC is not None and ('cl' not in CC or CC == 'clang'):
        sys.platform = 'linux'
    else:
        CC = 'cl.exe'
else:
    CC = 'gcc'

for src in CSOURCES:
    filename = src[:-2]
    if sys.platform == 'win32':
        command = [CC, src, f'/Fe{filename}.exe']
    else:
        command = [CC, src, f'-o{filename}']

    print(' '.join(command))
    subprocess.run(command)
