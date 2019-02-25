from rfsim.librfsim import librfsim
import unittest
import numpy as np


class TestBasicLandscape(unittest.TestCase):

    def testTenByTen(self):
        landscape = librfsim.CLandscape()
        landscape.setup(10, 10, 10)
        landscape.iterate(1)
        rabbits = landscape.get_rabbits()
        foxes = landscape.get_foxes()
        expected_rabbit_arr = np.array([[20, 7, 82, 89, 7, 7, 90, 12, 83, 11],
                                        [73, 48, 68, 71, 72, 56, 56, 72, 50, 83],
                                        [1, 63, 70, 59, 70, 68, 78, 61, 68, 4],
                                        [85, 63, 72, 77, 82, 80, 87, 72, 58, 3],
                                        [86, 79, 76, 76, 84, 79, 79, 75, 70, 4],
                                        [7, 61, 72, 89, 74, 85, 85, 61, 62, 10],
                                        [5, 54, 62, 86, 8, 86, 86, 72, 80, 14],
                                        [84, 66, 70, 67, 69, 84, 90, 76, 64, 5],
                                        [80, 62, 37, 58, 72, 61, 68, 57, 47, 85],
                                        [7, 84, 10, 3, 5, 8, 88, 2, 57, 16]])
        expected_fox_arr = np.array([[1, 2, 1, 1, 3, 1, 1, 2, 2, 4],
                                     [2, 2, 2, 2, 1, 3, 1, 2, 3, 1],
                                     [2, 1, 3, 1, 1, 2, 2, 1, 1, 4],
                                     [1, 1, 3, 1, 1, 3, 2, 1, 2, 2],
                                     [1, 2, 1, 2, 3, 1, 2, 2, 1, 1],
                                     [2, 2, 1, 2, 1, 1, 4, 1, 2, 2],
                                     [4, 2, 2, 2, 2, 2, 1, 1, 1, 2],
                                     [2, 2, 1, 1, 1, 3, 2, 4, 1, 1],
                                     [5, 1, 1, 1, 2, 3, 1, 1, 2, 2],
                                     [1, 1, 2, 1, 1, 3, 2, 2, 1, 1]])
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
