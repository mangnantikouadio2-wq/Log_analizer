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

Not yet implemented:

- Total request count
- Status-class statistics
- Method statistics
- IP frequency
- Most active IPs
- Time buckets
- Brute-force detection
- SQL injection/XSS detection
- Configurable thresholds
- Structured output
- Report export

So the project is currently:

> A streaming log parser and validator with a CLI demonstration.

It is not yet:

> A log-Analisys and anomaly detection system.
