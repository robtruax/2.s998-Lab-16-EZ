#!/usr/bin/python

import envoy

killall = envoy.run("mykill")
print 'killed all'
main = envoy.run("./launch_sim.sh", timeout = 1)

# genetic will return when it has produced its results
timeout = 200

# at a warp of thirty, this should take...compute our timeout
pythonTimeout = timeout

genetic = envoy.run("../../bin/pGenetic targ_shoreside.moos " + str(timeout), timeout = pythonTimeout)
print genetic.std_out

killall = envoy.run("mykill")
print 'killed all'

#// format is vname=aegir,error=22.41412,score=1048.26922

numIterations = 200

#for i in range(numIterations):
#    output = []
#    params = (a, b, c)
#    # genetic.set("
#    score = genetic.wait()
#    output.append((score, params))
#
#    main.write("2")
