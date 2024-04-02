#!/usr/bin/python3
import bottle
from bottle import route, run, template, static_file

file_to_download="HOC.svg"

@route('/')
def index():
    return template('mytemplate',param=file_to_download)

@route('/static/<filename>')
def serve_static(filename):
    return static_file(file_to_download, root='/dev/shm')

run(host='localhost', port=8080)
