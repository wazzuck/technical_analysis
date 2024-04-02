#!/usr/bin/python3
import leather

line_data = [
    (0, 1),
    (2, 5),
    (4, 4),
    (8, 3)
]

line_data2 = [
    (0, 2),
    (2, 6),
    (4, 5),
    (8, 4)
]

dot_data = [
    (1, 3),
    (2, 5),
    (6, 9),
    (10, 4)
]

chart = leather.Chart('Mixed shapes')
chart.add_line(line_data)
chart.add_line(line_data2,None,None,"c")
chart.add_dots(dot_data)
chart.to_svg('/dev/shm/mixed_shapes.svg')
