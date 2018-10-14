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

#Remove 10% of extreme upper values from dataset
u = data[data[,"UNIT_DIFF"] < quantile(data[,"UNIT_DIFF"], 0.90), "UNIT_DIFF"]
c = data[data[,"CLOCK_DIFF"] < quantile(data[,"CLOCK_DIFF"], 0.90), "CLOCK_DIFF"]

umean = mean(u)
umedian = median(u)
cmean = mean(c)
cmedian = median(c)
var = min(data[, "VAR"])

write(c(umean, umedian, cmean, cmedian, var), args[2]);
