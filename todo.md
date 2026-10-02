For Project #1, the objective is to build a solid foundation in system architecture, raw data processing, and security, while maintaining a pure implementation without third-party libraries.

## Specifications: Project #1 — CLI Log Analyzer & Anomaly Detector

Objective: Design a command-line interface (CLI) tool capable of parsing large system/server log files, extracting key metrics, and detecting suspicious behavior (brute-force attempts, port scans, malicious requests).

Technical constraints:

- Language of choice: Python (standard library only) or C.

- Zero external dependencies: Only the standard library. No pandas or external packages.

- Memory management: Process large files through a reading stream (streaming/iterators) without loading everything into RAM at once.

Expected features:

    1. Parsing: Clean extraction of each line's elements (IP, timestamp, HTTP status/error code, payload/endpoint).

    2. Statistics: Calculate the most active IPs, error rates (4xx/5xx codes or authentication failures), and time distribution.

    3. Detection engine:
        - Configurable threshold for failed requests/attempts within a time window (e.g., >$N$ failures in $X$ seconds from the same IP).
        - Detection of simple signatures (SQL injection/XSS keywords in requests).
    4. Output: A clear, structured report printed in the terminal (or exported to a custom structured file).

## My strategy

### Supported log format

```text
log_entry: IP separator timestamp request status
IP: number dot number dot number dot number
separator: - space -
timestamp: date colon time
request: method URL
status: number
```

### Data structures I will use

struct request {
char method[10];
char URL[255];
}

struct log_entity {
char IP[16];
char timestamp[32];
struct request rq;
char status[4]
}

### Tasks

I decided to divide the project's responsibilities into multiple responsibilities that
will be handled by:

- main.c: Handle command-line arguments (as I plan to add an option to provide a file to the program and display a single piece of information depending on what the user wants to see), open the file, and proceed to the main processing loop.
- reader.c: Read each line and pass it to the rest of the program. I don't know whether this is a good idea yet, so I am still considering it, but it is a possible addition.
- gettoken.c: Tokenize each line (as K&R did in their book) and handle the method and URL, since they are enclosed in double quotes (I don't know how yet, but I will definitely figure it out. I just need to give my brain a rest and process it again 😅).
- parser.c: Place the tokens in their appropriate fields in the structure.
- header.h: Declare the structures, limits, and other things that might be shared between the different files.
- validator.c: Validate the different tokens so that I can ensure they match the standards. I think this is also a good way to handle errors.
  I could even add the IP danger-level logic here.
- report.c: Handle the report. I don't know whether it will be displayed directly on the TTY or saved to a file, since this was not specified. Why not add a feature that lets the user decide whether they want the output in their terminal or in a file?

I plan to start with the easiest tasks and work toward the more complicated ones. I will therefore begin by creating a parser that can parse the status, then gradually add the
other features listed above.
