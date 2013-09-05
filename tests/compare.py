"""This unittest check the comparison operators."""

import unittest

from tests.base import BaseTestCase
from vector import Vector

class Compare(unittest.TestCase, BaseTestCase):

    """Check the consistency of the comparison operators.

    Tests (methods):
        test_equal -- test the == operator
        test_unequal -- test the != operator

    """

    def test_equal(self):
        """Test the == operator consistency."""
        v1 = Vector(10, 5, 18)
        v2 = Vector(10, 5, 1)
        self.assertNotEqual(v1, v2)
        v2.z = 18
        self.assertEqual(v1, v2)

