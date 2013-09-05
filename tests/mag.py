"""This unittest check the mag special attributes of a Vector."""

from math import *
import unittest

from tests.base import BaseTestCase
from vector import Vector

class Mag(unittest.TestCase, BaseTestCase):

    """Check the consistency of the mag attribute.

    Tests (methods):
        test_set -- try to set the vector's mag
        test_around_x -- turn around the X axis
        turn_around_y -- turn around the Y axis
        turn_around_z -- turn around the Z axis

    """

    def test_set(self):
        """Try to set the mag's attribute."""
        vector = Vector(3, 4.5, 2)
        vector.mag = 8
        self.check_almost_equal(vector.mag, 8)
    
    def test_around_x(self):
        """Try to turn around X (the mag should remain the same)."""
        vector = Vector(5, 8, 4)
        old_mag = vector.mag
        vector.around_x(pi / 2)
        self.check_almost_equal(vector.mag, old_mag)

    def test_around_y(self):
        """Try to turn around Y (the mag should remain the same)."""
        vector = Vector(5, 8, 4)
        old_mag = vector.mag
        vector.around_y(pi / 2)
        self.check_almost_equal(vector.mag, old_mag)

    def test_around_z(self):
        """Try to turn around Z (the mag should remain the same)."""
        vector = Vector(5, 8, 4)
        old_mag = vector.mag
        vector.around_z(pi / 2)
        self.check_almost_equal(vector.mag, old_mag)
