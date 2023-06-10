import math
import numpy as np
seconds_in_day = 86400

def getJdAndFr(jd_ep, fr_ep, x):
    jd = jd_ep + math.floor(x / 86400)
    fr = fr_ep +  (x % 86400) / 86400
    return jd, fr

def GetJdAndFrArrayForSattelite(sat, time_points):
    jd_list = []
    fr_list = []

    jd_ep = sat.jdsatepoch
    fr_ep = sat.jdsatepochF

    for time_point in time_points:
        jd, fr = getJdAndFr(jd_ep, fr_ep, time_point)
        jd_list.append(jd)
        fr_list.append(fr)

    jd = np.array(jd_list)
    fr = np.array(fr_list)

    return jd, fr

