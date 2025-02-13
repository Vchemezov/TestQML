import argparse
import pathlib
import platform
import os
import subprocess
import sys

BUILD_TYPE = 'Release' if 1 else 'Debug'
HOME_DIR = pathlib.Path(os.environ.get('CI_PROJECT_DIR', '.')).absolute()
BUILD_DIR = (HOME_DIR / 'cmake_build').absolute()
BUILD_DIR.mkdir(exist_ok=True)

OPTIONS = f'-DCMAKE_BUILD_TYPE={BUILD_TYPE}'

CONFIGS = {
    "win": [
        f'cmake {OPTIONS} -G"Visual Studio 16 2019" -H../testQML -B.',
        f'cmake --build . --config {BUILD_TYPE}'
    ],
    
    "lin": [
        f'cmake3 {OPTIONS} -G"Unix Makefiles" -H../testQML -B.',
        f'cmake3 --build . --config {BUILD_TYPE}  -- -j9'
    ],
}

args = argparse.ArgumentParser()
args.add_argument('--platform', type=str, required=True)
args = args.parse_args()

for x in CONFIGS[args.platform]:
    env = os.environ.copy()

    print("RUN: " + x)
    with subprocess.Popen(x, shell=True,
                          stderr=sys.stderr,
                          stdout=sys.stdout,
                          cwd=BUILD_DIR,
                          env=env) as p:
        while True:
            code = p.poll()
            if code is not None:
                if code != 0:
                    print(f'\n\nStatus code: {code}\n\n')
                    exit(code)
                break
