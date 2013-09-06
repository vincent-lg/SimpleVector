"""This unittest check the mathematic operators."""

import unittest

from tests.base import BaseTestCase
from vector import Vector

class MatManip(unittest.TestCase, BaseTestCase):

    """Check the consistency of the mathematic operators.

    Tests (methods):
        test_add -- test the + operator
        test_sub -- test the - operator

    """

    def test_add(self):
        """Test the + operator consistency."""
        v1 = Vector(2, 5, 1)
        v2 = Vector(5, 1, 1)
        v3 = v1 + v2
        self.assertEqual(v1.x, 2)
        self.assertEqual(v1.y, 5)
        self.assertEqual(v1.z, 1)
        self.assertEqual(v2.x, 5)
        self.assertEqual(v2.y, 1)
        self.assertEqual(v2.z, 1)
        self.assertEqual(v3.x, 7)
        self.assertEqual(v3.y, 6)
        self.assertEqual(v3.z, 2)

    def test_sub(self):
        """Test the - operator consistency."""
        v1 = Vector(2, 5, 1)
        v2 = Vector(5, 1, 1)
        v3 = v1 - v2
        self.assertEqual(v1.x, 2)
        self.assertEqual(v1.y, 5)
        self.assertEqual(v1.z, 1)
        self.assertEqual(v2.x, 5)
        self.assertEqual(v2.y, 1)
        self.assertEqual(v2.z, 1)
        self.assertEqual(v3.x, -3)
        self.assertEqual(v3.y, 4)
        self.assertEqual(v3.z, 0)

