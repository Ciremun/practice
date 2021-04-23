#!/usr/bin/env python3

import subprocess
import traceback
import sys
import os

from typing import Callable, Any, Optional, List

for v in 'CC', 'CFLAGS', 'CXX', 'CXXFLAGS':
    setattr(sys.modules[__name__], v, os.environ.get(v))

RC = 'rustc'

DIR = os.listdir('.')
build_functions = {}
compilers_and_flags = {
    CC: CFLAGS,
    CXX: CXXFLAGS
}


def not_msvc(C: str):
    return #TODO


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


def extract_source_extension(src: str) -> Optional[str]:
    ext = ''
    for char in src[::-1]:
        if char == '.':
            return ext[::-1]
        ext += char
    log_error(f'source `{src}` has no extension')
    return None


def build_source(src: str):
    ext = extract_source_extension(src)
    if ext:
        bf = build_functions.get(ext)
        if bf:
            bf(src)
        else:
            log_error(f'`{src}` unknown file extension: `.{ext}`')


def get_compiler_command(C: str) -> List[str]:
    flags = compilers_and_flags[C]
    if flags:
        return [C, *flags.split(' ')]
    else:
        return [C]


@build_function('c')
def build_c_source(src: str, C: str = 'CC'):
    command = get_compiler_command(C)
    command.extend(src)
    filename = src[:-2]
    if sys.platform == 'win32':
        command.extend((f'/Fe{filename}.exe',))
    else:
        command.extend((f'-o{filename}',))
    run_command(command)


@build_function('rs')
def build_rust_source(src: str):
    run_command([RC, src])


@build_function('cpp')
def build_cpp_source(src: str):
    build_c_source(src, 'CXX')


@catch_errors
def run_command(command: str):
    print(' '.join(command))
    subprocess.run(command)


def build_all():
    SOURCES = [s for e in list(build_functions.keys())
               for s in DIR if s.endswith(f'.{e}')]
    for src in SOURCES:
        build_source(src)


def build_all_with_extension(src: str):
    ext = extract_source_extension(src)
    if ext:
        bf = build_functions.get(ext)
        if bf:
            sources_with_extension = [
                s for s in DIR if s.endswith(f'.{ext}')]
            for s in sources_with_extension:
                bf(s)
        else:
            log_error(f'`{src}` unknown file extension: `.{ext}`')


def set_cc_and_cxx():
    if sys.platform == 'win32':
        if (CC or CXX) and all(c != CC or c != CXX for c in ('cl', 'cl.exe')):
            sys.platform = 'linux'
        else:
            CC = 'cl.exe'
    else:
        if not CC:
            CC = 'gcc'
        if not CXX:
            CXX = 'g++'


if __name__ == '__main__':
    set_cc_and_cxx()
    if len(sys.argv) > 1:
        for src in sys.argv[1:]:
            if src.startswith('*.'):
                build_all_with_extension(src)
            else:
                build_source(src)
    else:
        build_all()
