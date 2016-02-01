#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <stack>
#include <random>
#include <vector>
#include <time.h>
#include <math.h>
#include <png++/png.hpp>
#include <string>
#include <sstream>

#define TRUE	1
#define FALSE	0

class Draw {
    private:
	png::image < png::rgb_pixel > image;
    public:
	int screenwidth,screenheight;
	Draw(int,int,int); //Contructor 
	int pixwidth;
	void writePng(char *);
	void drawRectangle(int,int,int,int,png::rgb_pixel);
};


void Draw::writePng(char * name) {
    image.write(name);
}

void Draw::drawRectangle(int x,int y,int width,int height,png::rgb_pixel pixel) {

    for (int h=y;h<(y+height);h++) {
	for (int w=x; w<(x+width);w++) {
	    image[w][h]=pixel;
	}
    }
}


Draw::Draw(int xpixwidth,int width,int height) {
    screenwidth=width;
    screenheight=height;

    image.resize(width,height); 
    pixwidth=xpixwidth;
}





//pixwidth is the width in pixels that the walls are
void drawWall(std::vector <bool> wallx, int wallwidth,int wallheight, Draw * drawobj,png::rgb_pixel pixel,double xscale,double yscale,bool isvert) {
    int x;
    int y;
    int row=wallwidth;
    int colum=wallheight;
    int pw=drawobj->pixwidth;

    while (!wallx.empty()) {
	int pixwidth=pw;

	if (row<=0) {
	    colum--;
	    row=wallwidth;
	}

	x=(row-1)*pw;
	y=(colum-1)*pw;

	if (wallx.back()==1) {
	    int width = pw*xscale+(pw*(!isvert)*yscale);
	    int height = pw*yscale+(pw*isvert*xscale);
	    drawobj->drawRectangle(x,y,width,height,pixel);


	}

	row--;

	wallx.pop_back();
    }
}


class Maze {
    public:
	bool loops=0;
	int xpos=1;
	int ypos=1;
	int startx=1;
	int starty=1;
	struct walls { std::vector <bool> horwalls,vertwalls;int width; int height;}wall;
	enum direction {left,right,up,down};
	std::vector <std::vector<bool>> visited;
	std::stack <int> queue;
	Maze(int,int);
	void breakWall(direction);
	bool checkVisited(direction);
	void moveForward(direction);
	void startCreation(int);
	void nextStep();
	void prntdir(direction);
	bool checkAnyVis();
	int totalblocks;
	std::vector<std::vector<int>>values;
	void drawValues(Draw *,int,int,int,int,int,int,int);
};
//Height is the number of blocks along the horizontal axis, you can guess what width is
Maze::Maze (int height,int width) {

    //the inner vector is the height
    std::vector < bool > empty;
    empty.assign (height + 1, 0);

    std::vector < int >emptyint;
    emptyint.assign (height + 1, 0);

    visited.assign (width + 1, empty);
    values.assign (width + 1, emptyint);

    wall.horwalls.assign(width*height+width,1);
    wall.vertwalls.assign(width*height+height,1);


    wall.width=width;
    wall.height=height+1;

    values[xpos][ypos]=1;

    totalblocks=1;

}


void Maze::breakWall(direction dir) {

    int currentval=values[xpos][ypos];

    if (currentval>totalblocks)
	totalblocks=currentval;


    switch (dir) {
	case left:
	    wall.vertwalls[(ypos*wall.width)+xpos+ypos-wall.width-2]=0;
	    values[xpos-1][ypos]=currentval+1;
	    break;
	case right:
	    wall.vertwalls[(ypos*wall.width)+xpos+ypos-wall.width-1]=0; 
	    values[xpos+1][ypos]=currentval+1;
	    break;
	case down:
	    wall.horwalls[(ypos*wall.width)+xpos-1]=0;
	    values[xpos][ypos+1]=currentval+1;
	    break;
	case up:
	    wall.horwalls[(ypos*wall.width)+xpos-1-wall.width]=0;
	    values[xpos][ypos-1]=currentval+1;
	    break;
    }
}


bool Maze::checkVisited(direction dir) {
    int xcheck=0;
    int ycheck=0;

    switch (dir) {
	case right:
	    if (xpos==wall.width)
		return 1;
	    xcheck=xpos+1;
	    ycheck=ypos;
	    break;

	case down:
	    if (ypos==wall.height-1)
		return 1;
	    xcheck=xpos;
	    ycheck=ypos+1;
	    break;

	case up:
	    if (ypos==1)
		return 1;
	    xcheck=xpos;
	    ycheck=ypos-1;
	    break;

	case left:
	    if (xpos==1)
		return 1;
	    //if (visited[xpos-1][ypos]==1)
	    xcheck=xpos-1;
	    ycheck=ypos;
	    break;
    }

    if (visited[xcheck][ycheck]==1)
	return 1;


    return 0;
}




void Maze::startCreation(int seed) {
    srand(seed);
    xpos=(rand()%(wall.width-1))+1;
    ypos=(rand()%(wall.height-1))+1;
    queue.push(xpos);
    queue.push(ypos);
    queue.push(xpos);
    queue.push(ypos);

    startx=xpos;
    starty=ypos;

}

void Maze::moveForward(direction dir) {
    if (dir==left)
	xpos--;
    if (dir==right)
	xpos++;
    if (dir==down)
	ypos++;
    if (dir==up)
	ypos--;
}

bool Maze::checkAnyVis() {

    if (checkVisited(left)==1&&checkVisited(right)==1&&checkVisited(up)==1&&checkVisited(down)==1) {
	return 1;
    } else {
	return 0;
    }
}

void Maze::prntdir(direction dir) {
    if (dir==left)
	std::cout<<"left"<<std::endl;
    if (dir==right)
	std::cout<<"right"<<std::endl;
    if (dir==up)
	std::cout<<"up"<<std::endl;
    if (dir==down)
	std::cout<<"down"<<std::endl;
}


void Maze::nextStep() {
    do {

	visited[xpos][ypos]=1;

	direction dir=(direction)(rand()%4);

	if (checkAnyVis()==0) {

	    while (checkVisited(dir)==1) {
		dir=(direction)(rand()%4);
	    }

	    breakWall(dir);
	    moveForward(dir);
	    queue.push(xpos);
	    queue.push(ypos);
	} else {

	    ypos=queue.top();
	    queue.pop();


	    xpos=queue.top();
	    queue.pop();

	    //queue.pop(); queue.pop();
	}

    } while (startx!=xpos||starty!=ypos);

}

void drawBlock(int xp, int yp,Draw * drawobj,Maze * maze,int r, int g, int b) {
    int pixwidth=drawobj->pixwidth;
    int x=xp*pixwidth;
    int y=yp*pixwidth;

    drawobj->drawRectangle(x-pixwidth,y-pixwidth,pixwidth,pixwidth,png::rgb_pixel(r,g,b));
//    drawobj->pushRect(color,x-pixwidth,y-pixwidth,pixwidth,pixwidth);
}


void Maze::drawValues(Draw * xwin,int ra,int ga, int ba,int rb, int gb, int bb,int colormult) {

    for (int h=1;h<wall.height;h++) {
	for (int w=1;w<=wall.width;w++) {

	    double ratio = (float)values[w][h]*colormult/(float)(totalblocks+1);

	    if (ratio > 1.0)
		ratio -= (float)((int) ratio);

	    ratio+=0.0000000001;

	    int rc,gc,bc;

	    rc=(ra*ratio)+(rb*(1.0-ratio));
	    gc=(ga*ratio)+(gb*(1.0-ratio));
	    bc=(ba*ratio)+(bb*(1.0-ratio));

	    drawBlock(w,h,xwin,this,rc,gc,bc);
    
	}

    }

}




void desaturate(float k, int *r, int *g, int *b) {
    double rf = double(*r)/256.0, gf = double(*g)/256.0, bf = double(*b)/256.0;
    double offset = rf * 0.3 + gf * 0.59 + bf * 0.11;
    *r = (offset * k + rf * (1 - k)) * 256;
    *g = (offset * k + gf * (1 - k)) * 256;
    *b = (offset * k + bf * (1 - k)) * 256;
}


int main(int argc,char *argv[]) {

    png::rgb_pixel black = png::rgb_pixel(0,0,0);

    //default resolution
    int xres=1920;
    int yres=1080;
    //default size of the pixel
    int pixsize=10;
    // 0 - Original colors, 100 - Complete desaturation
    int saturation=0;
    //Color multiplier value
    int colormult=1;
    //seed
    int seed=time(NULL);

    //if wallstrue equals zero then there will be no walls, else there will be walls
    int wallstrue=1;
    char * name="maze.png";
    //true if user picks the colors
    bool setColor=false;
    std::string userColor;

    for (int i=1;i<argc;i++)
    {
	std::string flag=argv[i];
	if (flag=="-w"||flag=="--width")
	    xres=std::stoi(argv[i+1]);
	if (flag=="-h"||flag=="--height")
	    yres=std::stoi(argv[i+1]);
	if (flag=="-p"||flag=="--pixel-size")
	    pixsize=std::stoi(argv[i+1]);
	if (flag=="-b"||flag=="--walls")
	    wallstrue=std::stoi(argv[i+1]);
	if (flag=="-n"||flag=="--file-name")
	    name=argv[i+1];
	if (flag=="-d"||flag=="--desaturation")
	    saturation=std::stoi(argv[i+1]);
	if (flag=="-t"||flag=="--color-multiplier")
	    colormult=std::stoi(argv[i+1]);
	if (flag=="-s"||flag=="--seed")
	    seed=std::stoi(argv[i+1]);
	if (flag=="-c"||flag=="--set-color")
	    {
		setColor=true;
		userColor=argv[i+1];
	    }


    }

    //ignores drawing walls if pixel size is 1
    if (pixsize==1)
	wallstrue=0;

    Draw xwin(pixsize,xres,yres);

    int xblocks=xwin.screenwidth/xwin.pixwidth-1;
    int yblocks=xwin.screenheight/xwin.pixwidth-1;

    Maze maze(xblocks,yblocks);

    maze.startCreation(seed);

    maze.nextStep();

    int colors[6];

    if (setColor==false)
    {

    for (int i=0;i<6;i++) {
	colors[i]=rand()%256;
    }

    double distance =sqrt(  pow((colors[0]-colors[3]),2) + pow((colors[1]-colors[4]),2) + pow((colors[3]-colors[5]),2));

    while (distance<200|distance>300) {
	for (int i=0;i<6;i++)
	    colors[i]=rand()%256;
	distance =sqrt(  pow((colors[0]-colors[3]),2) + pow((colors[1]-colors[4]),2) + pow((colors[3]-colors[5]),2));
    }

    desaturate(saturation / 100.0, &colors[0], &colors[1], &colors[2]);
    desaturate(saturation / 100.0, &colors[3], &colors[4], &colors[5]);

    }
    else
    {
	for (int i=0;i<12;i+=2)
	{
	    unsigned int col;
	    std::stringstream ss;
	    ss << std::hex << std::string(userColor.begin()+i,userColor.begin()+i+2);
	    ss >> col;
	    colors[i/2]=col;
	}
	
    }

    maze.drawValues(&xwin,colors[0],colors[1],colors[2],colors[3],colors[4],colors[5],colormult);

    if (wallstrue!=0) {
	//horizontal
	drawWall(maze.wall.horwalls,maze.wall.width,maze.wall.height,&xwin,black,1.0,1.0/2.0,0);
	//verticle
	drawWall(maze.wall.vertwalls,maze.wall.width+1,maze.wall.height-1,&xwin,black,1.0/2.0,1.0,1);

    }

    xwin.writePng(name);

    return 0;
}
