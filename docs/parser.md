### IP

Currently supported source addresses: dotted-decimal IPv4 only.
Son addresses such as these are accepted: `001.002.003.004`.
It does not support:

- IPv6
- IPv4-mapped IPv6
- Hostnames
- Proxy-formatted addresses

### Timestamp

Accepted format:
`DD/Mon/YYYY:HH:MM:SS`

For example:

> 24/Aug/2026:10:15:30
> 01/Jan/2000:00:00:00

The validator checks timstamp format and component ranges, not full calendar validity.

### HTTP method

The getting http function works as an allowlist. Although it only accepts know method,
an unknown method will be presented as a WARNING by the programm so no method is ignored.

### URL

The parser validate the url only by checking that the field looks like an origin-form HTTP
target begining with `/` and having no "too special characters"

### Current state

The main program currently has no statistics or detection engine.

Implemented:

- File openning
- Line-by-line processing
- Parsing
- Field validation
- Human-redable reporting
- Basic warning/rejection categories
- Total request count
- Status-class statistics
- Most active IPs

Not yet implemented:

- Method statistics
- IP frequency
- Time buckets
- Brute-force detection
- SQL injection/XSS detection
- Configurable thresholds
- Structured output
- Report export

So the project is currently:

> A CLI log-Analisys.

It is not yet:

> An anomaly detection system.

## How the main program works

The current program accept a log file and report options as inputs. It parses the log file to display the informations
required by the user. It does it by reading the file line by line and apply the parsing algorithm to each single line.
The parsing algorithm works on the line by using a pointer to deference each element of the character array and
passing the appropriate position pointer to subsequents functions to get the items we want from the line. After
the function is done working, the parser algorithm advance the pointer to the next camp to be handled by another
function and so on.<br />
The usefull information are all represented by a structure of character arrays(IP, TIMESTAMP, METHOD, URL and
STATUS) called `Log_entity`. The parsing algorithm feed all the members of this structure the appropriate information
while parsing the data.<br />
After the parsing algorith finishes his work, all the fields of the structure are verified
by the fuctions in `validator.c`. If the verification is successful, the informations are displayed and the line is
given the status `[OK]` but if it is not, the incorrect information is reported as invalid and the line is given the
the status `[WARNING]` for the `--verbose` option. If the line format does not match the format accepted by the parser
or is someway corrupted, the line is reported as `[REJECTED!]` and the purpose of rejection is given.<br />
If the option `--stats` is selected, the algorithm parses each lines in a Log_entity and each Log_entity is given to`statistics.c`.
The functions contained in it extract information like the occurence of each ip or each methods or
each status. After this, the statistics are printed on the terminal. The output follows:

```text
=== Statistics ===
Read lines       : x
Valid lines      : y
Rejected lines   : z

2xx (succes)     : n
3xx (redirects)  : t
4xx (cli errors) : k
5xx (srv errors) : p
Other status code: m

Top 10 IPs
xxx.xxx.xxx.xxx : n requests
 .
 .
 .
```

<br />

If the option `--top-ips` is selected, the program will exept an positive number after this option. The number povided
of most actives ips will be printed on the screen.
