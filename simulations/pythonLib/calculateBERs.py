#!/usr/bin/env python3
# -*- coding: utf-8 -*- 
 
import numpy as np

fers = [0.1*x for x in range(0,11)]
# fers = [0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0]

s_frag = 11454 * 8
s_frame = s_frag + (64 + 20 + 28 + 36) * 8
print(fers)

bers = [round(1-(1-fer)**(1/float(s_frame)), 8) for fer in fers]

bers = [np.format_float_positional(num, trim='-') for num in bers]

s = ""
for ber in bers:
    s += ber
    s += ", "
print(s)
