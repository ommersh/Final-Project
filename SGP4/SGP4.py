# -*- coding: utf-8 -*-
"""
Created on Sat Jun  3 18:39:09 2023

@author: halel

data to maybe test:
STARLINK-4413           
1 53207U 22084U   23153.90342482  .00000251  00000+0  24123-4 0  9992
2 53207  97.6545 215.1916 0002424 101.1838 258.9658 15.01254820 47668
THORAD AGENA D DEB      
1 04649U 70025AH  23153.17202248  .00014936  00000+0  16060-2 0  9994
2 04649  99.8522 228.6648 0067113 119.5053 241.2871 14.87004351782441
"""

from sgp4.api import Satrec
import numpy as np
np.set_printoptions(precision=2)
from sgp4 import omm
np.set_printoptions(precision=2)
seconds_in_day = 60*60*24

def secToDayFraction(sec):
    return sec/seconds_in_day

s = '1 25544U 98067A   19343.69339541  .00001764  00000-0  38792-4 0  9991'
t = '2 25544  51.6439 211.2001 0007417  17.6667  85.6398 15.50103472202482'
satellite = Satrec.twoline2rv(s, t)


jd = np.array((0, 100, 222, 300))
fr = np.array((0.0001, 0.0002, 0.0003, 0.0004))

e, r, v = satellite.sgp4_array(jd, fr)

print(e)
print(r)
print(v)


with open('LEMUR2.xml') as f:
    fields = next(omm.parse_xml(f))
sat = Satrec()
omm.initialize(sat, fields)
jd_ep = sat.jdsatepoch
fr_ep = sat.jdsatepochF

jd = np.array((jd_ep,jd_ep))
fr = np.array((fr_ep,fr_ep + secToDayFraction(175.5612722)))

e, r, v = sat.sgp4_array(jd, fr)

print(e)
print(r)
print(v)