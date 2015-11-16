#!/bin/bash

#script to set the desktop background to a randomly generated maze
while true; do

feh --bg-center maze.png &

ODDS=3
T=1
number=$RANDOM

#location of mazepng
prog=~/documents/cpp/maze/png/mazepng

let "number %= $ODDS"
if [ "$number" -eq $T ]
then
$prog -w 1920 -h 1080 -p 5 -b 1 -n maze.png 
else
$prog -w 1920 -h 1080 -p 1 -b 0 -n maze.png 
fi  

#time till it generates a new background
sleep 600;
done
