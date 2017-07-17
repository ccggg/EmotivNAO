import sys
from naoqi import ALProxy

robotIp = "169.254.65.171"

def TextToSpeech(message):
    try:
        ttsProxy = ALProxy("ALTextToSpeech", robotIp, 9559)
    except Exception, e:
        print "Could not create proxy to ALTextToSpeech"
        print "Error was: ", e

    ttsProxy.say(message)
