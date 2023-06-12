from sgp4 import omm
from sgp4.api import Satrec
from SgpMathUtils import *

N_Catch_const = 16

def calculate_distance(tle1, tle2, t_end, st_min, p_max, factor, init_step_size, name):
    min_distance = float('inf')
    step_size = init_step_size
    min_time = None

    while True:
        num_point = math.ceil(t_end / step_size)
        num_blocks = math.ceil(num_point / p_max)
        last_min_distance = min_distance

        counter = 20

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

            savePointToFile(distance_vector, time_points, name + '_stepSize' + str(step_size) + '_block' + str(block))

            if distance_vector[min_idx] < min_distance:
                min_distance = distance_vector[min_idx]
                min_time = time_points[min_idx]
                print("new minimal distance: ", min_distance, "in , ",min_time," jd", jd[min_idx], "and fr", fr[min_idx], "with step size:", step_size)
                print("r1: ", r1[min_idx])
                print("r2: ", r2[min_idx])

        step_size /= factor
        if min_distance - last_min_distance >= 0:
            counter -= 1
        if step_size < st_min or counter <= 0:
            print("distance: ", min_distance)
            break

    return min_distance, min_time

def createTimePointsForAncasAndCatch(n, Gamma, t_end):
    time_points_ancas = []
    time_points_catch = []
    time_points_ancas.append(0)
    time_points_catch.append(0)
    Catch_points = calculateXforN(n, Gamma)
    Ancas_jump = Gamma / (n - 1)
    a = 0
    b = Gamma
    while b < t_end:
        for i in range(1, n):
            # create time points for ancas
            time_points_ancas.append(a + i * Ancas_jump)
            # create time points for catch
            time_points_catch.append(a + Catch_points[i])
        a = b
        b = b + Gamma
    return time_points_ancas, time_points_catch


def calculateAndSaveForAncasAndCatch(tle1, tle2, t_end, name):
    sat1 = Satrec()
    sat2 = Satrec()
    omm.initialize(sat1, tle1)
    omm.initialize(sat2, tle2)
    # mean motion is the number of revolutions per day, 1/mean motion is the time in days for each revolution
    # we change the time units to seconds and time by 1/2 to get the time of half revolution in seconds
    t_sec1 = seconds_in_day / float(tle1['MEAN_MOTION']) * 0.5
    t_sec2 = seconds_in_day / float(tle2['MEAN_MOTION']) * 0.5
    Gamma = min(t_sec1, t_sec2)
    time_points_ancas, time_points_catch = createTimePointsForAncasAndCatch(N_Catch_const, Gamma, t_end)
    sat_to_get_time = sat1 if sat1.jdsatepoch > sat2.jdsatepoch else sat2
    # create time points for ancas
    jd, fr = GetJdAndFrArrayForSattelite(sat_to_get_time, time_points_ancas)
    _, r1, v1 = sat1.sgp4_array(jd, fr)
    _, r2, v2 = sat2.sgp4_array(jd, fr)
    relative_distances = r2 - r1
    relative_vel = v1 - v2

    F = []
    dF = []
    for r_vector, v_vector in zip(relative_distances, relative_vel):
        F.append(r_vector[0] * r_vector[0] + r_vector[1] * r_vector[1] + r_vector[2] * r_vector[2])
        dF.append(r_vector[0] * v_vector[0] + r_vector[1] * v_vector[1] + r_vector[2] * v_vector[2])

    savePointForAncasAndCatch(time_points_ancas, r1, v1, r2, v2, F, dF, name + "_CONST")
    # create time points for catch
    jd, fr = GetJdAndFrArrayForSattelite(sat_to_get_time, time_points_catch)
    _, r1, v1 = sat1.sgp4_array(jd, fr)
    _, r2, v2 = sat2.sgp4_array(jd, fr)
    distances = r2 - r1
    F = []
    for point_vector in distances:
        F.append(np.linalg.norm(np.array(point_vector)))
    savePointForAncasAndCatch(time_points_catch, r1, v1, r2, v2, F, dF, name + "_GAUSS")


def calculate_distance_for_graph(tle1, tle2, t_end, init_step_size, name):
    min_distance = float('inf')
    step_size = init_step_size
    min_time = None

    num_point = math.ceil(t_end / step_size)
    last_min_distance = min_distance

    sat1 = Satrec()
    sat2 = Satrec()
    omm.initialize(sat1, tle1)
    omm.initialize(sat2, tle2)
    sat_to_get_time = sat1 if sat1.jdsatepoch > sat2.jdsatepoch else sat2

    print("num_point", num_point)

    start_time = 0
    end_time = t_end

    print("start_time", start_time, "end_time", end_time)

    time_points = np.arange(start_time, end_time + step_size, step_size)

    jd, fr = GetJdAndFrArrayForSattelite(sat_to_get_time, time_points)

    e1, r1, v1 = sat1.sgp4_array(jd, fr)
    e2, r2, v2 = sat2.sgp4_array(jd, fr)

    relative_distances = r2 - r1
    relative_vel = v1 - v2
    F = []
    dF = []
    for r_vector, v_vector in zip(relative_distances, relative_vel):
        F.append(r_vector[0] * r_vector[0] + r_vector[1] * r_vector[1] + r_vector[2] * r_vector[2])
        dF.append(r_vector[0] * v_vector[0] + r_vector[1] * v_vector[1] + r_vector[2] * v_vector[2])

    savePointForAncasAndCatch(time_points, r1, v1, r2, v2, F, dF, name + "_GraphData")
