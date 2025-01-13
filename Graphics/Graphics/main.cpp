//final project - ariela epstein 328606686 , first run erosion by right clicking and then selecting the first option from the menu
//then right click again and select place city. Erosion might take 30 seconds or so.
#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "glut.h"
#include <iostream>




const double PI = 3.14;
const int GSZ = 100; 
const int H = 600;
const int W = 600;

const int TH = 512; // must be a power of 2
const int TW = 512; // must be a power of 2

unsigned char tx0[TH][TW][3];

double angle = 0;

typedef struct {
	double x, y, z;
} POINT3;

POINT3 eye = {2,15,10 };

double sight_angle = PI;
POINT3 sight_dir = {sin(sight_angle),0,cos(sight_angle)}; // in plane X-Z
double speed = 0;
double angular_speed = 0;



double ground[GSZ][GSZ] = { 0 };
double river[GSZ][GSZ] = { 0 };
int riverX[10];
int riverY[10];
int housesX[GSZ * GSZ];

int housesY[GSZ * GSZ];
int houseColor[GSZ * GSZ];
double tmp[GSZ][GSZ];

bool isCaptured = false;

void UpdateGround2();
void UpdateGround3();
void Smooth();

void Smooth();


void init()
{
	//                 R     G    B
	glClearColor(0.5,0.7,1,0);// color of window background

	glEnable(GL_DEPTH_TEST);


	int i, j;
	double dist;

	srand(time(0));

	for (int i = 0; i < GSZ; ++i) {
		for (int j = 0; j < GSZ; j++)
		{
			ground[i][j] = 0;
		}
	}
	for (i = 0;i < 3000;i++)
		UpdateGround3();

	Smooth();
	for (i = 0;i < 1000;i++)
		UpdateGround3();

	for (int i = 0; i < GSZ*GSZ; ++i) {
		housesX[i] = -1;
	}
	for (int i = 0; i < GSZ * GSZ; ++i) {
		housesY[i] = -1;
	}
	for (int i = 0; i < 10; ++i) {
		riverX[i] = -1;
	}
	for (int i = 0; i < 10; ++i) {
		riverY[i] = -1;
	}
	for (int i = 0; i < GSZ * GSZ; ++i) {
		houseColor[i] = -1;
	}
	for (int i = 0; i < GSZ; ++i) {
		for (int j = 0; j < GSZ; j++)
		{
			river[i][j] =0;
		}
	}
	
	glEnable(GL_NORMALIZE);



}



void DrawRoof(int levels, int color)
{

	glBegin(GL_POLYGON);
	if (color == 1)
		glColor3d(0.6, 0.0, 0.0);
	else if (color == 2)
		glColor3d(0.6, 0.6, 0.6);
	else if (color == 3)
		glColor3d(0, 0, 0.6);
	glVertex3d(0.5 * levels, .5 + levels, 0.5 * levels);
	glVertex3d(1 * levels, 1 * levels, 0);
	glVertex3d(0, 1 * levels, 0);
	glEnd();

	glBegin(GL_POLYGON);
	if (color == 1)
		glColor3d(0.4, 0.0, 0.0);
	else if (color == 2)
		glColor3d(0.4, 0.4, 0.4);
	else if (color == 3)
		glColor3d(0, 0, 0.4);
	glVertex3d(1 * levels, 1 * levels, 0);
	glVertex3d(1 * levels, 1 * levels, 1 * levels);
	glVertex3d(0.5 * levels, .5 + levels, 0.5 * levels);
	glEnd();

	glBegin(GL_POLYGON);
	if (color == 1)
		glColor3d(0.2, 0.0, 0.0);
	else if (color == 2)
		glColor3d(0.2, 0.2, 0.2);
	else if (color == 3)
		glColor3d(0, 0, 0.2);
	glVertex3d(1 * levels, 1 * levels, 1 * levels);
	glVertex3d(0, 1 * levels, 1 * levels);
	glVertex3d(0.5 * levels, .5 + levels, 0.5 * levels);
	glEnd();

	glBegin(GL_POLYGON);
	if (color == 1)
		glColor3d(0.4, 0.0, 0.0);
	else if (color == 2)
		glColor3d(0.4, 0.4, 0.4);
	else if (color == 3)
		glColor3d(0, 0, 0.4);
	glVertex3d(0, 1 * levels, 1 * levels);
	glVertex3d(0, 1 * levels, 0);
	glVertex3d(0.5 * levels, .5 + levels, 0.5 * levels);
	glEnd();


}
void DrawBase(int levels)
{

	//draw walls
	glBegin(GL_POLYGON);
	glColor3d(1.0, 0.7, 0.3);
	glVertex3d(0, 0, 0);
	glVertex3d(1 * levels, 0, 0);
	glVertex3d(1 * levels, 1 * levels, 0);
	glVertex3d(0, 1 * levels, 0);

	glEnd();
	glBegin(GL_POLYGON);
	glColor3d(0.8, 0.56, 0.24);
	glVertex3d(1 * levels, 0, 0);
	glVertex3d(1 * levels, 1 * levels, 0);
	glVertex3d(1 * levels, 1 * levels, 1 * levels);
	glVertex3d(1 * levels, 0, 1 * levels);

	glEnd();
	glBegin(GL_POLYGON);
	glColor3d(0.6, 0.42, 0.18);
	glVertex3d(1 * levels, 1 * levels, 1 * levels);
	glVertex3d(1 * levels, 0, 1 * levels);
	glVertex3d(0, 0, 1 * levels);
	glVertex3d(0, 1 * levels, 1 * levels);

	glEnd();

	glBegin(GL_POLYGON);
	glColor3d(0.8, 0.56, 0.24);
	glVertex3d(0, 1 * levels, 1 * levels);
	glVertex3d(0, 0, 1 * levels);
	glVertex3d(0, 0, 0);
	glVertex3d(0, 1 * levels, 0);

	glEnd();

}
void DrawWindows(int n, int levels)
{
	//draw windows
	glColor3d(0, 0, 0.3);
	double windowHeight = 0.4;
	double wallSize = levels;
	double windowSize = 0.8 / (n + 1);
	double windowSpacing = (wallSize - (n * windowSize)) / (n + 1);
	double startX = windowSpacing;

	for (int l = 0; l < levels; l++)
	{
		for (int i = 0; i < n; ++i)
		{

			double currentStartX = startX + i * (windowSize + windowSpacing);
			double currentEndX = currentStartX + windowSize;

			//wall 1 windows
			glBegin(GL_POLYGON);
			glVertex3d(currentStartX, 0.3 + l, wallSize + .01);
			glVertex3d(currentEndX, 0.3 + l, wallSize + .01);
			glVertex3d(currentEndX, 0.3 + l + windowHeight, wallSize + .01);
			glVertex3d(currentStartX, 0.3 + l + windowHeight, wallSize + .01);
			glEnd();

			//wall 2 windows
			glBegin(GL_POLYGON);
			glVertex3d(wallSize + .01, .3 + l, currentStartX);
			glVertex3d(wallSize + .01, .3 + l, currentEndX);
			glVertex3d(wallSize + .01, 0.3 + l + windowHeight, currentEndX);
			glVertex3d(wallSize + .01, 0.3 + l + windowHeight, currentStartX);
			glEnd();

			//wall 3 windows
			glBegin(GL_POLYGON);
			glVertex3d(currentEndX, 0.3 + l + windowHeight, -.01);
			glVertex3d(currentEndX, .3 + l, -.01);
			glVertex3d(currentStartX, .3 + l, -.01);
			glVertex3d(currentStartX, 0.3 + l + windowHeight, -.01);
			glEnd();

			//wall 4 windows
			glBegin(GL_POLYGON);
			glVertex3d(-.01, 0.3 + l + windowHeight, currentEndX);
			glVertex3d(-.01, .3 + l, currentEndX);
			glVertex3d(-.01, .3 + l, currentStartX);
			glVertex3d(-.01, 0.3 + l + windowHeight, currentStartX);
			glEnd();
		}
	}

}

// simple random heights
void UpdateGround1()
{
	double delta = 0.2;
	if (rand() % 2 == 0)
		delta = -delta;
	int x, z;

	for (int i = 0;i < 100;i++) {
		x = rand() % GSZ;
		z = rand() % GSZ;

		ground[z][x] += delta;
	}

}

void UpdateGround2()
{
	double delta = 0.04;
	if (rand() % 2 == 0)
		delta = -delta;
	int x, z, num_pts = 5000;
	x = rand() % GSZ;
	z = rand() % GSZ;

	for (int i = 0;i < num_pts;i++) {
		ground[z][x] += delta;
		switch (rand() % 4)
		{
		case 0: // right
			x++;
			break;
		case 1: // left
			x--;
			break;
		case 2: // down
			z++;
			break;
		case 3: // up
			z--;
			break;
		}
		if (x > GSZ) x -= GSZ;
		if (x < 0) x += GSZ;
		if (z > GSZ) z -= GSZ;
		if (z < 0) z += GSZ;
	}

}

void UpdateGround3()
{
	double delta = 0.04;
	if (rand() % 2 == 0)
		delta = -delta;
	int x1, y1, x2, y2;
	x1 = rand() % GSZ;
	y1 = rand() % GSZ;
	x2 = rand() % GSZ;
	y2 = rand() % GSZ;
	double a, b;
	if (x1 != x2)
	{
		a = (y2 - y1) / ((double)x2 - x1);
		b = y1 - a * x1;
		for(int i=0;i<GSZ;i++)
			for (int j = 0;j < GSZ;j++)
			{
				if (i < a * j + b) ground[i][j] += delta;
				else ground[i][j] -= delta;
			}
	}


}

void Smooth()
{

	for(int i=1;i<GSZ-1;i++)
		for (int j = 1;j < GSZ - 1;j++)
		{
			tmp[i][j] = (ground[i-1][j-1]+ ground[i-1 ][j]+ ground[i - 1][j + 1]+
				ground[i][j - 1] + ground[i ][j] + ground[i ][j + 1]+
				ground[i + 1][j - 1] + ground[i + 1][j] + ground[i + 1][j + 1]) / 9.0;
		}

	for (int i = 1;i < GSZ - 1;i++)
		for (int j = 1;j < GSZ - 1;j++)
			ground[i][j] = tmp[i][j];

}


void SetColor(double h)
{
	h = h / 6;
	
		// sand
	    if (h > 0 && h < 0.02)
			glColor3d(0.8, 0.7, 0.5);
		else	if (h > 0 && h < 0.3)// grass
			glColor3d(0.3 + 0.7 * h, 0.6 - 0.6 * h, 0.2 + 0.2 * h);
		else if (h > 0 && h < 0.6) // stones
			glColor3d(0.3 + 0.25 * h, 0.3 + 0.18 * h, 0.3 + 0.13 * h);
		else if (h > 0 && h > 0.6)  // snow
			glColor3d(h, h, 1.1 * h);
		else 
			glColor3d(0.8, 0.7, 0.5);
	

}

void SetColorRiver(double h)
{
	h = h / 6;
	if (h >= 0 && h < 0.0002)
		glColor4d(0, 0.4, 0.7, 0.7);		
	else if (h >= 0.0003 && h < .0006)
		glColor4d(0.15f, 0.5f, 0.75f, 0.7);
	else  // h >=.666
	glColor4d(0.25f, 0.6f, 0.81f, 0.7);
	
	
}



void DrawFloor()
{
	int i,j;

	glColor3d(0, 0, 0.3);

	for(i=1;i<GSZ-1;i++)
		for (j = 1;j < GSZ-1;j++)
		{
			glBegin(GL_POLYGON);
			SetColor(ground[i][j]);
//			SetNormal(i, j);
			glVertex3d(j-GSZ/2, ground[i][j], i-GSZ/2);
			SetColor(ground[i-1][j]);
//			SetNormal(i-1, j);
			glVertex3d(j - GSZ / 2, ground[i - 1][j], i - 1 - GSZ / 2);
			SetColor(ground[i-1][j-1]);
//			SetNormal(i-1, j-1);
			glVertex3d(j - 1 - GSZ / 2, ground[i - 1][j - 1], i - 1 - GSZ / 2);
			SetColor(ground[i][j-1]);
//			SetNormal(i, j-1);
			glVertex3d(j - 1 - GSZ / 2, ground[i ][j - 1], i - GSZ / 2);
			glEnd();
		}
	

}
void drawRiver()
{ 

	int i, j;

	for (i = 1; i < GSZ - 1; i++)
	{
		for (j = 1; j < GSZ - 1; j++)
		{
			{

					glBegin(GL_POLYGON);
					SetColorRiver(river[i][j]);
					glVertex3d(j - GSZ / 2, river[i][j], i - GSZ / 2);
					SetColorRiver(river[i - 1][j]);
					glVertex3d(j - GSZ / 2, river[i - 1][j], i - 1 - GSZ / 2);
					SetColorRiver(river[i - 1][j - 1]);
					glVertex3d(j - 1 - GSZ / 2, river[i - 1][j - 1], i - 1 - GSZ / 2);
					SetColorRiver(river[i][j - 1]);
					glVertex3d(j - 1 - GSZ / 2, river[i][j - 1], i - GSZ / 2);
					glEnd();
				
			}
		}
	}

}
void drawHouses()
{
	//glPushMatrix();
	for (int i = 0; i <GSZ*GSZ; i++ )
	{
		int houseX = housesX[i];
		int houseY = housesY[i];
		if (housesX[i] != -1)
		{
			glPushMatrix();
			glTranslated(houseY - GSZ / 2, ground[houseX][houseY], houseX - GSZ / 2 );
			glScaled(.3, .3, .3);
			DrawBase(2);
			DrawRoof(2, houseColor[i]);
			DrawWindows(3, 2);
			glPopMatrix();
		}

	}
	//glPopMatrix();
	

}
// put all the drawings here
void display()
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); // clean frame buffer and Z-buffer
	glViewport(0, 0, W, H);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity(); // unity matrix of projection

	glFrustum(-1, 1, -1, 1, 0.75, 300);
	gluLookAt(eye.x, eye.y, eye.z,  // eye position
		eye.x+ sight_dir.x, eye.y-0.3, eye.z+sight_dir.z,  // sight dir
		0, 1, 0);


	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity(); // unity matrix of model


	DrawFloor();
	drawRiver();
	drawHouses();
	
	glutSwapBuffers(); // show all
}

void idle() 
{
	int i, j;
	double dist;
	angle += 0.1;

	

	// ego-motion  or locomotion
	sight_angle += angular_speed;
	// the direction of our sight (forward)
	sight_dir.x = sin(sight_angle);

	sight_dir.z = cos(sight_angle);
	// motion
	eye.x += speed * sight_dir.x;
	eye.y += speed * sight_dir.y;
	eye.z += speed * sight_dir.z;

	
	glutPostRedisplay();
}


void SpecialKeys(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_LEFT: // turns the user to the left
		angular_speed += 0.0004;
		break;
	case GLUT_KEY_RIGHT:
		angular_speed -= 0.0004;
		break;
	case GLUT_KEY_UP: // increases the speed
		speed+= 0.0005;
		break;
	case GLUT_KEY_DOWN:
		speed -= 0.0005;
		break;
	case GLUT_KEY_PAGE_UP:
		eye.y += 0.1;
		break;
	case GLUT_KEY_PAGE_DOWN:
		eye.y -= 0.1;
		break;

	}
}
int sumUp(int start, int limit)
{
	int summed = 0;
	while(start< limit)
	{
		summed += start;
		++start;
	}
	return summed;
}
void trace(int x, int y, double height, int counter, int riverX[], int riverY[])
{
	//we need to limit the recursion- so if we reached a neighbor who is already at/below sea level -> stop 
	//or if we've done 10 rounds for this raindrop -> stop
	if (height <= 0 || counter == 10)
	{
		riverX[counter - 1] = x;
		riverY[counter - 1] = y;
		std::cout << "trace finished"<< std::endl;	
	}
	else
	{
		//add the raindrop's stop to the arrays
		riverX[counter - 1] = x;
		riverY[counter-1] = y;

		double originalHeight = height;
		double min = height;
		int newX, newY;
		double slope = 0;

		//check this spot's surrounding neighbors- for shortest neighbor
		for (int i = x - 1; i < x + 2; i++)
		{
			for (int j = y - 1; j < y + 2; j++)
			{
				if (ground[i][j] < min)
				{
					min = ground[i][j];
					newX = i;
					newY = j;
					slope = (originalHeight - min)/6;
				}
			}
		}

		if (min == originalHeight) //if we've reached the lowest spot in this trace then raise this spot with all the dirt that came so far
		{
			if (slope >= .666)
				ground[x][y] += (.0001 * sumUp(2, counter+1));
			else if (slope >= .333) 
				ground[x][y] += (.0001 * sumUp(1, counter));
			else 
				ground[x][y] += (.0001 * sumUp(0, counter -1));
		}
		else
			//if we found a shorter neighbor then shorten this spot and push the dirt and water to the neighbor
		{
			if(slope >= .666) //then the slope is steep -> more dirt should trickle down
				ground[x][y] -= (.0001 * (counter+1));
			else if(slope >= .333) //the slope is avg -> an avg amount of dirt trickles down
				ground[x][y] -= (.0001 * (counter));
			else //the slope is tiny -> not much dirt trickles down
				ground[x][y] -= (.0001 * (counter -1));
			//run trace on the neighbor
			trace(newX, newY, min, ++counter, riverX, riverY);
		}

	}
	
}
void erode()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clean frame buffer and Z-buffer
	
	srand(time(NULL));
	int x, y, newX, newY, counter;
	double originalHeight, min;

	// Randomly selects 1000 spots in the matrix for rain to fall at (takes a bit of time)
	for (int spots = 0; spots < 1000; spots++)
	{
		counter = 0;
		do //pick random x, y, that has a height greater than the sea
		{
			x = rand() % (GSZ);
			y = rand() % (GSZ);
			if (ground[x][y] <= 0) 
				counter++;
		} while (ground[x][y] <= 0 && counter < 5);
		
		//if we could not find an x,y then break
		if (counter >= 5) 
			std::cerr << "error - cannot find a valid x,y value" << std::endl;

		//we have x,y -> shorten these spots and make a river/sea/lake
		else 
		{
			//empty riverX riverY arrays
			for (int i = 0; i < 10; ++i) 
			{
				riverX[i] = -1;
				riverY[i] = -1;
			}
			//get water trace (aka fill the riverX, riverY arrays and update the ground matrix)
			trace(x, y, ground[x][y], 1, riverX, riverY);

			//get the length of the riverX, riverY arrays
			int lastFilledIndex = -1;
			for (int i = 0; i < 10; ++i) 
			{
				if (riverX[i] != -1) 
					lastFilledIndex = i;
				else 
					break; 
			}
			//update/raise the river matrix
			for (int i = 0; i < lastFilledIndex; i++)
			{
				//calculate the slopes between each consecutive x,y in the trace
				double slope = (ground[riverX[i]][riverY[i]] - ground[riverX[i + 1]][riverY[i + 1]]) / 6;

				if (slope >= .666) //if the slope was steep -> so not much water collects above (most/all falls down)
					river[riverX[i]][riverY[i]] += 0;
				else if (slope >= .333) //slope was avg -> some water collects in this spot
				{
					//make sure new water added wont exceed too high above the ground
					if (river[riverX[i]][riverY[i]] + .0001 < ground[riverX[i]][riverY[i]] + .0002)
						river[riverX[i]][riverY[i]] += .0001;
				}
				else //slope was small -> so lots of water collects here (over a larger surface)
				{
					if (river[riverX[i]][riverY[i]] + .00015 < ground[riverX[i]][riverY[i]] + .0002)
						river[riverX[i]][riverY[i]] += .00015;
				}
				
			}
			
		}

	}
	// Redraw the new floor and new river
	DrawFloor();
	drawRiver();
	glutSwapBuffers();
		

}
bool distanceFromRiver2(int row, int col, int size, int max)
{
	// Check if the area exceeds the matrix bounds
	if (row - max < 0 || col - max < 0 || row - max + (2 * max) + size >= GSZ || col - max + (2 * max) + size >= GSZ)
		return false;
	//need to check that we are not too far from the river/lake/inlet
	for (int i = row - max; i < row - max + (2 * max) + size; i++)
	{
		for (int j = col - max; j < col - max + (2 * max) + size; j++)
		{
			if (river[i][j] > 0 && river[i][j] >= ground[i][j] + .0001)
			{

				//a raised water exists but check that its a river/lake (land on either side of it)
				for (int land = i - 1; land <= i + 1; land+=2)
				{
					for (int land2 = j - 1; land2 <= j + 1; land2+= 2)
					{	//check that theres land somewhere around the river/inlet/lake spot
						if (land < 0 || land2 < 0 || land > GSZ || land2 > GSZ)
							return false;
						if (ground[land][land2] < river[land][land2])
							return false;
					}
				}
				return true;
			}
				
		}
	}
	return false;
}
bool distanceFromRiver(int row, int col, int size, int min)
{
	// Check if the area exceeds the matrix bounds
	if (row - min < 0 || col - min < 0 || row - min + (2 * min) + size > GSZ || col - min + (2 * min) + size > GSZ)
		return false;
	//need to check that we are not too close to the river/lake/inlet
	for (int i = row - min; i < row - min + (2 * min) + size; i++)
	{
		for (int j = col - min; j < col - min + (2 * min) + size; j++)
		{
			//the river/lake/inlet is not in this radius = the river is > 0 and land is below the river
			if (river[i][j] > 0 && river[i][j] >= ground[i][j] + .0001)
				return false;
		}
	}

	return true;

}
bool distanceFromSea2(int row, int col, int size, int max)
{
	// Check if the area exceeds the matrix bounds
	if (row - max < 0 || col - max < 0 || row - max + (2 * max) + size >= GSZ || col - max + (2 * max) + size >= GSZ)
		return false;
	//need to check that we are not too far from the sea
	for (int i = row - max; i < row - max + (2 * max) + size; i++)
	{
		for (int j = col - max; j < col - max + (2 * max) + size; j++)
		{
			//the sea is in this radius = the sea is 0 and we dont have land here
			if (ground[i][j] < 0 && river[i][j] == 0)
				return true;
		}
	}
	return false;
}
bool distanceFromSea(int row, int col, int size, int min)
{
	// Check if the area exceeds the matrix bounds
	if (row - min < 0 || col - min < 0 || row - min + (2 * min) + size > GSZ || col - min + (2 * min) + size > GSZ)
		return false;
	//need to check that we are not too close to the sea
	for (int i = row - min; i < row - min + (2* min) + size; i++)
	{
		for (int j = col - min; j < col - min + (2 * min) + size; j++)
		{
			//the sea is not in this radius = land needs to be above 0
			if ( ground[i][j] <= 0)
				return false;
		}
	}
	
	return true;

}
bool subArea(int row, int col, int size)
{
	//check that we dont exceed the matrix bounds
	if (row + size > GSZ || col + size > GSZ)
		return false;

	for (int i = row; i < row + size; i++)
	{
		for (int j = col; j < col + size; j++)
		{
			//is this area above ground = land needs to be above 0
			if (ground[i][j] <= 0)
				return false;
			
		}
	}
	return true;
}
bool inTheArray(int x, int z)
{
	for (int i = 0; i < GSZ*GSZ; ++i) 
	{
		if (housesX[i] == x)
			if (housesY[i] == z)
				return true;
	}
	return false;
}
void findLand(int row, int col, int size, int min, int max)
{
	int numberOfHouses = 0;
	int houseColors = 1;
	//search through entire ground matrix
	for (int i = row; i < GSZ; i++)
	{
		for (int j = col; j < GSZ; j++)
		{
			//search is this area on land /already in a city
			if (subArea(i, j, 2) == true && inTheArray(i, j) == false)
			{
				//search is this area not too close to the sea/river
				if (distanceFromSea(i, j,size, min))
				{
					//search is this area not too far from the sea/river
					if (distanceFromSea2(i, j, size, max))
					{
						if (distanceFromRiver(i, j, size, min))
						{
							if (distanceFromRiver2(i, j, size, max))
							{
								//add houses to the houses array
								for (int x = i; x < i + size; x++)
								{
									for (int z = j; z < j + size; z++)
									{
										//check that this house isnt already in the arrays
										if (inTheArray(x, z) == false)
										{
											housesX[numberOfHouses] = x;
											housesY[numberOfHouses] = z;
											houseColor[numberOfHouses] = houseColors;
											++numberOfHouses;
										}

									}
								}

								std::cout << "Found enough land at index (" << i << ", " << j << ") that is close enough and not too far from the sea" << std::endl;

							}
						}
					}
				}
				
			}
			++houseColors;
			if (houseColors == 4)
				houseColors = 1;
			
		}
	}
	drawHouses();
}
void menu(int choice)
{
	switch (choice)
	{
	case 1:
		// Call the erode function 
		for (int i = 0; i < 1000; i++)
			erode();
		break;
	case 2:
		// Call the find land function - land needs to be 0<land<=2 distance from the river/sea (distances/size of town can be changed)
		findLand(0,0,2, 0, 2);
		break;

	}
}

void main(int argc, char* argv[]) 
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE|GLUT_DEPTH);
	glutInitWindowSize(W, H);
	glutInitWindowPosition(400, 100);
	glutCreateWindow("Final Project");

	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutSpecialFunc(SpecialKeys);
	

	glutCreateMenu(menu);
	glutAddMenuEntry("Erosion", 1);
	glutAddMenuEntry("Place City", 2);
	glutAttachMenu(GLUT_RIGHT_BUTTON);


	init();

	glutMainLoop();
}