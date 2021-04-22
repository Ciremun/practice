#!/usr/bin/env python3

import subprocess
import traceback
import sys
import os

from typing import Callable, Any

# TODO(#1): build from args
CC = os.environ.get('CC')
CFLAGS = os.environ.get('CFLAGS')
RC = 'rustc'

build_functions = {}

def build_function(ext: str) -> Callable:
    def decorator(func: Callable) -> Callable:
        def wrapper(*args, **kwargs) -> Any:
            return func(*args, **kwargs)
        wrapper.__name__ = func.__name__
        build_functions[ext] = wrapper
        return wrapper
    return decorator

def catch_errors(func: Callable) -> Callable:
    def wrapper(*args, **kwargs) -> Any:
        try:
            return func(*args, **kwargs)
        except Exception:
            log_error(f'function {func.__name__}({args}, {kwargs}) failed')
            traceback.print_exc()
    return wrapper

def log_error(err: str):
    print(f'ERROR: {err}')

def build_source(src: str):
    ext = ''
    for char in src[::-1]:
        if char == '.':
            ext = ext[::-1]
            bf = build_functions.get(ext)
            if bf is not None:
                bf(src)
            else:
                log_error(f'`{src}` unknown file extension: `.{ext}`')
            break
        ext += char
    else:
        log_error(f'source `{src}` has no extension')

@build_function('c')
def build_c_source(src: str):
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

@build_function('rs')
def build_rust_source(src: str):
    run_command([RC, src])

@catch_errors
def run_command(command: str):
    print(' '.join(command))
    subprocess.run(command)

def build_all():
    SOURCES = os.listdir('.')
    CSOURCES = [f for f in SOURCES if f.endswith('.c')]
    RSOURCES = [f for f in SOURCES if f.endswith('.rs')]
    for src in CSOURCES:
        build_c_source(src)
    for src in RSOURCES:
        build_rust_source(src)

if __name__ == '__main__':
    if sys.platform == 'win32':
        if CC and all(c != CC for c in ('cl', 'cl.exe')):
            sys.platform = 'linux'
        else:
            CC = 'cl.exe'
    elif not CC:
        CC = 'gcc'
    if len(sys.argv) > 1:
        for src in sys.argv[1:]:
            build_source(src)
    else:
        build_all()

