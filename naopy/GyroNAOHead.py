import sys
import time
import random
import almath
from naoqi import ALProxy

robotIp = "169.254.65.171"

def StiffnessOn(proxy):
    # We use the "Body" name to signify the collection of all joints
    pNames = "Body"
    pStiffnessLists = 1.0
    pTimeLists = 1.0
    proxy.stiffnessInterpolation(pNames, pStiffnessLists, pTimeLists)


def MoveHead(x1, y1):
    # Init proxies.
    try:
        motionProxy = ALProxy("ALMotion", "169.254.65.171", 9559)
    except Exception, e:
        print "Could not create proxy to ALMotion"
        print "Error was: ", e

    try:
        postureProxy = ALProxy("ALRobotPosture", "169.254.65.171", 9559)
    except Exception, e:
        print "Could not create proxy to ALRobotPosture"
        print "Error was: ", e

    # Set NAO in Stiffness On
    StiffnessOn(motionProxy)

    # Send NAO to Pose Init
    #postureProxy.goToPosture("StandInit", 1.0)

    # Move head randomly test
    names = ["HeadYaw", "HeadPitch"]
    angleLists = [x1 * almath.TO_RAD, y1 * almath.TO_RAD]
    timeLists = [1.0, 1.0]
    isAbsolute = True

    motionProxy.angleInterpolation(names, angleLists, timeLists, isAbsolute)
