//3456789_123456789_123456789_123456789_123456789_123456789_123456789_123456789_

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <GL/glut.h>

#include "SolverDemo.h"


void display()
//------------------------------------------------------------------------------
// GLUT calls this function when it needs the screen re-drawn. Your program 
// should NEVER call this function--that would confuse GLUT.  Instead, call 
// glutPostRedisplay() to force GLUT to start this callback properly.
//
// All drawing commands are applied to the back buffer. 
// The 'glutSwapBuffers()' call at the end of this function swaps the front
// and back buffers to display the completed drawing.
{
	GLfloat A_diffuse[]   = {1.0, 0.0, 0.0, 1.0};	// red,
	GLfloat B_diffuse[]   = {0.0, 0.0, 1.0, 1.0};	// blue,
	GLfloat O_diffuse[]   = {0.0, 1.0, 0.0, 1.0};	// green,
	GLfloat tree_diffuse[]   = {0.5, 0.3, 0.0, 1.0};	// green,

	GLfloat spring_diffuse[]   = {0.6, 1.0, 0.1, 1.0};	// spring,
	GLfloat summer_diffuse[]   = {0.4, 1.0, 0.1, 1.0};	// summer,
	GLfloat fall_diffuse[]   = {0.9, 0.9, 0.1, 1.0};	// autumn - leaves,
	GLfloat winter_diffuse[]   = {1.0, 1.0, 1.0, 1.0};	// winter - snow,
	GLfloat winter_ambient[]   = {1.0, 1.0, 1.0, 1.0};	// winter - snow,

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(0.0, 0.0, 10.0,		// World-space x,y,z eyepoint position
		      0.0, 0.0,  0.0,			// the x,y,z point you're looking at
			  0.0, 1.0,  0.0);			// the camera's 'up' direction

	// Clear the Z- and frame-buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	int season = glutGet(GLenum(GLUT_ELAPSED_TIME)) % 80000;

	if (season < 10000)
		glClearColor(0.0f, 0.5f, 1.0f, 0.0f);	// Set screen-clearing color 
	else if (season > 15000 && season < 25000)
		glClearColor(0.0f, 0.2f, 1.0f, 0.0f);	// Set screen-clearing color 
	else if (season > 30000 && season < 40000)
		glClearColor(0.2f, 0.4f, 0.6f, 0.0f);	// Set screen-clearing color 
	else if (season > 45000 && season < 65000)
		glClearColor(0.8f, 0.8f, 0.8f, 0.0f);	// Set screen-clearing color 
	else
		glClearColor(0.1f, 0.1f, 0.1f, 0.0f);	// Set screen-clearing color 

	glRotatef(myApp.y_pos * 10.0f, 1.0f, 0.0f, 0.0f);
	glRotatef(myApp.x_pos * 10.0f, 0.0f, 1.0f, 0.0f);
	//glTranslatef(x_pos, y_pos, 0);	

	glTranslatef(0.0, -18.0, 0.0);
	glPushMatrix();		// save current matrix;
	if (season < 15000)
		glMaterialfv(GL_FRONT, GL_DIFFUSE, spring_diffuse);
	else if (season < 30000)
		glMaterialfv(GL_FRONT, GL_DIFFUSE, summer_diffuse);
	else if (season < 45000)
		glMaterialfv(GL_FRONT, GL_DIFFUSE, fall_diffuse);
	else
	{
		glMaterialfv(GL_FRONT, GL_DIFFUSE, winter_diffuse);
		glMaterialfv(GL_FRONT, GL_AMBIENT, winter_ambient);
	}
				// move the modelview origin to Blob A's world-space position
	glutSolidSphere(16.0, 64, 64); 
						// draw blob with diameter 
						// proportional to its mass, as if all blobs were 
						// made of materials with the same density.
	glPopMatrix();		// go back to our original modelview matrix.

	glTranslatef(0.0, 16.0, 0.0);
	glPushMatrix();		// save current matrix;
	glRotatef(270.0, 1.0, 0.0, 0.0); 
	glMaterialfv(GL_FRONT, GL_DIFFUSE, tree_diffuse);
	glutSolidCone(0.5, 2.0, 16, 16);
	glTranslatef(0.0, 2.0, 0.0);
	glPopMatrix();		// go back to our original modelview matrix.

	for (int drawt=0; drawt<10; drawt++)
	{
		//draw sphere A
		glPushMatrix();		// save current matrix;
		if (season < 15000)
			glMaterialfv(GL_FRONT, GL_DIFFUSE, spring_diffuse);
		else if (season < 30000)
			glMaterialfv(GL_FRONT, GL_DIFFUSE, summer_diffuse);
		else if (season < 45000)
			glMaterialfv(GL_FRONT, GL_DIFFUSE, fall_diffuse);
		else
			break;
 		glTranslatef(myApp.treenow[drawt]->position.i, 
					 myApp.treenow[drawt]->position.j, 
					 myApp.treenow[drawt]->position.k);
		if (drawt%2 == 1)		
			glutSolidSphere(0.5f, 32, 32); 
		else
			glutSolidSphere(0.4f, 32, 32); 
							// draw blob with diameter 
							// proportional to its mass, as if all blobs were 
							// made of materials with the same density.
		glPopMatrix();		// go back to our original modelview matrix.
	}
	if (season > 45000)
	{
		for (int drawl=0; drawl<10; drawl++)
		{
			//draw sphere A
			glPushMatrix();		// save current matrix;
			glMaterialfv(GL_FRONT, GL_DIFFUSE, winter_diffuse);
						// move the modelview origin to Blob A's world-space position
 			glTranslatef(myApp.leafnow[drawl]->position.i, 
						 myApp.leafnow[drawl]->position.j, 
						 myApp.leafnow[drawl]->position.k);
			glutSolidSphere(0.2f, 32, 32); 
								// draw blob with diameter 
								// proportional to its mass, as if all blobs were 
								// made of materials with the same density.
			glPopMatrix();		// go back to our original modelview matrix.
		}
	}

	glPushMatrix();
	glDisable(GL_LIGHTING);
	glBegin(GL_LINES);
		glColor3f(0.3f, 0.3f, 0.3f);	// gray lines
	for (int drawm=0; drawm<10; drawm++)
	{
		glVertex3f(myApp.treenow[drawm]->pAnchor->position.i,
				   myApp.treenow[drawm]->pAnchor->position.j,// from Blob A's anchor pt.
				   myApp.treenow[drawm]->pAnchor->position.k);
		glVertex3f(myApp.treenow[drawm]->position.i,// to center of blob A,
				   myApp.treenow[drawm]->position.j,
				   myApp.treenow[drawm]->position.k);
	}

		glEnd();
	glEnable(GL_LIGHTING);
	glPopMatrix();	

	for (int drawi=0; drawi<10; drawi++)
	{
		//draw sphere A
		glPushMatrix();		// save current matrix;
		glMaterialfv(GL_FRONT, GL_DIFFUSE, A_diffuse);
					// move the modelview origin to Blob A's world-space position
 		glTranslatef(myApp.planenow[drawi]->position.i, 
					 myApp.planenow[drawi]->position.j, 
					 myApp.planenow[drawi]->position.k);
		glutSolidTeapot(0.4); 
							// draw blob with diameter 
							// proportional to its mass, as if all blobs were 
							// made of materials with the same density.
		glPopMatrix();		// go back to our original modelview matrix.
	}

/*	//draw triangles that shows blobs A,B connected to the anchor 'pOrigin'
	glPushMatrix();
	glDisable(GL_LIGHTING);
	glBegin(GL_LINES);
		glColor3f(0.3f, 0.3f, 0.3f);	// gray lines
	for (int drawj=0; drawj<10; drawj++)
	{
		glVertex3f(myApp.planenow[drawj]->pAnchor->position.i,
				   myApp.planenow[drawj]->pAnchor->position.j,// from Blob A's anchor pt.
				   myApp.planenow[drawj]->pAnchor->position.k);
		glVertex3f(myApp.planenow[drawj]->position.i,// to center of blob A,
				   myApp.planenow[drawj]->position.j,
				   myApp.planenow[drawj]->position.k);
	}

*/
	glEnd();
	glEnable(GL_LIGHTING);
	glPopMatrix();	

/*
	// Draw some axes:
	glDisable(GL_LIGHTING);
	glBegin(GL_LINES);
		glColor3f(1.0f, 0.0f, 0.0f);	// red +x axis,
//	glMaterialfv(GL_FRONT, GL_DIFFUSE, A_diffuse);
		glVertex3f(0.0, 0.0, 0.0);		
		glVertex3f(1.0, 0.0, 0.0);

		glColor3f(0.0f, 1.0f, 0.0f);	// green +y axis,
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(0.0, 1.0, 0.0);

		glColor3f(0.0f, 0.0f, 1.0f);	// blue +z axis,
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(0.0, 0.0, 1.0);
	glEnd();
	glEnable(GL_LIGHTING);
	//draw the anchor sphere
	glMaterialfv(GL_FRONT, GL_DIFFUSE, O_diffuse);
	glTranslatef(myApp.pOrigin->position.i, 
				 myApp.pOrigin->position.j, 
				 myApp.pOrigin->position.k);
	glutSolidSphere(0.05, 32, 32);
*/	
	glFlush();			// execute any unfinished drawing commands. We'll wait.

	glutSwapBuffers();
// Swap the frame buffer. Make the back buffer the new front buffer and
//vice-versa. To avoid flicker we use two buffers. While we display one,
//we make changes to the second hidden buffer.
}                                                                                                                                                                                 

void reshape(int w, int h)
//------------------------------------------------------------------------------
// GLUT calls this function whenever the user resizes the display window by
// dragging a corner, etc., and is also called on window createion.
// Re-initialize the Projection matrix here.
{
	glViewport(0, 0, (GLsizei) w, (GLsizei) h);

	// Set the projection matrix 
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40.0, (double)w/(double)h, NEAR, FAR);

	// Initialize the modelview matrix 
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}


void keyboard(unsigned char key, int x, int y)
//------------------------------------------------------------------------------
// GLUT calls this function when user hits an alpha-numeric key.
{
static float oldG;

	switch(key) 
	{
		case 'd':
		case 'D':
			myApp.planenow[0]->velocity.addIn( 1.0f, -1.0f, 0.01f);
			myApp.planenow[1]->velocity.addIn(-1.0f, -1.0f, 0.0f);
			myApp.planenow[2]->velocity.addIn(0.0f, 1.0f, 0.0f);
			//myApp.treenow[1]->velocity.addIn(0.1f, 0.0f, -0.1f);
			break;
		case 'g':
		case 'G':		// toggle gravity on/off.
			if( myApp.k_gravity > 0.0f)		// if we have gravity already,
			{
				oldG = myApp.k_gravity;		// save its old value, and
				myApp.k_gravity = 0.0f;		// zero gravity.
				printf("Gravity off...\n");
			}
			else
			{								// Restore gravity to its old value.
				myApp.k_gravity = oldG;
				printf("Gravity on.\n");
			}
			break;
		case 'p':	// toggle between pause and run.
		case 'P':
			if(myApp.isPaused==1)
			{
				myApp.isPaused = 0;
				printf("Resumed. Hit 'p' to Pause.\n");
			}
			else 
			{
				myApp.isPaused = 1;
				printf("Paused! Hit 'p' to resume ...\n ");
			}
			break;
		case 's':
		case 'S':
			if (myApp.solver_type == SOLV_IMPLICIT) 
			{
				myApp.solver_type = SOLV_EXPLICIT;
				printf("Switched to Explicit Solver...\n");
			} 
			else 
			{
				myApp.solver_type = SOLV_IMPLICIT;
				printf("Switched to Implicit Solver...\n");
			}
			break;
		case 27:	// Esc 
		case 'Q':
		case 'q':
			exit(0); // quit the program
			break;
	}

	// We might have changed something. Force a re-display.
	glutPostRedisplay();
}


void special(int key, int x, int y)
//------------------------------------------------------------------------------
// GLUT calls this function when user presses a non-numeric key, such as arrow
// keys.  We use arrow keys to rotate the camera around the origin.
{
	switch(key)	
	{
		case GLUT_KEY_UP:
			myApp.y_pos += 1.0f;
			break;
		case GLUT_KEY_DOWN:
			myApp.y_pos -= 1.0f;
			break;
		case GLUT_KEY_LEFT:
			myApp.x_pos -= 1.0f;
			break;
		case GLUT_KEY_RIGHT:
			myApp.x_pos += 1.0f;
			break;
		default:
			break;
	}
	// We might have changed something. Force a re-display.
	glutPostRedisplay();
}

void idle(void)
//------------------------------------------------------------------------------
// CAREFUL!  if you register this function, your program will call it VERY OFTEN
// GLUT will call idle() ANYTIME it has nothing else to do; even if you put 
// NOTHING in this function your program will use ~100% of CPU time.
//	If you need to put something here (e.g. animation) that will change the
// displayed image contents, remember to call glutPostRedisplay() at the end
// to force screen updating.
{
	//some frame-rate independent motion;
static int oldTime = 0;
int currentTime = glutGet(GLenum(GLUT_ELAPSED_TIME));
    // Ensures fairly constant framerate

	int timestamp = currentTime;
	for(int leafcount=0; leafcount<11; leafcount++)
	{
		if((myApp.leafnow[leafcount] != NULL) && (myApp.leafnow[leafcount]->position.j < -2.0f))
		{
			delete myApp.leafnow[leafcount];		// discard any dyn. alloc'd memory...
			delete myApp.leafnext[leafcount];
			myApp.leafnow[leafcount] = new Blob((4.0f-(leafcount%7)), (3.0f+(leafcount%3)), ((leafcount%5)-2.0f), myApp.lOrigin,
												(1.0f+(leafcount)), 1.0f, 4.0f, 50.0f);	
			myApp.leafnext[leafcount] = new Blob(*(myApp.leafnow[leafcount]));	// copy constructor
		}
	}

	if (currentTime - oldTime > ANIMATION_DELAY) 
	{
		// animate the scene
		if(myApp.isPaused==0)	// if 'paused', change nothing,
		{
			if (myApp.solver_type == SOLV_EXPLICIT) 
			{
				myApp.explicitSolve();
			} else 
			{
				myApp.implicitSolve();
			}
		}
        oldTime = currentTime;
        // notify window it has to be repainted
        glutPostRedisplay();
    }
}


void gl_init()
//------------------------------------------------------------------------------
//Initialize all opengl related stuff 
{
//	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);	// Set screen-clearing color 
	glEnable(GL_DEPTH_TEST)		;			// Enable the Z-Buffer 

	
	GLfloat light_ambient[]  = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat light_specular[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    //	light_position is NOT default value:
    GLfloat light_position[] = { 0.0f, 0.2f, 2.4f, 0.0f };
	
	glLightfv (GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv (GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv (GL_LIGHT0, GL_POSITION, light_position);

    glEnable (GL_LIGHTING);
    glEnable (GL_LIGHT0);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void glut_init(int *argc, char **argv)
//------------------------------------------------------------------------------
// Initialize all glut related stuff, including callback functions.
{
	// Call glut's initilization 
	glutInit(argc, argv);

	// double buffer, RGB color model, depth buffer 
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	// Now create a window of the required size 
	glutInitWindowSize(WIDTH, HEIGHT);
	glutInitWindowPosition(0, 0);
	glutCreateWindow(TITLE);

	// register all glut Callback functions. GlUT calls
	glutDisplayFunc(display);			// display() to redraw screen
	glutKeyboardFunc(keyboard);			// keyboard() when user presses key
	glutSpecialFunc(special);			// special() for non-alphanumeric keys 
	glutReshapeFunc(reshape);			// reshape() when window size changes 
	glutIdleFunc(idle);					// idle() when all other tasks done. 

	// These register callback functions for mouse clicks, mouse moves.
	// Implement for yourself as needed 
	glutMouseFunc(NULL);
	glutMotionFunc(NULL);

	printf("==============Keyboard Controls==========================\n");
	printf("    d or D    --displace the blobs a bit\n");
	printf("    g or G    --toggle gravity on/off\n");
	printf("    p or P    --toggle between paused/not paused.\n");
	printf("    s or S    --toggle between implicit/explicit solver \n");
	printf("                  (implicit at startup) \n");
	printf(" ARROW keys	  --Rotate camera in 10 degree increments\n");
	printf(" q, Q or ESC  --quit/end program\n");
	printf("==========================================================\n");
}

int main(int argc, char** argv)
//==============================================================================
{
	// Initialize: first, set the 'pOrigin' blob fixed in space 
	myApp.pOrigin = new Blob( 1.0f, 5.0f, -2.0f,	NULL, // position, anchor,
					  0.0f, 0.0f, 0.0f, 0.0f);	// mass, charge, spring rest 
												// length, spring strength
	myApp.lOrigin = new Blob( 0.0f, 1.0f, 0.0f,	NULL, // position, anchor,
					  0.0f, 0.0f, 0.0f, 0.0f);	// mass, charge, spring rest 
												// length, spring strength
	myApp.tOrigin = new Blob( 0.0f, 0.5f, 0.0f,	NULL, // position, anchor,
					  0.0f, 0.0f, 0.0f, 0.0f);	// mass, charge, spring rest 
												// length, spring strength

	myApp.treenow[0] = new Blob( 0.0f, 2.2f, 0.0f, myApp.tOrigin, 30.0f, 0.5f, 0.5f, 120.0f);
	myApp.treenow[1] = new Blob( 0.2f, 2.0f, 0.0f, myApp.tOrigin, 30.0f, 0.5f, 0.3f, 120.0f);
	myApp.treenow[2] = new Blob( 0.0f, 1.6f, 0.2f, myApp.tOrigin, 30.0f, 0.5f, 0.5f, 120.0f);
	myApp.treenow[3] = new Blob( 0.0f, 1.8f, -0.2f, myApp.tOrigin, 30.0f, 0.5f, 0.5f, 120.0f);
	myApp.treenow[4] = new Blob( -0.2f, 2.0f, 0.0f, myApp.tOrigin, 30.0f, 0.5f, 0.3f, 120.0f);
	myApp.treenow[5] = new Blob( 0.2f, 1.6f, 0.2f, myApp.tOrigin, 30.0f, 0.5f, 0.5f, 120.0f);
	myApp.treenow[6] = new Blob( 0.2f, 2.0f, -0.2f, myApp.tOrigin, 30.0f, 0.5f, 0.5f, 120.0f);
	myApp.treenow[7] = new Blob( -0.2f, 1.8f, 0.2f, myApp.tOrigin, 30.0f, 0.5f, 0.3f, 120.0f);
	myApp.treenow[8] = new Blob( -0.2f, 2.0f, -0.2f, myApp.tOrigin, 30.0f, 0.5f, 0.5f, 120.0f);
	myApp.treenow[9] = new Blob( 0.0f, 2.4f, 0.0f, myApp.tOrigin, 30.0f, 0.5f, 0.4f, 120.0f);
	myApp.treenow[10] = new Blob( 0.0f, 1.6f, 0.0f, myApp.tOrigin, 30.0f, 0.5f, 0.1f, 120.0f);

	for(int mainlp=0; mainlp<11; mainlp++)
	{
	myApp.planenow[mainlp] = new Blob((1.5f+(0.1f*mainlp)), (2.0f-(0.1f*mainlp)), (0.8f-(0.1f*mainlp)), myApp.pOrigin,
													20.0f, 4.0f, 5.0f, 180.0f);

	myApp.planenext[mainlp] = new Blob(*(myApp.planenow[mainlp]));	// copy constructor

	myApp.leafnow[mainlp] = new Blob((0.7f+(mainlp%4)), (4.5f-(mainlp%7)), (3.5f-(mainlp%3)), myApp.lOrigin,
													(1.0f+(mainlp%5)), 1.0f, 4.0f, 50.0f);

	myApp.leafnext[mainlp] = new Blob(*(myApp.leafnow[mainlp]));	// copy constructor

	myApp.treenext[mainlp] = new Blob(*(myApp.treenow[mainlp]));	// copy constructor
	}
	// Initialize GLUT 
	glut_init(&argc, argv);

	// Now initialize OpenGL stuff 
	gl_init();

	// Finally enter the GLUT infinite loop 
	glutMainLoop();
	// until user quits.
	return 0;
}
