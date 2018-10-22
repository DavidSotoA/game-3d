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

class relativeSpace {
	public:
		// static Vector3d less(Point3d a, Point3d b) {
		// 	int x = abs(a.get_x) 
		// }
};

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

class Vector3d {
	private:
		GLfloat x;
		GLfloat y;
		GLfloat z;

		GLfloat xyAngle;
		GLfloat xzAngle;
		GLfloat yzAngle;
	public:

		Vector3d(GLfloat x = 0.0, GLfloat y = 0.0, GLfloat z = 0.0, GLfloat xyAngle = 0.0, GLfloat xzAngle = 0.0, GLfloat yzAngle = 0.0){
			this->x = x;
			this->y = y;
			this->z = z;
			this->xyAngle = caculateDegrees(xyAngle);
			this->xzAngle = caculateDegrees(xzAngle);
			this->yzAngle = caculateDegrees(yzAngle);
		}

		GLfloat get_x  		(void) 	{return x;}
		GLfloat get_y		(void) 	{return y;}
		GLfloat get_z		(void) 	{return z;}
		GLfloat get_xyAngle	(void) 	{return xyAngle;}
		GLfloat get_xzAngle	(void) 	{return xzAngle;}
		GLfloat get_yzAngle	(void) 	{return yzAngle;}

		void set_x		 (GLfloat x) 		{this->x = x;}
		void set_y		 (GLfloat y) 		{this->y = y;}
		void set_z		 (GLfloat z) 		{this->z = z;}
		void set_xyAngle (GLfloat xyAngle) 	{this->xyAngle = xyAngle;}
		void set_xzAngle (GLfloat xzAngle) 	{this->xzAngle = xzAngle;}
		void set_yzAngle (GLfloat yzAngle) 	{this->yzAngle = yzAngle;}


		GLfloat caculateDegrees(GLfloat dumbDegrees) {	
			GLfloat degrees = (dumbDegrees < 0 ? 360: 0) + dumbDegrees;
			degrees =  (int)degrees % 360;
			return (GLfloat)degrees;
		}

		GLfloat toRadians(GLfloat degrees) {
			return ( degrees * (PI / 180.0) );
		}

		void add_xzAngle(int a) {
			xzAngle = caculateDegrees(xzAngle + a);
			GLfloat hip = sqrt(pow(x,2) + pow(z,2));
			x = cos(toRadians(xzAngle));
			z = sin(toRadians(xzAngle));
		}
		void add_xyAngle(int a) {
			xyAngle = caculateDegrees(xyAngle + a);
			GLfloat hip = sqrt(pow(x,2) + pow(y,2));
			x = cos(toRadians(xyAngle));
			y = sin(toRadians(xyAngle));
		}

		// void add_yzAngle(int a) {
		// 	yzAngle = caculateDegrees(yzAngle + a);
		// 	y = yComponent();
		// 	z = zComponent();
		// }

		void print() {
			cout << "Im an vector\n";
			cout << "--------------------------------\n";
			printf("components: (%f, %f, %f)\n", x, y, z);

			printf("xyAngle: %f\n", xyAngle);
			printf("xzAngle: %f\n", xzAngle);
			printf("yzAngle: %f\n", yzAngle);

			cout << "--------------------------------\n\n";
		}
};

class Entity {
	protected:
		Point3d center;
		GLboolean isSolid;
		GLboolean isLive;
		Vector3d target;
		int id;
	public:
		Entity (Point3d p, GLboolean s = false, GLboolean live = false, int idEnt = 0) {
			id = idEnt;
			center = p;
			isSolid = s;
			isLive = live;
		}

		Point3d 	get_point(void) 			{return center;}
		GLboolean 	get_isSolid(void)			{return isSolid;} 
		void 		set_point(Point3d p) 		{center = p;}
		void 		set_isSolid(GLboolean s)	{isSolid = s;}

		void addTarget(Point3d target) {
			if (isLive) {
				// this->target = target;
			}
		}

		virtual Entity& live() {
			isLive = true;
			return *this;
		}

		virtual Entity& solid() {
			isSolid = true;
			return *this;
		}

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
		Box(Point3d p, GLfloat w, GLfloat h, GLfloat d, GLboolean s = false, GLboolean live = false,int id = 0)
		: Entity(p, s, live, id){
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

		Box& live() {
			isLive = true;
			return *this;
		}

		Box& solid() {
			isSolid = true;
			return *this;
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
	public:
		static Entity entity(Point3d center) {
			return Entity(center);
		}

		static Box box(Point3d center, GLfloat heigth, GLfloat width, GLfloat deep) {
			return Box(center, heigth, width, deep);
		}
};

//Singleton
class World {
	private:
		deque<Entity*> entities;
		static World* world;
		World(){}
	public:

		static World* getInstance() {
			if (world == NULL) {
				world = new World();

			}
			return world;
		}

		deque<Entity*> 	get_entities(void) 					{return entities;}
		void 			set_entities(deque<Entity*> e) 		{entities = e;}

		void addEntity(Entity* entity) {
			entities.push_back(entity);
		}

		void print(){
			for (int i = 0; i<entities.size(); i++) {
				Entity* entity = entities[i];
				entity->print();
			}
		}

		void destroy() {}

		void makeWorld() {
			for (int i = 0; i<entities.size(); i++) {
				Entity* entity = entities[i];
				entity->make();
			}
		}
};
World* World::world = NULL;


World* world = World::getInstance();
GLenum doubleBuffer;
GLfloat worldVelocity = 0;

Vector3d vision(0.0, 0.0, -1.0, 0.0, 270.0);

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
	xEye = xCam + vision.get_x();
	zEye = zCam + vision.get_z();
}

void init(void) {
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glShadeModel (GL_FLAT);
}

void display(void) {
   	caculateCamRotation();
  
   	glClear(GL_COLOR_BUFFER_BIT);
   	glColor3f (0.7, 0.0, 0.0);
   	glLoadIdentity ();     
		   
   	gluLookAt (xCam, yCam, zCam,  								// cam position
   			  xEye , yEye, zEye, 	// target's cam 
			  0.0, 1.0, 0.0);   								// up

	world->makeWorld();
	
   	( doubleBuffer ? glutSwapBuffers() : glFlush() );

}

static void Key(unsigned char key, int x, int y) {

	//Vector unitario para movimientos laterales
	GLfloat toRad90 = toRadians(90.0);
	GLfloat X_Lmove = vision.get_x()*cos(toRad90) - vision.get_z()*sin(toRad90);
	GLfloat Z_Lmove = vision.get_x()*sin(toRad90) + vision.get_z()*cos(toRad90);

	switch (key) {
		// lineal moves
		case FRONT_MOVE:
			zCam += vision.get_z()*LINEAL_SPEED;
			xCam += vision.get_x()*LINEAL_SPEED;
			break;
		case BACK_MOVE:
			zCam -= vision.get_z()*LINEAL_SPEED;
			xCam -= vision.get_x()*LINEAL_SPEED;
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

void SpecialInput(int key, int x, int y){
	//Vector unitario para movimientos rotacionales
	switch(key) {
		case GLUT_KEY_LEFT:
			vision.add_xzAngle( -ROTATIONAL_SPEED );
		break;
		case GLUT_KEY_RIGHT:
			vision.add_xzAngle(  ROTATIONAL_SPEED );
		break;
	}

	glutPostRedisplay();
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

	Box box1  		= EntitiesFactory::box(Point3d(0.0, 0.0, -14.0), 4.0, 4.0, 4.0).solid();
	Box box2  		= EntitiesFactory::box(Point3d(-10.0, 0.0, 0.0), 7.0, 17.0, 7.0).solid();
	Box box3 		= EntitiesFactory::box(Point3d(10.0, 0.0, 0.0), 7.0, 7.0, 7.0).solid();

	Entity player 	= EntitiesFactory::entity(Point3d(0.0, 0.0, 10.0)).solid().live();
	player.addTarget(Point3d(0.0, 0.0, 0.0));

   	world->addEntity(&box1);
   	world->addEntity(&box2);
   	world->addEntity(&box3);
	world->addEntity(&player);

	init ();
	glutKeyboardFunc(Key);
	glutSpecialFunc(SpecialInput);
	glutDisplayFunc(display); 
	glutReshapeFunc(reshape);
	glutIdleFunc(display);  
	glutMainLoop();

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
	createInitWorld();

	return 0;
}