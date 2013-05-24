from math import degrees

from vector import Vector
import unittest

class TestVector(unittest.TestCase):

    """Unittest for vectors."""

    def test_argument(self):
        """Test the argument's consistency."""
        values = (
            #(x, y, angle)
            (1, 0, 0),
            (1, 1, 45),
            (0, 1, 90),
        )

        for x, y, angle in values:
            vector = Vector(x, y, 0)
            self.assertEqual(degrees(vector.argument), angle)
