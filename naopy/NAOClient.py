import os
import re
import sys
import socket
import struct
import Queue
import GyroNAOHead
import NAOSay
#from NAOPostures import Crouch
from threading import Thread
from time import sleep
#from naoqi import ALProxy

#def output_nao(text_input):
    #tts = ALProxy("ALTextToSpeech", "169.254.65.171", 9559)
    #tts.say(text_input)

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

server_addr = ('127.0.0.1', 1111);
print >>sys.stderr, 'ESTABLISHING A CONNECTION WITH THE SERVER'

connect_attempts = 0
not_connected = True
while not_connected:
    try:
        # Attempt to establish a connection
        sock.connect(server_addr)
    except socket.error, msg:
        print 'COULD NOT CONNECT TO THE SERVER, TRYING AGAIN IN 5 SECONDS!'
        # Try to connect 12 times, if no connection is established then quit.
        if connect_attempts > 12:
            sys.exit()
        # Increment the connection attempts variable
        connect_attempts += 1
        # Wait for 5 seconds before retrying connection
        sleep(5)
    else:
        # Connection has been made so we can exit the while loop
        not_connected = False

def client_thread():
    old_exp = "na"
    old_act = "na"
    while True:
        data = sock.recv(256)
        data = ''.join(data.partition('|')[0:2])
        data = data.replace('|', '')
        print '>> ', data
        try:
            exp_type,action,action_power,x1,y1 = data.split(',')
        except ValueError:
            continue
        print exp_type
        print repr(x1.rstrip('\x00'))
        print repr(y1.rstrip('\x00'))

        print(action)
        print(action_power)

        x1 = x1.rstrip('\x00')
        y1 = y1.rstrip('\x00')

        gyro_thread = Thread(target = GyroNAOHead.ControlRobot(float(x1), float(y1)))
        gyro_thread.setDaemon(True)
        gyro_thread.start()

        GyroNAOHead.ControlRobot(float(x1), float(y1))

        if exp_type != old_exp:
            if exp_type.lower() == 'n':
                say_message = "You are neutral"
                print "Neutral"
            elif exp_type.lower() == 's':
                say_message = "You are smiling"
                print "Smiling"
            elif exp_type.lower() == 'y':
                say_message = "You are surprised"
                print "Surprised"
            elif exp_type.lower() == 'f':
                say_message = "You are frowning"
                print "Frowning"
            elif exp_type.lower() == 't':
                say_message = "You are clenching your teeth"
                print "Teeth Clenched"
            else:
                say_message = ""
                print "Don't know"

        if action.lower() != old_act:
            if action.lower() == 'n':
                say_message = say_message + " and your brain is neutral."
            elif action.lower() == 'p':
                say_message = say_message + " and you are thinking push."
            elif action.lower() == 'l':
                say_message = say_message + " and you are thinking pull."
            else:
                say_message = say_message

        if say_message != "":
            say_thread = Thread(target = NAOSay.TextToSpeech(say_message))
            say_thread.setDaemon(True)
            say_thread.start()

        old_exp = exp_type
        old_act = action
        NAOSay.TextToSpeech(say_message)
		#output_nao(data)

def input_thread():
    while True:
        message = raw_input('')
        sock.sendall(message)

thread1 = Thread(target = client_thread)
thread2 = Thread(target = input_thread)
thread1.setDaemon(True)
thread2.setDaemon(True)
thread1.start()
thread2.start()
while True:
    pass
