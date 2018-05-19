import socket
import time
import numpy as np

HOST = "192.168.1.5"    # The remote host
PORT = 2000              # The same port as used by the server
print "Starting Program"
time_init = time.time()

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((HOST, PORT))


s.send("speedl([-0.0062745,0.0044,-0.00643,0,0,0],2,0);$$$speedl([0,0,0,0,0,0],2,0);")        #left arm  +x speed
#s.send("speedl([0.00668,-0.00443,0.006,0,0,0],2,0);$$$speedl([0,0,0,0,0,0],2,0);")         #left arm  -x speed

##.send("speedl([-0.00697,0.0006689,0.007262,0,0,0],2,0);$$$speedl([0,0,0,0,0,0],2,0);")    #left arm  +y speed
#s.send("speedl([0.00697,-0.0006689,-0.007262,0,0,0],2,0);$$$speedl([0,0,0,0,0,0],2,0);")   #left arm  -y speed
#s.send("speedl([0.003589,0.008959,0.00262,0,0,0],2,0);$$$speedl([0,0,0,0,0,0],2,0);")    # left arm +z speed
#s.send("speedl([-0.00276,-0.009034,-0.00348,0,0,0],2,0);$$$speedl([0,0,0,0,0,0],2,0);")     # left arm -z speed
time.sleep(2.0)
s.send("speedl([0,0,0,0,0,0],2,0);$$$speedl([0,0,0,0,0,0],2,0);")
time.sleep(0.5)
s.close()
print "Program finish"


""" s.send("speedl([-0.01,0,0,0,0,0],2,0);$$$speedl([0,0.01,0,0,0,0],2,0);")
time.sleep(2.0)
s.send("speedl([0.01,0,0,0,0,0],2,0);$$$speedl([0,-0.01,0,0,0,0],2,0);")
time.sleep(2.0)
s.send("speedl([0,0,0,0,0,0],2,0);$$$speedl([0,0,0,0,0,0],2,0);")
time.sleep(5)
s.send("speedl([-0.01,0,0,0,0,0],2,0);$$$speedl([0,0,0,0,0,0],0,0);")
time.sleep(2.0)
s.send("speedl([0.01,0,0,0,0,0],2,0);$$$speedl([0,0,0,0,0,0],0,0);")
time.sleep(2.0)
s.send("speedl([0,0,0,0,0,0],2,0);$$$speedl([0,0,0,0,0,0],2,0);")
time.sleep(0.5)
s.close()
print "Program finish" """

