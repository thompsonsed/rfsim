import unittest

import numpy as np

from rfsim.librfsim import librfsim


class TestBasicLandscape(unittest.TestCase):

    def testTenByTen(self):
        landscape = librfsim.CLandscape()
        landscape.setup(10, 10, 10)
        landscape.iterate(1)
        rabbits = landscape.get_rabbits()
        foxes = landscape.get_foxes()
        expected_rabbit_arr = np.array([[84, 84, 87, 79, 83, 82, 86, 86, 82, 88],
                                        [80, 88, 83, 85, 84, 80, 80, 82, 85, 86],
                                        [87, 83, 81, 83, 79, 83, 81, 79, 82, 81],
                                        [86, 85, 83, 85, 81, 86, 83, 77, 83, 84],
                                        [82, 79, 83, 83, 84, 84, 81, 79, 77, 86],
                                        [82, 83, 85, 83, 83, 81, 82, 84, 79, 85],
                                        [84, 83, 81, 76, 83, 80, 82, 82, 80, 82],
                                        [78, 76, 80, 82, 77, 83, 83, 83, 78, 81],
                                        [81, 79, 85, 83, 84, 84, 80, 80, 82, 87],
                                        [87, 84, 84, 86, 81, 79, 86, 87, 82, 83]])
        expected_fox_arr = np.array([[2, 2, 2, 2, 2, 2, 2, 2, 2, 2],
                                     [2, 2, 2, 2, 2, 2, 1, 3, 2, 2],
                                     [2, 2, 2, 2, 2, 1, 2, 2, 2, 2],
                                     [2, 2, 1, 2, 2, 2, 2, 2, 2, 2],
                                     [1, 2, 2, 2, 2, 2, 2, 2, 2, 2],
                                     [2, 2, 2, 2, 2, 2, 2, 3, 1, 2],
                                     [2, 2, 2, 2, 2, 2, 0, 3, 1, 2],
                                     [2, 2, 2, 3, 3, 3, 1, 2, 2, 2],
                                     [2, 2, 2, 2, 0, 1, 1, 2, 2, 2],
                                     [2, 2, 2, 2, 1, 2, 2, 2, 2, 2]])
        self.assertEqual(True, np.array_equal(expected_rabbit_arr, rabbits))
        self.assertEqual(True, np.array_equal(expected_fox_arr, foxes))


def testFiveByTen(self):
    landscape = librfsim.CLandscape()
    landscape.setup(10, 5, 10)
    landscape.iterate(1)
    rabbits = landscape.get_rabbits()
    foxes = landscape.get_foxes()
    self.assertEqual((10, 5), rabbits.shape)
    self.assertEqual((10, 5), foxes.shape)
