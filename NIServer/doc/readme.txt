About NIServer
==============
An OpenNI-based background server that retrieves data from Xtion sensor
and provides skeleton output for clients to read.

It's cross-platform and implemented in C++.

Supported OS
============
Windows 7 64bit, 


System
======
1. H/W
Asus Xtion Pro

2. M/W
OpenNI 1.5.2.23
NITE 1.5.2.21
Sensor driver 5.1.0.41

SimpleSockets 1.4.0
TinyThread++ 1.1


How to build
============


How to run
==========
1. run NIServer.exe
2. run NIClient.exe
   a. a new window for hand point drawing will be created.
   b. let this new window be in focus(to be able to response to key event)
   c. press 'l' to do query in loop.(press 'q' to do single query)
3. stand in front of the sensor, make sure you're in FOV.
4. raise hands, you'll see green and orange recangles on the window,
representing left and right hand respectively.
   a. the reatangle will move following your hands, and its size grows if your hand is reaching the sensor.







