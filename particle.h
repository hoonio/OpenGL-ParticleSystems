//3456789_123456789_123456789_123456789_123456789_123456789_123456789_123456789_
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

const int SOLV_IMPLICIT = 0;	// flag for implicit vs. explicit
const int SOLV_EXPLICIT = 1;	// 

const float SOLV_EPSILON = 1.0E-10f; // the smallest number treated as nonzero.


class Vector
//=============================================================================
// a column vector class
// All vectors are initialized with i,j,k values (this is a 3D system)
{
public:
	float i,j,k;				
//	float col[3];				// column vector of 3 elements.

	Vector();					// default constructor
	~Vector();					// default destructor
	Vector(Vector &v);			// copy constructor
	Vector(float _i, float _j, float _k);	// value-setting constructor


	void zero(void);			// set everything to zero.
	void put(float _i, float _j, float _k);
	void add(Vector &v1, Vector &v0);	// this = v1 + v0; Replace with '+' operator...
	void addIn(Vector &v);				// this += v;      Replace with '+=' operator...
	void addIn(float di, float dj, float dk); // this +=(di,dj,dk).
	void sub(Vector &v1, Vector &v0);	// this = v1 - v0; Replace with '-' operator...
	void subIn(Vector &v);				// this -= v;	   Replace with '-=' operator...
	void scale(float mag);				// this *= mag;	   Replace with '*=' operator...
	float dot(Vector &v1, Vector &v2);	// this = v1 dot v2; Replace with '*' operator...
	float normalize(void);				// this /= ||this||; return length ||this||
};

//=============================================================================
class Blob 
{						//A Simple mass-with-anchored-spring class
public:
	Vector position;	//blob position (x,y,z) in world space
	Vector velocity;	//velocity vector for this particle/blob.
	Blob *pAnchor;		//(ptr to the) blob at the other end of our spring.
						// if NULL, blob is fixed in space and ignores springs.
	float mass;			//the mass of the particle/blob.
	float charge;		//our strength of repulsion from other blobs
	float rest_length;	//the zero-force 'rest length' of the spring.
	float k_spring;		//the spring constant: force per unit length change

	Blob();				// Default constructor
	~Blob();			// Default destructor
	Blob(Blob &pt);		// copy constructor
	Blob(float _x, float _y, float _z, Blob* _pAnchor,// Value-setting constructor
		 float _mass, float _charge, float _rest_length, float _kspring);

	void fwdStep(Vector& force, float timestep);
};

//=============================================================================
class Swarm 
{						// a simple particle-system class
public:
	Blob *planenow[11]; 
						// ptr to the ten blobs in their current state.
	Blob *pOrigin;		// ptr to the fixed 'anchor' blob; every Blob is 
						// attached to this one in our example code.
	// double-buffering for Blobs;
	Blob *planenext[11]; // compute the next blob
	// (Why pointers? so that you could create/destroy blob objects, and so
	// that you can compute blobs 'Anext,Bnext' from current blob positions

	Blob *leafnow[11]; 
	Blob *lOrigin;		// ptr to the fixed 'anchor' blob; every Blob is 
	Blob *leafnext[11]; // compute the next blob

	Blob *treenow[11]; 
	Blob *tOrigin;		// ptr to the fixed 'anchor' blob; every Blob is 
	Blob *treenext[11]; // compute the next blob

	int solver_type;	// ==SOLV_IMPLICIT or SOLV_EXPLICIT
	float x_pos,y_pos;	// Current camera position (adjusted with arrow keys)
	int isPaused;		// ==1 wait; ==0 run.

	Vector down;		// direction of the force of gravity
	float k_gravity;	// gravitational constant: acceleration due to gravity
	float timestep;		// the timestep, ie the time-sampling rate for our system 
						//(dt, if you'd prefer)

public:
	Swarm(void);			// Default constructor.
	~Swarm(void);			// Default destructor.

	//********************* Here are our lovely solvers ***********************
	Vector getNetForceOn(Blob *pMe, Blob *pYou);	// find total force
	Vector leafMovement(Blob *pMe, Blob *pYou);	// find total force
	Vector treeMovement(Blob *pMe, Blob *pYou);	// find total force
	void blobSwap(void);			// Swap pA,pB blobs with pAnext, pBnext
	void explicitSolve(void);
	void implicitSolve(void);
};
