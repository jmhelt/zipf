#!/usr/bin/env python3
import argparse
import os
import pathlib
import shlex
import shutil
import subprocess

NUM_SAMPLES = 100000


def parse_args():
    parser = argparse.ArgumentParser(description="Plot distributions")
    parser.add_argument("-b", "--builddir", type=pathlib.Path, required=True,
                        help="path to build directory")

    return parser.parse_args()


def get_out_fname(dir_path, generator, num_elements, skew):
    fname = "{}_{}_{}.csv".format(generator, num_elements, skew)

    return os.path.join(dir_path, fname)


def get_gp_fname(dir_path, num_elements):
    return os.path.join(dir_path, "plot-{}.gp".format(num_elements))


def gen_gp_file(dir_path, generators, skews, num_elementss):
    template = """
set terminal png

set logscale x

set xlabel "Log(rank)"
set ylabel "CDF"

set output "cdf-{0}.png"

set title "CDF for {0} Keys"
set key below

plot \\
"""

    for num_elements in num_elementss:
        contents = template.format(num_elements)
        for generator in generators:
            for skew in skews:
                out_fname = get_out_fname(dir_path, generator,
                                          num_elements, skew)

                contents += '  "{}" using 1:2 smooth cumulative title "{} s={}" with linespoint, \\\n' \
                    .format(out_fname, generator, skew)

        contents = contents[0:len(contents)-4]

        # print(contents)
        fname = get_gp_fname(dir_path, num_elements)
        with open(fname, "w") as f:
            f.write(contents)


def main():
    args = parse_args()

    test_bin_path = os.path.join(args.builddir, "bin", "test")
    test_bin_path = os.path.abspath(test_bin_path)

    test_dir_path = os.path.dirname(os.path.realpath(__file__))
    test_out_path = os.path.join(test_dir_path, "out")
    shutil.rmtree(test_out_path, ignore_errors=True)
    os.makedirs(test_out_path)

    num_elementss = [2, 10]
    generators = ["ycsb", "rejinv"]
    skews = [0.1, 0.5, 0.99, 2.0, 3.0, 4.0]

    for num_elements in num_elementss:
        for generator in generators:
            for skew in skews:
                out_fname = get_out_fname(test_out_path, generator,
                                          num_elements, skew)

                cmd = "{} -g {} -e {} -s {} -n {}".format(test_bin_path,
                                                          generator, num_elements,
                                                          skew, NUM_SAMPLES)

                print("{} > {}".format(cmd, out_fname))
                with open(out_fname, "w") as f:
                    subprocess.call(shlex.split(cmd), stdout=f)

    gen_gp_file(test_out_path, generators, skews, num_elementss)

    for num_elements in num_elementss:
        cmd = "gnuplot {}".format(get_gp_fname(test_out_path, num_elements))
        print(cmd)
        subprocess.call(shlex.split(cmd), cwd=test_out_path)


if __name__ == "__main__":
    main()
