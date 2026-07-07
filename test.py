import urllib.request
import re

url = "https://invent.kde.org/frameworks/ktexteditor/-/raw/master/src/include/ktexteditor/view.h"
try:
    with urllib.request.urlopen(url) as response:
        content = response.read().decode("utf-8")
        print("\n".join(re.findall(r".{0,40}completion.{0,40}", content, re.IGNORECASE)))
except Exception as e:
    print(f"Error: {e}")
