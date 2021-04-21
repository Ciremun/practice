#!/usr/bin/env python3

import subprocess
import sys
import os

# TODO(#1): build from args

def run_command(command: str):
    print(' '.join(command))
    subprocess.run(command)

SOURCES = os.listdir('.')

CSOURCES = [f for f in SOURCES if f.endswith('.c')]
CC = os.environ.get('CC')
CFLAGS = os.environ.get('CFLAGS')

RSOURCES = [f for f in SOURCES if f.endswith('.rs')]
RC = 'rustc'

if len(sys.argv) > 1:
    CC = sys.argv[1]

if sys.platform == 'win32':
    if CC and all(c != CC for c in ('cl', 'cl.exe')):
        sys.platform = 'linux'
    else:
        CC = 'cl.exe'
elif not CC:
    CC = 'gcc'

for src in CSOURCES:
    if CFLAGS:
        command = [CC, *CFLAGS.split(' '), src]
    else:
        command = [CC, src]
    filename = src[:-2]
    if sys.platform == 'win32':
        command.extend((f'/Fe{filename}.exe',))
    else:
        command.extend((f'-o{filename}',))
    run_command(command)

for src in RSOURCES:
    run_command([RC, src])
