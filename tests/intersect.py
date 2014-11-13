"""This unittest check the intersect function."""

from math import *
import unittest

from tests.base import BaseTestCase
from vector import intersect

class Intersect(unittest.TestCase, BaseTestCase):

    """Check the consistency of the intersect function.

    Tests (methods):
        test_set -- try different segments to see if they intersect

    """

    def test_set(self):
        """Try different segments."""
        segments = [
                (0, 0, 2, 10, -1, 2, 15, 1, True),
        ]

        for ax, ay, bx, by, cx, cy, dx, dy, b in segments:
            self.assertEqual(intersect(ax, ay, bx, by, cx, cy, dx, dy), b)

