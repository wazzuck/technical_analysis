#!/usr/bin/python3
# vim:set bg=dark number : #
import sqlite3, re
from bottle import route, get, post, request, run # or route

@route('/')
def index():
        html="""
<!DOCTYPE html>
<html>
<body>

<h2>What Can JavaScript Do?</h2>

<p id="demo">JavaScript can change HTML content.</p>

<button type="button" onclick="document.getElementById('demo').innerHTML = 'Hello JavaScript!'">Click Me!</button>

</body>
</html>

"""

        return html

NIC="localhost"
PORT=8080

run(host=NIC, port=PORT, debug=True)
