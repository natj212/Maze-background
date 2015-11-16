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


/*Linux users will need to add -ldl to the Makefile to compile 
 *this example.
 */

#define TRUE	1
#define FALSE	0

class Draw {
private:
png::image < png::rgb_pixel > image;
std::stack <int> pointlist; //it is a stack of interger values, x, y,width,height in that order for drawing a rectangle
std::stack <char *> colorlist; //it is a stack of colors that the top color is used for the current rectangle
void renderRect(); //renders the rectangles inside of the drawScene

public:
int screenwidth,screenheight;
Draw(int,int,int); //Contructor 
void pushRect(char *,int,int,int,int);
void printPoints();//prints all the points
void redraw();
void clearStack(); 
int pixwidth;
void writePng(char *);
void drawRectangle(int,int,int,int,char *);
};


void Draw::writePng(char * name)
{
renderRect();
image.write(name);
}

void Draw::drawRectangle(int x,int y,int width,int height,char * color)
{

std::string rstr,gstr,bstr;

rstr+=color[1]; rstr+=color[2];

gstr+=color[3]; gstr+=color[4];

bstr+=color[5]; bstr+=color[6];

int r,g,b;

r=std::stoi(rstr,nullptr,16);
g=std::stoi(gstr,nullptr,16);
b=std::stoi(bstr,nullptr,16);


for (int h=y;h<(y+height);h++)
{
for (int w=x; w<(x+width);w++)
{
image[w][h]=png::rgb_pixel(r,g,b);
}

}
}


Draw::Draw(int xpixwidth,int width,int height)
{
screenwidth=width;
screenheight=height;

image.resize(width,height); 
pixwidth=xpixwidth;

}


void Draw::clearStack()
{
while (!pointlist.empty())
pointlist.pop();

while (!colorlist.empty())
colorlist.pop();
}

void Draw::pushRect(char * color,int x, int y, int width, int height)
{
pointlist.push(height);
pointlist.push(width);
pointlist.push(y);
pointlist.push(x);
colorlist.push(color);


}

void Draw::printPoints()
{
std::stack <int> temp;//temperarily stores the points
temp=pointlist;
while (!temp.empty())
{
std::cout<<temp.top()<<std::endl;
temp.pop();
}


}

void Draw::renderRect()
{
std::stack <int> tempPoints;
std::stack <char *> tempColor;

tempPoints=pointlist;
tempColor=colorlist;

if (pointlist.size()/4!=(colorlist.size()))
std::cout<<"Error,mismatch between number of colors and points in the rectangle"<<std::endl;

    while(!tempPoints.empty())
{
int x = tempPoints.top();
tempPoints.pop();
int y = tempPoints.top();
tempPoints.pop();
int width = tempPoints.top();
tempPoints.pop();
int height = tempPoints.top();
tempPoints.pop();



drawRectangle(x,y,width,height,tempColor.top());

tempColor.pop();

}


}

//template <class Type>
//std::vector <Type> flip(std::vector <Type> original)
//{
//    std::vector <Type> temp = original;
//    
//    int size=original.size();
//
//for (int i=0; i<=original.size();i++)
//{
//temp[size-i]=original[i];
//}
//
//
//return temp;
//}


//pixwidth is the width in pixels that the walls are
void drawWall(std::vector <bool> wallx, int wallwidth,int wallheight, Draw * drawobj,char * color,double xscale,double yscale,bool isvert)
{
int x;
int y;
int row=wallwidth;
int colum=wallheight;
int pw=drawobj->pixwidth;

//wallx=flip(wallx);

while (!wallx.empty())
{

    int pixwidth=pw;

    if (row<=0)
    {
    colum--;
    row=wallwidth;
    }

x=(row-1)*pw;
y=(colum-1)*pw;

//std::cout<<"row:"<<row<<"colum:"<<colum<<std::endl;
//std::cout<<"x:"<<x<<"	y:"<<y<<std::endl;

if (wallx.back()==1)
{
drawobj->pushRect(color,x,y,pixwidth*xscale+(pixwidth*(!isvert)*yscale),pixwidth*yscale+(pixwidth*isvert*xscale));
}

row--;

wallx.erase(wallx.end());
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
void startCreation(bool);
void nextStep();
void prntdir(direction);
bool checkAnyVis();
int totalblocks;
std::vector<std::vector<int>>values;
void drawValues(Draw *,int,int,int,int,int,int);
};

Maze::Maze (int height,int width)
{

    //the inner vector is the height
    std::vector<bool>empty;
    empty.assign(height+1,0);

    std::vector<int>emptyint;
    emptyint.assign(height+1,0);

    visited.assign(width+1,empty);
    values.assign(width+1,emptyint);

wall.horwalls.assign(width*height+width,1);
wall.vertwalls.assign(width*height+height,1);


wall.width=width;
wall.height=height+1;

values[xpos][ypos]=1;

totalblocks=1;

}


void Maze::breakWall(direction dir)
{

int currentval=values[xpos][ypos];

if (currentval>totalblocks)
    totalblocks=currentval;


switch (dir)
{
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


bool Maze::checkVisited(direction dir)
{
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




void Maze::startCreation(bool xloops)
{
    srand(time(NULL));
xpos=(rand()%(wall.width-1))+1;
ypos=(rand()%(wall.height-1))+1;
    queue.push(xpos);
    queue.push(ypos);
    queue.push(xpos);
    queue.push(ypos);

startx=xpos;
starty=ypos;

loops=xloops;

}

void Maze::moveForward(direction dir)
{
if (dir==left)
    xpos--;
if (dir==right)
    xpos++;
if (dir==down)
    ypos++;
if (dir==up)
    ypos--;
}

bool Maze::checkAnyVis()
{

if (checkVisited(left)==1&&checkVisited(right)==1&&checkVisited(up)==1&&checkVisited(down)==1)
{
    return 1;
}
else
    return 0;
}

void Maze::prntdir(direction dir)
{
if (dir==left)
std::cout<<"left"<<std::endl;
if (dir==right)
std::cout<<"right"<<std::endl;
if (dir==up)
std::cout<<"up"<<std::endl;
if (dir==down)
std::cout<<"down"<<std::endl;
}


void Maze::nextStep()
{
do
{

visited[xpos][ypos]=1;


direction dir=(direction)(rand()%4);

if (checkAnyVis()==0)
{

while (checkVisited(dir)==1)
{
dir=(direction)(rand()%4);
}

breakWall(dir);
moveForward(dir);
queue.push(xpos);
queue.push(ypos);


}
    else
{

ypos=queue.top();
queue.pop();


xpos=queue.top();
queue.pop();

//queue.pop(); queue.pop();


}

if (loops==FALSE)
    break;

}

while (startx!=xpos||starty!=ypos);

}


void drawBlock(int xp, int yp,Draw * drawobj,Maze * maze,char * color)
{
int pixwidth=drawobj->pixwidth;
int x=xp*pixwidth;
int y=yp*pixwidth;

drawobj->pushRect(color,x-pixwidth,y-pixwidth,pixwidth,pixwidth);
}

std::string stringColor(int r, int g, int b)
{
std::string text;

int colors[]={r,g,b};

text+='#';

for (int i=0;i<3;i++)
{
char foo[10];
sprintf(foo,"%x",colors[i]);

if (colors[i]<16)
text+='0';

text+=foo;
}


return text;
}

void Maze::drawValues(Draw * xwin,int ra,int ga, int ba,int rb, int gb, int bb)
{
for (int h=1;h<wall.height;h++)
{
for (int w=1;w<=wall.width;w++)
{

//    std::cout<<values[w][h]<<std::endl;

double ratio = (float)values[w][h]/(float)(totalblocks+1);


ratio+=0.0000000001;

int rc,gc,bc;

rc=(ra*ratio)+(rb*(1.0-ratio));
gc=(ga*ratio)+(gb*(1.0-ratio));
bc=(ba*ratio)+(bb*(1.0-ratio));


std::string tmp=stringColor(rc,gc,bc);


char *color = new char[6];
    
color=strcpy(color,tmp.c_str());

drawBlock(w,h,xwin,this,color);
}

}

}

int main(int argc,char *argv[]) {




char green[] ="#00FF00";
char red[] ="#ff0000";
char white[]="#FFFFFF";
char black[]="#000000";

int xres=1920;
int yres=1080;
int pixsize=10;
//if wallstrue equals zero then there will be no walls, else there will be walls
int wallstrue=1;
char * name="maze.png";

//if (argc>1)
//xres=std::stoi(argv[1]);
//if (argc>2)
//yres=std::stoi(argv[2]);
//if (argc>3)
//pixsize=std::stoi(argv[3]);
//if (argc>4)
//wallstrue=std::stoi(argv[4]);
//if (argc>5)
//name=argv[5];


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


}


Draw xwin(pixsize,xres,yres);

int xblocks=xwin.screenwidth/xwin.pixwidth-1;
int yblocks=xwin.screenheight/xwin.pixwidth-1;

Maze maze(xblocks,yblocks);

//1 means it loops
maze.startCreation(1);


maze.nextStep();


if (wallstrue!=0)
{
	//horizontal
	drawWall(maze.wall.horwalls,maze.wall.width,maze.wall.height,&xwin,black,1.0,1.0/2.0,0);
	//verticle
	drawWall(maze.wall.vertwalls,maze.wall.width+1,maze.wall.height-1,&xwin,black,1.0/2.0,1.0,1);

}

//	drawBlock(maze.xpos,maze.ypos,&xwin,&maze,red_gc);


	int colors[6];

	for (int i=0;i<6;i++)
	{
	colors[i]=rand()%256;
	}

	double distance =sqrt(  pow((colors[0]-colors[3]),2) + pow((colors[1]-colors[4]),2) + pow((colors[3]-colors[5]),2));

	while (distance<200|distance>300)
	{
	for (int i=0;i<6;i++)
	colors[i]=rand()%256;
	distance =sqrt(  pow((colors[0]-colors[3]),2) + pow((colors[1]-colors[4]),2) + pow((colors[3]-colors[5]),2));
	}


maze.drawValues(&xwin,colors[0],colors[1],colors[2],colors[3],colors[4],colors[5]);

//	maze.drawValues(&xwin,255,255,255,0,0,0);



xwin.writePng(name);



//	xwin.printPoints();
//	xwin.drawScene();



xwin.clearStack();

return 0;
}
