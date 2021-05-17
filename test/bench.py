#!/usr/bin/env python3
import argparse
import os
import pathlib
import shlex
import subprocess


def parse_args():
    parser = argparse.ArgumentParser(description="Plot distributions")
    parser.add_argument("-b", "--builddir", type=pathlib.Path, required=True,
                        help="path to build directory")

    return parser.parse_args()


def main():
    args = parse_args()

    test_bin_path = os.path.join(args.builddir, "bin", "bench")
    test_bin_path = os.path.abspath(test_bin_path)

    subprocess.call(shlex.split(test_bin_path))


if __name__ == "__main__":
    main()
