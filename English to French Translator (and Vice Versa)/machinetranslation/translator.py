"""
Module has to functions: eng-to-french translator and french-to-eng translator
"""
import os
from ibm_watson import LanguageTranslatorV3
from ibm_cloud_sdk_core.authenticators import IAMAuthenticator
from dotenv import load_dotenv

load_dotenv()

apikey = os.environ["apikey"]
url = os.environ["url"]

authenticator = IAMAuthenticator(apikey)
language_translator = LanguageTranslatorV3(version="2018-05-01", authenticator=authenticator)

language_translator.set_service_url(url)
# language_translator.set_disable_ssl_verification(True)


def english_to_french(english_text=None):
    """
    Translates english to french. Takes french text, return english translation
    """
    if not english_text:
        raise TypeError("Text must be provided")
    translation = language_translator.translate(text=english_text, model_id="en-fr").get_result()
    french_translation = translation["translations"][0]["translation"]
    return french_translation


def french_to_english(french_text=None):
    """
    Translates french to english. Takes english text, return french translation
    """
    if not french_text:
        raise TypeError("Text must be provided")
    translation = language_translator.translate(text=french_text, model_id="fr-en").get_result()
    english_translation = translation["translations"][0]["translation"]
    return english_translation
