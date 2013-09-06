"""This unittest check the distance method.."""

import unittest

from tests.base import BaseTestCase
from vector import Vector

class Distance(unittest.TestCase, BaseTestCase):

    """Check the consistency of the 'distance' method.

    Tests (methods):
        test_distance -- test the distance

    """

    def test_distance(self):
        """Test the distance between three vectors.

        When testing A.distance(B, C), the distance between the point C
        and the segment AB is returned.  For instance, if A(X=0, Y=0, Z=0),
        B(X=0, Y=5, Z=0) and C(X=1, Y+2, Z=0), then A.distance(B, c) = 1.

        """
        table = [
            ( 0  ,  0  ,  0  ,  5  ,  0  ,  0  ,  2  ,  0  ,  0  ,  0  ),
            ( 1  ,  0  ,  0  ,  1  ,  8  ,  0  ,  0  ,  3  ,  0  ,  1  ),
            (-5  , -2  ,  0  , -2  , -2  ,  0  , -3  , -4  ,  0  ,  2  ),
        ]

        for ax, ay, az, bx, by, bz, cx, cy, cz, d in table:
            A = Vector(ax, ay, az)
            B = Vector(bx, by, bz)
            C = Vector(cx, cy, cz)
            self.check_almost_equal(A.distance(B, C), d)

