#!/usr/bin/python

# copied the files to server (/moos-ivp and /2.s998-Lab-16-EZ/) recursively
# rm'd the build folders in each directory. then mkdir build in each directory
# yum install cmake
# yum install eigen3
# yum install 
# 102  yum install X11
# 103  sudo yum install X11
# 104  sudo yum install x11
# 105  sudo yum install libx11-dev
# 106  sudo yum install libx11
# 107  sudo yum install libX11
# 108  sudo yum install libX11-de
# 109  sudo yum install libX11-dev
# 110  sudo yum install libx11-dev
# 111  yum install libX11-devel
# 112  sudo yum install libX11-devel
# 114  sudo yum install libGl
# 115  sudo yum install ib64mesagl1-devel
# 116  sudo yum install mesa-common-dev
# 117  sudo yum install openGL
# 118  sudo yum install opengl
# 119  sudo yum install mesa-libGL
# 121  sudo yum install mesa-libGL-devel
# sudo yum install libtiff-devel

import envoy

killall = envoy.run('mykill')
print 'killed all'

# genetic will return when it has produced its results
timeoutLow, timeoutHigh, timeoutStep = 200, 2500, 20
averages = 10

i, totals = 0, len(range(timeoutLow, timeoutHigh, timeoutStep))

for timeout in xrange(timeoutLow, timeoutHigh, timeoutStep):
    for a in xrange(averages):
        print 'beginning trial ' + str(i) + ' of ' + str(totals)
        i += 1
        
        main = envoy.run('./launch_sim.sh', timeout = 1)
        # at a warp of thirty, this should take...compute our timeout
        pythonTimeout = timeout

        genetic = envoy.run("../../bin/pGenetic targ_shoreside.moos " + str(timeout), timeout = pythonTimeout)
        s = genetic.std_out
        f = open('log.txt', 'a')
        if s.split('\n')[-2].startswith('got score report vname=aegir'):
            score = s[s.rindex('=')+1:].replace('\n', '')
            error = s[s.index('error=') + len('error='):s.index(',', s.index('error='))]
            result = 'timeout: ' + str(timeout) + '\tscore: ' + score + '\terror: ' + error
            print result
            f.write(result)
        else:
            print s
            f.write('ERROR: ' + s)
        f.close()

        killall = envoy.run('mykill')
    #print 'killed all'

#output will be
#pGenetic launching as 200
#
#got score report vname=aegir,error=15.00377,score=1406.25472
