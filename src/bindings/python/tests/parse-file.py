#!/usr/bin/python
import picviz

image = picviz.Image("./simple-test.pgdl", None)
print(image)
print "***** Image properties:"
print "height %d; width %d" % (image['height'], image['width'])
print "***** Axes list:"
print image['axeslist']
print "***** Axes:"
print image['axes']
print "***** Lines:"
for line in image['lines']:
	print "y1=%d,y2=%d" % (line['y1'], line['y2'])

picviz.setAxesOrder(image,['axis2','axis1'])

print "New order set"
for line in image['lines']:
	print "y1=%d,y2=%d" % (line['y1'], line['y2'])

image2 = picviz.Image("./simple-test.pgdl", None)
print(image2)

#picviz.savePGDL(image, "local.pgdl")
