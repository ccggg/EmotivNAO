import sys
import time
import random
import almath
from naoqi import ALProxy

robotIp = "169.254.65.171"
proxies_created = False

def ControlRobot(x1, y1):
    if proxies_created == False:
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
    # Move head randomly test
    names = ["HeadYaw", "HeadPitch"]
    angleLists = [(x1 * almath.TO_RAD) * -1, (y1 * almath.TO_RAD) * -1]
    timeLists = [0.5, 0.5]
    isAbsolute = True

    motionProxy.angleInterpolation(names, angleLists, timeLists, isAbsolute)
