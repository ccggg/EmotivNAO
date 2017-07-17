import sys
import time
import random
import almath
from naoqi import ALProxy

robotIp = "169.254.65.171"

def ControlRobot(x1, y1):
    try:
        motionProxy = ALProxy("ALMotion", robotIp, 9559)
    except Exception, e:
        print "Could not create proxy to ALMotion"
        print "Error was: ", e

    try:
        awarenessProxy = ALProxy('ALBasicAwareness', robotIp, 9559)
    except Exception, e:
        print "Could not create proxy to ALBasicAwareness"
        print "Error was: ", e

    awarenessProxy.stopAwareness()

    MoveHead(motionProxy, awarenessProxy, x1, y1)

def MoveHead(motionProxy, awarenessProxy, x1, y1):
    angleLists = [(x1 * almath.TO_RAD) * -1, (y1 * almath.TO_RAD) * -1]
    motionProxy.angleInterpolation(["HeadYaw", "HeadPitch"],
                                   angleLists,
                                   [0.5, 0.5],
                                   True)
