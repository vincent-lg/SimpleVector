"""This unittest check the around_* methods.."""

from math import *
import unittest

from tests.base import BaseTestCase
from vector import Vector

class Around(unittest.TestCase, BaseTestCase):

    """Check the consistency of the around_* methods.

    Tests (methods):
        test_circle -- try to turn the vector in a circle
        test_around_x -- turn around the X axis
        turn_around_y -- turn around the Y axis
        turn_around_z -- turn around the Z axis

    """

    def test_circle(self):
        """Create a vector and turn it in a full circle."""
        vector = Vector(5, 12.5, 2)
        x, y, z = vector.x, vector.y, vector.z
        vector.around_x(pi * 2)
        vector.around_y(pi * 2)
        vector.around_z(pi * 2)
        self.check_almost_equal(vector.x, x)
        self.check_almost_equal(vector.y, y)
        self.check_almost_equal(vector.z, z)
    
    def test_around_x(self):
        """Try to turn around X."""
        vector = Vector(2, 3, 4)
        vector.around_x(pi / 2)
        self.check_almost_equal(vector.x, 2)
        self.check_almost_equal(vector.y, -4)
        self.check_almost_equal(vector.z, 3)

    def test_around_y(self):
        """Try to turn around Y."""
        vector = Vector(5, 10, 8)
        vector.around_y(pi / 2)
        self.check_almost_equal(vector.x, 8)
        self.check_almost_equal(vector.y, 10)
        self.check_almost_equal(vector.z, 5)

    def test_around_z(self):
        """Try to turn around Z."""
        vector = Vector(12.3, 18, 1)
        vector.around_z(pi / 2)
        self.check_almost_equal(vector.x, 18)
        self.check_almost_equal(vector.y, -12.3)
        self.check_almost_equal(vector.z, 1)

