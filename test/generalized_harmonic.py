#!/usr/bin/env python3
import argparse


def parse_args():
    parser = argparse.ArgumentParser(description="Plot distributions")
    parser.add_argument("-n", type=int, required=True,
                        help="number of elements")
    parser.add_argument("-m", type=float, required=True,
                        help="exponent")

    return parser.parse_args()


def main():
    args = parse_args()

    n = args.n
    m = args.m

    sum = 0
    for x in range(1, n+1):
        sum += 1/(x**m)

    print("H_{{{},{}}} = {}".format(n, m, sum))


if __name__ == "__main__":
    main()