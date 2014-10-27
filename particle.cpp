//3456789_123456789_123456789_123456789_123456789_123456789_123456789_123456789_

#include "particle.h"


//===================================Home-made vector library===================
Vector::Vector() 
{						// Default Constructor
	i=j=k=0.0f; 
}

Vector::~Vector()	// Default Destructor
{
	i=j=k=0.0f;
}

Vector::Vector(float _i, float _j, float _k)
{						// Value-setting constructor
	i = _i;
	j = _j;
	k = _k;
	normalize();
}

Vector::Vector(Vector &v) 
{						// copy constructor
	i = v.i;		
	j = v.j;
	k = v.k;
}

void Vector::zero(void) 
{
	i = 0.0;
	j = 0.0;
	k = 0.0;
}

void Vector::put(float _i, float _j, float _k) 
{
	i = _i;
	j = _j;
	k = _k;
}

void Vector::add(Vector &v1, Vector &v0)
{		// this = v1 + v0; Replace with '+' operator...
	i = v0.i + v1.i;
	j = v0.j + v1.j;
	k = v0.k + v1.k;
}

void Vector::addIn(Vector &v)
{		// this += v;      Replace with '+=' operator...
	i += v.i;
	j += v.j;
	k += v.k;
}

void Vector::addIn(float di, float dj, float dk)
{		 // this +=(di,dj,dk).
	i += di;
	j += dj;
	k += dk;
}

void Vector::sub(Vector &v1, Vector &v0)
{		// this = v1 - v0; Replace with '-' operator...
	i = v1.i - v0.i;
	j = v1.j - v0.j;
	k = v1.k - v0.k;
}

void Vector::subIn(Vector &v)
{		// this -= v;	   Replace with '-=' operator...
	i -= v.i;
	j -= v.j;
	k -= v.k;
}			
void Vector::scale(float mag) 
{		// this *= mag;	   Replace with '*=' operator...
	i *= mag;
	j *= mag;
	k *= mag;
}
float Vector::dot(Vector &v1, Vector &v0)
{		// return(v1 dot v2); Replace with '*' operator...
	return(v1.i * v0.i +
		   v1.j * v0.j +
		   v1.k * v0.k);
}

float Vector::normalize(void) 
//normalize the vector (divide by its length if length is nonzero) and
// return its length
{
float mag;

	mag = i*i + j*j + k*k;
	if(mag <= SOLV_EPSILON) 
	{
		return 0.0;
		printf("\n?!?! Normalized zero-length vector?!?! \n");
	}
	mag = (float)sqrt(mag);
	i /= mag;
	j /= mag;
	k /= mag;
	return mag;
}

//===================================Blob-class member fcns=====================

Blob::Blob()		// default constructor--all zero
{ 
	position.zero();
	mass = rest_length = k_spring = 0.0;
	velocity.zero();
	pAnchor = NULL;		// spring isn't connected to anyone;
						// we're fixed in space.
}

Blob::~Blob()		// default destructor
{
	pAnchor = NULL;			// this just pointed to another existing blob.
}

Blob::Blob(float _x, float _y, float _z, Blob* _pAnchor,
		   float _mass, float _charge, float _rest_length, float _kspring) 
{					// constructor that sets all values.
	position.put(_x, _y, _z);	// set position of the blob center
	velocity.zero();			// initially, blob isn't moving.
	pAnchor = _pAnchor;			// tie us to an existing blob
								// (if NULL, we never move in space; the spring
								// and the mass settings have no effect)
	mass = _mass;				// set blob mass
	charge = _charge;			// set blob charge
	rest_length = _rest_length;	// the spring's zero-force or 'rest' length
	k_spring = _kspring;		// the spring constant: how much force per
								// unit length-change
}

Blob::Blob(Blob &pt)	//copy constructor
{
	position = pt.position;		// copy the position vector 
	velocity = pt.velocity;		// and the velocity vector
	pAnchor = pt.pAnchor;		// and the ptr to the anchor blob...
	mass = pt.mass;
	charge = pt.charge;
	rest_length = pt.rest_length;
	k_spring = pt.k_spring;	
}

void Blob::fwdStep(Vector& force, float timestep) 
//Move the blob forward by applying a (fixed) force vector during one timestep.  
// Recall:
//		F = ma = m * dv/dt 
// and hence:
//		dv = (F/m) * dt		
// Where we approximate the infinitesimal amount of time 'dt' by 'timestep',
// and where 'dv' is how much the velocity changes during that timestep.
//
// We calculate dv (in each direction i,j,k) first and use it to change the
// current velocity, and then we move the blob forward for one timestep using
// the new velocity we calculated.
{
	velocity.addIn( (force.i / mass) * timestep,
					(force.j / mass) * timestep,
                    (force.k / mass) * timestep);
	
	position.addIn( velocity.i * timestep,
					velocity.j * timestep,
					velocity.k * timestep);
}

//========================Swarm (set of Blobs) member fcns====================

Swarm::Swarm(void) 				// default constructor
{
	for(int i=0; i<11; i++)
		planenow[i] = planenext[i] = treenow[i] = leafnext[i] = treenow[i] = treenext[i] = NULL;		// no blobs at all.
	pOrigin = lOrigin = tOrigin = NULL;
	solver_type = SOLV_IMPLICIT;// default uses implicit(so it is stable)
	isPaused = 0;				// default; run, don't pause.
	k_gravity = 1.0f;			// default: earth-like gravity strength;
	down.put(0.0f, -1.0f, 0.0f);// default: gravity 'down' direction is -y.
	timestep = 0.05f;			// how much time between each new position
	x_pos = y_pos = 0.0f;		// Camera position (adjusted by arrow keys)
}

Swarm::~Swarm(void)			// default destructor
{
	for(int i=0; i<11; i++)
	{
		if(planenow[i] != NULL) delete planenow[i];		// discard any dyn. alloc'd memory...
		if(planenext[i] != NULL) delete planenext[i];
		if(leafnow[i] != NULL) delete leafnow[i];		// discard any dyn. alloc'd memory...
		if(leafnext[i] != NULL) delete leafnext[i];
		if(treenow[i] != NULL) delete treenow[i];		// discard any dyn. alloc'd memory...
		if(treenext[i] != NULL) delete treenext[i];
	}
	if(lOrigin !=NULL) delete lOrigin;
	if(pOrigin !=NULL) delete pOrigin;
	if(tOrigin !=NULL) delete tOrigin;
	k_gravity = 0.0f;				// set 'no gravity' and impossible timestep
	timestep = 0.0f;
}

void Swarm::blobSwap(void)
// A generalized form of double-buffering for blobs;
// Exchange current blobs for updated 'next' blobs, if they exist.
{
Blob *pTmp;
Blob *lTmp;
Blob *tTmp;

	for(int i=0; i<11; i++)
	{
		if(planenow[i]==NULL || planenext[i]==NULL || leafnow[i]==NULL || leafnext[i]==NULL || treenow[i]==NULL || treenext[i]==NULL) 
		{
			printf("!!Swarm::blobSwap() can't swap non-existent A blobs!\n");
			return;
		}
	pTmp = planenow[i];				// all is OK: swap the 'A' blobs
	planenow[i] = planenext[i];				// current gets next,
	planenext[i] = pTmp;				// next gets current.
	lTmp = leafnow[i];				// all is OK: swap the 'A' blobs
	leafnow[i] = leafnext[i];				// current gets next,
	leafnext[i] = lTmp;				// next gets current.
	tTmp = treenow[i];				// all is OK: swap the 'A' blobs
	treenow[i] = treenext[i];				// current gets next,
	treenext[i] = tTmp;				// next gets current.
	}
}


Vector Swarm::getNetForceOn(Blob *pMe, Blob *pYou) 
//==============================================================================
// This is where much of the magic happens.  
// To find the total force on the 'me' blob, we have to sum up forces from
// gravity, from the spring that ties us to our 'anchor' blob, and the
// repulsive force that pushes us away from all other blobs.  In this simple
// starter code, there is only one other repulsive blob; call it 'you'.
// The forces are:
//	--Gravitational Force:  
//				fGrav = mass * k_gravity in the 'down' direction 
//	--Repulsive Force (think of two protons repelling each other): 
//				fRepel = (me.charge * you.charge) / dist^2, 
//						in the direction that pushes them apart.
//	--Spring Force (from the spring that attaches this blob to an anchor)
//				fSpring = k_spring * stretch, 
//						in the direction that returns spring to its rest-length;  k_spring is spring strength, 
//						(stretch = spring_length - rest_length)
//
{
Vector fGrav,fRepel,fSpring,fTot;

	// Find the force vector on 'me' due to gravity
	fGrav = down;					// gravity direction is straight down, and
	fGrav.scale(pMe->mass * k_gravity);// scaled by mass & gravity constant

	// Find the force vector on 'Me' that pushes it away from 'You'
	fRepel.put(	pMe->position.i - pYou->position.i,
				pMe->position.j - pYou->position.j,
				pMe->position.k - pYou->position.k);// vector from You to Me;
	float dist = fRepel.normalize();	// find length; then make unit vector
	fRepel.scale( (pMe->charge * pYou->charge) / (dist * dist)  ); // find force.

	// calculate the spring force on the 'Me' blob.  The 'me' blob has a spring
	// whose other end of is fixed to an 'anchor' blob. First, make a vector 
	// from the 'anchor' blob to 'Me' blob:
	fSpring.put(pMe->pAnchor->position.i - pMe->position.i,
				pMe->pAnchor->position.j - pMe->position.j, 
				pMe->pAnchor->position.k - pMe->position.k);
	dist = fSpring.normalize();				// find length, normalize the it.
	float stretch = dist - pMe->rest_length;// positive if spring is longer,
											// negative if spring is shorter.
	fSpring.scale(pMe->k_spring * stretch);	// Find force strength.
	// Add up all the force vectors;
	fTot.add(fGrav,fRepel);
	fTot.addIn(fSpring);

	return fTot;
}

Vector Swarm::treeMovement(Blob *pMe, Blob *pYou) 
{
Vector fRepel,fSpring,fTot;

	// Find the force vector on 'Me' that pushes it away from 'You'
	fRepel.put(	pMe->position.i - pYou->position.i, 0.0f, pMe->position.k - pYou->position.k);// vector from You to Me;
	float dist = fRepel.normalize();	// find length; then make unit vector
	fRepel.scale( (pMe->charge * pYou->charge) / (dist * dist)  ); // find force.
	fRepel.scale(0.1f);

	// calculate the spring force on the 'Me' blob.  The 'me' blob has a spring
	// whose other end of is fixed to an 'anchor' blob. First, make a vector 
	// from the 'anchor' blob to 'Me' blob:
	fSpring.put(pMe->pAnchor->position.i - pMe->position.i,
				0.0f, 
				pMe->pAnchor->position.k - pMe->position.k);
	dist = fSpring.normalize();				// find length, normalize the it.
	float stretch = dist - pMe->rest_length;// positive if spring is longer,
											// negative if spring is shorter.
	fSpring.scale(pMe->k_spring * stretch);	// Find force strength.
	// Add up all the force vectors;
	fSpring.scale(0.1f);
	fTot.add(fSpring,fRepel);

	return fTot;
}
Vector Swarm::leafMovement(Blob *pMe, Blob *pYou) 
{
Vector fGrav,fRepel,fTot;

	// Find the force vector on 'me' due to gravity
	fGrav.put(0.0f, -0.01f, 0.0f);					// gravity direction is straight down, and
	fGrav.scale(pMe->mass * k_gravity);// scaled by mass & gravity constant

	// Find the force vector on 'Me' that pushes it away from 'You'
	fRepel.put(	pMe->position.i - pYou->position.i, 0.0f, pMe->position.k - pYou->position.k);// vector from You to Me;
	float dist = fRepel.normalize();	// find length; then make unit vector
	fRepel.scale( (pMe->charge * pYou->charge) / (dist * dist)  ); // find force.
	fRepel.scale(0.001f);
	fTot.add(fGrav,fRepel);

	return fTot;
}

void Swarm::explicitSolve(void) 
//==============================================================================
// Explicit Solver.  From the current positions of blobs pA,pB, find the
// position at the end of one timestep.
{
Vector fnow[11];
Vector gnow[11];
Vector hnow[11];

	for(int i=0; i<10; i++)
	{
			*planenext[i] = *planenow[i];			// Begin: copy current planenow blobs to the planenext blobs
			*planenext[i+1] = *planenow[i+1];			// Begin: copy current planenow blobs to the planenext blobs
			// Find the total force acting on blobs at their current positions.
			fnow[i] = getNetForceOn(planenow[i],planenow[i+1]);
			fnow[i+1] = getNetForceOn(planenow[i+1],planenow[i]);
			planenext[i]->fwdStep(fnow[i],timestep);
			planenext[i+1]->fwdStep(fnow[i+1],timestep);

			*leafnext[i] = *leafnow[i];			// Begin: copy current planenow blobs to the planenext blobs
			*leafnext[i+1] = *leafnow[i+1];			// Begin: copy current planenow blobs to the planenext blobs
			// Find the total force acting on blobs at their current positions.
			gnow[i] = getNetForceOn(leafnow[i],leafnow[i+1]);
			gnow[i+1] = getNetForceOn(leafnow[i+1],leafnow[i]);
			leafnext[i]->fwdStep(gnow[i],timestep);
			leafnext[i+1]->fwdStep(gnow[i+1],timestep);

			*treenext[i] = *treenow[i];			// Begin: copy current planenow blobs to the planenext blobs
			*treenext[i+1] = *treenow[i+1];			// Begin: copy current planenow blobs to the planenext blobs
			// Find the total force acting on blobs at their current positions.
			hnow[i] = getNetForceOn(treenow[i],treenow[i+1]);
			hnow[i+1] = getNetForceOn(treenow[i+1],treenow[i]);
			hnow[i].j = 0.0;
			hnow[i+1].j = 0.0;
			treenext[i]->fwdStep(hnow[i],timestep);
			treenext[i+1]->fwdStep(hnow[i+1],timestep);
	}

	// use that force to adjust the blob velocities, 
	// then move each blob forward using that velocity for one timestep;
//	for(int i=0; i<11; i++)
//	pBnext->fwdStep(fBnow,timestep);
//	pCnext->fwdStep(fCnow,timestep);
	blobSwap();						// Replace existing blobs with new ones.
}

void Swarm::implicitSolve(void) 
//==============================================================================
/* Recall that the explicit solver will
	1) find forces fAnow,fBnow 
			that apply at current positions Anow,Bnow.
	2) apply those forces to Anow,Bnow 
			to find new blob positions Anext,Bnext. 
	3) call blobSwap() so that Anext,Bnext blobs become the current blobs.

 Explicit solvers assume that:
  -- forces can be approximated as constant for one tiny timestep 
	*AFTER* from the current time. 
 Implicit solvers do the  opposite; they assume 
  -- forces can be approximated as constant for one tiny timestep 
    *BEFORE* the current time.    

 The implicit solver is very different, and at first may seem impossible. 
 Instead of finding the forces at pAnow,pBnow and applying them at pAnow, pBNow,
 (e.g. using *current* forces FORWARD in time) we want want to somehow find the
 forces at Anext, pBnext and use them instead (using *future* forces BACKWARD 
 in time).   We already know pAnow,pBnow, but we must somehow find Anext,pBnext 
 whose forces would move the pAnow,pBnow blobs to the pAnext,pBnext position. 
 But how can we find fAnext, fBnext forces if we don't know the Anext, Bnext 
 positions where we'll measure them!?!?
 
  The answer is to follow root-finding methods.  First, we make a crude guess 
  at fAnext, fBnext forces and then progressively refine that guess.  If we
  have found the right forces fAnext,fBnext, then we can apply them to current 
  blobs Anow,Bnow to find Anext,Bnext, where forces match fAnext,fBnext well.
  Viewed another way, we could say we somehow know positions Anext,Bnext,
  we find the forces there, and then go backwards in time by one timestep to
  arrive at the current blob positions Anow,Bnow (some people say implicit 
  solvers find the 'backwinded' solution).

	 *** HOW? ***
 1) Start with a guess at the *forces* at Anext,Bnext.  We know all our forces
 vary smoothly with position, so our first guess is that the forces at 
 Anext,Bnext are same as those at Anow,Bnow: 
	fAnext = getNetForceOn(pAnow,pBnow); fBnext = getNetForceOn(pBnow,pAnow);
 2) Rather than apply forces at Anext,Bnext and go backwards in time, lets apply
 the forces fAnext and fBnext to current blobs and go FORWARDS in 
 time instead, taking known blob positions pAnow,pBnow forward to make an 
 estimate of the Anext,Bnext positions:
	Anext = Anow;	Bnext = Bnow;		
	Anext.fwdStep(fAnext,timestep); 
	Bnext.fwdStep(fBnext,timestep);
 3) If our estimate is correct, then the forces at these Anext,Bnext positions 
 will match fAnext,fBnext closely. If not, we can improve our estimate by using 
 the forces found at Anext,Bnext instead:
	 fAnext = getNetForceOn(Anext,Bnext); fBnext = getNetForceOn(Bnext,Anext);
 4) go back to step 2.

// Why do this at all? Because it greatly improves stability--it reduces the
// likelihood that our system of blobs will 'blow up', a common occurence for 
// explicit solvers.  If the forces on blobs *REALLY ARE* approximately constant
// during the timestep, then it doesn't matter if we apply forces found at 
// Anow,Bnow or at Anext,Bnext--they will be the same.  But if the current 
// positions pAnow,pBnow have very strong forces an explicit solver will move 
// them very long distances in just one timestep, to a place where forces may 
// be very different. 
// Instead, an implicit solver finds a position whose forces best match the 
// blob-displacement needed to get there.  Large but very localized forces 
// (e.g. repulsion of two blobs that are almost touching each other) do not 
// cause an implicit solver to 'blow up', because these large forces only
// exist in a small neighborhood; the neighborhood is surrounded by nearby 
// positions that have small forces that can only justify small movements.  
// Even though current forces may be VERY large, they can't cause a large 
// displacements because we can't find far-away destinations whose forces 
// are large enough; when those forces are applied in reverse, they are not
// enough to return the displaced blob to its current position.  Instead,
// only nearby points will have a force large enough to justify the
// displacement needed to get there.
*/
{
Vector fnext[11];
Vector gnext[11];
Vector hnext[11];
// 1st Guess: find forces at the
//Vector fBnext = getNetForceOn(pBnow, pAnow);// current blob positions Anow,Bnow		critical point
//Vector fCnext = getNetForceOn(pCnow, pBnow);// current blob positions Anow,Bnow

// This 'for' loop uses estimated positions Anext,Bnext to find forces fAnext,
// fBnext, then applies these forces to current blob positions pAnow, pBnow to 
// find better next-position estimates Anext,Bnext.  For speed, we only  
// use a few refinement passes, as this is enough to make our simple anchored 
// blobs system very stable. Note that you may need more iterations for
// more complex, stiffer particle systems such as cloth... You might also
// want to make an error measurement and continue iteration until error is
// below some desired threshold.

//	for(int i=0; i<11; i++)
//		*planenext[i] = *planenow[i];			// Begin: copy current planenow blobs to the planenext blobs

	for(int i=0; i<10; i++)
	{
			for(int k=0; k<4; k++)
			{
			//Get new force estimates from these Anext,Bnext position estimates.
					*planenext[i] = *planenow[i];	// Starting from current blobs Anow,Bnow
					*planenext[i+1] = *planenow[i+1];	// Starting from current blobs Anow,Bnow
					planenext[i]->fwdStep(fnext[i], timestep);//get Anext, Bnext position estimates.
					planenext[i+1]->fwdStep(fnext[i+1], timestep);//get Anext, Bnext position estimates.
					fnext[i] = getNetForceOn(planenext[i], planenext[i+1]);
					fnext[i+1] = getNetForceOn(planenext[i+1], planenext[i]);

					*leafnext[i] = *leafnow[i];	// Starting from current blobs Anow,Bnow
					*leafnext[i+1] = *leafnow[i+1];	// Starting from current blobs Anow,Bnow
					leafnext[i]->fwdStep(gnext[i], timestep);//get Anext, Bnext position estimates.
					leafnext[i+1]->fwdStep(gnext[i+1], timestep);//get Anext, Bnext position estimates.
					gnext[i] = leafMovement(leafnext[i], leafnext[i+1]);
					gnext[i+1] = leafMovement(leafnext[i+1], leafnext[i]);

					*treenext[i] = *treenow[i];	// Starting from current blobs Anow,Bnow
					*treenext[i+1] = *treenow[i+1];	// Starting from current blobs Anow,Bnow
					treenext[i]->fwdStep(hnext[i], timestep);//get Anext, Bnext position estimates.
					treenext[i+1]->fwdStep(hnext[i+1], timestep);//get Anext, Bnext position estimates.
					hnext[i] = treeMovement(treenext[i], treenext[i+1]);
					hnext[i+1] = treeMovement(treenext[i+1], treenext[i]);
			}
	//Refinement is finished.
	//Move current blobs forward using our best fAnext,fBnext force estimate.
		planenow[i]->fwdStep(fnext[i],timestep);
		leafnow[i]->fwdStep(gnext[i],timestep);
		treenow[i]->fwdStep(hnext[i],timestep);
	//(NOTE! we don't swap blobs--we updated pAnow,Bnow instead)

	}

// DIAGNOSTIC: Error measurement
Vector err;
float pct;

	err.put(planenow[0]->position.i - planenext[0]->position.i,
			planenow[0]->position.j - planenext[0]->position.j,
			planenow[0]->position.k - planenext[0]->position.k);
	pct = err.normalize();

//ONSCREEN TIC_MARKS:----------------------
//if(pct<0.000001) printf(".");	// error less than 1 in a million; ignore.
//else printf("\n Residual error ||updated_Anow - Anext|| = %f", pct);

	//Note that increasing the number of passes in the 'for' loop reduces
	// error; 4 iterations keeps error < 1 in a million at all times...

}


