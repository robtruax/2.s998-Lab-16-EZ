import math

def dcos(x): return math.cos(x * math.pi / 180)
def dsin(x): return math.sin(x * math.pi / 180)


def makeCurve(startDeg, endDeg, reverse):
    radius = 80.0
    xOffset = 55
    yOffset = -105
    
    points, final = [], []
    
    for i in range(startDeg, endDeg):
        points.append((int(radius*dcos(i) + xOffset), int(radius*dsin(i) + yOffset)))

    if reverse:
        points.reverse()
    for i in range(0, len(points), 5):
        final.append(points[i])

    print 'polygon = pts = {',
    for x, y in final:
        print str(x) + ', ' + str(y) + ':',
    print '}'

print 'A'
makeCurve(-90, 90, True)

print 'B'
makeCurve(90, 270, False)
