#!/usr/bin/python

import cgi
import cgitb
import urllib
import os
import csv

cgitb.enable()



def verify_user(user):
    with open('../data/LoggedIn.csv', 'rb') as loggedin:
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
if (not verify_user(user)):
    with open('http://cs.mcgill.ca/~scoope27/error.html', 'rb') as error:
        for row in error:
            if "{{errormessage}}" in row:
                print "<center><h3 style=\"color:red\">User not logged in</h3></center>\n"
            else:
                print row

total = 0.0

newRows = []

with open('../data/Inventory.csv', 'rb') as inventory:
    reader = csv.reader(inventory)
    for row in reader:
        if form.getvalue("num" + row[0]):
            amount = int(form.getvalue("num" + row[0]))
            if int(row[1]) < amount:
                amount = int(row[1])
            total += float(row[2]) * amount
            row[1] = int(row[1]) - amount
        newRows.append(row)

with open('../data/Inventory.csv', 'wb') as inventory:
    reader = csv.writer(inventory)
    reader.writerows(newRows)

print """
<!DOCTYPE html>
<html>
<head>
 <title>Home</title>
 <link href="http://cs.mcgill.ca/~scoope27/css/main.css" rel="stylesheet">
</head>
<body>
<div class="background"></div>

<table class="container">
 <th class="headers">
  <a class="nav-button" href="http://cs.mcgill.ca/~scoope27/index.html">Home</a>
 </th>
 <th class="headers">
  <a class="nav-button" href="http://cs.mcgill.ca/~scoope27/catalogue.html">Catalogue</a>
 </th>
 <th style="width:100%">
  <h2 class="header-center">Welcome To The Everything Store</h2>
 </th>
 <th class="headers" style="right:0;">
  <a class="nav-button" href="http://cs.mcgill.ca/~scoope27/login.html">Login</a>
 </th>
</table>

<div style="display: block; padding: 100px;">

</div>
"""
print "<h3>Total: {0}</h3>".format(total)
print """
</body>

</html>

"""
