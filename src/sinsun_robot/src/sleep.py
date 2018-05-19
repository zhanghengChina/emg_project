# Echo client program
import socket
import time
import numpy as np

HOST = "192.168.1.5"    # The remote host
PORT = 2000              # The same port as used by the server
print "Starting Program"
time_init = time.time()

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((HOST, PORT))

#s.send("speedl([0,0,0,0,0,0],0.001,2);$$$speedl([0,0.005,0,0,0,0],0.001,2);")
#time.sleep(5)
#s.send("speedl([0,0,0,0,0,0],1,1);$$$speedl([0,0,0,0,0,0],1,1);")
#time.sleep(2.0)
#s.send("sleep(1000);$$$sleep(1000);")
s.send("Sleep(10);$$$Sleep(10);")
time.sleep(0.2)
s.close()
print "Program finish"
