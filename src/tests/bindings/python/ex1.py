#!/usr/bin/env python

import picviz

filename = "test1.pcv"
lnb = 0
dnb = 0

print "Picviz Python API version: %s" % picviz.Version()
print "Loading file '%s'" % filename
data = picviz.ParseImage(filename, None)


print "The image height is %d " % data['height']
print "There are %d axes" % data['axes_number']
for line in data['lines']:
        print str(line[1]['y'])

print "Whole data structure:"
print str(data)

