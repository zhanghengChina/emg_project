<<<<<<< HEAD

# communication with sinsun robot, meanwhile, we should run this script before other ros node to move the robot to the initial place.
=======
# Echo client program
>>>>>>> 0bcb0d2209f44d23003687044b46464286c1b7d4
import socket
import time
import numpy as np

HOST = "192.168.1.5"    # The remote host
PORT = 2000              # The same port as used by the server
print "Starting Program"
time_init = time.time()

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((HOST, PORT))

#s.send("speedl([-0.01336,0.00886,-0.01201,0,0,0],2,0);$$$speedl([0,0,0,0,0,0],2,0);")
#time.sleep(3)
#s.send("speedl([0,0,0,0,0,0],1,1);$$$speedl([0,0,0,0,0,0],1,1);")
#time.sleep(2.0)
#s.send("sleep(1000);$$$sleep(1000);")
#s.send("speedl([0,0,0,0,0,0],2,0);$$$speedl([0,0,0,0,0,0],2,0);")
#s.send("movej([0,0,0,0,0,0,0],40,100);$$$movej([0,0,0,0,0,0,0],40,100);")
<<<<<<< HEAD
# In the movej command, the number 20 below is the percentage of the max vel.
s.send("movej([27.26,16.7,8.45,-91.44,-20.84,25.57,0],20,100);$$$movej([-27.26,-16.71,-8.45,91.44,20.84,-25.57,0],20,100);")
=======
s.send("movej([47.2,-4.62,-10.14,-85.35,-10.36,44.13,28.28],20,100);$$$movej([-27.26,-16.71,-8.45,91.44,20.84,-25.57,-81.19],20,100);")
>>>>>>> 0bcb0d2209f44d23003687044b46464286c1b7d4
time.sleep(0.5)
s.close()
print "Program finish"
