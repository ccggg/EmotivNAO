import sys
import time
import random
from naoqi import ALProxy

def StiffnessOn(proxy):
    # We use the "Body" name to signify the collection of all joints
    pNames = "Body"
    pStiffnessLists = 1.0
    pTimeLists = 1.0
    proxy.stiffnessInterpolation(pNames, pStiffnessLists, pTimeLists)


def main(robotIP):
    # Init proxies.
    try:
        motionProxy = ALProxy("ALMotion", robotIP, 9559)
    except Exception, e:
        print "Could not create proxy to ALMotion"
        print "Error was: ", e

    try:
        postureProxy = ALProxy("ALRobotPosture", robotIP, 9559)
    except Exception, e:
        print "Could not create proxy to ALRobotPosture"
        print "Error was: ", e

    # Set NAO in Stiffness On
    StiffnessOn(motionProxy)

    # Send NAO to Pose Init
    postureProxy.goToPosture("StandInit", 1.0)

    testTime = 10
    t = 0
    dt = 0.2

    while (t < testTime):
        # Move head randomly test
        motionProxy.setAngles("HeadYaw", random.uniform(-1.0, 1.0), 0.6)
        motionProxy.setAngles("HeadPitch", random.uniform(-0.5, 0.5), 0.6)

        t = t + dt
        time.sleep(dt)


if __name__ == "__main__":
    robotIp = "127.0.0.1"

    if len(sys.argv) <= 1:
        print ""
    else:
        robotIp = sys.argv[1]

    main(robotIp)
