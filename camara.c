#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <iostream>
#include <stdio.h>
#include <math.h> 
#include <string.h>
#include <deque>
#include <string>

using namespace std;

#define PI 3.14159265
#define GRAVITY 9.8
#define LINEAL_SPEED 1
#define ROTATIONAL_SPEED 5

#define FRONT_MOVE 'w'
#define RIGTH_MOVE 'd'
#define BACK_MOVE 's'
#define LEFT_MOVE 'a'

#define ROTATE_RIGTH 'e'
#define ROTATE_LEFT 'q'

class Point3d {
	private:
		GLfloat x, y, z;
	public:
		Point3d (GLfloat a = 0.0, GLfloat b = 0.0, GLfloat c = 0.0) {
			x = a;
			y = b;
			z = c;
		}

		GLfloat get_x(void) 		{return x;}
		GLfloat get_y(void) 		{return y;}
		GLfloat get_z(void) 		{return z;}
		void 	set_x(GLfloat a) 	{x = a;}
		void 	set_y(GLfloat b) 	{y = b;}
		void 	set_z(GLfloat c) 	{z = c;}

		void print() {
			cout << "Im a point\n";
			cout << "--------------------------------\n";
			printf("x: %f\n", x);
			printf("y: %f\n", y);
			printf("z: %f\n", z);
			cout << "--------------------------------\n\n";
		}
};

class Entity {
	protected:
		Point3d center;
		GLboolean isSolid;
		int id;
	public:
		Entity (int idEnt, Point3d p, GLboolean s = true) {
			id = idEnt;
			center = p;
			isSolid = s;
		}

		Point3d 	get_point(void) 			{return center;}
		GLboolean 	get_isSolid(void)			{return isSolid;} 
		void 		set_point(Point3d p) 		{center = p;}
		void 		set_isSolid(GLboolean s)	{isSolid = s;}


		virtual void print() {
			cout << "Im an entity\n";
			cout << "--------------------------------\n";
			printf("Id: %d\n", id);
			printf("Center: (%f, %f, %f)\n", center.get_x(), center.get_y(), center.get_z());
			cout << "--------------------------------\n\n";
		}

		virtual void make(){};
		virtual void destroy(){};
};

class Box: public Entity {
	private:
		GLfloat heigth, width, deep;
	public:
		Box(int id, Point3d p, GLfloat w, GLfloat h, GLfloat d)
		: Entity(id, p){
			heigth = h;
			width = w;
			deep = d;
		}

		GLfloat get_heigth(void) 		{return heigth;}
		GLfloat get_width(void) 		{return width;}
		GLfloat get_deep(void) 			{return deep;}
		void 	set_heigth(GLfloat h) 	{heigth = h;}
		void 	set_width(GLfloat w) 	{width = w;}
		void 	set_deep(GLfloat d) 	{deep = d;}

		void make() {
			glPushMatrix( );
				glTranslatef(center.get_x(), center.get_y(), center.get_z());
				glScalef(width, heigth, deep);
				glutSolidCube(1.0);
			glPopMatrix( );
		}

		void print() {
			cout << "Im a box\n";
			cout << "--------------------------------\n";
			printf("Id: %d\n", id);
			printf("Center: (%f, %f, %f)\n", center.get_x(), center.get_y(), center.get_z());
			printf("Width: %f\n", width);
			printf("Heigth: %f\n", heigth);
			printf("Deep: %f\n", deep);
			cout << "--------------------------------\n\n";

		}

		void destroy() {}
};


class EntitiesFactory {
	private:
		static int idEntity;
	public:
		static Entity entity(Point3d center) {
			idEntity += 1;
			return Entity(idEntity, center);
		}
		static Box box(Point3d center, GLfloat heigth, GLfloat width, GLfloat deep) {
			idEntity += 1;
			return Box(idEntity, center, heigth, width, deep);
		}
};
int EntitiesFactory::idEntity = 0;

//Singleton
class World {
	private:
		// deque<Entity*> entities;
		Entity *entities[10];
		int i;
		static World* world;
		World(){}
	public:

		static World* getInstance() {
			if (world == NULL) {
				world = new World();

			}
			return world;
		}

		// deque<Box> 	get_entities(void) 				{return entities;}

		void addEntity(Entity* entity) {
			entities[i] = entity;
			i += 1;
			// entities.push_back(entity);
		}

		void print(){
			printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
			for (int x = 0; x < i; x++){
				Entity *e = entities[x];
				e->print();
			}
		}

		void destroy() {
			
		}

		void makeWorld() {

			for (int x = 0; x < i; x ++) {
				Entity *entity =  entities[x];
				entity->print();
				entity->make();

			}
		}
};
World* World::world = NULL;


World* world = World::getInstance();
GLenum doubleBuffer;
GLfloat worldVelocity = 0;

Entity *entity;

GLfloat zCam = 10.0, 
		xCam = 0.0,
		yCam = 0.0,

		zEye = 0.0,
		xEye = 0.0,
		yEye = 0.0,

		yVelocity = 0.0,
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

void updateVelocityGravity() {
	yCam += yVelocity;
	yVelocity += GRAVITY;
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
		   
   	gluLookAt (xCam, yCam, zCam,  // cam position
   			  xEye , yEye, zEye, // target's cam 
			  0.0, 1.0, 0.0);   // up

	world->makeWorld();
	
   	( doubleBuffer ? glutSwapBuffers() : glFlush() );

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
		case ROTATE_RIGTH:
			xzAngle = caculateDegrees( xzAngle + ROTATIONAL_SPEED);
			break;
		case ROTATE_LEFT:
			xzAngle = caculateDegrees( xzAngle - ROTATIONAL_SPEED);
			break;

		// lineal moves
		case FRONT_MOVE:
			zCam += Z_FBmove*LINEAL_SPEED;
			xCam += X_FBmove*LINEAL_SPEED;
			break;
		case BACK_MOVE:
			zCam -= Z_FBmove*LINEAL_SPEED;
			xCam -= X_FBmove*LINEAL_SPEED;
			break;
		case RIGTH_MOVE:
			zCam += Z_Lmove*LINEAL_SPEED;
			xCam += X_Lmove*LINEAL_SPEED;
			break;
		case LEFT_MOVE:
			zCam -= Z_Lmove*LINEAL_SPEED;
			xCam -= X_Lmove*LINEAL_SPEED;
			break;

		// otherwise
		case 27:
			exit(0);
    }
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

void createInitWorld() {
	Box box2  = EntitiesFactory::box(Point3d(-10.0, 0.0, 0.0), 7.0, 17.0, 7.0);
	Box box3 = EntitiesFactory::box(Point3d(10.0, 0.0, 0.0), 7.0, 7.0, 7.0);
	Entity entity = EntitiesFactory::entity(Point3d(0.0, 0.0, 0.0));

   	world->addEntity(&box2);
   	world->addEntity(&box3);
	world->addEntity(&entity);

	init ();
	glutKeyboardFunc(Key);
	glutDisplayFunc(display); 
	glutReshapeFunc(reshape);
	glutIdleFunc(display);  
	glutMainLoop();
}

int main(int argc, char** argv) {

	Box box = EntitiesFactory::box(Point3d(10.0, 0.0, 0.0), 7.0, 15.0, 7.0);
	entity = &box;

	GLenum type;

	glutInit(&argc, argv);
	Args(argc, argv);

	type = GLUT_RGB;
	type |= (doubleBuffer) ? GLUT_DOUBLE : GLUT_SINGLE;

	glutInitDisplayMode (type);
	glutInitWindowSize (500, 500); 
	glutInitWindowPosition (100, 100);
	glutCreateWindow (argv[0]);
	createInitWorld();

	return 0;
}