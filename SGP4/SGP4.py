from SGP_Gen_Data import *

t_end = 604800  # two weeks
st_min = 0.0001  # millisecond

factor = 2
p_max = 193536000 #avoid MemoryError

init_step_size = 0.1

sat1_name = 'LEMUR2'
sat2_name = 'COSMOS'
with open(sat1_name + '.xml') as f:
    tle1 = next(omm.parse_xml(f))

with open(sat2_name + '.xml') as f:
    tle2 = next(omm.parse_xml(f))

filename = sat1_name + '_' + sat2_name

calculateAndSaveForAncasAndCatch(tle1, tle2, t_end, filename)

result = calculate_distance(tle1, tle2, t_end, st_min, p_max, factor, init_step_size, filename)
print("The minimal distance is:", result[0], "in time:", result[1])

