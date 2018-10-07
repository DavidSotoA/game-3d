#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdio.h>
#include <math.h> 
#include <string.h>

#define PI 3.14159265
#define LINEAL_SPEED 1
#define ROTATIONAL_SPEED 5
#define FRONT_MOVE 'u'
#define RIGTH_MOVE 'r'
#define BACK_MOVE 'b'
#define LEFT_MOVE 'l'

GLenum doubleBuffer;

GLfloat zCam = 10.0, 
		xCam = 0.0,
		zEye = 0.0,
		xEye = 0.0,
		xzAngle = 270.0;

GLfloat caculateDegrees(GLfloat dumbDegrees) {	
	GLfloat degrees = (dumbDegrees < 0 ? 360: 0) + dumbDegrees;
	degrees =  (int)degrees % 360;
	return (GLfloat)degrees;
}

GLfloat toRadians(GLfloat degrees) {
	return ( degrees * (PI / 180.0) );
}

void caculateCamRotation(void) {
	GLfloat hip = sqrt( pow(zCam, 2) + pow(xCam, 2) );
	GLfloat rads = toRadians(xzAngle);	
	xEye = xCam + hip*cos(rads);
	zEye = zCam + hip*sin(rads);
}

void init(void) {
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glShadeModel (GL_FLAT);
}

void display(void) {
   caculateCamRotation();
  
   glClear(GL_COLOR_BUFFER_BIT);
   glColor3f (1.0, 0.0, 0.0);
   glLoadIdentity ();     
		   
   gluLookAt (xCam, 0.0, zCam,  // cam position
   			  xEye , 0.0, zEye, // target's cam 
			  0.0, 1.0, 0.0);   // up

   glScalef(7.0,7.0,7.0);
   glutSolidCube (1.0);
   
   if (doubleBuffer) {
    glutSwapBuffers();
  } else {
    glFlush();
  }
  
}

static void Key(unsigned char key, int x, int y) {

	GLfloat rads = toRadians(xzAngle);	

	//Vector unitario para movimiento hacia adelante y hacia atras.
	GLfloat X_FBmove = cos(rads);
	GLfloat Z_FBmove = sin(rads);

	//Vector unitario para movimientos laterales
	GLfloat toRad90 = toRadians(90.0);
	GLfloat X_Lmove = X_FBmove*cos(toRad90) - Z_FBmove*sin(toRad90);
	GLfloat Z_Lmove = X_FBmove*sin(toRad90) + Z_FBmove*cos(toRad90);

	switch (key) {

		//rotational moves
		case 'e':
			xzAngle = caculateDegrees( xzAngle + ROTATIONAL_SPEED);
			break;
		case 'q':
			xzAngle = caculateDegrees( xzAngle - ROTATIONAL_SPEED);
			break;

		// lineal moves
		case 'w':
			zCam += Z_FBmove*LINEAL_SPEED;
			xCam += X_FBmove*LINEAL_SPEED;
			break;
		case 's':
			zCam -= Z_FBmove*LINEAL_SPEED;
			xCam -= X_FBmove*LINEAL_SPEED;
			break;
		case 'd':
			zCam += Z_Lmove*LINEAL_SPEED;
			xCam += X_Lmove*LINEAL_SPEED;
			break;
		case 'a':
			zCam -= Z_Lmove*LINEAL_SPEED;
			xCam -= X_Lmove*LINEAL_SPEED;
			break;

		// otherwise
		case 27:
			exit(0);
    }

	display();
}

void reshape (int w, int h) {
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   glFrustum (-1.0, 1.0, -1.0, 1.0, 1.5, 100.0);
   glMatrixMode (GL_MODELVIEW);
}

static void Args(int argc, char **argv) {
  GLint i;

  doubleBuffer = GL_TRUE;

  for (i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-sb") == 0) {
      doubleBuffer = GL_FALSE;
    } else if (strcmp(argv[i], "-db") == 0) {
      doubleBuffer = GL_TRUE;
    }
  }
}


int main(int argc, char** argv) {
   GLenum type;

   glutInit(&argc, argv);
   Args(argc, argv);

   type = GLUT_RGB;
   type |= (doubleBuffer) ? GLUT_DOUBLE : GLUT_SINGLE;

   glutInitDisplayMode (type);
   glutInitWindowSize (500, 500); 
   glutInitWindowPosition (100, 100);
   glutCreateWindow (argv[0]);
   init ();
   glutKeyboardFunc(Key);
   glutDisplayFunc(display); 
   glutReshapeFunc(reshape);
   glutMainLoop();
   return 0;
}