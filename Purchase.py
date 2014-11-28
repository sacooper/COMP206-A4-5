#!/usr/bin/python

import cgitb
import cgi
import urllib
import os
import csv

cgitb.enable()

def verify_user(user):
    with open('data/LoggedIn.csv', 'r') as loggedin:
        reader = csv.reader(loggedin, delimiter=',')
        for row in reader:
            if row[0] == user:
                return True

    return False

if os.environ['REQUEST_METHOD'] == 'POST':
    # we only accept post requests
    form = cgi.FieldStorage()
    user = form['user'].value
    print "Content-Type:text/html\r\n"
    if (verify_user(user)):
        print "User logged in"
    else:
        print "User not logged in"


else:
    # error
    print "Content-Type:text/html\r\n"
    print "ERROR"
