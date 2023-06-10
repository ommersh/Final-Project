import math
import numpy as np
import csv

seconds_in_day = 86400

def getJdAndFr(jd_ep, fr_ep, x):
    jd = jd_ep + math.floor(x / seconds_in_day)
    fr = fr_ep + (x % seconds_in_day) / seconds_in_day
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

def savePointToFile(point, time_points, name):
    with open('data/' + name + ".csv", 'w') as file:
        writer = csv.writer(file)
        writer.writerow(['distance', 'time'])
        for row in range(len(time_points)):
            writer.writerow([point[row], time_points[row]])
        file.close()


def savePointForAncasAndCatch(time_points, r1, v1, r2, v2, f, name):
    with open('data/' + name + ".csv", 'w') as file:
        writer = csv.writer(file)
        writer.writerow(['t', 'r1x', 'r1y', 'r1z', 'v1x', 'v1y', 'v1z', 'r2x', 'r2y', 'r2z', 'v2x', 'v2y', 'v2z', 'f'])
        for row in range(len(time_points)):
            writer.writerow(
                [time_points[row], r1[row][0], r1[row][1], r1[row][2], v1[row][0], v1[row][1], v1[row][2], r2[row][0],
                 r2[row][1], r2[row][2], v2[row][0], v2[row][1], v2[row][2], f[row]])
        file.close()


def calculateXforN(n, Gamma):
    times = []
    for i in range(0, n):
        j = n - 1 - i
        times.append(calculateXj(0, Gamma, j, n))
    return times


def calculateXj(a, b, j, n):
    x = ((b - a) / 2) * np.cos(np.pi * j / (n-1)) + (b + a) / 2
    return x