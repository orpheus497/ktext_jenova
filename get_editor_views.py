import urllib.request
import re

url = "https://api.kde.org/frameworks/ktexteditor/html/classKTextEditor_1_1Editor.html"
try:
    req = urllib.request.Request(url, headers={'User-Agent': 'Mozilla/5.0'})
    with urllib.request.urlopen(req) as response:
        content = response.read().decode("utf-8")
        print("\n".join(re.findall(r".{0,40}views.{0,40}", content, re.IGNORECASE)))
except Exception as e:
    print(f"Error: {e}")
