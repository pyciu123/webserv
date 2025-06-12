import os
import sys
import urllib.parse

length = int(os.environ.get("CONTENT_LENGTH", 0))
body = sys.stdin.read(length)

params = urllib.parse.parse_qs(body)
a = int(params.get("a", [0])[0])
b = int(params.get("b", [0])[0])
result = a + b

print("Content-Type: text/html")
print()
print(f"<html><body><h1>Wynik: {a} + {b} = {result}</h1></body></html>")