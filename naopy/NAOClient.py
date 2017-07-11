import os
import re
import sys
import socket
import struct
import Queue
from threading import Thread
from time import sleep
from naoqi import ALProxy

def output_nao(text_input):
    tts = ALProxy("ALTextToSpeech", "169.254.65.171", 9559)
    tts.say(text_input)

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
    while True:
        data = sock.recv(256)
		output_nao(data)
        print '>> ', data

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
