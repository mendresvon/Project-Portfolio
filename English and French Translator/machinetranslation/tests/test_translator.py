"""
Series of tests to test translators.py programs
"""
import unittest
from translator import english_to_french, french_to_english


class TestEnglishToFrench(unittest.TestCase):
    def test_1(self):
        self.assertEqual(english_to_french("Hello"), "Bonjour")
        self.assertNotEqual(english_to_french("Hi"), "Bonjour")


class TestFrenchToEngligh(unittest.TestCase):
    def test_2(self):
        self.assertEqual(french_to_english("Bonjour"), "Hello")
        self.assertNotEqual(french_to_english("Bonjour"), "Hi")


if __name__ == "__main__":
    unittest.main()
    