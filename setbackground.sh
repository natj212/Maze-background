#!/bin/bash

#script to set the desktop background to a randomly generated maze
while true; do

feh --bg-center maze.png &

ODDS=2
T=1
number=$RANDOM
colmult=$RANDOM

#location of mazepng
prog=~/documents/git/myprojects/Maze-background/Maze-background/mazepng

let "number %= $ODDS"
let "colmult %=100"
if [ "$number" -eq $T ]
then
$prog -w 1920 -h 1080 -p 20 -b 1 -n -t $colmult maze.png 
else
$prog -w 1920 -h 1080 -p 1 -b 0 -n maze.png 
fi  

#time till it generates a new background
sleep 600;
done
