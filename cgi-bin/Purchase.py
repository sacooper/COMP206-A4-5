#!/usr/bin/python

import cgi
import cgitb
import urllib
import os
import csv

cgitb.enable()



def verify_user(user):
    with open('data/LoggedIn.csv', 'rb') as loggedin:
        reader = csv.reader(loggedin)
        for row in reader:
            if row[0] == user:
                return True

    return False

if os.environ['REQUEST_METHOD'] != 'POST':
    print "Content-Type:text/html\r\n"
    print "ERROR"
    exit(0);

# we only accept post requests
form = cgi.FieldStorage()
user = form['user'].value
print "Content-Type:text/html\r\n"
if (verify_user(user)):
    print "User logged in"
else:
    print "User not logged in"
    exit(0);

total = 0.0

newRows = []

with open('data/Inventory.csv', 'rb') as inventory:
    reader = csv.reader(inventory)
    for row in reader:
        if form.getvalue("num" + row[0]):
            amount = int(form.getvalue("num" + row[0]))
            if int(row[1]) < amount:
                amount = int(row[1])
            total += float(row[2]) * amount
            row[1] = int(row[1]) - amount
        newRows.append(row)

print total

with open('data/Inventory.csv', 'wb') as inventory:
    reader = csv.writer(inventory)
    reader.writerows(newRows)
