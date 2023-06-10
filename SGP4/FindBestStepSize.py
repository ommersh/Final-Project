import numpy as np
import math
from sgp4 import omm
from sgp4.api import Satrec
from SgpMathUtils import GetJdAndFrArrayForSattelite, savePointToFile


def calculate_distance(tle1, tle2, t_end, st_min, p_max, factor, init_step_size, name):
    min_distance = float('inf')
    step_size = init_step_size
    min_time = None


    while True:
        num_point = math.ceil(t_end / step_size)
        num_blocks = math.ceil(num_point / p_max)
        last_min_distance = min_distance

        counter = 2

        sat1 = Satrec()
        sat2 = Satrec()
        omm.initialize(sat1, tle1)
        omm.initialize(sat2, tle2)
        sat_to_get_time = sat1 if sat1.jdsatepoch > sat2.jdsatepoch else sat2

        print("num_point", num_point)

        for block in range(num_blocks):
            start_time = block * p_max * step_size
            end_time = min(t_end, (block + 1) * p_max * step_size)

            print("start_time", start_time, "end_time", end_time)

            time_points = np.arange(start_time, end_time + step_size, step_size)

            jd, fr = GetJdAndFrArrayForSattelite(sat_to_get_time, time_points)

            _, r1, _ = sat1.sgp4_array(jd, fr)
            _, r2, _ = sat2.sgp4_array(jd, fr)

            distances = r2 - r1

            distance_vector = []

            for point_vector in distances:
                distance_vector.append(np.linalg.norm(np.array(point_vector)))

            min_idx = np.argmin(distance_vector)

            savePointToFile(distance_vector, time_points, name + '_' + str(step_size))

            if distance_vector[min_idx] < min_distance:
                min_distance = distance_vector[min_idx]
                min_time = time_points[min_idx]
                print("new minimal distance: ", min_distance, "in jd", jd[min_idx], "and fr", fr[min_idx], "with step size:", step_size)
                print("r1: ", r1[min_idx])
                print("r2: ", r2[min_idx])

        step_size /= factor
        if min_distance - last_min_distance >= 0:
            counter = counter -1
        if step_size < st_min or counter <= 0:
            print("distance: ", min_distance)
            break

    return min_distance, min_time


t_end = 1209600  # two weeks
st_min = 0.01  # millisecond

factor = 2
p_max = t_end / st_min

init_step_size = 0.4

sat1_name = 'LEMUR2'
sat2_name = 'COSMOS'
with open(sat1_name + '.xml') as f:
    tle1 = next(omm.parse_xml(f))

with open(sat2_name + '.xml') as f:
    tle2 = next(omm.parse_xml(f))

filename = sat1_name + '_' + sat2_name
result = calculate_distance(tle1, tle2, t_end, st_min, p_max, factor, init_step_size, filename)

print("The minimal distance is:", result[0], "in time:", result[1])

