library(dplyr)
library(tidyr)
library(stringr)

trace2csv <- function(trace_path, csv_path) {
  result <- data.frame(
    matlab = factor(),
    threads = factor(),
    process = factor(),
    benchmark = factor(),
    version = factor(),
    N = factor(),
    in_process = factor(),
    time = numeric(),
    metrics = factor(),
    value = numeric()
  )
  
  trace_file  <- file(trace_path, open = "r")
  while (length(str_line <-
                readLines(trace_file, n = 1, warn = FALSE)) > 0) {
    if (str_starts(str_line, "@perf_events")) {
      perf_events <- str_split(str_line, ":")[[1]][2]
      header <- str_split(perf_events, ",")[[1]]
      df <-
        setNames(data.frame(matrix(
          ncol = length(header), nrow = 0
        )), header)
    }
    else if (str_starts(str_line, "@trace")) {
      if (exists("df", inherits = FALSE) && nrow(df) > 0) {
        df2 <- df %>%
          group_by(matlab,
                   threads,
                   process,
                   benchmark,
                   version,
                   N,
                   in_process) %>%
          mutate_at(function(x) {
            x = x - lag(x, default = 0)
          },
          .vars = (ncol(df) - length(header) + 2):ncol(df)) %>%
          gather("metrics", "value", (ncol(df) - length(header) + 2):ncol(df)) %>%
          rename(time = ncol(df) - length(header) + 1)
        result <- bind_rows(result, df2)
      }
      # Read the subtrace info
      subtrace <- str_split(str_line, ":")[[1]]
    } else {
      # Read the data
      values <- as.numeric(str_split(str_line, ",")[[1]])
      row <-
        setNames(data.frame(matrix(
          data = values, ncol = length(header)
        )), header)
      row <- cbind(
        matlab = subtrace[2],
        threads = subtrace[3],
        process = subtrace[4],
        benchmark = subtrace[5],
        version = subtrace[6],
        N = subtrace[7],
        in_process = subtrace[8],
        row
      )
      df <- rbind(df, row)
    }
  }
  close(trace_file)
  write.csv(result, csv_path, quote = F, row.names = F)
}
