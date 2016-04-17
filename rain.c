// gcc fireworks_glut.c -lglut -lGL -lGLU -lm -o fireworks_glut
#include <GL/glut.h>  // Header File For The GLUT Library 
#include <GL/gl.h>	  // Header File For The OpenGL32 Library
#include <GL/glu.h>	  // Header File For The GLu32 Library
#include <math.h>
#include <unistd.h>
#include <stdio.h>

#define ESCAPE 27
#define NUM_PARTICLES 1000
#define GRAVITY 0.000

int num_ativas=1;

struct s_pf {
  float x, y, veloc_x, veloc_y;
  unsigned lifetime;
} particles[NUM_PARTICLES];

int window; 

// Initialize the firework
void InitParticle(int pause)
{
  int i;

  if(pause) usleep(20000 + rand() % 2000000);

  for(i=0;i<NUM_PARTICLES;i++) {
    float velocity = (float)(rand() % 100)/5000.0;
    int angle = rand() % 360;
    particles[i].veloc_x = -0.04;//cos( (M_PI * angle/180.0) ) * velocity;
    particles[i].veloc_y = -0.05;//sin( (M_PI * angle/180.0) ) * velocity;
    particles[i].x = (float)(rand()%9);
    particles[i].x += (float)(rand()%10)/10; 
    particles[i].y = 0.00;
    particles[i].lifetime = 1000;//rand() % 100;
  }
}

/* A general OpenGL initialization function.  Sets all of the initial parameters. */
void InitGL(int Width, int Height)
{
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);		// This Will Clear The Background Color To Black
  glClearDepth(1.0);				// Enables Clearing Of The Depth Buffer
  glDepthFunc(GL_LESS);				// The Type Of Depth Test To Do
  glEnable(GL_DEPTH_TEST);			// Enables Depth Testing
  glShadeModel(GL_SMOOTH);			// Enables Smooth Color Shading

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();				// Reset The Projection Matrix

  gluPerspective(45.0f,(GLfloat)Width/(GLfloat)Height,0.1f,100.0f);	// Calculate The Aspect Ratio Of The Window

  glMatrixMode(GL_MODELVIEW);

  InitParticle(0); // first firework
}

/* The function called when our window is resized (which shouldn't happen, because we're fullscreen) */
void ReSizeGLScene(int Width, int Height)
{
  if (Height==0)				// Prevent A Divide By Zero If The Window Is Too Small
    Height=1;

  glViewport(0, 0, Width, Height);		// Reset The Current Viewport And Perspective Transformation

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  gluPerspective(45.0f,(GLfloat)Width/(GLfloat)Height,0.1f,100.0f);
  glMatrixMode(GL_MODELVIEW);
}

/* The main drawing function. */
void DrawGLScene()
{
  int i, ative_particles=0;
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Clear The Screen And The Depth Buffer
  glLoadIdentity();				// Reset The View

  glTranslatef(0.0f,0.0f,-6.0f);		// Move particles 6.0 units into the screen
	
  glBegin(GL_POINTS);
  for(i=0;i<num_ativas;i++) {
    if(particles[i].lifetime) {
      ative_particles++;
      particles[i].veloc_y -= GRAVITY;
      particles[i].y += particles[i].veloc_y;
      particles[i].x += particles[i].veloc_x;
      particles[i].lifetime--;
      //num_ativas++;
      glVertex3f( particles[i].x-4, particles[i].y+2.5, 0.0f); // draw pixel
    }
  }
  if(num_ativas!=NUM_PARTICLES)num_ativas++;
  glEnd();
    printf("%i %i\n",num_ativas,ative_particles);
  // swap buffers to display, since we're double buffered.
  glutSwapBuffers();

  if(!ative_particles){
      InitParticle(1); 
      num_ativas=1;
  }// reset particles
}

/* The function called whenever a key is pressed. */
void keyPressed(unsigned char key, int x, int y) 
{
  usleep(100);

  if (key == ESCAPE) 
  { 
	glutDestroyWindow(window); 
	
	exit(0);                   
  }
}

int main(int argc, char **argv) 
{  
  glutInit(&argc, argv);  
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);  
  glutInitWindowSize(640, 480);  
  glutInitWindowPosition(0, 0);  
  window = glutCreateWindow("Rain");  
  glutDisplayFunc(&DrawGLScene);  
  glutFullScreen();
  glutIdleFunc(&DrawGLScene);
  glutReshapeFunc(&ReSizeGLScene);
  glutKeyboardFunc(&keyPressed);
  InitGL(640, 480);
  glutMainLoop();  

  return 0;
}

