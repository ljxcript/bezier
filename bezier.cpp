#include <GL/freeglut.h>
#include <time.h>
#include <math.h>
#include <iostream>
#include <vector>
using namespace std;


#define vertexN 5
#define PI 3.1415926
#define vertexRadius 0.02f

#define RED 1
#define GREEN 2
#define BLUE 3




class coordinate {
public:
    coordinate() {

    }
    coordinate(float Cx, float Cy) {
        x = Cx;
        y = Cy;

    }
    float x;
    float y;    
};




static int windowSize = 500;

static vector<coordinate> dots;

static int bezierColor[][3] = {
	{25,148,117},
	{179, 24, 0},
	{119, 195, 79},
	{0, 176, 240}
};

static int bezierC[3] = {bezierColor[0][0],bezierColor[0][1],bezierColor[0][2]};

static coordinate vertices[vertexN] = {}; 

static int colors[][3] = {
	{255, 233,87},
	{242, 159, 63},
	{242, 117, 63},
	{232, 126 ,81}
};


static float u = 0.0;

static vector<coordinate> beizer;

static vector<coordinate> mouseTester;


void mouseCB(int button, int state, int x, int y) 
{
	if (button == GLUT_LEFT_BUTTON)
	{
		if (state == GLUT_DOWN)
		{


			mouseTester.push_back(coordinate(((int)x - windowSize/2)/(float)250, (windowSize/2 - (int)y)/(float)250));
			if (mouseTester.size() == vertexN) {
				for (int i = 0; i < vertexN; i++) {
					vertices[i] = mouseTester[i];
				}
				u = 1.0;
			} else {
				if (mouseTester.size() > vertexN) {
					mouseTester.pop_back();
				}
			}
		}
	}
}



void motionMouseCB(int x, int y) {
	float aX = (x - windowSize/2)/(float)250;
	float aY = (windowSize/2 - y)/(float)250;
	for (int i = 0; i < mouseTester.size(); i++) {
		if (pow((aX - mouseTester[i].x),2) + pow((aY- mouseTester[i].y),2) < pow(vertexRadius*2, 2)) {
			mouseTester[i].x = aX;
			mouseTester[i].y = aY;
			vertices[i].x = aX;
			vertices[i].y = aY;
			u = 1.0f;
		}
	}

}



void glArc( double x, double y, double start_angle, double end_angle, double radius )
{
    /* 开始绘制曲线 */
    glBegin( GL_LINE_STRIP );
    /* 每次画增加的弧度 */
    double delta_angle = PI / 180;
    /* 画圆弧 */
    for ( double i = start_angle; i <= end_angle; i += delta_angle )
    {
        /* 绝对定位加三角函数值 */
        double    vx    = x + radius * cos( i );
        double    vy    = y + radius*sin( i );
        glVertex2d( vx, vy );
    }
    /* 结束绘画 */
    glEnd();
}
 

void computeCoor() {
    vector<coordinate> temp(vertices, vertices+vertexN);
    vector<coordinate> temp_0;
    for (int i = 0; i < vertexN - 1; i++) {
	    glColor3ub(colors[i][0], colors[i][1], colors[i][2]);
        glBegin(GL_LINE_STRIP);
        for (int j = 0; j < vertexN - 1 - i; j++) {
            float newX = temp[j].x + (temp[j+1].x - temp[j].x) * u;
            float newY = temp[j].y + (temp[j+1].y - temp[j].y) * u;
            coordinate tempC(newX, newY);
        	glVertex2f(newX, newY);

            temp_0.push_back(tempC);
        }
        glEnd();
        temp = temp_0;
        temp_0.clear();
    }

    beizer.push_back(temp[0]);
}




void drawMouseTester() {
	glColor3ub(0, 0, 0);
	glBegin(GL_LINE_STRIP);
		for (int i = 0; i < mouseTester.size(); i++) {

			glVertex2f(mouseTester[i].x, mouseTester[i].y);
		}
	glEnd();
	for (int i = 0; i < mouseTester.size(); i++) {

		glArc(mouseTester[i].x, mouseTester[i].y, 0, 2*PI, 0.01f);
	}
}

void drawBeizer() {
    glColor3ub(bezierC[0], bezierC[1], bezierC[2]);
	glBegin(GL_POINTS);
		for (int i = 0; i < beizer.size(); i++) {
			glVertex2f(beizer[i].x, beizer[i].y);
		}
	glEnd();
}

void drawLineStrip() {

    glBegin(GL_LINE_STRIP);
        for (int i = 0; i < vertexN; i++) {
            glVertex2f(vertices[i].x, vertices[i].y);
        }

    glEnd();

}



void processMenuEvents(int option) {
	bezierC[0] = bezierColor[option][0];
	bezierC[1] = bezierColor[option][1];
	bezierC[2] = bezierColor[option][2];
}


void createGLUTMenus() {
	int menu;
	menu = glutCreateMenu(processMenuEvents);

	glutAddMenuEntry("red______bezier", RED);
	glutAddMenuEntry("green____bezier", GREEN);
	glutAddMenuEntry("blue_____bezier", BLUE);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}





void myDisplay() 
{

    glEnable(GL_POLYGON_SMOOTH_HINT);  
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);  
	glEnable(GL_DEPTH_TEST);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

 
    glColor3ub(0, 0, 0);

    drawLineStrip();

    computeCoor();

    drawBeizer();

    drawMouseTester();

	glFlush();
	glutSwapBuffers();
}


void myIdle()
{	
	if (u < 1.0)
		u = (u + 0.001);
	else {
		u = 0.0;
		beizer.clear();
	}
	myDisplay();

}

int main(int argc, char *argv[]) {
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE|GLUT_MULTISAMPLE);

	glutInitWindowPosition(100,100);
	glutInitWindowSize(windowSize, windowSize);
	glutCreateWindow("orthographic projection");
	glutDisplayFunc(&myDisplay);
	glutIdleFunc(&myIdle);



	//adding the mouse clicking callbacks
	glutMouseFunc(mouseCB);
	glutMotionFunc(motionMouseCB);

	createGLUTMenus();

	glutMainLoop();
	return 0;
}




