#include <bits/stdc++.h>
#include <GL/glut.h>
#include "imageloader.h"
using namespace std;


#define PI 3.141592653589
#define DEG2RAD(deg) (deg * PI / 180)

void drawScene();
void drawBox(float i,float j);
void update(int value);
void initRendering();
void handleResize(int w, int h);
void handleKeypress1(unsigned char key, int x, int y);
void handleKeypress2(int key, int x, int y);
void drawbot();
void inertia(unsigned char key, int x, int y);
void inertia2(int x);
void updown(int x);
void GetOGLPos1(int x, int y);
void GetOGLPos2(int x, int y);
void handleMouseclick(int button, int state, int x, int y) ;
int detect_collision();
void tile_cam();
void helicopter1();
void helicopter2();
void mouse(int button, int state, int x, int y);
void explode(int x);
void display_projector();
void draw_rays(int x);
void proj_rotate(int z);
void drawbamboo(double a,double b,double c,int d,int e);
void bamboo_updown(int x);
void reset();
void draw_spidey();
void process_spidey(int x);
void draw_spidey();

float box_len = 4.0f;
int color_flag=0;
float w=1.6f;
float t=0.6f;
int front_leg=0;
int motion=0;
int score=0;
float bot_x=-2.0;
float bot_y=0;
float bot_z=1;
float theta=0.0f;
float speed=0.1f;
float up_bot=0.2;
float down_bot=1.4;

int tps=0;
int fps=0;
int jump_flag=0;
int jump_ct=0;
float cam_param=0;
int inertia_flag=0;

int up_down[10][10];
//int tile_view[10][10];
float motion_floor[10][10];
int current_i=0;
int current_j=0;

float max_y=1.0f;
float min_y=-1.0f;
float itr_y=0.2;

float jump_y_crd=0.0f;

int res_of_collision;

float tile_theta=0.0f;

typedef struct floor_tile
{
	float top_left_x;
	float top_left_z;
	float top_right_x;
	float top_right_z;
	float bottom_left_x;
	float bottom_left_z;
	float bottom_right_x;
	float bottom_right_z;
	float height;
}floor_tile;

floor_tile tile_array[100];
int tile_no=-1;

int current_tile;  // to check intersection of bot
float prev_x=bot_x;
float prev_y=bot_y;
float prev_z=bot_z;

int tile_view_flag=0;

/* new tile_cam crdtns */

floor_tile new_tile[10][10];
int new_tile_i=0;
int new_tile_j=0;


/* Helicopter */

float pos1X,pos1Z;
float pos2X,pos2Z;

int heli_view_flag=0;

float tower_x=-8.0f;
float tower_y=3.0f;
float tower_z=8.0f;
float heli_dis=10.0f;

int click_flag=0;
int died=0;
float j_speed=0.1;

int bomb_array[10][10];

#define MP		make_pair
#define PB		push_back
#define VI              vector<int>
#define PLL             pair<int,int>
#define VP		vector<PLL>

VP bomb_vec;

//Makes the image into a texture, and returns the id of the texture
GLuint loadTexture(Image* image) {
	GLuint textureId;
	glGenTextures(1, &textureId); 								//Make room for our texture
	glBindTexture(GL_TEXTURE_2D, textureId); 						//Tell OpenGL which texture to edit
	//Map the image to the texture
	glTexImage2D(GL_TEXTURE_2D,                						//Always GL_TEXTURE_2
			0,                            								//0 for now
			GL_RGB,                 								//Format OpenGL uses for image
			image->width, image->height,								//Width and height
			0,                    								        //The border of the image
			GL_RGB,											//GL_RGB, because pixels are stored in RGB format
			GL_UNSIGNED_BYTE,									//GL_UNSIGNED_BYTE, because pixels are stored
			image->pixels
		    ); 										        //The actual pixel data
	return textureId;									//Returns the id of the texture
}


GLuint _textureId1;
GLuint _textureId2;
GLuint _textureId3;
GLuint _textureId4;
GLuint _textureId5;
GLuint _textureId6;
GLuint _textureId7;
GLuint _textureId8;
GLuint _textureId88;//sun
GLuint _textureId9;//teleportation tile

float star_theta=0;
int star_array[10][10]; /* star on which ?? */

/* Projector */

typedef struct projector
{
	float a;
	float b;
	float c;
	float d;
	float d1;
	float d_x;
	float d_y;
	int highlight;
	int n_division;
}proj;

proj project[400];
float proj_x=9,proj_z=-9,proj_theta=0;
int n_project=1;
int strip_flag=0;
//latest
int star_taken=0;
float oneup_theta=0;
int not_die=0;
int star_toggle=1;
int star_timer=1000;

float g_old_x,g_old_z,g_new_x,g_new_z;

#define PAT1 0XFF00
#define PAT2 0X00FF

int level=1;
int bakchod=0;
int bamboo[10][10];
int level_3[10][10];
int bamboo_type[10][10];
float bamboo_height[10][10];
int level3_die[10][10];

//sun
float r_x=90;
float r_y=0;
float r_z=-200;
int suraj_flag=0,change_flag=0;
float day_r=0;
float day_g=0;
float day_b=0;

/* teloprtation */
int tele[10][10];
int tele_flag=0;

float L3_up_bot=0;
float L3_up_bot_z=0;
float L32_up_bot=0;
float L32_up_bot_z=0;

int abc=0;
int def=0;

void lines(float x1,float y1,float z1,float x2,float y2,float z2)
{
	glLineWidth(3.0);
	glBegin(GL_LINES);
	glVertex3f(x1, y1, z1);
	glVertex3f(x2, y2, z2);
	glEnd();
}

//spider
float r_middle=0.0,l_middle=0.0;
float r_front=0.0,l_front=0.0;
float r_back=0.0,l_back=0.0;
int r_f=1,l_f=1;
int r_m=1,l_m=1;
int r_b=1,l_b=1;
int rf_flag=0,lf_flag=0;
int rm_flag=0,lm_flag=0;
int rb_flag=0,lb_flag=0;

float spi_x=10;
float spi_y=1.5;
float spi_z=-6;
float spi_theta=0;

int spi_i=5;
int spi_j=7;

float die_bot_y;

int main(int argc, char **argv) {

	for(int i=0;i<10;i++)
	{
		for(int j=0;j<10;j++)
		{
			up_down[i][j]=rand()%8;
			motion_floor[i][j]=0.0f;
			bomb_array[i][j]=5;
		}
	}
	for(int i=0;i<10;i++)
		for(int j=0;j<10;j++)
			tele[i][j]=rand()%7;

	for(int i=0;i<10;i++)
		for(int j=0;j<10;j++)
			star_array[i][j]=rand()%8;

	for(int i=0;i<10;i++)
	{
		for(int j=0;j<10;j++)
		{	level_3[i][j]=0;
			level3_die[i][j]=0;
		}
	}

	star_array[0][5]=2;
	star_array[0][0]=2;
	star_array[9][9]=2;

	for(int i=0;i<10;i++)
	{
		for(int j=0;j<10;j++)
		{
			//srand (time(NULL));
			bamboo[i][j]=rand()%8; // if==3 draw bamboo
			int x;
			//srand (time(NULL));
			x=rand()%2;
			bamboo_type[i][j]=x; // 0 down 1 up
			if(x==0)
			{
				if(i>=0 && i<3)
					bamboo_height[i][i]=0.0;
				else if(i>=3 && i<7)
					bamboo_height[i][i]=1.5;				
				else if(i>=7 && i<10)
					bamboo_height[i][i]=3.0;
			}
			else
			{
				if(i>=0 && i<3)
					bamboo_height[i][i]=0.0-0.8;
				else if(i>=3 && i<7)
					bamboo_height[i][i]=1.5-0.8;				
				else if(i>=7 && i<10)
					bamboo_height[i][i]=3.0-0.8;

			}
		}
	}
	bamboo[3][9]=2;
	bamboo[2][9]=2;
	bamboo[6][0]=2;
	bamboo[7][0]=2;

	star_array[9][9]=1;
	project[0].a=-0.5;
	project[0].b=0;
	project[0].c=0.5;
	project[0].d=0;
	project[0].d1=0.5;


	project[0].n_division=8;

	// Initialize GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB |  GLUT_DEPTH);

	int w = glutGet(GLUT_SCREEN_WIDTH);
	int h = glutGet(GLUT_SCREEN_HEIGHT);
	int windowWidth = w *1;
	int windowHeight = h *1;

	glutInitWindowSize(windowWidth, windowHeight);
	glutInitWindowPosition((w - windowWidth) / 2, (h - windowHeight) / 2);

	glutCreateWindow("CSE251_sampleCode");  // Setup the window
	initRendering();

	// Register callbacks
	glutDisplayFunc(drawScene);
	glutIdleFunc(drawScene);
	glutKeyboardFunc(handleKeypress1);
	glutSpecialFunc(handleKeypress2);
	glutKeyboardUpFunc(inertia);
	glutPassiveMotionFunc(GetOGLPos1); // without click
	glutMotionFunc(GetOGLPos2); // with click
	glutReshapeFunc(handleResize);
	glutMouseFunc(mouse);
	glutTimerFunc(10, update, 0);
	glutTimerFunc(70,inertia2, 0);
	glutTimerFunc(10,updown, 0);
	glutTimerFunc(100,bamboo_updown, 0);
	glutTimerFunc(50,process_spidey, 0);
	glutMainLoop();

	return 0;
}

void drawcylin(double a,double b,double c,int d,int e)
{
	GLUquadric *qobj = gluNewQuadric();
	gluQuadricTexture(qobj,GL_TRUE);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,_textureId7);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//GLUquadricObj *quadratic;
	//quadratic = gluNewQuadric();
	gluCylinder(qobj,a,b,c,d,e);
	gluDeleteQuadric(qobj);
	glDisable(GL_TEXTURE_2D);
}

void drawbamboo(double a,double b,double c,int d,int e)
{
	glColor3f(0.4,0.4,0.4);
	GLUquadric *qobj = gluNewQuadric();
	gluQuadricTexture(qobj,GL_TRUE);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,_textureId8);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	//GLUquadricObj *quadratic;
	//quadratic = gluNewQuadric();
	gluCylinder(qobj,a,b,c,d,e);
	gluDeleteQuadric(qobj);
	glDisable(GL_TEXTURE_2D);
}

// Function to draw objects on the screen
void drawScene() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(0,0,-2);

	if(heli_view_flag==1)
	{
		if(click_flag==1)
			helicopter1();
		else if(click_flag==2)
			helicopter2();
		//cout << "ghusa in helicopter " << endl;
	}
	else if(tile_view_flag==1)
	{
		tile_cam();
		//cout << "ghusa in tile " << endl;
	}
	else if(tps==1 || fps==1)
	{
		//cout << "ghusa in fps " << endl;

		float lok_x,lok_y,lok_z;
		float cam_x,cam_y,cam_z;

		lok_x=bot_x;
		lok_y=bot_y+1.5+jump_y_crd;
		lok_z=bot_z;

		cam_x=bot_x;
		cam_y=bot_y+1.5+jump_y_crd;
		cam_z=bot_z;

		if(fps==1)
			cam_param=-0.9;
		else if(tps==1)
			cam_param=2;

		float mycos=cos(DEG2RAD(theta));
		float mysin=sin(DEG2RAD(theta));
		if(theta>=0 && theta <=90)
		{
			lok_z+=(10 * mycos);
			lok_x+=(10 * mysin);

			cam_z-=(cam_param * mycos);
			cam_x-=(cam_param * mysin);
		}
		if(theta>90 && theta <=180)
		{
			lok_z-=(10 * ((-1)*(mycos)));
			lok_x+=(10 * mysin);

			cam_z+=(cam_param * (-1*mycos));
			cam_x-=(cam_param * mysin);
		}
		if(theta>180 && theta <=270)
		{
			lok_z-=(10 * ((-1)*(mycos)));
			lok_x-=(10 * ((-1)*(mysin)));

			cam_z+=(cam_param * (-1*mycos));
			cam_x+=(cam_param * (-1*mysin));
		}
		if(theta>270 && theta <=360)
		{
			lok_z+=(10 * mycos);
			lok_x-=(10 * ((-1)*(mysin)));

			cam_z-=(cam_param * mycos);
			cam_x+=(cam_param * (-1*mysin));
		}
		if(tps==1)
		{
			gluLookAt(cam_x,cam_y+jump_y_crd,cam_z,lok_x,bot_y,lok_z,0,1,0);
		}
		else if(fps==1)
			gluLookAt(cam_x,cam_y+0.20,cam_z,lok_x,0,lok_z,0,1,0);
	}
	else
	{
		gluLookAt(6.0f,3.0f,10.0f,6.0f,0.0f,0.0f,0.0f,1.0f,0.0f);
	}


	/*	GLfloat ambientColor[] = {0.2f, 0.2f, 0.2f, 1.0f};
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);
	 */
	if(level==2)
	{
		if(star_taken==1 && star_toggle==0 && star_timer>0)
		{
			star_timer--;
			GLfloat lightColor0[] = {255,0,0,1.0f};
			GLfloat lightPos0[] = {bot_x,bot_y+4.0f,bot_z,1.0f};
			glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 10.0);
			GLfloat spot_direction[] = {0.0, -1.0, 0.0 };
			glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_direction);
			glLightfv(GL_LIGHT0,GL_DIFFUSE,lightColor0);
			glLightfv(GL_LIGHT0,GL_DIFFUSE,lightPos0);

			GLfloat lightColor1[] = {255,0,0,1.0f};
			GLfloat lightPos1[] = {bot_x,10.0f,bot_z,1.0f};
			glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 10.0);
			GLfloat spot_direction1[] = {0.0, -1.0, 0.0 };
			glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_direction1);
			glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor1);
			glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);
			star_toggle=1;
		}
		else if(star_taken==1 && star_toggle==1 && star_timer>0)
		{
			star_timer--;
			GLfloat lightColor0[] = {0,255,0,1.0f};
			GLfloat lightPos0[] = {bot_x,bot_y+4.0f,bot_z,1.0f};
			glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 10.0);
			GLfloat spot_direction[] = {0.0, -1.0, 0.0 };
			glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_direction);
			glLightfv(GL_LIGHT0,GL_DIFFUSE,lightColor0);
			glLightfv(GL_LIGHT0,GL_DIFFUSE,lightPos0);

			GLfloat lightColor1[] = {0,255,0,1.0f};
			GLfloat lightPos1[] = {bot_x,10.0f,bot_z,1.0f};
			glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 10.0);
			GLfloat spot_direction1[] = {0.0, -1.0, 0.0 };
			glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_direction1);
			glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor1);
			glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);
			star_toggle=0;
		}
		else
		{
			GLfloat lightColor0[] = {255,255,255,1.0f};
			GLfloat lightPos0[] = {bot_x,bot_y+4.0f,bot_z,1.0f};
			glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 10.0);
			GLfloat spot_direction[] = {0.0, -1.0, 0.0 };
			glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_direction);
			glLightfv(GL_LIGHT0,GL_DIFFUSE,lightColor0);
			glLightfv(GL_LIGHT0,GL_DIFFUSE,lightPos0);

			GLfloat lightColor1[] = {255,255,255,1.0f};
			GLfloat lightPos1[] = {bot_x,10.0f,bot_z,1.0f};
			glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 10.0);
			GLfloat spot_direction1[] = {0.0, -1.0, 0.0 };
			glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_direction1);
			glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor1);
			glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);
		}
	}
	else
	{
		GLfloat lightColor0[] = {255,255,255,1.0f};
		GLfloat lightPos0[] = {bot_x,bot_y+4.0f,bot_z,1.0f};
		glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 10.0);
		GLfloat spot_direction[] = {0.0, -1.0, 0.0 };
		glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_direction);
		glLightfv(GL_LIGHT0,GL_DIFFUSE,lightColor0);
		glLightfv(GL_LIGHT0,GL_DIFFUSE,lightPos0);

		GLfloat lightColor1[] = {255,255,255,1.0f};
		GLfloat lightPos1[] = {bot_x,10.0f,bot_z,1.0f};
		glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 10.0);
		GLfloat spot_direction1[] = {0.0, -1.0, 0.0 };
		glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_direction1);
		glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor1);
		glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);

	}
	if(level==2)
	{
		if(star_timer<=0)
		{
			//star_timer=1000;
			not_die=0;
			star_taken=0;
		}
	}

	//drawBox(box_len);

	float c_i,c_j;
	c_j=2;
	for(int i=0;i<10;i++)
	{
		c_i=-3;
		for(int j=0;j<10;j++)
		{
			if(level==2)
			{
				oneup_theta+=0.1;
				if(oneup_theta>=360)
					oneup_theta=0.0f;
			}
			current_i=i;
			current_j=j;
			tile_no++;

			if((i+j)%2==0)
				color_flag=1;
			else
				color_flag=0;
			if(level==2)
			{
				if(star_array[current_i][current_j]==3)
				{
					glEnable(GL_TEXTURE_2D);
					glBindTexture(GL_TEXTURE_2D,_textureId5);

					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
					glPushMatrix();
					float red = (float)rand() / (float)RAND_MAX;//generate randon red component..etc
					float green = (float)rand() / (float)RAND_MAX;
					float blue = (float)rand() / (float)RAND_MAX;
					glColor3f(red,green,blue);
					//glClear (GL_COLOR_BUFFER_BIT);
					float star_x=new_tile[current_i][current_j].top_left_x+w/2;
					float star_z=new_tile[current_i][current_j].top_left_z-w/2;

					float star_y=new_tile[current_i][current_j].height;
					glTranslatef(star_x,star_y,star_z);

					glRotatef(45.0,0.0,0.0,1.0);
					glRotatef(star_theta,1.0,1.0,0.0);
					star_theta+=0.3;
					if(star_theta>=360)
						star_theta=0.0f;
					glBegin(GL_QUADS); // Start drawing a quad primitive  
					glTexCoord2f(0.0f, 0.0f);
					glVertex3f(-0.3f, -0.3f, 0.0f); // The bottom left corner  
					glTexCoord2f(0.0f, 1.0f);
					glVertex3f(-0.3f, 0.3f, 0.0f); // The top left corner
					glTexCoord2f(1.0f, 1.0f);  
					glVertex3f(0.3f, 0.3f, 0.0f); // The top right corner
					glTexCoord2f(1.0f, 0.0f);  
					glVertex3f(0.3f, -0.3f, 0.0f); // The bottom right corner  

					glEnd(); 

					glPopMatrix();
					glDisable(GL_TEXTURE_2D); 

				}
			}
			if(i==0 && j==0)
			{
				glPushMatrix();
				glColor3f(0.3,0.8,0.5);
				glTranslatef(c_i+0.9,motion_floor[0][0],c_j-0.9);
				glRotatef(-90,1.0,0.0,0.0);
				drawcylin(0.6f,0.6f,1.2f,32,32);
				glTranslatef(0.0,0.0,1.0);
				drawcylin(0.5,0.8,0.5,32,32);
				glPopMatrix();
			}
			if(i==9 && j==9)
			{
				glPushMatrix();
				glColor3f(0.3,0.8,0.5);
				glTranslatef(c_i+0.9,motion_floor[9][9],c_j-0.9);
				glRotatef(-90,1.0,0.0,0.0);
				drawcylin(0.6f,0.6f,1.2f,32,32);
				glTranslatef(0.0,0.0,1.0);
				drawcylin(0.5,0.8,0.5,32,32);
				glPopMatrix();
			}
			if(level==2)
			{
				if(current_i==0 && current_j==5 && star_taken==0)
				{
					glPushMatrix();
					float star_x=new_tile[current_i][current_j].top_left_x+w/2;
					float star_z=new_tile[current_i][current_j].top_left_z-w/2;
					float star_y=new_tile[current_i][current_j].height;
					glTranslatef(star_x,star_y,star_z);
					glLineWidth(5);
					glRotatef(oneup_theta,0.0,1.0,0.0);
					glColor3f(0.0,0.0,0.0);
					glBegin (GL_LINE_LOOP);
					glVertex3f(0.0,0.2,0.0);
					glVertex3f(0.1,0.1,0.0);
					glVertex3f(0.2,0.05,0.0);
					glVertex3f(0.1,0.0,0.0);
					glVertex3f(0.2,-0.1,0.0);
					glVertex3f(0.0,0.0,0.0);
					glVertex3f(-0.2,-0.1,0.0);
					glVertex3f(-0.1,0.0,0.0);
					glVertex3f(-0.2,0.05,0.0);
					glVertex3f(-0.1,0.1,0.0);
					glEnd();
					glPopMatrix();
				}
				glLineWidth(1);
			}
			drawBox(c_i,c_j);
			c_i+=w;
		}
		c_j-=w;
	}
	tile_no=-1;

	glColor3f(0.0,0.6,0.0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,_textureId1);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glBegin(GL_QUADS);
	glNormal3f(0.0,1.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-100.0f,-3.5f, 100.0f);
	glTexCoord2f(8.0f, 0.0f);
	glVertex3f(100.0f,-3.5f, 100.0f);
	glTexCoord2f(8.0f, 8.0f);
	glVertex3f(100.0f,-3.5f, -100.0f);
	glTexCoord2f(0.0f, 8.0f);
	glVertex3f(-100.0f,-3.5f, -100.0f);
	glEnd();

	glDisable(GL_TEXTURE_2D);

	drawbot();
	if(level==3)
		draw_spidey();
	if(level==2)
	{
		display_projector();
		if(tps==0 && fps==0 && tile_view_flag==0 && heli_view_flag==0)
		{
			string message="Score:";
			stringstream ss;
			ss << score;
			string str = ss.str();
			message=message+str;
			for(int i=0;i<message.length();i++)
			{
				glPushMatrix();
				glColor3f(1.0,1.0,1.0);
				glTranslatef(i*10+70,15.0,-200);
				glScalef(0.08,0.08,0.08);
				glutStrokeCharacter(GLUT_STROKE_ROMAN,message[i]);
				glPopMatrix();
			}}
		if(tps==0 && fps==0 && tile_view_flag==0 && heli_view_flag==0 && star_taken==1)
		{
			string message="Timer:";
			stringstream ss;
			ss << star_timer;
			string str = ss.str();
			message=message+str;
			for(int i=0;i<message.length();i++)
			{
				glPushMatrix();
				glColor3f(1.0,1.0,1.0);
				glTranslatef(i*10-150,15.0,-200);
				glScalef(0.08,0.08,0.08);
				glutStrokeCharacter(GLUT_STROKE_ROMAN,message[i]);
				glPopMatrix();
			}}
	}
	if(level==2 && bakchod==0)
	{
		bakchod=1;
		glutTimerFunc(400,explode, 0);
		glutTimerFunc(50,proj_rotate,0);
	}
	//sun moon
	glPushMatrix();
	glTranslatef(r_x,r_y,r_z);
	r_x-=0.15;
	if(r_y>=27)
	{
		suraj_flag=1;
	}
	if(suraj_flag==1)
		r_y-=0.025;
	else
		r_y+=0.03;
	//cout<<r_y<<endl;
	if(r_y>=3 && suraj_flag==0)
	{
		day_r=0.1;
		day_g=0.1;
		day_b=0.2;
		change_flag=1;
	}
	if(r_y<=20 && suraj_flag==1)
	{
		day_r=0.125;
		day_g=0.125;
		day_b=0.125;
		change_flag=1;
	}
	if(change_flag==1)
	{
		glClearColor((float)(day_r),(float)(day_g),(float)(day_b),1.0f);
		glShadeModel(GL_SMOOTH);
		change_flag=0;
	}
	glColor3f(255,69,0);
	GLUquadric *qobj = gluNewQuadric();
	gluQuadricTexture(qobj,GL_TRUE);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,_textureId88);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	gluSphere(qobj,5,20,20);
	gluDeleteQuadric(qobj);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();


	glutSwapBuffers();
}

void update(int value){

	//float prev_ht=bot_y;

	if(jump_flag==1 && jump_ct<8)
	{
		jump_ct++;
		jump_y_crd+=0.2f;
		prev_x=bot_x;
		prev_z=bot_z;
		float mycos=cos(DEG2RAD(theta));
		float mysin=sin(DEG2RAD(theta));
		if(theta>=0 && theta <=90)
		{
			bot_z+=(j_speed * mycos);
			bot_x+=(j_speed * mysin);
		}
		if(theta>90 && theta <=180)
		{
			bot_z-=(j_speed * ((-1)*(mycos)));
			bot_x+=(j_speed * mysin);
		}
		if(theta>180 && theta <=270)
		{
			bot_z-=(j_speed * ((-1)*(mycos)));
			bot_x-=(j_speed * ((-1)*(mysin)));
		}
		if(theta>270 && theta <=360)
		{
			bot_z+=(j_speed * mycos);
			bot_x-=(j_speed * ((-1)*(mysin)));
		}
		motion=1;
		if(front_leg==0)
			front_leg=1;
		else
			front_leg=0;
		j_speed+=0.03f;
		int x=detect_collision();
		if(x==0)
		{
			bot_x=prev_x;
			bot_z=prev_z;
		}
		else if(x==1)
		{
			// valid
		}
		else
		{
			exit(0);
		}

	}
	if(jump_flag==1 && jump_ct>=8 && jump_ct<16)
	{
		prev_x=bot_x;
		prev_z=bot_z;
		jump_ct++;
		jump_y_crd-=0.2f;
		int x=detect_collision();
		if(x==0)
		{
			bot_x=prev_x;
			bot_z=prev_z;
		}
		else if(x==1)
		{
			// valid
		}
		else
		{
			exit(0);
		}

	}
	if(jump_ct==16)
	{
		jump_y_crd=0.0f;
		jump_flag=0;
		jump_ct=0;
		j_speed=0.0f;
		int x=detect_collision();
		if(x==0)
		{
			bot_x=prev_x;
			bot_z=prev_z;
		}
		else if(x==1)
		{
			// valid
		}
		else if(x==2)
		{
			exit(0);
		}
	}
	//cout << " From JUMP FUNCTION " << " Height = " << bot_y << " jump_ct = " << jump_ct << endl;
	glutTimerFunc(10, update, 0);
}

void drawBox(float i,float j)
{
	if(level==1 || level==2)
	{
		new_tile[current_i][current_j].top_left_x=i;
		new_tile[current_i][current_j].top_left_z=j;
		new_tile[current_i][current_j].height=motion_floor[current_i][current_j]+1.0f;

		//cout << "NEW " << new_tile[current_i][current_j].height << endl;

		tile_array[tile_no].top_left_x=i;
		tile_array[tile_no].top_left_z=j;
		tile_array[tile_no].top_right_x=i+w;
		tile_array[tile_no].top_right_z=j;
		tile_array[tile_no].bottom_right_x=i+w;
		tile_array[tile_no].bottom_right_z=j-w;
		tile_array[tile_no].bottom_left_x=i;
		tile_array[tile_no].bottom_left_z=j-w;
		tile_array[tile_no].height=motion_floor[current_i][current_j];
	}
	if(level==2)
	{
		if(up_down[current_i][current_j]==-1)
		{
			return;
		}

		if(star_array[current_i][current_j]==4)
		{
			bomb_vec.PB(MP(current_i,current_j));
			GLUquadric *qobj = gluNewQuadric();
			gluQuadricTexture(qobj,GL_TRUE);
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D,_textureId6);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glPushMatrix();
			//glColor3f(0.0f,0.0f,0.0f);
			float yh=tile_array[tile_no].height;
			glTranslatef((tile_array[tile_no].top_left_x + tile_array[tile_no].bottom_right_x)/2 , yh , (tile_array[tile_no].top_left_z +tile_array[tile_no].bottom_right_z )/2);
			float factor=bomb_array[current_i][current_j]-4;
			//glScalef(bomb_array[current_i][current_j],bomb_array[current_i][current_j],bomb_array[current_i][current_j]);
			glScalef(factor,factor,factor);
			// previously generated texture

			gluSphere(qobj, 0.2f, 20, 20);
			//	glutSolidSphere(0.2,20.0,20.0);
			glPopMatrix();
			gluDeleteQuadric(qobj);
			glDisable(GL_TEXTURE_2D);
		}
	}
	if(level==1 || level==2)
	{
		if(up_down[current_i][current_j]==0)
			glColor3f(1.0,0.0f,0.0f);
		else if(up_down[current_i][current_j]==1)
			glColor3f(0.0,1.0f,0.0f);
		else if(up_down[current_i][current_j]==2)
			glColor3f(0.0,0.0f,1.0f);
		else
			glColor3f(1,1,0);
	}
	if(level==1)
	{
		if(tele[current_i][current_j]==2 && up_down[current_i][current_j]>=3)//teleportation
		{
			glColor3f(0.9,0.5,0.5);
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D,_textureId9);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

			glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(i,0,j);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(i+w,0,j);
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(i+w,0,j-w);
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(i,0,j-w);
			glEnd();

			// Upper face drawn

			glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(i,-t,j);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(i+w,-t,j);
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(i+w,-t,j-w);
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(i,-t,j-w);
			glEnd();

			// Lower face drawn

			glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(i,0,j);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(i,0,j-w);
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(i,-t,j-w);
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(i,-t,j);
			glEnd();

			// Left face drawn

			glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(i+w,0,j);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(i+w,0,j-w);
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(i+w,-t,j-w);
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(i+w,-t,j);
			glEnd();

			// Right face drawn

			glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(i,0,j-w);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(i+w,0,j-w);
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(i+w,-t,j-w);
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(i,-t,j-w);
			glEnd();

			// Front face drawn

			glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(i,0,j);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(i+w,0,j);
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(i+w,-t,j);
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(i,-t,j);
			glEnd();

			glDisable(GL_TEXTURE_2D);
			return;
		}
	}
	if(level==1 || level==2)
	{
		if(up_down[current_i][current_j]==0)
		{
			//glColor3f(0.51,0.51,0.51);
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D,_textureId2);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);


			glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(i,4,j);
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(i+w,4,j);
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(i+w,4,j-w);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(i,4,j-w);
			glEnd();

			// Upper face drawn

			glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(i,0,j);
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(i+w,0,j);
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(i+w,0,j-w);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(i,0,j-w);
			glEnd();

			// Lower face drawn

			glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(i,4,j);
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(i,4,j-w);
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(i,0,j-w);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(i,0,j);
			glEnd();

			// Left face drawn

			glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(i+w,4,j);
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(i+w,4,j-w);
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(i+w,0,j-w);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(i+w,0,j);
			glEnd();

			// Right face drawn

			glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(i,4,j-w);
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(i+w,4,j-w);
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(i+w,0,j-w);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(i,0,j-w);
			glEnd();

			// Front face drawn

			glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(i,3,j);
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(i+w,3,j);
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(i+w,0,j);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(i,0,j);
			glEnd();

			//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			//glPopMatrix();
			glDisable(GL_TEXTURE_2D);
			return;

		}
		if(current_i==new_tile_i && current_j==new_tile_j && tile_view_flag==1)
			glColor3f(1.0f,0.41f,0.703f);

		glPushMatrix(); 
		glTranslatef(0.0f,motion_floor[current_i][current_j],0.0f);

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D,_textureId3);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(i,0,j);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(i+w,0,j);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(i+w,0,j-w);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(i,0,j-w);
		glEnd();

		// Upper face drawn

		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(i,-t,j);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(i+w,-t,j);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(i+w,-t,j-w);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(i,-t,j-w);
		glEnd();

		// Lower face drawn

		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(i,0,j);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(i,0,j-w);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(i,-t,j-w);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(i,-t,j);
		glEnd();

		// Left face drawn

		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(i+w,0,j);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(i+w,0,j-w);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(i+w,-t,j-w);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(i+w,-t,j);
		glEnd();

		// Right face drawn

		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(i,0,j-w);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(i+w,0,j-w);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(i+w,-t,j-w);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(i,-t,j-w);
		glEnd();

		// Front face drawn

		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(i,0,j);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(i+w,0,j);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(i+w,-t,j);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(i,-t,j);
		glEnd();

		//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glPopMatrix();
	}
	if(level==3)
	{
		if(tile_no==39 || tile_no==70)
		{
			int x=tile_no/10;
			int y=tile_no%10;
			glPushMatrix();
			float star_x=new_tile[x][y].top_left_x+0.1;
			float star_z=new_tile[x][y].top_left_z+0.1;
			float star_y=new_tile[x][y].height-1.5;
			glTranslatef(star_x,star_y,star_z);
			//glRotatef(-45,1,0,0);
			glColor3f(0.0, 0.0, 0.0);
			lines(0.0,0.0,0.0,0.0,1.6,0.0);
			for(int i=0;i<4;i++)
			{
				glTranslatef(0.0,0.4,0.0);
				lines(0.0,0.0,0.0,w-0.2,0.0,0.0);
			}
			glTranslatef(w-0.2,0.0,0.0);
			lines(0.0,0.0,0.0,0.0,-1.6,0.0);
			glPopMatrix();
		}
		if(current_tile==tile_no)
		{
			if(level3_die[current_i][current_j]==-1)
				exit(0);
		}
		else
		{
			if(current_i>=0 && current_i<3 && up_down[current_i][current_j]==0)
			{
				new_tile[current_i][current_j].height=0;
				tile_array[tile_no].height=0;
				motion_floor[current_i][current_j]=0;
				up_down[current_i][current_j]=0;
				level_3[current_i][current_j]=0;
			}
			if(current_i>=3 && current_i<7  && up_down[current_i][current_j]==0)
			{
				new_tile[current_i][current_j].height=1.5;
				tile_array[tile_no].height=1.5;
				motion_floor[current_i][current_j]=1.5;
				level_3[current_i][current_j]=0;	
				up_down[current_i][current_j]=0;
			}
			if(current_i>=7 && current_i<10  && up_down[current_i][current_j]==0)
			{
				new_tile[current_i][current_j].height=3;
				tile_array[tile_no].height=3;
				motion_floor[current_i][current_j]=3;
				level_3[current_i][current_j]=0;	
				up_down[current_i][current_j]=0;
			}
		}
		new_tile[current_i][current_j].top_left_x=i;
		new_tile[current_i][current_j].top_left_z=j;

		tile_array[tile_no].top_left_x=i;
		tile_array[tile_no].top_left_z=j;
		tile_array[tile_no].top_right_x=i+w;
		tile_array[tile_no].top_right_z=j;
		tile_array[tile_no].bottom_right_x=i+w;
		tile_array[tile_no].bottom_right_z=j-w;
		tile_array[tile_no].bottom_left_x=i;
		tile_array[tile_no].bottom_left_z=j-w;

		if(current_i>=0 && current_i<3)
			glColor3f(1.0,1.0f,0.0f);

		if(current_i>=3 && current_i<7)
			glColor3f(0.0,1.0f,0.0f);

		if(current_i>=7 && current_i<10)
			glColor3f(0.0,0.0f,1.0f);


		if(current_i==new_tile_i && current_j==new_tile_j && tile_view_flag==1)
			glColor3f(1.0f,0.41f,0.703f);

		glPushMatrix(); 
		glTranslatef(0.0f,motion_floor[current_i][current_j],0.0f);

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D,_textureId3);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(i,0,j);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(i+w,0,j);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(i+w,0,j-w);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(i,0,j-w);
		glEnd();

		// Upper face drawn

		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(i,-t,j);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(i+w,-t,j);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(i+w,-t,j-w);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(i,-t,j-w);
		glEnd();

		// Lower face drawn

		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(i,0,j);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(i,0,j-w);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(i,-t,j-w);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(i,-t,j);
		glEnd();

		// Left face drawn

		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(i+w,0,j);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(i+w,0,j-w);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(i+w,-t,j-w);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(i+w,-t,j);
		glEnd();

		// Right face drawn

		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(i,0,j-w);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(i+w,0,j-w);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(i+w,-t,j-w);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(i,-t,j-w);
		glEnd();

		// Front face drawn

		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(i,0,j);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(i+w,0,j);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(i+w,-t,j);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(i,-t,j);
		glEnd();

		//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();

		if((bamboo[current_i][current_j]==3 || bamboo[current_i][current_j]==4) && current_tile==tile_no && bamboo_height[current_i][current_j] + 0.8 >=bot_y)
		{
			exit(0);
		}
		else if(bamboo[current_i][current_j]==3 || bamboo[current_i][current_j]==4)
		{
			glPushMatrix();
			float star_x=new_tile[current_i][current_j].top_left_x+w/2;
			float star_z=new_tile[current_i][current_j].top_left_z-w/2;
			float star_y=bamboo_height[current_i][current_j];
			glTranslatef(star_x,star_y,star_z);
			glRotatef(-90,1.0,0.0,0.0);
			drawbamboo(0.4f,0.1f,0.8f,32,32);
			glPopMatrix();

		}
	}
}

void initRendering() {

	glEnable(GL_DEPTH_TEST);      
	glEnable(GL_COLOR_MATERIAL); 
	glEnable(GL_NORMALIZE);
	glEnable(GL_DEPTH);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_NORMALIZE);
	//glShadeModel(GL_SMOOTH);

	//glClearColor(0.0f, 0.0f, 0.0f, 1.0f); 
	//	if(level==2)
	//	{
	glClearColor((float)(day_r),(float)(day_g),(float)(day_b),1.0f);
	glShadeModel(GL_SMOOTH);
	//	}

	Image* image1 = loadBMP("water.bmp");
	_textureId1 = loadTexture(image1);
	delete image1;

	Image* image2 = loadBMP("wall.bmp");
	_textureId2 = loadTexture(image2);
	delete image2;

	Image* image3 = loadBMP("tile.bmp");
	_textureId3 = loadTexture(image3);
	delete image3;

	Image* image5 = loadBMP("gold.bmp");
	_textureId5 = loadTexture(image5);
	delete image5;

	Image* image6 = loadBMP("pipe.bmp");
	_textureId6 = loadTexture(image6);
	delete image6;

	Image* image7 = loadBMP("sun.bmp");
	_textureId7 = loadTexture(image7);
	delete image7;

	Image* image8 = loadBMP("thorns.bmp");
	_textureId8 = loadTexture(image8);
	delete image8;

	Image* image88 = loadBMP("sun.bmp");
	_textureId88 = loadTexture(image88);
	delete image88;

	Image* image9 = loadBMP("tele.bmp");
	_textureId9 = loadTexture(image9);
	delete image9;
}

void handleResize(int w, int h) {

	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (float)w / (float)h, 0.1f, 200.0f);
	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();
}

void handleKeypress1(unsigned char key, int x, int y) {

	if (key == 27) {
		exit(0);     // escape key is pressed
	}
	if(key == 'w')
	{
		prev_x=bot_x;
		prev_z=bot_z;
		/*for(int i_it=0;i_it<100;i_it++)
		  {
		  }
		 */
		if(speed<1.0f)
			speed+=0.1f;
		float mycos=cos(DEG2RAD(theta));
		float mysin=sin(DEG2RAD(theta));
		if(theta>=0 && theta <=90)
		{
			bot_z+=(speed * mycos);
			bot_x+=(speed * mysin);
		}
		if(theta>90 && theta <=180)
		{
			bot_z-=(speed * ((-1)*(mycos)));
			bot_x+=(speed * mysin);
		}
		if(theta>180 && theta <=270)
		{
			bot_z-=(speed * ((-1)*(mycos)));
			bot_x-=(speed * ((-1)*(mysin)));
		}
		if(theta>270 && theta <=360)
		{
			bot_z+=(speed * mycos);
			bot_x-=(speed * ((-1)*(mysin)));
		}
		motion=1;
		if(front_leg==0)
			front_leg=1;
		else
			front_leg=0;
		int x=detect_collision();
		if(x==0)
		{
			bot_x=prev_x;
			bot_z=prev_z;
		}
		else if(x==2)
		{
			exit(0);
		}
	}
	if(key == 's')
	{
		prev_x=bot_x;
		prev_z=bot_z;

		float mycos=cos(DEG2RAD(theta));
		float mysin=sin(DEG2RAD(theta));
		if(theta>=0 && theta <=90)
		{
			bot_z-=(0.2 * mycos);
			bot_x-=(0.2 * mysin);
		}
		if(theta>90 && theta <=180)
		{
			bot_z+=(0.2 * ((-1)*(mycos)));
			bot_x-=(0.2 * mysin);
		}
		if(theta>180 && theta <=270)
		{
			bot_z+=(0.2 * ((-1)*(mycos)));
			bot_x+=(0.2 * ((-1)*(mysin)));
		}
		if(theta>270 && theta <=360)
		{
			bot_z-=(0.2 * mycos);
			bot_x+=(0.2 * ((-1)*(mysin)));
		}
		motion=1;
		if(front_leg==0)
			front_leg=1;
		else
			front_leg=0;
	}
	if(key == 'a')
	{
		theta+=10.0f;
		if(theta>360)
			theta=0;
	}
	if(key == 'd')
	{
		theta-=10.0f;
		if(theta<0)
			theta=theta+360;
	}
	if(key == 't' && tps==0)
	{
		fps=0;
		heli_view_flag=0;
		tile_view_flag=0;
		tps=1;
	}
	else if(key == 't' && tps==1)
	{
		tps=0;
	}
	if(key == 'j')
	{
		jump_flag=1;
	}
	if(key == 'f' && fps==0)
	{
		fps=1;
		tps=0;
		heli_view_flag=0;
		tile_view_flag=0;
	}
	else if(key == 'f' && fps==1)
	{
		fps=0;
	}
	if(key == 'l' && tile_view_flag==0)
	{
		fps=0;
		tps=0;
		heli_view_flag=0;
		tile_view_flag=1;
	}
	else if(key == 'l' && tile_view_flag==1)
	{
		tile_view_flag=0;
	}
	if(key == 'i')
	{
		tile_theta+=10.0f;
		if(tile_theta>360)
			tile_theta=0;
	}
	if(key == 'p')
	{
		tile_theta-=10.0f;
		if(tile_theta<0)
			tile_theta=tile_theta+360;
	}
	if(key == 'h' && heli_view_flag==0)
	{
		fps=0;
		tps=0;
		tile_view_flag=0;
		heli_view_flag=1;
	}
	else if(key == 'h' && heli_view_flag==1)
	{
		heli_view_flag=0;
	}
	if(key=='2')
	{
		bot_x=-2;
		bot_y=0;
		bot_z=1;
		up_bot=0;
		down_bot=1.4;
		level=2;
		r_x=90;
		r_y=0;
		r_z=-200;
		suraj_flag=0,change_flag=0;
		day_r=0;
		day_g=0;
		day_b=0;
		tps=0;
		fps=0;
		heli_view_flag=0;
		tile_view_flag=0;
	}
	if(key=='3')
	{
		bot_x=-2;
		bot_y=0;
		bot_z=1;
		up_bot=0;
		down_bot=1.4;
		level=3;
		r_x=90;
		r_y=0;
		r_z=-200;
		suraj_flag=0,change_flag=0;
		day_r=0;
		day_g=0;
		day_b=0;
		tps=0;
		fps=0;
		heli_view_flag=0;
		tile_view_flag=0;

		reset();
	}
	if(key=='b')
	{
		tele_flag=1;
	}
}

void handleKeypress2(int key, int x, int y){

	if (key == GLUT_KEY_LEFT)
	{
		if(new_tile_j!=0)
			new_tile_j--;
	}
	if (key == GLUT_KEY_RIGHT)
	{
		if(new_tile_j!=9)
			new_tile_j++;
	}
	if (key == GLUT_KEY_UP)
	{
		if(new_tile_i!=9)
			new_tile_i++;
	}
	if (key == GLUT_KEY_DOWN)
	{
		if(new_tile_i!=0)
			new_tile_i--;
	}
}

void drawbot()
{
	glPushMatrix();
	int i_it;
	for(i_it=0;i_it<100;i_it++)
	{
		if(bot_x > tile_array[i_it].top_left_x && bot_x < tile_array[i_it].top_right_x && bot_z < tile_array[i_it].top_left_z && bot_z > tile_array[i_it].bottom_left_z)
		{
			break;
		}
	}
	int x=i_it/10,y=i_it%10;
	if(level==2)
	{
		if(star_array[x][y]==-2)
		{
			exit(0);
		}
		if(star_array[x][y]==3)
		{
			star_array[x][y]=-1;
			score++;
		}
		if(x==0  && y==5 && star_taken==0)
		{
			star_taken=1;
			not_die=1;
		}
	}
	current_tile=i_it;
	if(level==3)
	{
		if(fabs(spi_x-bot_x)<0.5 && fabs(spi_z-bot_z)<0.5)
			exit(0);		

		if(level_3[x][y] < 50 && (x!=2 && y!=9) && (x!=6 && y!=0))
		{
			level_3[x][y]++;
		}
		else
		{
			new_tile[x][y].height-=0.02;
			tile_array[i_it].height-=0.02;
			motion_floor[x][y]-=0.02;
		}
		if(x>=0 && x<3 && motion_floor[x][y]<=-1.4)
		{
			level3_die[x][y]=-1;
		}
		else if(x>=3 && x<7 && motion_floor[x][y]<=0.5)
		{
			level3_die[x][y]=-1;
		}
		else if(x>=7 && x<10 && motion_floor[x][y]<=1.8)
		{
			level3_die[x][y]=-1;
		}

	}

	bot_y=tile_array[i_it].height+jump_y_crd;
	glTranslatef(bot_x,bot_y,bot_z);
	if(x==0 && y==0 && up_bot<=1.3)
	{
		glTranslatef(0.0,up_bot,0.0);
		up_bot+=0.009;
	}
	else if(x==0 && y==0)
	{
		glTranslatef(0.0,up_bot,0.0);
	}
	if(x==9 && y==9 && down_bot>0)
	{
		glTranslatef(12.3-bot_x,down_bot,-13.3-bot_z);
		down_bot-=0.009;
		string message="YOU WON!!";
		message=message;
		for(int i=0;i<message.length();i++)
		{
			glPushMatrix();
			glColor3f(1.0,1.0,1.0);
			glTranslatef(i*10-150,15.0,-200);
			glScalef(0.08,0.08,0.08);
			glutStrokeCharacter(GLUT_STROKE_ROMAN,message[i]);
			glPopMatrix();
		}

	}
	if(x==9 && y==9 && down_bot<=0)
	{
		level++;
		bot_x=-2;
		bot_y=0;
		bot_z=1;
		up_bot=0;
		down_bot=1.4;
		if(level==3)
			reset();
	}
	if(level==1)
	{
		if(tele[x][y]==2 && tele_flag==1)
		{
			int i,j,xx=0;
			for(i=x+1;i<10;i++)
			{
				for(j=y+1;j<10;j++)
				{
					if(tele[i][j]==2 && up_down[i][j]!=0 && up_down[i][j]!=1 && up_down[i][j]!=2 && i!=x && j!=y)
					{
						xx=1;
						break;
					}
				}
				if(xx==1)
					break;
			}
			if(xx==0)
			{
				for(j=0;j<10;j++)
				{
					for(i=0;i<10;i++)
					{
						if(tele[i][j]==2 && up_down[i][j]!=0 && up_down[i][j]!=1 && up_down[i][j]!=2 && i!=x && j!=y)
						{
							xx=1;
							break;
						}
					}
					if(xx==1)
						break;
				}

			}
			float star_x=new_tile[i][j].top_left_x+w/2;
			float star_z=new_tile[i][j].top_left_z-w/2;

			float star_y=new_tile[i][j].height;
			bot_x=star_x;
			bot_y=star_y;
			bot_z=star_z;
			tele_flag=0;
		}

	}
	glRotatef(theta,0.0f,1.0f,0.0f);
	glPushMatrix();
	glColor3f(0.99,0.902,0.72);
	glTranslatef(0.0f,1.1f,0.0f);
	glutSolidSphere(0.2,20.0,20.0);
	glTranslatef(0.1f,0.09f,0.16f);
	glColor3f(0,0,0);
	glutSolidSphere(0.02,20.0,20.0);
	glTranslatef(-0.16f,0.0f,0.00f);
	glutSolidSphere(0.02,20.0,20.0);
	glPopMatrix();

	//	Head Drawn back 2 origin

	glPushMatrix();
	glColor3f(0.539f,0.167f,0.881f);
	glTranslatef(0.0f,0.5f,0.0f);
	glRotatef(-90.0f,1.0f, 0.0f, 0.0f);
	GLUquadricObj *q1;
	q1 = gluNewQuadric();
	gluCylinder(q1,0.25f,0.25f,0.4f,20,20);

	gluDisk(q1,0.00,0.25,32,32); // disc @ bottom of cylinder

	glPopMatrix();

	// 	Body drawn back 2 origin

	glColor3f(0.48,0.488,0.488);
	glPushMatrix();
	glTranslatef(0.0f,0.5f,0.0f);
	glRotatef(-90.0f,1.0f, 0.0f, 0.0f);

	gluDisk(q1,0.00,0.25,32,32); // disc @ top of cylinder

	glTranslatef(0.29f,0.0f,0.4f);
	glutSolidSphere(0.05,20.0,20.0);   

	glColor3f(1,0.4,0.4);
	gluDisk(q1,0.00,0.05,32,32); // disc @ Right arm

	glTranslatef(0.02f,0.0f,-0.2f);
	if(motion==1 && front_leg==0)
	{
		glTranslatef(0.0f,-0.1f,0.0f);
		glRotatef(-45.0f,1.0f,0.0f,0.0f);
	}
	gluCylinder(q1,0.05f,0.05f,0.14f,32,32);
	gluDisk(q1,0.00,0.05,32,32);

	glColor3f(0.0f,0.4f,0.4f);
	glTranslatef(0.0f,0.0f,-0.20f);

	gluCylinder(q1,0.01f,0.05f,0.18f,32,32);


	glPopMatrix();

	//	right sholder drawn back 2 origin

	glColor3f(0.48,0.488,0.488);
	glPushMatrix();
	glTranslatef(0.0f,0.5f,0.0f);
	glRotatef(-90.0f,1.0f, 0.0f, 0.0f);

	gluDisk(q1,0.00,0.25,32,32);

	glTranslatef(-0.29f,0.0f,0.4f);
	glutSolidSphere(0.05,20.0,20.0);

	glColor3f(1,0.4,0.4);
	gluDisk(q1,0.00,0.05,32,32); // disc @ top of cylinder

	glTranslatef(-0.02f,0.0f,-0.2f);

	if(motion==1 && front_leg==1)
	{
		glTranslatef(0.0f,-0.1f,0.0f);
		glRotatef(-45.0f,1.0f,0.0f,0.0f);
	}
	gluCylinder(q1,0.05f,0.05f,0.14f,32,32);
	gluDisk(q1,0.00,0.05,32,32);

	glColor3f(0.0f,0.4f,0.4f);
	glTranslatef(0.0f,0.0f,-0.20f);
	gluCylinder(q1,0.01f,0.05f,0.18f,32,32);


	glPopMatrix();

	//	left shoulder drawn back 2 origin

	glPushMatrix();
	glColor3f(1,0.4,0.4);
	glTranslatef(-0.1,0.24,0.0);
	if(motion==1 && front_leg==0)
	{
		glTranslatef(0.0,0.0,0.1f);
		glRotatef(-125.0f,1.0f, 0.0f, 0.0f);
	}
	else if(motion==1 && front_leg==1)
	{
		glRotatef(-90.0f,1.0f, 0.0f, 0.0f);
	}
	else if(motion==0)
	{
		glRotatef(-90.0f,1.0f, 0.0f, 0.0f);
	}

	gluCylinder(q1,0.05f,0.05f,0.22f,32,32);

	gluDisk(q1,0.00,0.05,32,32);

	glPopMatrix();

	glPushMatrix();
	glColor3f(0.0f,0.4f,0.4f);
	glTranslatef(-0.1,0,0);
	glRotatef(-90.0f,1.0f, 0.0f, 0.0f);
	if(motion==1 && front_leg==0)
	{
		glTranslatef(0.0,-0.1,0.0f);
	}
	gluCylinder(q1,0.01f,0.05f,0.22f,32,32);


	glPopMatrix();

	//	Left leg drawn back 2 origin

	glPushMatrix();
	glColor3f(1,0.4,0.4);
	glTranslatef(0.1,0.24,0.0);
	if(motion==1 && front_leg==1)
	{
		glTranslatef(0.0,0.0,0.1f);
		glRotatef(-125.0f,1.0f, 0.0f, 0.0f);
	}
	else if(motion==1 && front_leg==0)
	{
		glRotatef(-90.0f,1.0f, 0.0f, 0.0f);
	}
	else if(motion==0)
	{
		glRotatef(-90.0f,1.0f, 0.0f, 0.0f);
	}
	gluCylinder(q1,0.05f,0.05f,0.22f,32,32);
	gluDisk(q1,0.00,0.05,32,32);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.0f,0.4f,0.4f);
	glTranslatef(0.1,0,0);
	glRotatef(-90.0f,1.0f, 0.0f, 0.0f);
	if(motion==1 && front_leg==1)
	{
		glTranslatef(0.0,-0.1,0.0f);
	}
	gluCylinder(q1,0.01f,0.05f,0.22f,32,32);

	glPopMatrix();

	//	Right leg drawn back 2 origin

	glPopMatrix();
}

void inertia(unsigned char key, int x, int y)
{
	if(key == 'w')
	{
		inertia_flag=1;
	}
}

void inertia2(int x)
{
	if(inertia_flag==1)
	{
		if(speed>0.1f)
			speed-=0.1f;
		else
		{
			inertia_flag=0;
			speed=0.0f;
			glutTimerFunc(70,inertia2, 0);
			return;
		}
		prev_x=bot_x;
		prev_z=bot_z;
		float mycos=cos(DEG2RAD(theta));
		float mysin=sin(DEG2RAD(theta));
		if(theta>=0 && theta <=90)
		{
			bot_z+=(speed * mycos);
			bot_x+=(speed * mysin);
		}
		if(theta>90 && theta <=180)
		{
			bot_z-=(speed * ((-1)*(mycos)));
			bot_x+=(speed * mysin);
		}
		if(theta>180 && theta <=270)
		{
			bot_z-=(speed * ((-1)*(mycos)));
			bot_x-=(speed * ((-1)*(mysin)));
		}
		if(theta>270 && theta <=360)
		{
			bot_z+=(speed * mycos);
			bot_x-=(speed * ((-1)*(mysin)));
		}
		motion=1;
		if(front_leg==0)
			front_leg=1;
		else
			front_leg=0;
		int x=detect_collision();
		if(x==0)
		{
			bot_x=prev_x;
			bot_z=prev_z;
		}
		else if(x==2)
		{
			exit(0);
		}

	}
	glutTimerFunc(70,inertia2, 0);
}

void updown(int x1)
{
	for(int i=0;i<10;i++)
	{
		for(int j=0;j<10;j++)
		{

			if(level!=3)
			{
				int u_i=up_down[i][j];
				if(u_i==0)
				{
					motion_floor[i][j]=4.0f;
				}
				else if(u_i==1)
				{
					motion_floor[i][j]+=0.02;
					if(motion_floor[i][j]>=2.2f)
						up_down[i][j]=2;

				}
				else if(u_i==2)
				{
					motion_floor[i][j]-=0.02;
					if(motion_floor[i][j]<=-2.2f)
						up_down[i][j]=1;
				}
				else
				{
					motion_floor[i][j]=0.0f;
				}
			}
		}
	}
	glutTimerFunc(1,updown, 0);
}

void GetOGLPos1(int x, int y)
{
	pos1X=x;
	pos1Z=y;
	click_flag=1;
}

void GetOGLPos2(int x ,int y)
{
	pos2X=x;
	pos2Z=y;
	//cout << pos2X << " " << pos2Z << endl;
	//cout << " click_detecyyed\n";
	click_flag=2;
}

int detect_collision()
{
	if(bot_x < -3.2 || bot_x > 13 || bot_z > 2 || bot_z < -13.5)
	{
		cout << " From Boundary\n";
		return 2;
	}

	float t_l_x=tile_array[current_tile].top_left_x;
	float t_l_z=tile_array[current_tile].top_left_z;
	float t_r_x=tile_array[current_tile].top_right_x;
	float t_r_z=tile_array[current_tile].top_right_z;

	float b_l_x=tile_array[current_tile].bottom_left_x;
	float b_l_z=tile_array[current_tile].bottom_left_z;
	float b_r_x=tile_array[current_tile].bottom_right_x;
	float b_r_z=tile_array[current_tile].bottom_right_z;

	int i;

	if(current_tile==99)
		cout << " You Won " << endl;

	if(bot_x > t_l_x && bot_x < t_r_x && bot_z < t_l_z && bot_z > b_l_z)
		return 1;
	else
	{
		for(i=0;i<100;i++)
		{
			if(bot_x > tile_array[i].top_left_x && bot_x < tile_array[i].top_right_x && bot_z < tile_array[i].top_left_z && bot_z > tile_array[i].bottom_left_z)
				break;
		}
		int bomb_i=i/10;
		int bomb_j=i%10;

		if(tile_array[i].height==4)
		{
			return 0;
		}

		if(tile_array[i].height <= bot_y)
		{
			return 1; // no collision == valid
		}

		if(tile_array[i].height > bot_y && tile_array[i].height < bot_y+0.7)
		{
			return 0; // intersecting with chest so only show motion dont move
		}

		if(tile_array[i].height < bot_y+0.8)
		{
			return 2; // die out
		}
	}
	return 0;
}

void tile_cam()
{
	float cam_x=new_tile[new_tile_i][new_tile_j].top_left_x;
	float cam_y=new_tile[new_tile_i][new_tile_j].height;
	float cam_z=new_tile[new_tile_i][new_tile_j].top_left_z;

	float lok_x = cam_x;
	float lok_y = cam_y+1.2f;
	float lok_z = cam_z;

	float mycos=cos(DEG2RAD(tile_theta));
	float mysin=sin(DEG2RAD(tile_theta));
	if(tile_theta>=0 && tile_theta <=90)
	{
		lok_z+=(10 * mycos);
		lok_x+=(10 * mysin);
	}
	if(tile_theta>90 && tile_theta <=180)
	{
		lok_z-=(10 * ((-1)*(mycos)));
		lok_x+=(10 * mysin);
	}
	if(tile_theta>180 && tile_theta <=270)
	{
		lok_z-=(10 * ((-1)*(mycos)));
		lok_x-=(10 * ((-1)*(mysin)));
	}
	if(tile_theta>270 && tile_theta <=360)
	{
		lok_z+=(10 * mycos);
		lok_x-=(10 * ((-1)*(mysin)));
	}

	gluLookAt(cam_x,cam_y,cam_z,lok_x,lok_y,lok_z,0,1,0);
}

void helicopter1()
{
	float heli_angle=((1200-pos1X)*360)/1200;

	float mycos=cos(DEG2RAD(heli_angle));
	float mysin=sin(DEG2RAD(heli_angle));

	float lok_x,lok_y,lok_z;

	lok_x=tower_x;
	lok_y=tower_y;
	lok_z=tower_z;

	if(heli_angle >=0 && heli_angle <=90)
	{
		lok_z+=(heli_dis * mycos);
		lok_x+=(heli_dis * mysin);
	}
	if(heli_angle >90 && heli_angle <=180)
	{
		lok_z-=(heli_dis * ((-1)*(mycos)));
		lok_x+=(heli_dis * mysin);
	}
	if(heli_angle >180 && heli_angle <=270)
	{
		lok_z-=(heli_dis * ((-1)*(mycos)));
		lok_x-=(heli_dis * ((-1)*(mysin)));
	}
	if(heli_angle >270 && heli_angle <=360)
	{
		lok_z+=(heli_dis * mycos);
		lok_x-=(heli_dis * ((-1)*(mysin)));
	}

	gluLookAt(tower_x,tower_y,tower_z,lok_x,0,lok_z,0,1,0);
	// gluLookAt(0,0,6,0,0,1,0,0,0);
}

void helicopter2()
{
	float heli_angle=((1200-pos2X)*360)/1200;

	float mycos=cos(DEG2RAD(heli_angle));
	float mysin=sin(DEG2RAD(heli_angle));

	float lok_x,lok_y,lok_z;


	if(heli_angle >=0 && heli_angle <=90)
	{
		tower_z=-5+(heli_dis * mycos);
		tower_x=5-(heli_dis * mysin);
	}
	if(heli_angle >90 && heli_angle <=180)
	{
		tower_z=-5-(heli_dis * ((-1)*(mycos)));
		tower_x=5-(heli_dis * mysin);
	}
	if(heli_angle >180 && heli_angle <=270)
	{
		tower_z=-5-(heli_dis * ((-1)*(mycos)));
		tower_x=5+(heli_dis * ((-1)*(mysin)));
	}
	if(heli_angle >270 && heli_angle <=360)
	{
		tower_z=-5+(heli_dis * mycos);
		tower_x=5+(heli_dis * ((-1)*(mysin)));
	}
	gluLookAt(tower_x,tower_y,tower_z,0,0,0,0,1,0);
	// gluLookAt(0,0,6,0,0,1,0,0,0);
}

void mouse(int button, int state, int x, int y)
{
	if(state== GLUT_DOWN)
	{
		if(button==3)
		{
			if(heli_view_flag==1)
			{
				if(heli_dis-2.2f>=0)
					heli_dis-=2.2f;
			}
		}
		else if(button==4)
		{
			if(heli_view_flag==1)
			{
				if(heli_dis+2.2f<=20)
					heli_dis+=2.2f;
			}
		}
	}
}

void explode(int x)
{
	srand (time(NULL));
	if(bomb_vec.size()==0)
		return;
	int sz=(rand())%(bomb_vec.size());
	int i=bomb_vec[sz].first;
	int j=bomb_vec[sz].second;
	//	cout << " i is " << i << " j is " << j << endl;
	if(star_array[i][j]==4)
	{
		bomb_array[i][j]-=3;
	}
	if(bomb_array[i][j]<0)
	{
		if(level==2)
			up_down[i][j]=-1;
		star_array[i][j]=-2;
		bomb_vec.erase(bomb_vec.begin() + sz);
		if(level==2)
		{
			motion_floor[i][j]=3.5;
			tile_array[i*10 + j].height=motion_floor[i][j];

		}
	}
	if(level==2)
		glutTimerFunc(400,explode, 0);
}

void display_projector() {

	glPushMatrix();
	glTranslatef(proj_x,4.5f,proj_z);
	glRotatef(180.0,0.0,0.0,1.0);
	glRotatef(proj_theta,0.0f,1.0f,0.0f);
	for(int j=0;j<n_project;j++)
	{
		float a,b,c,d,d1,d_x,d_y;
		a=project[j].a;
		b=project[j].b;
		c=project[j].c;
		d=project[j].d;
		d1=project[j].d1;
		float xdiff=c-a;
		float ydiff=d-b;
		float theta= atan2(ydiff,xdiff);
		float dis=project[j].d1;
		float med_x = (a+c)/2;
		float med_y = (b+d)/2;
		project[j].d_x= (dis)*(sin(theta)) + med_x;
		project[j].d_y= (-dis)*(cos(theta)) + med_y;

		d_x=project[j].d_x;
		d_y=project[j].d_y;

		glBegin(GL_TRIANGLES);
		glColor3f(0.5f, 0.0f, 1.0f);

		glVertex3f(a,b,0.0f);

		glColor3f(1.0f, 0.0f, 0.0f);

		glVertex3f(c,d,0.0f);

		glColor3f(0.0f, 1.0f, 0.0f);

		glVertex3f(d_x,d_y,0.0f);

		glEnd();

		glBegin(GL_POINTS);
		glColor3f(0.0f, 0.0f,1.0f);
		glVertex2f(project[j].d_x,project[j].d_y);
		glVertex2f(project[j].c,project[j].d);
		glVertex2f(project[j].a,project[j].b);
		glEnd();
		//color_ct=0;	
		draw_rays(j);
		if(fabs(proj_z-bot_z)<=1.0f && fabs(proj_x-bot_x)<=1.0f)
		{
			if(not_die!=1)
				exit(0);
		}
	}
	glPopMatrix();
}

void draw_rays(int x)
{
	float a,b,c,d,d_x,d_y,div,len,old_xdiff,old_theta,old_ydiff,old_x,old_y;
	a=project[x].a;
	b=project[x].b;
	c=project[x].c;
	d=project[x].d;
	d_x=project[x].d_x;
	d_y=project[x].d_y;
	div=project[x].n_division;
	len=sqrt(((c-a)*(c-a))+ ((d-b)*(d-b)));
	len/=(div-1);
	old_x=a;
	old_y=b;
	old_ydiff=d-b;
	old_xdiff=c-a;
	old_theta;
	//current_div_color=div;
	if(old_xdiff == 0)
	{
		if(old_ydiff>0)
			old_theta = PI/2;
		else
			old_theta = -PI/2;
	}
	else if(old_xdiff>0)
	{
		old_theta = atan(old_ydiff/old_xdiff);
	}
	else if(old_xdiff<0)
	{
		old_theta = PI+ atan(old_ydiff/old_xdiff);
	}

	for(int i=0;i<div;i++)
	{
		float y_diff = (old_y-d_y);
		float x_diff = (old_x-d_x);
		float slope;
		float theta;
		if(x_diff == 0)
		{
			if(y_diff>0)
				theta = PI/2;
			else
				theta = -PI/2;
		}
		else if(x_diff>0)
		{
			theta = atan(y_diff/x_diff);
		}
		else if(x_diff<0)
		{
			theta = PI+ atan(y_diff/x_diff);
		}

		float new_x,new_y;
		new_x = d_x + 8*cos(theta);
		new_y = d_y + 8*sin(theta);

		if(i<(div/3))
			glColor3f(1.0, 0.0, 0.0);
		else if(i >= div/3 && i <= (2*div)/3)
			glColor3f(0.0,1.0,0.0);
		else
			glColor3f(0.0,0.0,1.0);

		g_old_x=old_x;
		g_old_z=old_y;
		g_new_x=new_x;
		g_new_x=new_y;

		glBegin(GL_LINES);
		glLineWidth(4.5);
		glVertex3f(old_x,old_y,0.0);
		glVertex3f(new_x,new_y,0.0);
		glEnd();

		float x1=g_old_x+proj_x , y1=4.5 , z1=g_old_z+proj_z;
		float x2=g_new_x+proj_x , y2=0.0 , z2=g_old_z+proj_z;
		float x3=x2-x1 , y3=y2-y1 , z3=z2-z1;
		float x4=(bot_x-x1)/x3 , y4=(bot_y-y1)/y3 , z4=(bot_z-z1)/z3;

		if(fabs(x4-y4)<=0.5f || fabs(x4-z4)<=0.5f || fabs(y4-z4)<=0.5f)
		{
			if(not_die!=1)
				exit(0);
		}

		old_x = old_x + len*cos(old_theta);
		old_y = old_y + len*sin(old_theta);

	}
}

void proj_rotate(int z)
{
	//srand (time(NULL));
	int x=rand()%6;
	float prev_x=proj_x;
	float prev_z=proj_z;
	if(x==0)
	{
		proj_x+=1.0f;
	}
	else if(x==1)
	{
		proj_x-=1.0f;
	}
	else if(x==2)
	{
		proj_z+=1.0f;
	}
	else if(x==3)
	{
		proj_z-=1.0f;
	}
	else if(x==4)
	{
		proj_theta+=10.0;
	}
	else if(x==5)
	{
		proj_theta-=10.0;
	}
	if(proj_x < -3.2 || proj_x > 13 || proj_z > 2 || proj_z < -12.5)
	{
		proj_x=prev_x;
		proj_z=prev_z;
	}
	if(proj_theta>=360)
	{
		proj_theta=0;
	}
	if(proj_theta<=0)
	{
		proj_theta=0;
	}

	glutTimerFunc(50,proj_rotate, 0);
}

void bamboo_updown(int x1)
{
	for(int i=0;i<10;i++)
	{
		for(int j=0;j<10;j++)
		{

			if((bamboo[i][j]==3 || bamboo[i][j]==4) && i==current_tile/10 && j==current_tile%10 && bamboo_height[i][j]+0.8 < bot_y)
			{
				int x=2+3;
				int y=45+64;
			}
			else
			{
				// 0 down & 1 up
				int x;
				x=bamboo_type[i][j];
				if(x==0)
				{
					// move down
					bamboo_height[i][j]-=0.2;
					if(i>=0 && i<3)
					{
						if(bamboo_height[i][j]<=-1.4)
							bamboo_type[i][j]=1;
					}
					else if(i>=3 && i<7)
					{
						if(bamboo_height[i][j]<=0.5)
							bamboo_type[i][j]=1;
					}
					else if(i>=7 && i<10)
					{
						if(bamboo_height[i][j]<=1.8)
							bamboo_type[i][j]=1;
					}
				}
				else
				{
					// move up
					bamboo_height[i][j]+=0.2;
					if(i>=0 && i<3)
					{
						if(bamboo_height[i][j]>=0.0)
							bamboo_type[i][j]=0;
					}
					else if(i>=3 && i<7)
					{
						if(bamboo_height[i][j]>=1.5)
							bamboo_type[i][j]=0;
					}
					else if(i>=7 && i<10)
					{
						if(bamboo_height[i][j]>=3.0)
							bamboo_type[i][j]=0;
					}
				}
			}
		}
	}
	glutTimerFunc(100,bamboo_updown, 0);
}

void reset()
{
	for(int i=0;i<10;i++)
	{
		for(int j=0;j<10;j++)
		{
			up_down[i][j]=0;
			int x=i;
			int y=j;
			int i_it=10*i + j;
			if(i>=0 && i<3)
			{
				new_tile[x][y].height=0;
				tile_array[i_it].height=0;
				motion_floor[x][y]=0;
			}
			else if(i>=3 && i<7)
			{
				new_tile[x][y].height=1.5;
				tile_array[i_it].height=1.5;
				motion_floor[x][y]=1.5;

			}
			else if(i>=7 && i<10)
			{
				new_tile[x][y].height=3.0;
				tile_array[i_it].height=3.0;
				motion_floor[x][y]=3.0;
			}
		}
	}
}

void draw_spidey()
{
	glPushMatrix();
	glTranslatef(spi_x,spi_y+0.5,spi_z);
	glRotatef(spi_theta,0,1,0);
	glScalef(0.15,0.15,0.15);

	glColor3f(1.0,0.0,0.0);
	glutSolidSphere(2.0,20,20);

	//eyes
	glPushMatrix();
	glColor3f(0.0,0.0,0.0);
	glTranslatef(-0.3,0.5,2.0);
	glutSolidSphere(0.3,20,20);
	glTranslatef(0.6,0.0,0.0);
	glutSolidSphere(0.3,20,20);
	glPopMatrix();

	//right middle
	glPushMatrix();
	if(r_m==1)
	{
		glRotatef(r_middle,0.0,0.0,1.0);
		if(r_middle>=8)
			rm_flag=1;
		if(r_middle<=0)
			rm_flag=0;
		if(rm_flag==1)
			r_middle-=0.1;
		else if(rm_flag==0)
			r_middle+=0.1;
	}

	glColor3f(1.0,0.0,0.0);
	glTranslatef(2.0,0.0,0.0);
	glutSolidSphere(0.3,20,20);
	lines(0.0,0.0,0.0,2.0,3.0,0.0);
	glTranslatef(2.0,3.0,0.0);
	glutSolidSphere(0.2,20,20);
	lines(0.0,0.0,0.0,0.0,-5.0,0.0);
	glPopMatrix();

	//right back
	glPushMatrix();
	if(r_b==1)
	{
		glRotatef(r_back,0.0,0.0,1.0);
		if(r_back>=10)
			rb_flag=1;
		if(r_back<=0)
			rb_flag=0;
		if(rb_flag==1)
			r_back-=0.1;
		else if(rb_flag==0)
			r_back+=0.1;
	}
	glColor3f(1.0,0.0,0.0);
	glRotatef(40,0.0,1.0,0.0);
	glTranslatef(2.0,0.0,0.0);
	glutSolidSphere(0.3,20,20);
	lines(0.0,0.0,0.0,2.0,3.0,0.0);
	glTranslatef(2.0,3.0,0.0);
	glutSolidSphere(0.2,20,20);
	lines(0.0,0.0,0.0,0.0,-5.0,0.0);
	glPopMatrix();

	//right front
	glPushMatrix();
	if(r_f==1)
	{
		glRotatef(r_front,0.0,0.0,1.0);
		if(r_front>=9)
			rf_flag=1;
		if(r_front<=0)
			rf_flag=0;
		if(rf_flag==1)
			r_front-=0.1;
		else if(rf_flag==0)
			r_front+=0.1;
	}
	glTranslatef(0.0,0.0,-1.0);
	glColor3f(1.0,0.0,0.0);
	glRotatef(-40,0.0,1.0,0.0);
	glTranslatef(2.0,0.0,0.0);
	glutSolidSphere(0.3,20,20);
	lines(0.0,0.0,0.0,2.0,3.0,0.0);
	glTranslatef(2.0,3.0,0.0);
	glutSolidSphere(0.2,20,20);
	lines(0.0,0.0,0.0,0.0,-5.0,0.0);
	glPopMatrix();


	//left middle
	glPushMatrix();	
	if(l_m==1)
	{
		glRotatef(-l_middle,0.0,0.0,1.0);
		if(l_middle>=8)
			lm_flag=1;
		if(l_middle<=0)
			lm_flag=0;
		if(lm_flag==1)
			l_middle-=0.1;
		else if(lm_flag==0)
			l_middle+=0.1;
	}

	glTranslatef(-2.0,0.0,0.0);
	glColor3f(1.0,0.0,0.0);
	glutSolidSphere(0.3,20,20);
	lines(0.0,0.0,0.0,-2.0,3.0,0.0);
	glTranslatef(-2.0,3.0,0.0);
	glutSolidSphere(0.2,20,20);
	lines(0.0,0.0,0.0,0.0,-5.0,0.0);
	glPopMatrix();

	//left front
	glPushMatrix();  
	if(l_b==1)
	{
		glRotatef(-l_back,0.0,0.0,1.0);
		if(l_back>=10)
			lb_flag=1;
		if(l_back<=0)
			lb_flag=0;
		if(lb_flag==1)
			l_back-=0.1;
		else if(lb_flag==0)
			l_back+=0.1;
	}
	glTranslatef(0.0,0.0,-1.0);
	glColor3f(1.0,0.0,0.0);
	glRotatef(40,0.0,1.0,0.0);
	glTranslatef(-2.0,0.0,0.0);
	glutSolidSphere(0.3,20,20);
	lines(0.0,0.0,0.0,-2.0,3.0,0.0);
	glTranslatef(-2.0,3.0,0.0);
	glutSolidSphere(0.2,20,20);
	lines(0.0,0.0,0.0,0.0,-5.0,0.0);
	glPopMatrix();

	//left back
	glPushMatrix();
	if(l_f==1)
	{
		glRotatef(-l_front,0.0,0.0,1.0);
		if(l_front>=8)
			lf_flag=1;
		if(l_front<=0)
			lf_flag=0;
		if(lf_flag==1)
			l_front-=0.1;
		else if(lf_flag==0)
			l_front+=0.1;
	}

	glTranslatef(0.0,0.0,1.0);
	glColor3f(1.0,0.0,0.0);
	glRotatef(-40,0.0,1.0,0.0);
	glTranslatef(-2.0,0.0,0.0);
	glutSolidSphere(0.3,20,20);
	lines(0.0,0.0,0.0,-2.0,3.0,0.0);
	glTranslatef(-2.0,3.0,0.0);
	glutSolidSphere(0.2,20,20);
	lines(0.0,0.0,0.0,0.0,-5.0,0.0);
	glPopMatrix();

	glPopMatrix();
}

void process_spidey(int x)
{
	float prev_spi_x=spi_x;
	float prev_spi_y=spi_y;
	float prev_spi_z=spi_z;
	int prev_spi_i=spi_i;
	int prev_spi_j=spi_j;
	int x1=rand()%6;

	if(x1==0)
	{
		spi_x-=1.0;
	}
	else if(x1==1)
	{
		spi_x+=1.0;
	}
	else if(x1==2)
	{
		spi_z-=1.0;		
	}
	else if(x1==3)
	{
		spi_z+=1.0;
	}
	else if(x1==4)
	{
		spi_theta+=10;
	}
	else if(x1==5)
	{
		spi_theta-=10;
	}
	if(spi_x < -3.2 || spi_x > 13 || spi_z > 2 || spi_z < -12.5)
	{
		spi_x=prev_spi_x;
		spi_z=prev_spi_z;
	}
	if(spi_theta>=360)
	{
		spi_theta=0;
	}
	if(spi_theta<=0)
	{
		spi_theta=0;
	}
	int fk;
	int flag=0;

	for(int i=0;i<10;i++)
	{
		for(int j=0;j<10;j++)
		{
			fk=10*i+j;
			if(spi_x >= tile_array[fk].top_left_x && spi_x <= tile_array[fk].top_right_x && spi_z <= tile_array[fk].top_left_z && spi_z >= tile_array[i].bottom_left_z)
			{
				spi_i=i;
				spi_j=j;
				if(bamboo[i][j]==3 || bamboo[i][j]==4)
				{
					flag=1;
					break;
				}
				else if(tile_array[fk].height!=spi_y)
				{
					flag=1;
					break;
				}
			}
		}
		if(flag==1)
			break;
	}
	if(flag==1 || spi_x < -3.2 || spi_x > 13 || spi_z > -2.8 || spi_z < -9)
	{
		spi_x=prev_spi_x;
		spi_z=prev_spi_z;
		spi_i=prev_spi_i;
		spi_j=prev_spi_j;
	}

	glutTimerFunc(200,process_spidey, 0);
}
