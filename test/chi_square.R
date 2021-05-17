#!/usr/bin/env Rscript
library("argparse")
library("readr")

parser <- ArgumentParser()

parser$add_argument("-e", "--num_elements", type = "integer", help = "number of elements", required = TRUE)
parser$add_argument("-n", "--num_samples", type = "integer", help = "number of samples", required = TRUE)
parser$add_argument("-s", "--skew", type = "double", help = "skew", required = TRUE)
parser$add_argument("-i1", "--in1", type = "character", help = "csv file with histogram 1", required = TRUE)
parser$add_argument("-i2", "--in2", type = "character", help = "csv file with histogram 2", required = TRUE)

args <- parser$parse_args()

e <- args$num_elements
n <- args$num_samples
s <- args$skew

in1 <- read_delim(args$in1, " ", col_names = c("x", "count"))
in2 <- read_delim(args$in2, " ", col_names = c("x", "count"))

x <- (1:e)^(-s)
p <- x / sum(x)

print("np")
n*p

print("in1")
in1$count

print("in2")
in2$count

print(chisq.test(x = in1$count, p = p, correct = FALSE))
print(chisq.test(x = in2$count, p = p, correct = FALSE))