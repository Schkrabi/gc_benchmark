#!/usr/bin/env Rscript
args = commandArgs(trailingOnly=TRUE)

if (length(args)==0) {
  stop("At least one argument must be supplied (input file).n", call.=FALSE)
}

data = read.csv(args[1], header=TRUE)

#Compute diffs
data[,"NSEC_DIFF"] <- (1e9 * data[,"SEC_START"] + data[,"NSEC_END"]) - (1e9 * data[,"SEC_START"] + data[,"NSEC_START"])
data[,"CLOCK_DIFF"] <- data[,"CLOCK_END"] - data[,"CLOCK_START"]

nmean = mean(data[,"NSEC_DIFF"])
nmedian = median(data[,"NSEC_DIFF"])
cmean = mean(data[,"CLOCK_DIFF"])
cmedian = median(data[,"CLOCK_DIFF"])

write(c(nmean, nmedian, cmean, cmedian), args[2]);
