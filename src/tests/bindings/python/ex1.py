#!/usr/bin/env python

import pcoords

filename = "test1.pcv"
lnb = 0
dnb = 0

print "Pcoords Python API version: %s" % pcoords.Version()
print "Loading file '%s'" % filename
data = pcoords.ParseImage(filename, None)


print "The image height is %d " % data['height']
print "There are %d axes" % data['axes_number']
for line in data['lines']:
        print str(line[1]['y'])

print "Whole data structure:"
print str(data)

