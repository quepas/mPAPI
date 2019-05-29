library(dplyr)
library(tidyr)
library(stringr)

trace2csv <- function(trace_path, csv_path) {
  trace_file  <- file(trace_path, open = "r")
  # Read the perf event header
  if (length(str_line <- readLines(trace_file, n = 1, warn = FALSE)) > 0) {
    header <- str_split(str_line, ",")[[1]]
    df <- setNames(data.frame(matrix(ncol = length(header), nrow = 0)), header)
  }
  while (length(str_line <- readLines(trace_file, n = 1, warn = FALSE)) > 0) {
    if (str_starts(str_line, "@trace")) {
      # Read the subtrace info
      subtrace <- str_split(str_line, ":")[[1]]
    } else {
      # Read the data
      values <- as.numeric(str_split(str_line, ",")[[1]])
      row <- setNames(data.frame(matrix(data=values, ncol=3)), header)
      row <- cbind(matlab=subtrace[2],
                   threads=subtrace[3],
                   process=subtrace[4],
                   benchmark=subtrace[5],
                   version=subtrace[6],
                   N=subtrace[7],
                   in_process=subtrace[8],
                   row)
      df <- rbind(df, row)
    }
  }
  close(trace_file)
  df <- df %>%
    group_by(matlab,threads,process,benchmark,version,N,in_process) %>%
    mutate_at(
      function(x) {x = x - lag(x, default = 0)},
      .vars=(ncol(df) - length(header) + 2):ncol(df)) %>%
    gather("metrics", "value", (ncol(df) - length(header) + 2):ncol(df)) %>%
    rename(time=ncol(df) - length(header) + 1)
  write.csv(df, csv_path, quote = F, row.names = F)
}

trace2csv("trace.t", "trace.csv")