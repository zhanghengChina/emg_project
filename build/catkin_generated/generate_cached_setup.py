# -*- coding: utf-8 -*-
from __future__ import print_function
import argparse
import os
import stat
import sys

# find the import for catkin's python package - either from source space or from an installed underlay
if os.path.exists(os.path.join('/opt/ros/indigo/share/catkin/cmake', 'catkinConfig.cmake.in')):
    sys.path.insert(0, os.path.join('/opt/ros/indigo/share/catkin/cmake', '..', 'python'))
try:
    from catkin.environment_cache import generate_environment_script
except ImportError:
    # search for catkin package in all workspaces and prepend to path
<<<<<<< HEAD
    for workspace in "/home/leon/kinect2/kinect_ros_support/devel;/home/leon/zzm/devel;/home/leon/zack/devel;/home/leon/emg_project/devel;/home/leon/graspdemo/devel;/home/leon/mobile_manipulator/devel;/home/leon/catkin/devel;/opt/ros/indigo".split(';'):
=======
    for workspace in "/home/leon/emg_project/devel;/opt/ros/indigo".split(';'):
>>>>>>> 0bcb0d2209f44d23003687044b46464286c1b7d4
        python_path = os.path.join(workspace, 'lib/python2.7/dist-packages')
        if os.path.isdir(os.path.join(python_path, 'catkin')):
            sys.path.insert(0, python_path)
            break
    from catkin.environment_cache import generate_environment_script

code = generate_environment_script('/home/leon/emg_project/devel/env.sh')

output_filename = '/home/leon/emg_project/build/catkin_generated/setup_cached.sh'
with open(output_filename, 'w') as f:
    #print('Generate script for cached setup "%s"' % output_filename)
    f.write('\n'.join(code))

mode = os.stat(output_filename).st_mode
os.chmod(output_filename, mode | stat.S_IXUSR)
