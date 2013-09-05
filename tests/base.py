"""This module contains a base class for vector unittests."""

class BaseTestCase:

    """Base class for vector unittests.

    This class implement methods to facilitate the vector testings.

    """

    def check_almost_equal(self, value1, value2):
        """Check that two values are AlMOST equal.

        Try to round the tested values.

        """
        self.assertEqual(round(value1, 2), round(value2, 2))

