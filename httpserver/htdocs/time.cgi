#!/usr/bin/python
# -*- coding: UTF-8 -*-

import time
print "Content-type:text/html\r\n\r\n"
print '<html>'
print '<head>'
print '<title>Time</title>'
print '</head>'
print '<body>'
print '<h2>Http Server Current Time</h2>'
print time.strftime( '%Y-%m-%d %H:%M:%S' )
print '</body>'
print '</html>'
