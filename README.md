# Log analyzer program

A lightweigt and fast HTTP log analyzer, written in C.<br />
It parses logs in the following format: `IP - - [TIMESTAMP TIME-ZONE] "METHOD URL HTTP-VERSION" STATUT OCTETS`(the Apache/Ngix format),
validates their structure and data, and generates detailed traffic statistics.

## Features

- Accept a log file as input
- Robust parsing (strict validation of IPs, timestamps, methods, etc.)
- Resilient to corrupted or excessively long lines (no crashes).
- Statistics by HTTP status code (2xx, 3xx, etc.).
- Ranking of the most active IPs (Top N).
- Terminal-based interface.
- Report the result in a verbose way

## Project structure

```text
log_analizer/
├── docs
│   └── parser.md
├── log.txt
├── Makefile
├── README.md
├── src
│   ├── gettoken.c
│   ├── main.c
│   ├── parser.c
│   ├── parser.h
│   ├── reader.c
│   ├── report.c
│   ├── statistics.c
│   ├── statistics.h
│   ├── validator.c
│   └── validator.h
├── tests
│   ├── generate_log.py
│   ├── test_log.txt
│   ├── test_parser.c
│   ├── test_statistics.c
│   └── test_validator.c
└── todo.md
```

## How to use

Clone the repository first:

```text
git clone git@github.com:mangnantikouadio2-wq/Log_analizer.git
cd Log_analizer
```

Then compile the program: `make` and use it with the following syntax `./loganalyzer <file> [--verbose] [--stats] [--top-ips N]`.
Replace `<file>` by your log file name an choose the report option(s) you want.

To do tests, you can run the command `make test1 && make test2` or whatever test you want to do.

## Future improvements

### Basic statistics

- Count requests by HTTP method
- Count requests by IP address

### Time Analysis

- Parse timestamp into a comparable form
- Count request per time bucket
- Other that are not decided yet

## Roadmap

See todo.md

<em><strong>For more information, you can visit the docs repertory</strong></em>
