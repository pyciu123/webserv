#!/Users/kubapyciarz/.pyenv/versions/3.10.12/bin/python3
import cgi
import cgitb
import os

cgitb.enable()  # Włącz debugowanie CGI

form = cgi.FieldStorage()
a = form.getvalue('a')
b = form.getvalue('b')

print("Content-Type: text/plain")
print()
if a and b:
    try:
        result = int(a) + int(b)
        print(f"Result: {result}")
    except ValueError:
        print("Error: Invalid input for a or b")
else:
    print("Error: Missing a or b parameters")