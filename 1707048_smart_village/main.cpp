#include<GL/gl.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <stdlib.h>
#include <windows.h>
#include<bits/stdc++.h>
#define PI 3.1416


using namespace std;



float colR,colG,colB;
float angle1 = 0.0, mobus=-4;
int ti=1,mark=0;

bool light_on =true;
bool light_on2 =false;
bool light_on1 =false;
bool diff_light_on=true, spec_light_on=true, amb_light_on=true;
float l0=1,cl=1,moo=1;
GLfloat yaw_angle = 270.0, pitch_angle = 270.0, roll_angle = 90.0;

unsigned int ID;
vector<int>v;

void set_mat_prop(float colR=0.5, float colG=0.5, float colB=0.5, bool em=false)
{
    GLfloat m_no[] = { 0.0, 0.0, 0.0, 1.0 };
GLfloat mat_ambient[] = { colR, colG, colB, 1.0 };
GLfloat mat_diffuse[] = { colR, colG, colB, 1.0 };
GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat mat_shininess[] = {10};
GLfloat m_em[] = {colR,colG,colB,1};


glMaterialfv(GL_FRONT, GL_AMBIENT,mat_ambient);
glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
if(em & light_on1) glMaterialfv(GL_FRONT, GL_EMISSION, m_em);
else glMaterialfv(GL_FRONT, GL_EMISSION, m_no);



}
void circle2(GLdouble rad)
{
    GLint points = 50;
    GLdouble delTheta = (2.0 * PI) / (GLdouble)points;
    GLdouble theta = 0.0;

    glBegin(GL_POLYGON);
    {
        for(int i = 0; i <=50; i++, theta += delTheta )
        {
            glVertex2f(rad * cos(theta),rad * sin(theta));
        }
    }
    glEnd();
}
static void getNormal3p(GLfloat x1, GLfloat y1,GLfloat z1, GLfloat x2, GLfloat y2,GLfloat z2, GLfloat x3, GLfloat y3,GLfloat z3)
{
    GLfloat Ux, Uy, Uz, Vx, Vy, Vz, Nx, Ny, Nz;

    Ux = x2-x1;
    Uy = y2-y1;
    Uz = z2-z1;

    Vx = x3-x1;
    Vy = y3-y1;
    Vz = z3-z1;

    Nx = Uy*Vz - Uz*Vy;
    Ny = Uz*Vx - Ux*Vz;
    Nz = Ux*Vy - Uy*Vx;

    glNormal3f(Nx,Ny,Nz);
}


static GLfloat v_cube[8][3] =
{
    {0,0,0},
    {0,0,1},
    {0,1,0},
    {0,1,1},

    {1,0,0},
    {1,0,1},
    {1,1,0},
    {1,1,1}
};

static GLubyte c_ind[6][4] =
{
    {0,2,6,4},
    {1,5,7,3},
    {0,4,5,1},
    {2,3,7,6},
    {0,1,3,2},
    {4,6,7,5}
};


void cube(float colR, float colG, float colB, float alpha=1,bool em=false,float val=1)
{
    set_mat_prop(colR,colG,colB,em);

     if (val){

    glBegin(GL_QUADS);
    for (GLint i = 0; i <6; i++)
    {


        getNormal3p(v_cube[c_ind[i][0]][0], v_cube[c_ind[i][0]][1], v_cube[c_ind[i][0]][2],
                    v_cube[c_ind[i][1]][0], v_cube[c_ind[i][1]][1], v_cube[c_ind[i][1]][2],
                    v_cube[c_ind[i][2]][0], v_cube[c_ind[i][2]][1], v_cube[c_ind[i][2]][2]);

        for (GLint j=0; j<4; j++)
        {



            glTexCoord2f(0,val);
        glVertex3fv(&v_cube[c_ind[i][0]][0]);
        glTexCoord2f(0,0);
        glVertex3fv(&v_cube[c_ind[i][1]][0]);
        glTexCoord2f(val,0);
        glVertex3fv(&v_cube[c_ind[i][2]][0]);
        glTexCoord2f(val,val);
        glVertex3fv(&v_cube[c_ind[i][3]][0]);
        }

    }
    glEnd();
}
}
void drawCylinder(GLfloat difX, GLfloat difY, GLfloat difZ, GLfloat ambX, GLfloat ambY, GLfloat ambZ, GLfloat shine=40, GLdouble b_rad=.2, GLdouble t_rad=.2, GLdouble height=6)
{
    GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat mat_ambient[] = { ambX, ambY, ambZ, 1.0 };
    GLfloat mat_diffuse[] = { difX, difY, difZ, 1.0 };
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = {shine};

    glMaterialfv( GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv( GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv( GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv( GL_FRONT, GL_SHININESS, mat_shininess);
    glMaterialfv( GL_FRONT, GL_EMISSION, no_mat);

    GLUquadricObj *quadratic;
    quadratic = gluNewQuadric();
    gluCylinder(quadratic, b_rad, t_rad, height, 32, 32);


}
class BmpLoader
{
public:
    unsigned char* textureData;
    int iWidth, iHeight;

    BmpLoader(const char*);
    ~BmpLoader();

private:
    BITMAPFILEHEADER bfh;
    BITMAPINFOHEADER bih;
};

BmpLoader::BmpLoader(const char* filename)
{
    FILE *file=0;
    file=fopen(filename, "rb");
    if(!file)
        cout<<"File not found"<<endl;
    fread(&bfh, sizeof(BITMAPFILEHEADER),1,file);
    if(bfh.bfType != 0x4D42)
        cout<<"Not a valid bitmap"<<endl;
    fread(&bih, sizeof(BITMAPINFOHEADER),1,file);
    if(bih.biSizeImage==0)
        bih.biSizeImage=bih.biHeight*bih.biWidth*3;
    textureData = new unsigned char[bih.biSizeImage];
    fseek(file, bfh.bfOffBits, SEEK_SET);
    fread(textureData, 1, bih.biSizeImage, file);
    unsigned char temp;
    for(int i=0; i<bih.biSizeImage; i+=3)
    {
        temp = textureData[i];
        textureData[i] = textureData[i+2];
        textureData[i+2] = temp;

    }

    iWidth = bih.biWidth;
    iHeight = bih.biHeight;
    fclose(file);
}

BmpLoader::~BmpLoader()
{
    delete [] textureData;
}


void LoadTexture(const char*filename)
{
    glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_2D, ID);
    glPixelStorei(GL_UNPACK_ALIGNMENT, ID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    BmpLoader bl(filename);
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, bl.iWidth, bl.iHeight, GL_RGB, GL_UNSIGNED_BYTE, bl.textureData );
}



static void resize(int width, int height)
{
    const float ar = (float) width / (float) height;

    glViewport(0, 0, width, height);

}

const int width = 1900;
const int height = 1900;

static int slices = 16;
static int stacks = 16;

GLfloat eyeX = 18;
GLfloat eyeY = 20;
GLfloat eyeZ = -120;

GLfloat lookX = 11;
GLfloat lookY = 5;
GLfloat lookZ = 28;
float a=5,b=-5;
float rot = 0;
double angle = 0 ;

int stop=0;

void init(void)
{

	glClearColor(58.0/255.0, 157.0/255.0, 216.0/255.0, 0.0);


	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0.0, 1000.0, 0.0, 500.0);


}

void sky()
{
    GLUquadric *quad;
    quad = gluNewQuadric();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,v[4]);
    glPushMatrix();
    glTranslatef(0,10.2,0);
    glScalef(145,230,170 );
    cube(1,1,1,1,0,0);
    gluQuadricTexture(quad,1);
    gluSphere(quad,1,100,100);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

void circle(float colR=0.5, float colG=0.5, float colB=0.5, bool em=false)
{

glPushMatrix();
       set_mat_prop(colR,colG,colB,em);
        glTranslated(2.4,0.8,-6);
        glutSolidSphere(1,slices,stacks);
    glPopMatrix();
}

int li21=0,li22=0,li23=0;
void light0()
{

    GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0};
    GLfloat l_amb[]={l0*ti,l0*ti,l0*ti,1.0};
    GLfloat l_diff[]={1*ti,1*ti,1*ti,1};
    GLfloat l_spec[]={1*ti,1*ti,1*ti,1};
    GLfloat l_pos[]={12,70,-86,1};
    glEnable(GL_LIGHT0);


    if(amb_light_on)
        glLightfv(GL_LIGHT0, GL_AMBIENT, l_amb);
    else
        glLightfv(GL_LIGHT0, GL_AMBIENT, no_mat);

    if(diff_light_on)
        glLightfv(GL_LIGHT0, GL_DIFFUSE, l_diff);

    else
        glLightfv(GL_LIGHT0, GL_DIFFUSE, no_mat);

    if(spec_light_on)
        glLightfv(GL_LIGHT0, GL_SPECULAR, l_spec);
    else
        glLightfv(GL_LIGHT0, GL_SPECULAR, no_mat);

     glLightfv(GL_LIGHT0,GL_POSITION,l_pos);


}


void light2()
{

    GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0};
    GLfloat l_amb[]={0.5,0.5,0.5,1.0};
    GLfloat l_diff[]={1,1,1,1};
    GLfloat l_spec[]={1,1,1,1};
    GLfloat l_pos[]={3,3,-9,1};
    glEnable(GL_LIGHT2);

    GLfloat spot_dir[]={0,-1,0,1};
    GLfloat spot_c[]={65};
    glLightfv(GL_LIGHT2,GL_SPOT_DIRECTION,spot_dir);
    glLightfv(GL_LIGHT2,GL_SPOT_CUTOFF,spot_c);

    if(light_on2)
{


glLightfv(GL_LIGHT2,GL_AMBIENT,l_amb);
            glLightfv(GL_LIGHT2,GL_DIFFUSE,l_diff);
            glLightfv(GL_LIGHT2,GL_SPECULAR,l_spec);



}
else
{
            glLightfv(GL_LIGHT2,GL_AMBIENT,no_mat);
            glLightfv(GL_LIGHT2,GL_DIFFUSE,no_mat);
            glLightfv(GL_LIGHT2,GL_SPECULAR,no_mat);

}
glLightfv(GL_LIGHT2,GL_POSITION,l_pos);



}
void Sun(){

    glPushMatrix();
    cube(1,1,0,1,0,0);
    glScalef(3,3,3);
    glTranslatef(5,a+30,1);

    circle(1,1,0);
    glPopMatrix();

}
void moon(){
    glPushMatrix();

    glScalef(3,3,3);
    glTranslatef(5,b-35,1);
    circle(1,1,1);
    glPopMatrix();
}

void field()
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,v[0]);
    glPushMatrix();
     glScalef(80,1,170);
    glTranslatef(-0.87,-8,-0.9);
    cube(1,1,1,1,0,20);
    glPopMatrix();
     glPushMatrix();
     glScalef(80,1,170);
    glTranslatef(0.39,-8,-0.9);
    cube(1,1,1,1,0,20);
    glPopMatrix();


    glDisable(GL_TEXTURE_2D);

}

void tree()
{
    glPushMatrix();glScalef(.5, .36, .5); glTranslatef(-0,6,20);glRotatef(90, 1, 0, 0);drawCylinder(0.5, 0.2, 0.2,  0.2,0.2,0.2   ,40, .5,.65,6);glPopMatrix();
    glPushMatrix();glScalef(.5, .36, .5); glTranslatef(-0,9,20);glRotatef(90, 1, 0, 0);drawCylinder(0.0, 0.2, 0.0,  0.0,0.2,0.0   ,60, .1,1.7,5.5);glPopMatrix();
    glPushMatrix();glScalef(.5, .36, .5); glTranslatef(-0,7.5,20);glRotatef(90, 1, 0, 0);drawCylinder(0.0, 0.2, 0.0,  0.0,0.2,0.0   ,60, .1,1.7,5.5);glPopMatrix();

    glPushMatrix();glScalef(.5, .36, .5); glTranslatef(-0,6,22);glRotatef(90, 1, 0, 0);drawCylinder(0.4, 0.2, 0.2,  0.2,0.2,0.2   ,40, .5,.65,6);glPopMatrix();
    glPushMatrix();glScalef(.5, .36, .5); glTranslatef(-0,9,22);glRotatef(90, 1, 0, 0);drawCylinder(0.0, 0.4, 0.0,  0.0,0.3,0.0   ,60, .1,1.7,5.5);glPopMatrix();
    glPushMatrix();glScalef(.5, .36, .5); glTranslatef(-0,7.5,22);glRotatef(90, 1, 0, 0);drawCylinder(0.0, 0.4, 0.0,  0.0,0.3,0.0   ,60, .1,1.7,5.5);glPopMatrix();
}
void river()
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,v[1]);
    glPushMatrix();

    glTranslatef(10.2,-25,-145);
    glScalef(21,1,170);
    cube(1,1,1,1,0,4);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,v[10]);
    glPushMatrix();
    glScalef(0.3,20,170);
    glTranslatef(35,-1.22,-0.87);
    cube(1,1,1,1,0,5);
    glPopMatrix();

    glPushMatrix();
    glScalef(0.3,20,170);
    glTranslatef(103,-1.22,-0.87);
    cube(1,1,1,1,0,5);
    glPopMatrix();

    glPushMatrix();
    glScalef(20,20,1);
    glTranslatef(0.55,-1.2,20);
    cube(1,1,1,1,0,5);
    glPopMatrix();


    glDisable(GL_TEXTURE_2D);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,v[11]);

     glPushMatrix();
    glScalef(24,9,1);
    glTranslatef(0.35,-0.3,-20);
    cube(1,1,1);
    glPopMatrix();

    glPushMatrix();
    glScalef(24,9,1);
    glTranslatef(0.35,-0.3,-32);
    cube(1,1,1);
    glPopMatrix();

  glDisable(GL_TEXTURE_2D);


}
double anglecar=0;
void car()
{

   glPushMatrix(); //Right Wheel Far
   cube(0,0,0,1,0,0);
   glTranslated(0.75,-0.5, -0.5);
   glRotated(anglecar,0,0,1);
   glutWireTorus (0.1,0.2,10,10);
   glPopMatrix();

   glPushMatrix(); //Left Wheel Far
   cube(0,0,0,1,0,0);
   glTranslated(-0.75,-0.5, -0.5);
   glRotated(anglecar,0,0,1);
   glutWireTorus (0.1,0.2,10,10);
   glPopMatrix();

   glPushMatrix(); //Car BOX
   cube(0,0,1,1,0,0);
   glScaled(2,1,1);
   glutSolidCube(1);
   //glDisable(GL_TEXTURE_2D);
   cube(0,0,0,1,0,0);
   glutWireCube(1);
   glPopMatrix();
   //glDisable(GL_TEXTURE_2D);


   glPushMatrix(); //Right Wheel Far
   cube(0,0,0,1,0,0);
   glTranslated(0.75,-0.5, 0.5);
   glRotated(anglecar,0,0,1);
   glutWireTorus (0.1,0.2,10,10);
   glPopMatrix();

   glPushMatrix(); //Left Wheel Far
   cube(0,0,0,1,0,0);
   glTranslated(-0.75,-0.5, 0.5);
   glRotated(anglecar,0,0,1);
   glutWireTorus (0.1,0.2,10,10);
   glPopMatrix();




}
void busMain()
{
   glTranslated(2,-0.5, -0.3);
   glPushMatrix(); //Left Wheel Near
   cube(1,1,1,1,0,0);
   glTranslated(2,-0.5, -0.5);
   glRotated(angle,0,0,1);
   glutWireTorus (0.1,0.2,10,10);
   glPopMatrix();

   glPushMatrix(); //Right Wheel Near
   cube(1,1,1,1,0,0);
   glTranslated(-0.75,-0.5, -0.5);
   glRotated(angle,0,0,1);
   glutWireTorus (0.1,0.2,10,10);
   glPopMatrix();


   glPushMatrix(); //Bus BOX
   cube(0.275,0.510,0.706,1,0,0);
   glScaled(5,3,1);
   glTranslated(0.1,0.4, 0.13);
   glutSolidCube(1);
   cube(0,0,0,1,0,0);
   glutWireCube(1);
   glPopMatrix();

   //Bus front
    glPushMatrix();
   cube(0,0,0,1,0,0);
   glScaled(0.2,1.3,0.8);
   glTranslated(15,1.3, 0.17);
   glutSolidCube(1);
   glPopMatrix();

   glPushMatrix(); //Bus door
   cube(0,0,0,1,0,0);
   glScaled(0.8,2,0.8);
   glTranslated(3,0.3, 0.03);
   glutSolidCube(1);
   glPopMatrix();

   glPushMatrix(); //Bus window1
   cube(0,0,0,1,0,0);
   glScaled(1,1,0.8);
   glTranslated(1.3,1.8, 0.03);
   glutSolidCube(1);
   glPopMatrix();

   glPushMatrix(); //Bus window2
   cube(0,0,0,1,0,0);
   glScaled(1,1,.8);
   glTranslated(0,1.8, 0.03);
   glutSolidCube(1);
   //cube(0,0,0,1,0,0);
   //glutWireCube(1);
   glPopMatrix();

   glPushMatrix(); //Bus window3
   cube(0,0,0,1,0,0);
   glScaled(1,1,.8);
   glTranslated(-1.3,1.8, 0.03);
   glutSolidCube(1);
   //cube(0,0,0,1,0,0);
   glPopMatrix();


   glPushMatrix(); //Left Wheel Far
   cube(1,1,1,1,0,0);
   glTranslated(2,-0.5, 0.5);
   glRotated(angle,0,0,1);
   glutWireTorus (0.1,0.2,10,10);
   glPopMatrix();

   glPushMatrix(); //Right Wheel Far
   cube(0,0,0,1,0,0);
   glTranslated(-0.75,-0.5, 0.5);
   glRotated(angle,0,0,1);
   glutWireTorus (0.1,0.2,10,10);
   glPopMatrix();

   angle-=1;
}

void bus ()
{

   glTranslatef(mobus,0,0); // to move bus
   glScaled(0.5,0.5,0.5);

   glPushMatrix();
   glTranslated (-2.4,0,0);
   busMain();
   glPopMatrix();
   /*glPushMatrix();
   glTranslated (0,0,0);
   trainMain();
   glPopMatrix();
   glPushMatrix();
   glTranslated (2.4,0,0);
   trainMain();
   glPopMatrix();*/
   if (mobus<6)
  	mobus+=0.02; //control train speed from here
   else
  	mobus=-6;



   }

void bus_stop()
{
    glEnable(GL_TEXTURE_2D);
     glBindTexture(GL_TEXTURE_2D,v[5]);
    glPushMatrix();
    glScalef(18,17,6);
    glTranslatef(2,-0.6,-0.4);
    cube(1,1,1);
    glPopMatrix();

    glPushMatrix();
    glScalef(12,15,6);
    glTranslatef(3.3,0.3,-0.4);
    cube(1,1,1);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

     glPushMatrix();
     glEnable(GL_TEXTURE_2D);
     glBindTexture(GL_TEXTURE_2D,v[9]);
    glScalef(4,14,1);
    glTranslatef(11,-0.5,-3);
    cube(1,1,1);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

   glEnable(GL_TEXTURE_2D);
     glBindTexture(GL_TEXTURE_2D,v[6]);
    glPushMatrix();
    glScalef(9,4,1);
    glTranslatef(4.55,3,-3.5);
    cube(1,1,1,1,0,1);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

}
void library()
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,v[13]);
    glPushMatrix();
    glScalef(18,17,6);
    glTranslatef(2,-0.6,-0.4);
    cube(0.690, 0.878, 0.902,1,0,2);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);


    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,v[14]);
     glPushMatrix();
    glScalef(4,9,1);
    glTranslatef(11,-1.2,-2.5);
    cube(1,1,1);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);


     glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,v[13]);
    glPushMatrix();
   glScalef(19,8,7);
    glTranslatef(0.9,-1.3,-0.3);
    cube(0.690, 0.878, 0.902);
    glPopMatrix();
  glDisable(GL_TEXTURE_2D);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,v[15]);
    glPushMatrix();
    glScalef(19,2,1);
    glTranslatef(1.85,1.5,-2.5);
    cube(1, 1, 1);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,v[14]);
     glPushMatrix();
    glScalef(4,7,1);
    glTranslatef(6,-1.5,-3);
    cube(1,1,1);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    /* HOSPITAL */

    //left_buil
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,v[16]);
    glPushMatrix();
    glScalef(18,17,6);
    glTranslatef(4.4,-0.6,6.4);
    cube(0.690, 0.878, 0.902,1,0,2);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    //right_buil
     glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,v[16]);
    glPushMatrix();
   glScalef(19,8,7);
    glTranslatef(3.3,-1.3,5.5);
    cube(0.690, 0.878, 0.902);
    glPopMatrix();
   glDisable(GL_TEXTURE_2D);

    glPushMatrix();
    glScalef(7,1.5,0.1);
    glTranslatef(12.25,1.5,380);
    cube(1,0,0);
    glPopMatrix();


    glPushMatrix();
    glScalef(1.5,5,0.1);
    glTranslatef(59.3,0.1,380);
    cube(1,0,0);
    glPopMatrix();

    //left door
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,v[17]);
     glPushMatrix();
    glScalef(4,9,1);
    glTranslatef(22,-1.2,38);
    cube(1,1,1);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    //right door
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,v[17]);
    glPushMatrix();
    glScalef(4,7,1);
    glTranslatef(17.5,-1.5,38);
    cube(1,1,1);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    //Emergency
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,v[18]);
    glPushMatrix();
    glScalef(19,2,1);
    glTranslatef(3.18,-1,37);
    cube(1, 1, 1);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

}

void powerplant()
{
   glEnable(GL_TEXTURE_2D);
     glBindTexture(GL_TEXTURE_2D,v[3]);
    glPushMatrix();
    glScalef(18,17,6);
    glTranslatef(2,-0.6,-0.4);
    cube(1,1,1);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,v[9]);
     glPushMatrix();
    glScalef(4,9,1);
    glTranslatef(11,-1.2,-3);
    cube(1,1,1);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

   glEnable(GL_TEXTURE_2D);
     glBindTexture(GL_TEXTURE_2D,v[12]);
    glPushMatrix();
    glScalef(10,4,1);
    glTranslatef(4,0,-3);
    cube(1,1,1,1,0,1);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

void Windmill() {
	int i;
	cube(0,0,0,1,0,0);
	glBegin(GL_POLYGON);
	glVertex3f(-0.05f, 0,0);
	glVertex3f(0.05f, 0,0);
	glVertex3f(0.05f, 3,0);
	glVertex3f(-0.05f, 3,0);
	glEnd();
	glTranslatef(0, 3, 0);
	glRotated(angle1, 0, 0, 1);
	cube(0,0,0,1,0,0);
	//glColor3f(1, 1, 1);
	for (i = 0; i < 3; i++) {
		glRotated(120, 0, 0, 1);  // Note: These rotations accumulate.
		glBegin(GL_POLYGON);
		glVertex3f(0,0,0);
		glVertex3f(0.5, 0.1,0);
		glVertex3f(1.5f,0,0);
		glVertex3f(0.5f, -0.1f,0);
		glEnd();
	}
}

void school()
{

   glPushMatrix(); //main building
   cube(0.91, 0.76,0.65,1,0,0);
   glTranslated(0,0.4,0);
   glScaled(2,0.8,1);
   glutSolidCube(1);
   cube(0.91, 0.76,0.65,1,0,0);
   glutWireCube(1);
   glPopMatrix();

   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D,v[9]);
   glPushMatrix(); //door
   glTranslated(0,0,-0.54);
   glScaled(0.3,0.4,0.2);
   cube(1,1,1);
   glPopMatrix();
   glDisable(GL_TEXTURE_2D);

   glPushMatrix(); //main building roof
   cube(0,0,0,1,0,0);
   glTranslated(0,0.8,-0.05);
   glScaled(2,0.02,1);
   glutSolidCube(1);
   cube(0,0,0,1,0,0);
   glutWireCube(1);
   glPopMatrix();

   glPushMatrix(); //room on floor
   cube(0.91, 0.76,0.65,1,0,0);
   glTranslated(0,1.1,0);
   glScaled(1.4,0.6,1);
   glutSolidCube(1);
   cube(0.65, 0.49,0.24,1,0,0);
   glutWireCube(1);
   glPopMatrix();

   glPushMatrix(); //top room roof
   cube(0,0,0,1,0,0);
   glTranslated(0,1.4,-0.09);
   glScaled(2,0.07,1);
   glutSolidCube(1);
   cube(0,0,0,1,0,0);
   glutWireCube(1);
   glPopMatrix();

    glEnable(GL_TEXTURE_2D);
     glBindTexture(GL_TEXTURE_2D,v[8]);
   glPushMatrix(); //top room window
   glTranslated(0,1,-0.55);
   glScaled(0.4,0.3,1);
   cube(1,1,1);
   glPopMatrix();
   glDisable(GL_TEXTURE_2D);

   glPushMatrix(); //room on floor
   cube(1,1,1,1,0,0);
   glTranslated(0,0.6,0);
   glScaled(.6,0.2,0.1);
   glutSolidCube(1);
   glPopMatrix();

   glEnable(GL_TEXTURE_2D);
     glBindTexture(GL_TEXTURE_2D,v[7]);
   glPushMatrix();//signboard
   glScalef(0.9,0.2,0.2);
   glTranslatef(-0.5,2.5,-2.6);
   cube(1,1,1);
   glPopMatrix();
   glDisable(GL_TEXTURE_2D);

}



void house1(float R1,float G1,float B1) {



    /* main rec */
    cube (R1+1, G1+.8,B1+0.6,1,0,0);
    glBegin(GL_POLYGON);
    glVertex3f (0.2, 0.1, 0.0);
    glVertex3f (0.9, 0.1, 0.0);
    glVertex3f (0.9, 0.575, 0.0);
    glVertex3f (0.2, 0.575, 0.0);
    glEnd();

    /*back rec*/
    cube (R1+1, G1+.8,B1+0.6,1,0,0);
    glBegin(GL_POLYGON);
    glVertex3f (0.2, 0.1, 0.5);
    glVertex3f (0.9, 0.1, 0.5);
    glVertex3f (0.9, 0.575, 0.5);
    glVertex3f (0.2, 0.575, 0.5);
    glEnd();

    /* left rec */
    cube (R1+1, G1+.8,B1+0.6,1,0,0);
    glBegin(GL_POLYGON);
    glVertex3f (0.2, 0.1, 0.5);
    glVertex3f (0.2, 0.1, 0.0);
    glVertex3f (0.2, 0.575, 0.0);
    glVertex3f (0.2, 0.575, 0.5);
    glEnd();

    /* right rec */
    cube (R1+1, G1+1,B1+0.8,1,0,0);
    glBegin(GL_POLYGON);
    glVertex3f (0.9, 0.1, 0.5);
    glVertex3f (0.9, 0.1, 0.0);
    glVertex3f (0.9, 0.575, 0.0);
    glVertex3f (0.9, 0.575, 0.5);
    glEnd();

    /* left tri */
    cube (R1+.8, G1+0.4, B1+0,1,0,0);
    glBegin(GL_TRIANGLES);
    glVertex3f (0.9, 0.575, 0.0);
    glVertex3f (0.9, 0.575, 0.5);
    glVertex3f (0.9, 0.8, 0.25);
    glEnd();

    /* right tri */
    cube (R1+.8, G1+0.4, B1+0,1,0,0);
    glBegin(GL_TRIANGLES);
    glVertex3f (0.2, 0.575, 0.0);
    glVertex3f (0.2, 0.575, 0.5);
    glVertex3f (0.2, 0.8, 0.25);
    glEnd();

    /* roof */
    cube (R1+.4, G1+.2, B1+0,1,0,0);
    glBegin(GL_POLYGON);
    glVertex3f (0.2, 0.575, 0.0);
    glVertex3f (0.9, 0.575, 0.0);
    glVertex3f (0.9, 0.8, 0.25);
    glVertex3f (0.2, 0.8, 0.25);
    glEnd();

    /*back roof */
    cube (R1+.4, G1+0.2, B1+0,1,0,0);
    glBegin(GL_POLYGON);
    glVertex3f (0.2, 0.575, 0.5);
    glVertex3f (0.9, 0.575, 0.5);
    glVertex3f (0.9, 0.8, 0.25);
    glVertex3f (0.2, 0.8, 0.25);
    glEnd();

    /* door */
    cube(R1+.376, G1+0.376, B1+.376,1,0,0);
    glBegin(GL_POLYGON);
    glVertex3f (0.47, 0.105, -0.01);
    glVertex3f (0.65, 0.105, -0.01);
    glVertex3f (0.65, 0.46, -0.01);
    glVertex3f (0.47, 0.46, -0.01);
    glEnd();

    /* window 1 */
    cube(R1+.3, G1+0.2, B1+.1,1,0,0);
    glBegin(GL_POLYGON);
    glVertex3f (0.72, 0.25, -0.01);
    glVertex3f (0.83, 0.25, -0.01);
    glVertex3f (0.83, 0.4, -0.01);
    glVertex3f (0.72, 0.4, -0.01);
    glEnd();

    /* window 2 */
    cube(R1+.3, G1+0.2, B1+.1,1,0,0);
    glBegin(GL_POLYGON);
    glVertex3f (0.27, 0.25, -0.01);
    glVertex3f (0.38, 0.25, -0.01);
    glVertex3f (0.38, 0.4, -0.01);
    glVertex3f (0.27, 0.4, -0.01);
    glEnd();

}
void road1()
{
     glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,v[2]);
     glPushMatrix();

    glTranslatef(-7.8,-3.4,-3.6);
    glScalef(20,0.1,0.5);
    cube(1,1,1,1,0,20);
    glPopMatrix();
     glDisable(GL_TEXTURE_2D);
}
 void road()
 {
     //Front Road
      glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,v[2]);
     glPushMatrix();

    glTranslatef(-6.3,-3.7,-3.6);
    glScalef(18.1,0.1,0.5);
    cube(1,1,1,1,0,20);
    glPopMatrix();
     glDisable(GL_TEXTURE_2D);

    //Back Road
     glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,v[2]);
     glPushMatrix();

    glTranslatef(-6.3,-3.7,2);
    glScalef(18.1,0.1,0.5);
    cube(1,1,1,1,0,20);
    glPopMatrix();
     glDisable(GL_TEXTURE_2D);

    glDisable(GL_TEXTURE_2D);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,v[11]);

    glPushMatrix();
    glScalef(3,1.5,0.07);
    glTranslatef(0.4,-2.5,-40);
    cube(1,1,1);
    glPopMatrix();

    glPushMatrix();
    glScalef(3,1.5,0.07);
    glTranslatef(0.4,-2.5,-55);
    cube(1,1,1);
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);


 }
 void roadlight()
 {
     glPushMatrix();
     glScalef(0.07,2.5,0.06);
     glTranslatef(63,-1.5,-50);
     cube(0,0,0);
     glPopMatrix();

     glPushMatrix();
     glScalef(0.3,0.1,0.08);
     glTranslatef(13.8,-13.1,-38.5);
     cube(1,1,1);
     glPopMatrix();
 }

float k=0;
void spin()
{
    angle1 = (angle1+k)*stop;
    k=k+.001;
    if (angle1 >= 360)
        angle1 = 0;
}


int anglex= 0, angley = 0, anglez = 0;          //rotation angles
int wired=0;
int shcpt=1;
int animat = 0;
const int L=20;
const int dgre=3;
int ncpt=L+1;
int clikd=0;
const int nt = 40;				//number of slices along x-direction
const int ntheta = 20;

GLfloat ctrlpoints[L+1][3] =
{

    {0,0,0},{ -0.025, 0.05,0},{0.1, 0.675,0},
    {0.5 ,1.6,0},{1.075, 2.6,0},{1.725, 3.525,0},
    { 2.475, 3.95,0},{2.925 ,4.1,0},
       {3.175, 3.35,0}, {3.15, 2.675,0},{ 3.125, 1.875,0},
       {3.075, 0.975,0},{3 ,0.225,0}
};

long long nCr(int n, int r)
{
    if(r > n / 2) r = n - r; // because C(n, r) == C(n, n - r)
    long long ans = 1;
    int i;

    for(i = 1; i <= r; i++)
    {
        ans *= n - r + i;
        ans /= i;
    }

    return ans;
}
void setNormal(GLfloat x1, GLfloat y1,GLfloat z1, GLfloat x2, GLfloat y2,GLfloat z2, GLfloat x3, GLfloat y3,GLfloat z3)
{
    GLfloat Ux, Uy, Uz, Vx, Vy, Vz, Nx, Ny, Nz;

    Ux = x2-x1;
    Uy = y2-y1;
    Uz = z2-z1;

    Vx = x3-x1;
    Vy = y3-y1;
    Vz = z3-z1;

    Nx = Uy*Vz - Uz*Vy;
    Ny = Uz*Vx - Ux*Vz;
    Nz = Ux*Vy - Uy*Vx;

    glNormal3f(-Nx,-Ny,-Nz);
}


void straw()
{
    int i, j;
    float x, y, z, r;				//current coordinates
    float x1, y1, z1, r1;			//next coordinates
    float theta;
    cube(1,1,0,1,0,0);

    const float startx = 0, endx = ctrlpoints[L][0];
    //number of angular slices
    const float dx = (endx - startx) / nt;	//x step size
    const float dtheta = 2*PI / ntheta;		//angular step size

    float t=0;
    float dt=1.0/nt;
    float xy[2];
    x = xy[0];
    r = xy[1];
    //rotate about z-axis
    float p1x,p1y,p1z,p2x,p2y,p2z;
    for ( i =0 ; i <40; ++i )  			//step through x
    {
        theta = 0;
        t+=dt;
        x1 = xy[0];
        r1 = xy[1];

        //draw the surface composed of quadrilaterals by sweeping theta
        glBegin( GL_QUAD_STRIP );
        for ( j = 0; j <= 20; ++j )
        {
            theta += dtheta;
            double cosa = cos( theta );
            double sina = sin ( theta );
            y = r * cosa;
            y1 = r1 * cosa;	//current and next y
            z = r * sina;
            z1 = r1 * sina;	//current and next z

            //edge from point at x to point at next x
            glVertex3f (x, y, z);

            if(j>0)
            {
                setNormal(p1x,p1y,p1z,p2x,p2y,p2z,x, y, z);
            }
            else
            {
                p1x=x;
                p1y=y;
                p1z=z;
                p2x=x1;
                p2y=y1;
                p2z=z1;

            }
            glVertex3f (x1, y1, z1);

            //forms quad with next pair of points with incremented theta value
        }
        glEnd();
        x = x1;
        r = r1;
    } //for i

}
void showControlPoints()
{
    glPointSize(5.0);
    cube(1.0, 0.0, 1.0,1,0,0);
    glBegin(GL_POINTS);
    for (int i = 0; i <=L; i++)
        glVertex3fv(&ctrlpoints[i][0]);
    glEnd();
}
float shift = 0;


static void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-3, 3, -3, 3, 2.0, 500.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(eyeX,eyeY,eyeZ, lookX,lookY,lookZ, 0,1,0);
    //glViewport(0, 0, width, height);

    glRotatef(rot, 0,1,0);

    glTranslatef(0,0,2);
    sky();
    Sun();
    light0();
    moon();
    field();
    glTranslatef(0,-3,-2);
    river();
    glTranslatef(8,3,0);
    bus_stop();
    glPushMatrix();
    glScalef(3,9,4);
    glTranslatef(18.5,1,-0.5);

    glScalef(0.9,0.9,0.9);
    glTranslatef(-9,1,0);

    glScalef(1.5,1.5,1.5);
    glTranslatef(10,0,0);

    glScalef(1,1,1);
    glTranslatef(4,0,0);
    glScalef(0.2,0.2,0.2);
    glTranslatef(-190,0,-2);
    powerplant();
    glScalef(4,4,4);
    glTranslatef(8,0,-2.3);
    glScalef(0.2,0.2,0.2);
    glTranslatef(100,-1.7,-35);
    library();

    glScalef(19,13,13);
    glTranslatef(2.5,-1,-1.5);
   house1(0,0,0);
   glPopMatrix();

   glPushMatrix();
   glScalef(0.4,1,87);
   glTranslatef(107,-2.5,2.53);
   road1();
   glPopMatrix();

   glPushMatrix();
    glScalef(15,25,15);
    glTranslatef(4.8,-0.5,-4);
    house1(0.1,0.03,0.03);
    glRotatef(-90,0,1,0);
     glScalef(0.06,0.2,0.06);
    glTranslatef(-1,2,2);
    straw();
    glPopMatrix();

    glPushMatrix();
    glScalef(3,7,5);
     glTranslatef(25,2,-10);
    glScalef(1,1,1);
     glTranslatef(-14,0,0);

    glScalef(1,1,1);
    glTranslatef(-1,0,-5);

    glScalef(5,2.5,2.5);
    glTranslatef(2,-1.2,-1);
    house1(0.2,0.3,0.1);
    glScalef(0.2,0.4,0.2);
    glTranslatef(-0.5,2,3);

   glScalef(0.5,0.5,0.5);
   glTranslatef(6,0,3);



    glScalef(8,8,8);
    glTranslatef(-9,-0.5,4);
    school();
    glScalef(0.5,0.7,0.5);
    glTranslatef(-4,0.6,0);

    glScalef(0.5,0.7,0.5);
    glTranslatef(-1,0.6,-8);
    glScalef(8,2,2);
    glTranslatef(1.5,-0.7,-4);
    house1(0.1,0.03,0.03);
    glScalef(0.1,0.4,0.1);
    glTranslatef(0,1,0);
    glScalef(6,4,4);
    glTranslatef(-1,-0.5,0);
    house1(0.1,0.04,0.04);
    glScalef(0.5,0.2,1.2);
    glTranslatef(4,2.1,-9);
    car();
    glScalef(0.2,1,1);
    glTranslatef(5,-1,-7);
    tree();
    glScalef(0.2,1,1);
    glTranslatef(140,0,2);
    tree();
    glScalef(1,1,1);
    glTranslatef(60,0,-2);
    tree();
    glPopMatrix();

    glPushMatrix();
    glScalef(25,20,15);
    glTranslatef(-2,-0.4,-8);
    house1(0,0,0);
    glScalef(0.2,0.5,0.2);
    glTranslatef(0,2.2,0);


    glPopMatrix();
    glPushMatrix();
    glScalef(6,11,6);
    glTranslatef(0,-0.2,-2);
    Windmill();
    glPopMatrix();
    glPushMatrix();
    glScalef(10,17,10);
    glTranslatef(-1,-0.4,0);
    Windmill();
    glPopMatrix();

    glPushMatrix();
    glScalef(5,15,5);
    glTranslatef(-2.5,-1,-2);
    Windmill();
    glPopMatrix();
    glPushMatrix();
    glScalef(10,10,13);
    glTranslatef(-1.5,3,-4);
    road();
    roadlight();
    light2();
    glScalef(0.1,0.1,0.1);
    glTranslatef(15,-30,-60);
    glScalef(3,6,3);
    glTranslatef(20,2,0);
    glPopMatrix();
     glScalef(8,8,8);
    glTranslatef(-0.5,0,-2.8);
    bus();

    glFlush();
    glutSwapBuffers();

}



static void key(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 27 :

        case '0':
            ti=1-ti;
            mark=ti;

            break;
        case '3':

            light_on2=1-light_on2;
            break;
        case 'q':
            exit(0);
            break;

        case ',':

        rot++;
        break;
     //night
        case 'n':
            l0=0.1;
            cl=
            a=a-60;
            b=b+60;

            //PlaySound("cricket-2.wav", NULL, SND_ASYNC|SND_FILENAME);
            break;



       //day
        case 'd':
            //light_on=1-light_on;
            l0=1;
            cl=1;
            b=b-60;
            a=a+60;
            //PlaySound(TEXT("bird-2.wav"), NULL, SND_ASYNC);
            break;
    case '.':
        rot--;
        break;

        case '+':
            eyeZ++;
            break;

        case '-':
            eyeZ--;
            break;
         case 'l':
        eyeX++;
        break;
    case 'r':
        eyeX--;
        break;

    case 'u':
        eyeY++;
        break;
    case 'b':
        eyeY--;
        break;

    case 'w':
        lookY++;
        break;
    case 'e':
        lookY--;
        break;

    case 'o':
        lookX++;
        break;
    case 'p':
        lookX--;
        break;

    case 'k':
        lookZ++;
        break;
    case 'm':
        lookZ--;
        break;

     case 'z':
        amb_light_on = !amb_light_on;
        break;
    // Specular light on-off
    case 'x':
        diff_light_on = !diff_light_on;
        break;
    // Diffuse light on-off
    case 'c':
        spec_light_on = !spec_light_on;
        break;
    case 's':
        stop=0;
        break;

    case 'f':
        angle=0,stop=1,k=0;
        break;


}
    glutPostRedisplay();
}


static void idle(void)
{
    glutPostRedisplay();
    spin();
}

void texture_image()
{
    LoadTexture("C:\\Users\\user\\Desktop\\assignment\\1707048_smart_village\\grass.bmp");//0
    v.push_back(ID);
    LoadTexture("C:\\Users\\user\\Desktop\\assignment\\1707048_smart_village\\water1.bmp");//1
    v.push_back(ID);
    LoadTexture("C:\\Users\\user\\Desktop\\assignment\\1707048_smart_village\\road.bmp");//2
    v.push_back(ID);
    LoadTexture("C:\\Users\\user\\Desktop\\assignment\\1707048_smart_village\\bricks.bmp");//3
    v.push_back(ID);
    LoadTexture("C:\\Users\\user\\Desktop\\assignment\\1707048_smart_village\\sky.bmp");//4
    v.push_back(ID);
    LoadTexture("C:\\Users\\user\\Desktop\\assignment\\1707048_smart_village\\bus_buil2.bmp");//5
    v.push_back(ID);
    LoadTexture("C:\\Users\\user\\Desktop\\assignment\\1707048_smart_village\\bus_stop.bmp");//6
    v.push_back(ID);
    LoadTexture("C:\\Users\\user\\Desktop\\assignment\\1707048_smart_village\\school.bmp");//7
    v.push_back(ID);
    LoadTexture("C:\\Users\\user\\Desktop\\assignment\\1707048_smart_village\\window.bmp");//8
    v.push_back(ID);
     LoadTexture("C:\\Users\\user\\Desktop\\assignment\\1707048_smart_village\\door.bmp");//9
    v.push_back(ID);
    //LoadTexture("C:\\Users\\user\\Desktop\\assignment\\1707048_smart_village\\road.bmp");//10
    v.push_back(ID);
    LoadTexture("C:\\Users\\user\\Desktop\\assignment\\1707048_smart_village\\bridge2.bmp");//11
    v.push_back(ID);
     LoadTexture("C:\\Users\\user\\Desktop\\assignment\\1707048_smart_village\\power.bmp");//12
    v.push_back(ID);
    LoadTexture("C:\\Users\\user\\Desktop\\assignment\\1707048_smart_village\\lib3.bmp");//13
    v.push_back(ID);
    LoadTexture("C:\\Users\\user\\Desktop\\assignment\\1707048_smart_village\\l_door.bmp");//14
    v.push_back(ID);
    LoadTexture("C:\\Users\\user\\Desktop\\assignment\\1707048_smart_village\\heading.bmp");//15
    v.push_back(ID);
    LoadTexture("C:\\Users\\user\\Desktop\\assignment\\1707048_smart_village\\hos_buil.bmp");//16
    v.push_back(ID);
    LoadTexture("C:\\Users\\user\\Desktop\\assignment\\1707048_smart_village\\hdoor.bmp");//17
    v.push_back(ID);
    LoadTexture("C:\\Users\\user\\Desktop\\assignment\\1707048_smart_village\\emer.bmp");//18
    v.push_back(ID);

}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(width,height);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("Smart Village");
    init();

    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutIdleFunc(idle);

    glutReshapeFunc(resize);
    //glClearColor(1,1,1,1);

    glEnable(GL_DEPTH_TEST);
    glShadeModel( GL_SMOOTH );
    glEnable(GL_NORMALIZE);
    glEnable(GL_BLEND);
    glEnable(GL_LIGHTING);
    GLfloat globalAmbient[] = {0.0,0.0,0.0,0.0};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT,globalAmbient);
    texture_image();


    printf("####################################################################################\n");
    printf("##########                                                         #################\n");
    printf("##########           PLEASE FOLLOW THE BELOW INSTRUCTIONS          #################\n");
    printf("##########                                                         #################\n");
    printf("########################################################################################\n\n\n");

    printf("Use '+' to zoom in and '-' to zoom out.\n\n\n");
    printf("3. Press 'u' for up.\n4. press 'b' for down.\n5. press 'r' for right.\n6. press 'l' for left.\n");
    printf("7. Press 'o' to move camera left.\n8. Press 'p' to move camera right.\n");
    printf("9. Press 'n' for night mode .\n10. Press 'd' for day mode.\n11.Press '0' for no light\n10.press '12' for spot light\n");
    printf("13. Press 'f' to run windmill.\n14. Press 's' to stop windmill.\n\n\n");


    glutMainLoop();

    return EXIT_SUCCESS;

}
