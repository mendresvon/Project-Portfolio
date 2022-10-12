from machinetranslation import translator
from flask import Flask, render_template, request
import json
import machinetranslation

app = Flask("Web Translator")


@app.route("/englishToFrench")
def englishToFrench():
    textToTranslate = request.args.get("textToTranslate")
    # Write your code here
    french_translation = machinetranslation.translator.english_to_french(textToTranslate)
    return f"Translated to French successfully: {french_translation}"


@app.route("/frenchToEnglish")
def frenchToEnglish():
    textToTranslate = request.args.get("textToTranslate")
    # Write your code here
    english_translation = machinetranslation.translator.french_to_english(textToTranslate)
    return f"Translated to English successfully: {english_translation}"


@app.route("/")
def renderIndexPage():
    # Write the code to render template
    return render_template("index.html")


if __name__ == "__main__":
    app.run(debug=True)
