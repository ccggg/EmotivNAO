import socket
import sys

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

server_addr = ('127.0.0.1', 1111);
print >>sys.stderr, 'connecting to %s port %s' % server_addr
sock.connect(server_addr)

data = sock.recv(256)
print data
