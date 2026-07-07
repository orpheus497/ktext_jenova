import urllib.request
import re
url = "https://invent.kde.org/kdevelop/kdevelop/-/raw/master/kdevplatform/interfaces/idocumentcontroller.h"
try:
    with urllib.request.urlopen(url) as response:
        content = response.read().decode("utf-8")
        if "textdocumentcreated" in content.lower():
            print("textDocumentCreated")
except Exception:
    pass
