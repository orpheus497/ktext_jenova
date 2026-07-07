import urllib.request
import re
url = "https://invent.kde.org/frameworks/ktexteditor/-/raw/master/src/include/ktexteditor/mainwindow.h"
try:
    with urllib.request.urlopen(url) as response:
        content = response.read().decode("utf-8")
        print("\n".join(re.findall(r".{0,40}views\(\).{0,40}", content, re.IGNORECASE)))
except Exception:
    pass
