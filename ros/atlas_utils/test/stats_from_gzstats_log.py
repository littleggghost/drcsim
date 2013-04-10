#! /usr/bin/env python
import sys

try:
    file = open (sys.argv[1], 'r')
except IOError as e:
    print "I/O error({0}): {1}".format(e.errno, e.strerror)
    sys.exit(1)

start        = False
total_probes = 0
total_rtf    = 0.0
max_probe    = 0
min_probe    = 1.0

for line in file:
    if start:
        total_probes = total_probes + 1
        t = float(line.split(',', 1)[0])
        # Max 
        if t > max_probe:
            max_probe = t
        # Min 
        if t < min_probe:
            min_probe = t

        total_rtf = total_rtf + t 
        continue

    # Real probes start after the comment line
    if (line[0] == '#'):
        start = True

file.close()

print  ("mean: %.2f, max: %.2f, min: %.2f" % ((total_rtf / total_probes), max_probe, min_probe))