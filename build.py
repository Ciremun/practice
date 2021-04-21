#!/usr/bin/env python3

import subprocess
import traceback
import sys
import os

# TODO(#1): build from args

def run_command(command: str):
    print(' '.join(command))
    try:
        subprocess.run(command)
    except Exception as e:
        print(f"ERROR: command {command} failed:")
        traceback.print_exc()

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
