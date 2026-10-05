# Log analyzer program

The repo is actually a log paser that accept lines in format `IP - - [TIMESTAMP TIME-ZONE] "METHOD URL HTTP-VERSION" STATUT OCTETS`.
My goal is to progressively build a log-analysis and security-detection tool from the current working parser into a complete,
modular CLI application.

## Features

- Accept a log file as input
- Parce each lines
- Validate each parsed entry
- Handle invalid inputs
- Report the result in a verbose way

## Project structure

```text
log_analizer/
├── log.txt
├── Makefile
├── README.md
├── src/
│   ├── gettoken.c
│   ├── main.c
│   ├── parser.c
│   ├── parser.h
│   ├── reader.c
│   ├── validator.c
│   └── validator.h
├── tests/
│   ├── test_log.txt
│   ├── test_parser.c
│   └── test_validator.c
└── todo.md
```

## How it works

The current program accept a log file as an input and parse it to display le different importants componants
of the lines. It does it by reading the file line by line and apply the parsing algorith to each single line.
The process of reading line should be normaly handeled by the `get_line` function in `reader.c` but it not the
case actually because the actual main is more likely a test to modular structure of the program than the real
main of the program.<br />
The parsing algorith works on the line by using a pointer to deference each element of the character array and
passing the appropriate position pointer to subsequents fonctions to get the items we want from the line. After
the fonction is done working, the parser algorithm advance the pointer to the next camp to be handled by another
function and so on.<br />
The usefull informations are all represented by a structure of character arrays(IP, TIMESTAMP, METHOD, URL and
STATUS) called `Log_entity`. The parsing algorithm feed all the members of this structure the appropriate information
while parsing the data. After the parsing algorith finishes his work, all the fields of the structure are verified
by the fuctions in `validator.c`. If the verification is successful, the informations are displayed and the line is
given the status `[OK]` but if it is not, the incorect information is reported as invalid and the line is given the
the status `[WARNING]`. If the line format does not match the format accepted by the parser or is someway corrupted(
we assumed here corruption as missing brackets or missing quotes), the line is reported as corrupted.

## Future improvements

### Basic statistics

- Count total requests
- Count successfull requests
- Count client errors
- Count server errors
- Count requests by HTTP method
- Count requests by status code
- Count requests by IP address
- Determine most active IPs
- Display some "top" IPs(did not decided yet how many)

### Time Analysis

- Parse timestamp into a comparable form
- Count request per time bucket
- Other that are not decided yet
