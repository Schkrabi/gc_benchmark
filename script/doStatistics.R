#!/usr/bin/env Rscript
args = commandArgs(trailingOnly=TRUE)

if (length(args)==0) {
  stop("At least one argument must be supplied (input file).n", call.=FALSE)
}

data = read.csv(args[1], header=TRUE)

unitDiff <- function (x,y) {
                    d <- x - y
                    if (d < 0) {
                        2^32 + x - y
                    }
                    else {
                        d
                    }
                }

#Compute diffs
data[,"UNIT_DIFF"] <- mapply(unitDiff, data[,"UNIT_END"], data[,"UNIT_START"])
# data[,"NSEC_DIFF"] <- (1e9 * data[,"SEC_START"] + data[,"NSEC_END"]) - (1e9 * data[,"SEC_START"] + data[,"NSEC_START"])
data[,"CLOCK_DIFF"] <- data[,"CLOCK_END"] - data[,"CLOCK_START"]

# nmean = mean(data[,"NSEC_DIFF"])
# nmedian = median(data[,"NSEC_DIFF"])
umean = mean(data[,"UNIT_DIFF"])
umedian = median(data[,"UNIT_DIFF"])
cmean = mean(data[,"CLOCK_DIFF"])
cmedian = median(data[,"CLOCK_DIFF"])
var = min(data[, "VAR"])

write(c(umean, umedian, cmean, cmedian, var), args[2]);
