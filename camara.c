#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <iostream>
#include <stdio.h>
#include <math.h> 
#include <cmath> 
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

class Angle {
	private:
		GLfloat angle;
	public:
		Angle(GLfloat angle = 0.0) {
			this->angle = caculateDegrees(angle);
		}

		GLfloat get_angle (void) 	{return this->angle;}
		void 	set_angle (GLfloat) 	{this->angle = angle;}

		GLfloat caculateDegrees(GLfloat dumbDegrees) {	
			GLfloat degrees = (dumbDegrees < 0 ? 360: 0) + dumbDegrees;
			degrees =  (int)(round(degrees)) % 360;
			return (GLfloat)degrees;
		}

		GLfloat toRadians() {
			return ( this->angle * (PI / 180.0) );
		}


		static GLfloat toRadians(GLfloat angle) {
			return ( angle * (PI / 180.0) );
		}

		static GLfloat toGrades(GLfloat grades) {
			return round( grades * (180.0 / PI) ); 
		}

		Angle add(Angle a) {
			return Angle(this->angle + a.get_angle());
		}

		Angle less(Angle a) {
			return Angle(this->angle - a.get_angle());
		}
};

class Vector3d {
	private:
		GLfloat x;
		GLfloat y;
		GLfloat z;

		Angle xyAngle;
		Angle xzAngle;
		Angle yzAngle;
	public:

		Vector3d(GLfloat x = 0.0, GLfloat y = 0.0, GLfloat z = 0.0) {
			this->x = x;
			this->y = y;
			this->z = z;
			calculateAngles();
		}

		Vector3d(Angle xyAngle, Angle xzAngle, Angle yzAngle) {
			this->xyAngle = xyAngle;
			this->xzAngle = xzAngle;
			this->yzAngle = yzAngle;
			calculateComponents();
		}

		GLfloat get_x  		(void) 	{return x;}
		GLfloat get_y		(void) 	{return y;}
		GLfloat get_z		(void) 	{return z;}
		Angle get_xyAngle	(void) 	{return xyAngle;}
		Angle get_xzAngle	(void) 	{return xzAngle;}
		Angle get_yzAngle	(void) 	{return yzAngle;}

		void set_x		 (GLfloat x) 		{this->x = x;calculateAngles();}
		void set_y		 (GLfloat y) 		{this->y = y;calculateAngles();}
		void set_z		 (GLfloat z) 		{this->z = z;calculateAngles();}
		void set_xyAngle (Angle xyAngle) 	{this->xyAngle = xyAngle;}
		void set_xzAngle (Angle xzAngle) 	{this->xzAngle = xzAngle;}
		void set_yzAngle (Angle yzAngle) 	{this->yzAngle = yzAngle;}

		void calculateComponents() {
			GLfloat xz_angle = xzAngle.toRadians();
			GLfloat xy_angle = xyAngle.toRadians();
			GLfloat yz_angle = yzAngle.toRadians();

			z = cos(xz_angle);
			x = cos(xy_angle);
			y = cos(yz_angle);
		}

		void calculateAngles() {

			Angle xz = Angle(Angle::toGrades( atan( Angle::toRadians(x)/Angle::toRadians(z) )));
			Angle yz = Angle(Angle::toGrades( atan( Angle::toRadians(z)/Angle::toRadians(y) )));
			Angle xy = Angle(Angle::toGrades( atan( Angle::toRadians(y)/Angle::toRadians(x) )));
			
			this->xzAngle 	= Angle( resolveAngle( xz.get_angle(), z, x )) ;
			this->yzAngle 	= Angle( resolveAngle( yz.get_angle(), y, z )) ;
			this->xyAngle 	= Angle( resolveAngle( xy.get_angle(), x, y )) ;
		}


		Vector3d unitVector(){
			GLfloat magnitud = this->magnitud();
			return Vector3d(x/magnitud, y/magnitud, z/magnitud);
		}

		GLfloat magnitud() {
			return sqrt(pow(x,2) + pow(y,2) + pow(z,2));
		}

		GLfloat resolveAngle(GLfloat angle, GLfloat x, GLfloat y) {

			if (x == 0.0 && y == 0.0){
				return 0.0;
			}
			// primer cuadrante
			if ( x > 0.0 && y >= 0.0 ){
				if (angle >= 180.0 && angle <270.0) {
					angle += 180.0;
				} else {
					angle = angle;
				}
			// segundo cuadrante
			} else if( x <= 0.0 && y > 0.0){
				if (angle >= 270.0 && angle <360.0) {
					angle += 180.0;
				} else {
					angle = angle;
				}
			// tercer cuadrante
			} else if(x < 0.0 && y <= 0.0){
				if (angle >= 0.0 && angle <90.0) {
					angle += 180.0;
				} else {
					angle = angle;
				}
			// cuarto cuadrante
			} else if(x >= 0.0 && y < 0.0) {
				if (angle >= 90.0 && angle <180.0) {
					angle += 180.0;
				} else {
					angle = angle;
				}
			}	

			return angle;
		}

		Vector3d add_xzAngle(Angle a) {
			GLfloat zx = xzAngle.add(a).toRadians();

			GLfloat z = cos(zx);
			GLfloat x = sin(zx);
			GLfloat d = y;
			return Vector3d(x,d,z);
		}

		// void add_xyAngle(Angle a) {
		// 	xyAngle.add(a);
		// 	calculateComponents();
		// }

		Vector3d add_yzAngle(Angle a) {
			GLfloat yz = yzAngle.add(a).toRadians();

			GLfloat y = cos(yz);
			GLfloat z = sin(yz);
			GLfloat d = x;

			// printf("y: %f", y);
			// printf("z: %f\n", z);

			return Vector3d(d,y,z);
		}

		void print() {
			cout << "Im an vector\n";
			cout << "--------------------------------\n";
			printf("components: (%f, %f, %f)\n", x, y, z);

			printf("xyAngle: %f\n", xyAngle.get_angle());
			printf("xzAngle: %f\n", xzAngle.get_angle());
			printf("yzAngle: %f\n", yzAngle.get_angle());

			cout << "--------------------------------\n\n";
		}
};


class RelativeSpace {
	public:
		static Vector3d less(Point3d a, Point3d b) {
			int x = a.get_x() - b.get_x();
			int y = a.get_y() - b.get_y();
			int z = a.get_z() - b.get_z();

			return Vector3d(x, y, z);
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

		Point3d 	get_center	(void) 			{return center;}
		GLboolean 	get_isSolid (void)			{return isSolid;}
		GLboolean	get_isLive	(void)			{return isLive;}
		Vector3d 	get_target	(void)			{return target;}

		void 		set_center	(Point3d center) 	{this->center  = center;}
		void 		set_isSolid	(GLboolean isSolid)	{this->isSolid = isSolid;}
		void 		set_isLive	(GLboolean isLive)	{this->isLive  = isLive;}
		void 		set_target	(Vector3d target)	{this->target  = target;}

		void addTarget(Point3d target) {
			if (isLive) {
				this->target = RelativeSpace::less(target, this->center).unitVector();
			}
		}

		Point3d getPosition() {
			return center;
		}

		Point3d getVision() {
			GLfloat x = target.get_x() + center.get_x();
			GLfloat y = target.get_y() + center.get_y();
			GLfloat z = target.get_z() + center.get_z();
			return Point3d(x,y,z);
		}

		void xzMove(Angle angle) {
			this->target = this->target.add_xzAngle(angle);
		}

		// void xyMove(Angle angle) {
		// 	this->target.add_xyAngle(angle);
		// }

		void yzMove(Angle angle) {
			this->target = this->target.add_yzAngle(Angle(angle.get_angle())) ;
		}

		void frontalMove(GLfloat speed) {
			center.set_x( center.get_x() + target.get_x()*speed );
			center.set_y( center.get_y() + target.get_y()*speed );
			center.set_z( center.get_z() + target.get_z()*speed );
		}

		void backMove(GLfloat speed) {
			center.set_x( center.get_x() - target.get_x()*speed );
			center.set_y( center.get_y() - target.get_y()*speed );
			center.set_z( center.get_z() - target.get_z()*speed );
		}

		void rightMove(GLfloat speed) {
			Vector3d lateralVector = target.add_xzAngle(90);
			center.set_x( center.get_x() - lateralVector.get_x()*speed );
			center.set_y( center.get_y() - lateralVector.get_y()*speed );
			center.set_z( center.get_z() - lateralVector.get_z()*speed );
		}

		void leftMove(GLfloat speed) {
			Vector3d lateralVector = target.add_xzAngle(90);
			center.set_x( center.get_x() + lateralVector.get_x()*speed );
			center.set_y( center.get_y() + lateralVector.get_y()*speed );
			center.set_z( center.get_z() + lateralVector.get_z()*speed );
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
			target.print();
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

Entity player 	= EntitiesFactory::entity(Point3d(0.0, 0.0, 10.0)).solid().live();

GLfloat zCam = 10.0, 
		xCam = 0.0,
		yCam = 0.0,

		zEye = 0.0,
		xEye = 0.0,
		yEye = 0.0,

		yVelocity = 0.0,
		xzAngle = 270.0;

GLfloat toRadians(GLfloat degrees) {
	return ( degrees * (PI / 180.0) );
}

void init(void) {
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glShadeModel (GL_FLAT);
}

void display(void) {
  
   	glClear(GL_COLOR_BUFFER_BIT);
   	glColor3f (0.7, 0.0, 0.0);
   	glLoadIdentity ();   

	// player.print();  	

	Point3d position = player.getPosition();
	Point3d vision	 = player.getVision();
		   
   	gluLookAt (position.get_x(), position.get_y(), position.get_z(),
   			   vision.get_x(), vision.get_y(), vision.get_z(), 								
			   0.0, 1.0, 0.0);   								// up

	world->makeWorld();
	
   	( doubleBuffer ? glutSwapBuffers() : glFlush() );

}

static void Key(unsigned char key, int x, int y) {

	switch (key) {
		// lineal moves
		case FRONT_MOVE:
			player.frontalMove(LINEAL_SPEED);
			break;
		case BACK_MOVE:
			player.backMove(LINEAL_SPEED);
			break;
		case RIGTH_MOVE:
			player.rightMove(LINEAL_SPEED);
			break;
		case LEFT_MOVE:
			player.leftMove(LINEAL_SPEED);
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
			player.xzMove( Angle(ROTATIONAL_SPEED) );
		break;
		case GLUT_KEY_RIGHT:
			player.xzMove( Angle(-ROTATIONAL_SPEED) );
		break;
		case GLUT_KEY_UP:
			player.yzMove( Angle(ROTATIONAL_SPEED) );
		break;
		case GLUT_KEY_DOWN:
			player.yzMove( Angle(-ROTATIONAL_SPEED) );
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
	glutInitWindowPosition (800, 200);
	glutCreateWindow (argv[0]);
	createInitWorld();

	return 0;
}