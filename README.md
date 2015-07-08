# SimpGCS

This program is a simple ground control station program. You can use the program to show MAV's position and attitude information through Mavlink protocol. Current, this program only tested for ArduCopter, although it may support other flight controller which use Mavlink. The map widget used in this program is based on opmapcontrol. 

## Requirements:
* Qt4 or Qt5 (sudo apt-get install libqt4-core libqt4-dev)
* QGLViewer (sudo apt-get install libqglviewer-dev)

## Compile:

`qmake SimpGCS.pro`

`make`


## Usage:
First combin map data into a single map file "Data.qmdb". 
 `cd data; cat Data.qmdbaa Data.qmdbab > Data.qmdb; cd ..`


```
./SimpGCS
    -port               [s] UART port (default is /dev/ttyUSB0)
    -baud               [s] baud rate (default is 115200)
    -h  (print usage)
```

## Plateform:
Only test on LinuxMint 17.1 64-bit, may be other distributions are also support. 


## Screenshot:
-![alt text](http://blog.adv-ci.com/wp-content/uploads/2013/05/Screenshot-2D-SLAM-1.png "Screenshot 1")
-![alt text](http://blog.adv-ci.com/wp-content/uploads/2013/05/Screenshot-2D-SLAM.png "Screenshot 2")


## Project homepage:
http://www.adv-ci.com/blog/project/simpgcs


