from rfsim.librfsim import librfsim
import unittest
import numpy as np


class TestBasicLandscape(unittest.TestCase):

    def testTenByTen(self):
        landscape = librfsim.CLandscape()
        landscape.setup(10, 10, 10)
        landscape.iterate(1)
        rabbits = landscape.get_rabbits()
        expected_arr = np.array([[30, 7, 92, 99, 7, 7, 100, 22, 93, 21],
                                 [83, 58, 78, 81, 82, 66, 66, 82, 60, 93],
                                 [1, 73, 80, 69, 80, 78, 88, 71, 78, 4],
                                 [95, 73, 82, 87, 92, 90, 97, 82, 68, 3],
                                 [96, 89, 86, 86, 94, 89, 89, 85, 80, 14],
                                 [17, 71, 82, 99, 84, 95, 95, 71, 72, 10],
                                 [5, 64, 72, 96, 8, 96, 96, 82, 90, 24],
                                 [94, 76, 80, 77, 79, 94, 100, 86, 74, 5],
                                 [90, 72, 47, 68, 82, 71, 78, 67, 57, 95],
                                 [7, 94, 10, 3, 5, 8, 98, 12, 67, 26]])
        self.assertEqual(True, np.array_equal(expected_arr, rabbits))
