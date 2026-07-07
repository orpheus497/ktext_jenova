import urllib.request
url = "https://invent.kde.org/frameworks/ktexteditor/-/raw/master/src/include/ktexteditor/application.h"
try:
    with urllib.request.urlopen(url) as response:
        content = response.read().decode("utf-8")
        if "viewcreated" in content.lower():
            print("viewCreated in application.h")
except Exception:
    pass

url2 = "https://invent.kde.org/frameworks/ktexteditor/-/raw/master/src/include/ktexteditor/editor.h"
try:
    with urllib.request.urlopen(url2) as response:
        content = response.read().decode("utf-8")
        if "viewcreated" in content.lower():
            print("viewCreated in editor.h")
except Exception:
    pass
