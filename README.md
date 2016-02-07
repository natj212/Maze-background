# Maze-background

![alt tag](http://i.imgur.com/TRiEHc3.png)


Run this program to generate a randomly generated png file of a coloured maze.

Dependancies

[png++](http://www.nongnu.org/pngpp/)  
  
Options

    -w, --width
        Sets the width of the png.

    -h, --height
        Sets the height of the png.
        
    -p, --pixel-size
        Sets the size of each block in the maze.
        
    -b, --walls
        To disable walls set as 0
        
    -n, --file-name
        Set the file name for the output.
        
    -d, --desaturate
        Desaturate colors where 0 is original color and 100 is complete desaturation
        
    -s, --seed
	Sets the random seed.
	
    -t, --color-multiplier
	Sets the number of times you will shift through the colours

    -c, --set-color
	Sets the colors to shift through, for example FDF0AB42F8CD
	



Here's the blogpost that give me the inspiration for this
https://danceswithpotatoes.wordpress.com/

Original idea:
http://www.gamasutra.com/blogs/LaurentVictorino/20141202/231321/Mazes_hidden_beauty.php

Tmathmeyer has created a very similar maze generator to this which is much faster. If the performance for this project is a problem check out his project.
https://github.com/tmathmeyer/maze

