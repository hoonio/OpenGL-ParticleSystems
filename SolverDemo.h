//3456789_123456789_123456789_123456789_123456789_123456789_123456789_123456789_
/*
	Title: Implicit/Explicit Solver Demo
	Note:  Built up from CS351 starter code written by Ankit Mohan
	2/2/2004 David Feng -- Created
	2/6/2004 J. Tumblin -- removed unneeded pointers, fixed scope errors, 
						   replaced scattered globals with 'swarm' class,
						   updated comments, simplified display, added
						   'anchor', drew axes, added 'pause' button.

	Keyboard Controls===================================================
	* ARROW keys	--Rotate the camera position in 10 degree increments
	* q  Q or ESC	--quit/end program
	* s				--toggle between implicit/explicit solver. 
						(set to implicit on startup)
	* p				--toggle between paused/not paused.
	* d				--disturb the system a bit
	====================================================================

	The 3D system being emulated here is a simple one:  there are two masses 
	or 'Blobs' (A and B) attached to springs whose other ends are attached to an
	anchor point at the center of the screen.  
	These masses as an added bonus repel each other, just like two particles
	with strong, similar charge.  Gravity pulls both particles downward.
	We compute particle positions only in discrete timesteps. During each
	timestep, we pretend the masses move at a constant velocity with fixed 
	forces acting upon them.  We can simulate the system of moving particles
	by constantly recalculating the velocity of the mass and moving it 
	at that velocity for each timestep.  This begs two questions: 1) how do
	we calculate the velocity, and 2) how big a timestep should we use?

 EXPLICIT SOLVER
	The explicit solver answers those two questions in a rather simply way.
	At the beginning of every timestep it recalculates the forces acting on 
	each mass, and then moves it forward at constant velocity computed from 
	the force, as if velocity were constant for one timestep.
	This seems logical, except for the fact that this method only works
	accurately for an infinitely small timestep (like in real life!), anything
	larger accrues error with each step.  Choose a small enough timestep and
	the system will appear stable.  Choose a large one, and the error will
	pile on so fast that the particles will freak out and explode.  Seriously.
	Explicit solvers are nice because they can be fast and are easy to 
	understand and  implement.  Explicit solvers are not so nice because of 
	their tendency to explode, throwing particles everywhere.

IMPLICIT SOLVER
	Enter the implicit solver.  There are many implicit methods -- the one
	that we're doing here is called the Backward Euler Method (See pg XXX
	in YYY for more detail).  It basically sets up an ODE which, when solved, 
	is guaranteed to be stable.
	BUT, in this tool we're only solving the ODE with a first order iterative
	solver, which unfortunately is not always completely stable. It IS, however, 
	much, MUCH more stable than the traditional explicit solver.  
	Here's how it works:

		step 1) Solve the force explicitly as described above, and move particle
					forward.
		step 2) Solve the force explicitly at the NEW position and move it 
					forward FROM THE ORIGINAL POSITION.
		step 3) Repeat step 2 until you can take the newly calculated force, 
					move the particle in reverse, and end up back at the 
					original position.  OR, just repeat it a bunch of times.

	The more we iterate, the closer we get.  Once we get close enough (as
	determined by you), use the vector you found with the timestep to
	move the particle forward. Unfortunately, this vector that we're 
	solving for is going to introduce some error.  Fortunately, as opposed 
	to overstep our ideal goal (as we did with the explicit solver), 
	the implicit solver (generally) tends to understep, as if some external
	drag or damping force were present. This means that the movements will
	be smoother and less dramatic than an explicit solver's results, but it 
	also means that you can let it run for as long as you like and it won't	
	explode.  And it's slower.

	That's about it -- please note that explicit/implicit solvers aren't
	relegated to physical, force-based systems.  The basic principle can 
	be applied to many interesting integration problems, including fluids
	and mechanically connected parts, cannon balls, airplanes, ...
*/
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <GL/glut.h>

#include "particle.h"

#define TITLE "CS 352 Computer Graphics MP1 - Seunghoon Kim"
#define WIDTH 800			// Display window width, height
#define HEIGHT 450

// Near and far planes of the OpenGL camera's viewing frustum 
#define NEAR 0.1
#define FAR 100

//Global Variables (Always A Very Bad Idea)

const int ANIMATION_DELAY = 5; //to stabilize the framerate

// The one-and-only global variable: a 'swarm' object that describes
// one entire particle system. In this case, it has just 3 particles
// (A,B, and origin).
Swarm myApp;			// not very imaginative name for it, I know..

//GL functions
void display();
void reshape(int w, int h);
void keyboard(unsigned char key, int x, int y);
void special(int key, int x, int y);
void idle(void);
void gl_init();
void glut_init(int *argc, char **argv);