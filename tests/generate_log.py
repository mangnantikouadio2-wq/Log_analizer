import random

def normal_line(x):
    x_ip = x % 256
    x_sec = x % 60
    return(f'192.168.1.{x_ip} - - [10/Oct/2026:10:00:{x_sec:02d} +0000] "GET /normal{x} HTTP/1.1" 200 1234')
def error_line(x, motif):
    x_ip = x % 256
    x_sec = x % 60
    lines = []
    if motif == 'incorrect_ip':
        lines.append(f'256.168.1.{x_ip} - - [10/Oct/2026:10:00:{x_sec:02d} +0000] "GET /{motif}{x} HTTP/1.1" 500 5678')
    elif motif == 'invalid_timestamp':
        lines.append(f'192.168.1.{x_ip} - - [32/Oct/2026:10:00:0{x_sec:02d} +0000] "GET /{motif}{x} HTTP/1.1" 500 5678')
        lines.append(f'10.0.0.{x_ip} - - [24/Xxx/2020:10:00:{x_sec:02d} +0000] "POST /{motif}{x} HTTP/1.1" 400 5678')
        lines.append(f'127.16.8.{x_ip} - - [24/Aug/2025:25:17:25 +0000] "DELETE /{motif}{x} HTTP/1.1" 400 5678')
    elif motif == 'invalid_method':
        lines.append(f'192.168.1.{x_ip} - - [10/Dec/2026:12:30:{x_sec:02d} +0000] "get /{motif}{x} HTTP/1.1" 500 5678')
        lines.append(f'198.51.100.{x_ip} - - [23/Nov/2025:15:45:{x_sec:02d} +0000] "ALPHA /{motif}{x} HTTP/1.1" 400 5678')
        lines.append(f'198.51.100.{x_ip} - - [23/Nov/2025:15:45:{x_sec:02d} +0000] "FAS@T /{motif}{x} HTTP/1.1" 400 5678')
    elif motif == 'invalid_url':
        lines.append(f'192.168.1.{x_ip} - - [10/Oct/2026:10:00:{x_sec:02d} +0000] "GET bad-url{x} HTTP/1.1" 500 5678')
        lines.append(f'192.168.1.{x_ip} - - [10/Oct/2026:10:00:{x_sec:02d} +0000] "GET /#$%&**\car%mpth#&~bad$url() HTTP/1.1" 500 5678')
        lines.append(f'192.168.1.{x_ip} - - [10/Oct/2026:10:00:{x_sec:02d} +0000] "GET {motif}{x} HTTP/1.1" 500 5678')
    elif motif == 'invalid_status_code':
        lines.append(f'192.168.1.{x_ip} - - [10/Oct/2026:10:00:{x_sec:02d} +0000] "GET /{motif}{x} HTTP/1.1" 999 5678')
        lines.append(f'192.168.1.{x_ip} - - [10/Oct/2026:10:00:{x_sec:02d} +0000] "GET /{motif}{x} HTTP/1.1" 600 5678')
        lines.append(f'192.168.1.{x_ip} - - [10/Oct/2026:10:00:{x_sec:02d} +0000] "GET /{motif}{x} HTTP/1.1" 700 5678')
    elif motif == 'malformed_line':
        lines.append(f'192.168.1.{x_ip} - - 10/Oct/2026:10:00:{x_sec:02d} +0000] "GET /{motif}{x} HTTP/1.1" 500 5678')
        lines.append(f'192.168.1.{x_ip} - - [10/Oct/2026:10:00:{x_sec:02d} +0000] GET /{motif}{x} HTTP/1.1" 500 5678')
        lines.append(f'192.168.1.{x_ip} - - [10/Oct/2026:10:00:{x_sec:02d} +0000] GET /{motif}{x} HTTP/1.1 500 5678')
        lines.append(f'192.168.1.{x_ip} - - [10/Oct/2026:10:00:{x_sec:02d} +0000] "GET /{motif}{x} HTTP/1.1" 500')
        lines.append(f'192.168.1.{x_ip} - - [10/Oct/2026:10:00:{x_sec:02d} +0000] "GET /{motif}{x} HTTP/1.1"')
        lines.append(f'192.168.1.{x_ip} - - [10/Oct/2026:10:00:{x_sec:02d} +0000] "GET /{motif}{x} HTTP/1.1" 500 5678 extra_field')
    elif motif == 'missing_fields':
        lines.append(f'192.168.1.{x_ip} - - [10/Oct/2026:10:00:{x_sec:02d} +0000] "GET /{motif}{x} HTTP/1.1" 500')
        lines.append(f'192.168.1.{x_ip} - - [10/Oct/2026:10:00:{x_sec:02d} +0000] "GET /{motif}{x} HTTP/1.1"')
        lines.append(f'192.168.1.{x_ip}[10/Oct/2026:10:00:{x_sec:02d} +0000] "GET /{motif}{x} HTTP/1.1" 500 5678')
        lines.append(f'192.168.1.{x_ip} - - [10/Oct/2026:10:00:{x_sec:02d} +0000] 500 5678')
        lines.append(f' - - [10/Oct/2026:10:00:{x_sec:02d} +0000] "GET /{motif}{x} HTTP/1.1"')
    return lines

def ridicule_lines(x):
    x_ip = x % 256
    x_sec = x % 60
    lines = []
    lines.append(f'192.168.1.{x_ip} - - [10/Oct/2026:10:00:{x_sec:02d} +0000] "GET /' + 'r' * 480 + ' HTTP/1.1" 200 1234')
    lines.append(f'ligne complètement pourrie sans structure')
    lines.append(f'192.168.1.{x_ip} - - [10/Oct/2026:10:00:{x_sec:02d} +0000] "GET /' + 'ridicule' * 500 + ' HTTP/1.1" 200 1234')
    return lines

def generate_log_file(filename, num_lines=100):
    motifs = ['incorrect_ip', 'invalid_timestamp', 'invalid_method', 'invalid_url', 'invalid_status_code', 'malformed_line', 'missing_fields']
    lines_types = ['normal', 'error', 'ridicule']
    with open(filename, 'w') as f:
        for i in range(num_lines):
            line_type = random.choice(lines_types)
            if line_type == 'normal':
                line = normal_line(i)
                f.write(line + '\n')
            elif line_type == 'error':
                motif = random.choice(motifs)
                error_lines = error_line(i, motif)
                for line in error_lines:
                    f.write(line + '\n')
            elif line_type == 'ridicule':
                ridicule_lines_list = ridicule_lines(i)
                for line in ridicule_lines_list:
                    f.write(line + '\n')
        last_line = '192.168.1.18 - - [13/Oct/2026:10:42:08 +0000] "GET /end HTTP/1.1" 200 700'
        f.write(last_line)

generate_log_file("log_long.txt")
print("Generated file: log_long.txt")