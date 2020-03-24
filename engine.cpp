#include "engine.hpp"
#include <iostream>
#include <iomanip>


/* Specialized constructor */

Engine::Engine(){};

Engine::Engine(const Engine& other){}

Engine::Engine(int counts) 
{

	world = 99.99;
	dt1 = 0.005;
	dt2 = dt1*dt1;
	planets = counts;
	
	space = new real** [counts];
	distance = 0.0 ; /* it is the square of distance between p1 and p2*/
	
	for (int i=0; i < counts; i++)
	{
		space[i] = new real* [counts];
	}
	
	for (int i=0; i < counts; i++)
	{
		for (int j=0; j < counts; j++)
		{
			space[i][j] = new real[4]();
		}
	}
	
}

Engine::Engine(int counts, int mode) 
{

	world = 99.99;
	dt1 = 0.005;
	dt2 = dt1*dt1;
	planets = counts;
	
	space = new real** [counts];
	distance = 0.0 ; /* it is the square of distance between p1 and p2*/
	
	for (int i=0; i < counts; i++)
	{
		space[i] = new real* [counts];
	}
	
	for (int i=0; i < counts; i++)
	{
		for (int j=0; j < counts; j++)
		{
			space[i][j] = new real[4]();
		}
	}

	switch (mode)
	{ 
		case 0:
			integrator = std::bind(&Engine::integrateEuler, this, std::placeholders::_1);
			break;
		case 1:
			integrator = std::bind(&Engine::integrateEulerMidPoint, this, std::placeholders::_1);
			break;
		case 2:
			integrator = std::bind(&Engine::integrateEulerHalfStep, this, std::placeholders::_1);	
			break;
		case 4:
			integrator = std::bind(&Engine::integrateEulerCromer, this, std::placeholders::_1);
			break;
		case 8:
			integrator = std::bind(&Engine::integrateEulerRichardson, this, std::placeholders::_1);
			break;
		case 16:
			integrator = std::bind(&Engine::integrateVerlet, this, std::placeholders::_1);
			break;
		case 32:
			integrator = std::bind(&Engine::integrateVelocityVerlet, this, std::placeholders::_1);
			break;
		default:
			std::cerr << "Wrong options. How did you come here?\n";
			exit(-1);
	}
}

/* Specialized destructor */

Engine::~Engine()
{
	for (int i=0; i < planets; i++)
	{
		for (int j=0; j < planets; j++)
	  {
		  delete[] space[i][j];
	  }
	  delete[] space[i];
	}
	delete[] space;
}

/* Private */
void Engine::fillSpace(Planet *p1, Planet *p2, int row, int col)
{
	real t;

	computeDistance(p1, p2);
	t = sqrt(distance);
	t = 1.0/t;
	space[row][col][XX] = (p1->x[XX] - p2->x[XX])*t;
	space[row][col][YY] = (p1->x[YY] - p2->x[YY])*t;
	space[row][col][ZZ] = (p1->x[ZZ] - p2->x[ZZ])*t;
	space[row][col][NN] = 1.0/distance; /* 1/(r*r) */
	
	space[col][row][XX] = -space[row][col][XX];
	space[col][row][YY] = -space[row][col][YY];
	space[col][row][ZZ] = -space[row][col][ZZ];
	space[col][row][NN] = +space[row][col][NN];
	
	return;
}

void Engine::computeAccell(Planet *p1, Planet *p2, int row, int col)
{
	p1->acc[XX] = p1->acc[XX]+G*p2->mass*space[row][col][XX]*space[row][col][NN];
	p1->acc[YY] = p1->acc[YY]+G*p2->mass*space[row][col][YY]*space[row][col][NN];
	p1->acc[ZZ] = p1->acc[ZZ]+G*p2->mass*space[row][col][ZZ]*space[row][col][NN];

	return;
}

void Engine::computeAccellMiddlePoint(Planet *p1, Planet *p2, int row, int col)
{
	p1->acc[XX] = p1->acc[XX]+G*p2->mass*space[row][col][XX]*space[row][col][NN];
	p1->acc[YY] = p1->acc[YY]+G*p2->mass*space[row][col][YY]*space[row][col][NN];
	p1->acc[ZZ] = p1->acc[ZZ]+G*p2->mass*space[row][col][ZZ]*space[row][col][NN];

	return;
}

void Engine::resetAccell(Planet *p1)
{
	p1->acc[XX] = 0.0;
	p1->acc[YY] = 0.0;
	p1->acc[ZZ] = 0.0;

	return;
}

/* INTERGATION ENGINES*/

/* Euler */
void Engine::integrateEuler(Planet *p1)
{
	real q[NN];
	/* contribution from acceleration */
	q[XX] = p1->acc[XX]*dt1;
	q[YY] = p1->acc[YY]*dt1;
	q[ZZ] = p1->acc[ZZ]*dt1;

	/* new position */	
	p1->x[XX] = p1->x[XX] + p1->vel[XX]*dt1;
	p1->x[YY] = p1->x[YY] + p1->vel[YY]*dt1;
	p1->x[ZZ] = p1->x[ZZ] + p1->vel[ZZ]*dt1;
	
	/* new velocity */	
	p1->vel[XX] = p1->vel[XX] + q[XX]; 
	p1->vel[YY] = p1->vel[YY] + q[YY]; 
	p1->vel[ZZ] = p1->vel[ZZ] + q[ZZ]; 	
	
	resetAccell(p1);
	
	return;
}

/* Euler-Cromer integration */
void Engine::integrateEulerCromer(Planet *p1)
{
	/* contribution from acceleration */
	t[XX] = p1->acc[XX]*dt1;
	t[YY] = p1->acc[YY]*dt1;
	t[ZZ] = p1->acc[ZZ]*dt1;

	/* new velocity */	
	p1->vel[XX] = p1->vel[XX] + t[XX]; 
	p1->vel[YY] = p1->vel[YY] + t[YY]; 
	p1->vel[ZZ] = p1->vel[ZZ] + t[ZZ]; 
	
	/* new position */	
	p1->x[XX] = p1->x[XX] + p1->vel[XX]*dt1;
	p1->x[YY] = p1->x[YY] + p1->vel[YY]*dt1;
	p1->x[ZZ] = p1->x[ZZ] + p1->vel[ZZ]*dt1;
	
	resetAccell(p1);
	
	return;
}

/* Euler midpoint */
void Engine::integrateEulerMidPoint(Planet *p1)
{
	/* contribution from acceleration */
	t[XX] = p1->acc[XX];
	t[YY] = p1->acc[YY];
	t[ZZ] = p1->acc[ZZ];

	/* new position */	
	p1->x[XX] = p1->x[XX] + p1->vel[XX]*dt1 + 0.5f*t[XX]*dt2;
	p1->x[YY] = p1->x[YY] + p1->vel[YY]*dt1 + 0.5f*t[YY]*dt2;
	p1->x[ZZ] = p1->x[ZZ] + p1->vel[ZZ]*dt1 + 0.5f*t[ZZ]*dt2;
	
	/* new velocity */	
	p1->vel[XX] = p1->vel[XX] + t[XX]*dt1; 
	p1->vel[YY] = p1->vel[YY] + t[YY]*dt1; 
	p1->vel[ZZ] = p1->vel[ZZ] + t[ZZ]*dt1; 	
	
	resetAccell(p1);

	return;
}

/* Euler half-step */
bool initEulerHalfStep = false;

bool Engine::initIntegrateEulerHalfStep(Planet *p1)
{
	/* new half-step velocity */	
	p1->vel[XX] = p1->vel[XX] + 0.5f*p1->acc[XX]*dt1; 
	p1->vel[YY] = p1->vel[YY] + 0.5f*p1->acc[YY]*dt1; 
	p1->vel[ZZ] = p1->vel[ZZ] + 0.5f*p1->acc[ZZ]*dt1;
	
	return true;
}

void Engine::integrateEulerHalfStep(Planet *p1)
{
	if (initEulerHalfStep == false)
	{
		initEulerHalfStep = initIntegrateEulerHalfStep(p1);
	}
	
	/* contribution from acceleration */
	t[XX] = p1->acc[XX];
	t[YY] = p1->acc[YY];
	t[ZZ] = p1->acc[ZZ];

	/* new velocity */	
	p1->vel[XX] = p1->vel[XX] + t[XX]*dt1; 
	p1->vel[YY] = p1->vel[YY] + t[YY]*dt1; 
	p1->vel[ZZ] = p1->vel[ZZ] + t[ZZ]*dt1;
	
	/* new position */	
	p1->x[XX] = p1->x[XX] + p1->vel[XX]*dt1;
	p1->x[YY] = p1->x[YY] + p1->vel[YY]*dt1;
	p1->x[ZZ] = p1->x[ZZ] + p1->vel[ZZ]*dt1;
	
	resetAccell(p1);

	return;
}

/* Euler-Richardson */
/* This method is a general on but specialized for Forces that
 * depend on the velocity. The following is the special case for the
 * current example */
void Engine::computeHalfStepEulerRichardson(Planet *p1)
{
	real q = 0.5*dt1;
	
	/* velocity at half-step to compute new position */
	p1->u_vel[XX] = p1->vel[XX] + p1->acc[XX]*q;
	p1->u_vel[YY] = p1->vel[YY] + p1->acc[YY]*q;
	p1->u_vel[ZZ] = p1->vel[ZZ] + p1->acc[ZZ]*q;
	
	p1->u_x[XX] = p1->x[XX] + p1->vel[XX]*q;
	p1->u_x[YY] = p1->x[YY] + p1->vel[YY]*q;
	p1->u_x[ZZ] = p1->x[ZZ] + p1->vel[ZZ]*q;

	return;
}

void Engine::refillSpace(Planet *p1, Planet *p2, int row, int col)
{
	real t, tx, ty, tz;
	
	tx = p1->u_x[XX] - p2->u_x[XX];
	ty = p1->u_x[YY] - p2->u_x[YY];
	tz = p1->u_x[ZZ] - p2->u_x[ZZ];

	distance = tx*tx + ty*ty + tz*tz;
	
	t = sqrt(distance);
	t = 1.0/t;
	space[row][col][XX] = tx*t;
	space[row][col][YY] = ty*t;
	space[row][col][ZZ] = tz*t;
	space[row][col][NN] = 1.0/distance; /* 1/(r*r) */
	
	space[col][row][XX] = -space[row][col][XX];
	space[col][row][YY] = -space[row][col][YY];
	space[col][row][ZZ] = -space[row][col][ZZ];
	space[col][row][NN] = +space[row][col][NN];
	
	return;
}

void Engine::computeHalfStepAccEulerRichardson(Planet *p1)
{
	/* Fill the new matrix space at half step  */
	for (int i=0; i < planets; i++)
	{
		for (int j=i+1; j < planets; j++)
		{
			refillSpace(&p1[i], &p1[j], i, j);
		}
	}
	
	for (int i=0; i < planets; i++)
	{
		resetAccell(&p1[i]);
	}
	/* Compute half step acelleration */
	for (int i=0; i < planets; i++)
	{
		for (int j=0; j < planets; j++)
		{
			computeAccellMiddlePoint(&p1[i], &p1[j], i, j);
		}
	}
	
	return;
}

void Engine::integrateEulerRichardson(Planet *p1)
{
	
	t[XX] = p1->acc[XX]*dt1;
	t[YY] = p1->acc[YY]*dt1;
	t[ZZ] = p1->acc[ZZ]*dt1;
	
	p1->vel[XX] = p1->vel[XX] + t[XX]; 
	p1->vel[YY] = p1->vel[YY] + t[YY]; 
	p1->vel[ZZ] = p1->vel[ZZ] + t[ZZ];
	
	p1->x[XX] = p1->x[XX] + p1->u_vel[XX]*dt1;
	p1->x[YY] = p1->x[YY] + p1->u_vel[YY]*dt1;
	p1->x[ZZ] = p1->x[ZZ] + p1->u_vel[ZZ]*dt1;
	
	return;
}

/* Verlet's methods */
/* We need to precompute the initial 2 steps */
void Engine::initPositionVerletStepOne(Planet *p1)
{
#if 0
	computeVelocityEulerRichardson(p1);
	computePosition(p1);
	updateInitVerletStepOne(p1);
#endif
	integrateEulerCromer(p1);
	
	return;
}

void Engine::updateInitVerletStepOne(Planet *p1)
{
#if 0
	for (int i=0; i < planets; i++)
	{
		p1->x_v[XX] = p1->u_x[XX];
		p1->x_v[YY] = p1->u_x[YY];
		p1->x_v[ZZ] = p1->u_x[ZZ];
		
		p1->x[XX] = p1->u_x[XX];
		p1->x[YY] = p1->u_x[YY];
		p1->x[ZZ] = p1->u_x[ZZ];
	}
#endif
	for (int i=0; i < planets; i++)
	{
		p1->x_v[XX] = p1->x[XX];
		p1->x_v[YY] = p1->x[YY];
		p1->x_v[ZZ] = p1->x[ZZ];
	}

	return;
}

void Engine::initPositionVerletStepTwo(Planet *p1)
{
#if 0
	computeVelocityEulerRichardson(p1);
	computePosition(p1);
	updateInitVerletStepTwo(p1);
#endif
	integrateEulerCromer(p1);
	
	return;
}

void Engine::updateVerlet(Planet *p1)
{
	p1->x_v[XX] = p1->x[XX];
	p1->x_v[YY] = p1->x[YY];
	p1->x_v[ZZ] = p1->x[ZZ];
	
	p1->x[XX] = p1->u_x[XX];
	p1->x[YY] = p1->u_x[YY];
	p1->x[ZZ] = p1->u_x[ZZ];
	
	return;
}

void Engine::integrateVerlet(Planet *p1)
{
	p1->u_x[XX] = 2.0f*p1->x[XX] - p1->x_v[XX] + p1->acc[XX]*dt2;
	p1->u_x[YY] = 2.0f*p1->x[YY] - p1->x_v[YY] + p1->acc[YY]*dt2;
	p1->u_x[ZZ] = 2.0f*p1->x[ZZ] - p1->x_v[ZZ] + p1->acc[ZZ]*dt2;

	return;
}

/* Velocity Verlet's methods */
void Engine::integrateVelocityVerlet(Planet *p1)
{
	p1->x[XX] = p1->x[XX] + p1->vel[XX]*dt1 + 0.5f*p1->acc[XX]*dt2;
	p1->x[YY] = p1->x[YY] + p1->vel[YY]*dt1 + 0.5f*p1->acc[YY]*dt2;
	p1->x[ZZ] = p1->x[ZZ] + p1->vel[ZZ]*dt1 + 0.5f*p1->acc[ZZ]*dt2;

	return;
}

void Engine::computeAceVelVerlet(Planet *p)
{
	/* Fill matrix space */
	for (int i=0; i < planets; i++)
	{
		for (int j=i+1; j < planets; j++)
		{
			fillSpace(&p[i], &p[j], i, j);
		}
	}
	
	/* Compute acc */
	for (int i=0; i < planets; i++)
	{
		for (int j=0; j < planets; j++)
		{
			computeAccell(&p[i], &p[j], i, j);
		}
	}
	
	return;
}

void Engine::updateAceVelVerlet(Planet *p1)
{
	p1->u_acc[XX] = p1->acc[XX];
	p1->u_acc[YY] = p1->acc[YY];
	p1->u_acc[ZZ] = p1->acc[ZZ];	
	
	resetAccell(p1);

	return;
}

void Engine::updateVelocityVerlet(Planet *p1)
{
	p1->vel[XX] = p1->vel[XX] + 0.5f*(p1->acc[XX] + p1->u_acc[XX])*dt1;
	p1->vel[YY] = p1->vel[YY] + 0.5f*(p1->acc[YY] + p1->u_acc[YY])*dt1;
	p1->vel[ZZ] = p1->vel[ZZ] + 0.5f*(p1->acc[ZZ] + p1->u_acc[ZZ])*dt1;

	return;
}

/* @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ */

/* Methods to compute acceleration */
real Engine::invertMass(real m)
{
  return 1.0/m;
}

void Engine::computeDistance(Planet *p1, Planet *p2)
{
	/* it is distance^2 */
	real tx, ty, tz;

	tx = p1->x[XX] - p2->x[XX];
	ty = p1->x[YY] - p2->x[YY];
	tz = p1->x[ZZ] - p2->x[ZZ];
	
	distance = tx*tx + ty*ty + tz*tz;
	
	return;	
}

/* Public */

void Engine::computePotential(Planet *p1, int row, int col)
{
		
	for (int i=0; i < row; i++)
	{
		potential[XX] = 0.0; 
		potential[YY] = 0.0; 
		potential[ZZ] = 0.0; 
		
		for (int j=i; j < col; j++)
		{
			real t = space[i][j][NN]; //sqrt(space[i][j][NN]);
			
			potential[XX] += p1[j].mass*space[i][j][XX]*t;
			potential[YY] += p1[j].mass*space[i][j][YY]*t;
			potential[ZZ] += p1[j].mass*space[i][j][ZZ]*t;
		}
		real t = potential[XX] + potential[YY] + potential[ZZ];
		potential[NN] = G*p1[i].mass*t;
		p1[i].U = 2.0*potential[NN];
	}

	return;
}

void Engine::computeKinetic(Planet *p1)
{
	real C = 0.5 * p1->mass;
	kinetic[XX] = p1->vel[XX] * p1->vel[XX];
	kinetic[YY] = p1->vel[YY] * p1->vel[YY];
	kinetic[ZZ] = p1->vel[ZZ] * p1->vel[ZZ];
	
	p1->K = kinetic[XX] + kinetic[YY] + kinetic[ZZ];
	p1->K *= C;
	
	return;
}

bool Engine::setVerlet(bool in)
{
	verlet = in;
	return verlet;
}

bool Engine::getVerlet(void) const
{
	return verlet;
}

/*
 *     MAIN INTEGRATION 
 */

void Engine::integrate(Planet *p)
{ 
#if 0
	/* Velocity Verlet */
	if (true){
		if (initVelVerlet == false)
		{
			initVelVerlet = true;
			computeAceVelVerlet(p);
		}
		
		for (int i=0; i < planets; i++)
		{
			integrateVelocityVerlet(&p[i]);
		}
		
		for (int i=0; i < planets; i++)
		{
			updateAceVelVerlet(&p[i]);
		}
		
		computeAceVelVerlet(p);
		
		for (int i=0; i < planets; i++)
		{
			updateVelocityVerlet(&p[i]);
		}
		
		return;
	}
#endif
/*  REGULAR */
	/* Fill matrix space */
	for (int i=0; i < planets; i++)
	{
		for (int j=i+1; j < planets; j++)
		{
			fillSpace(&p[i], &p[j], i, j);
		}
	}

	/* Compute acc */
	for (int i=0; i < planets; i++)
	{   
		for (int j=0; j < planets; j++)
		{   
			computeAccell(&p[i], &p[j], i, j);
		}
	}
#if 1
	if (true){
		for (int i=0; i < planets; i++)
		{
		//	integrateEuler(&p[i]);
		//	integrateEulerMidPoint(&p[i]);
		//	integrateEulerHalfStep(&p[i]);
		//	integrateEulerCromer(&p[i]);
			integrator(&p[i]);
		}
		return;
	}
#endif
/* VERLET */	
	/* Enter in Verlet if verlet = true */
	if (verlet)
	{
		if (first == false)
		{
			for (int i=0; i < planets; i++)
			{
				initPositionVerletStepOne(&p[i]);
			}
			for (int i=0; i < planets; i++)
			{
				updateInitVerletStepOne(&p[i]);
			}
			first = true;

			return;
		}
	
		if (second == false)
		{
			for (int i=0; i < planets; i++)
			{
				initPositionVerletStepTwo(&p[i]);
			}
			for (int i=0; i < planets; i++)
			{
				updateInitVerletStepTwo(&p[i]);
			}
			second = true;

			return;
		}
	
		for (int i=0; i < planets; i++)
		{
			integrateVerlet(&p[i]);
		}
    	
		for (int i=0; i < planets; i++)
		{
			updateVerlet(&p[i]);
			resetAccell(&p[i]);
		}

		return;
	}

	/* Regular Euler-Richardson */
	for (int i=0; i < planets; i++)
	{
		computeHalfStepEulerRichardson(&p[i]);
	}
	
	computeHalfStepAccEulerRichardson(p);
	
	for (int i=0; i < planets; i++)
	{
		integrateEulerRichardson(&p[i]);
	}
	
	for (int i=0; i < planets; i++)
	{
		resetAccell(&p[i]);
	}
	
	return;
}

/* @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ 
 * 
 *                     UNUSED
 * 
   @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ */
 
void Engine::update(Planet p1) // not used
{
	
	p1.x_v[XX] = p1.x[XX];
	p1.x_v[YY] = p1.x[YY];
	p1.x_v[ZZ] = p1.x[ZZ];
	p1.x_v[NN] = p1.x[NN];
	
	p1.x[XX] = p1.u_x[XX];
	p1.x[YY] = p1.u_x[YY];
	p1.x[ZZ] = p1.u_x[ZZ];
	p1.x[NN] = p1.u_x[NN];
		
	p1.vel[XX] = p1.u_vel[XX];
	p1.vel[YY] = p1.u_vel[YY];
	p1.vel[ZZ] = p1.u_vel[ZZ];
	p1.vel[NN] = p1.u_vel[NN];
	
#if 0
	//std::cerr << p1.u_vel[XX] << " " << p1.u_vel[YY] << " " << p1.u_vel[ZZ] << "\n";
#endif
	p1.u_vel[XX] = 0.0;
	p1.u_vel[YY] = 0.0;
	p1.u_vel[ZZ] = 0.0;
	p1.u_vel[NN] = 0.0;
	
	return;
}

void Engine::computeVelocityEulerRichardson(Planet *p1)
{
	/* half-step acceleration */
	t[XX] = 0.5*p1->acc[XX]*dt1;
	t[YY] = 0.5*p1->acc[YY]*dt1;
	t[ZZ] = 0.5*p1->acc[ZZ]*dt1;
	
	/* velocity at half-step to compute new position */
	p1->u_vel[XX] = p1->vel[XX] + t[XX]; //0.5*p1->acc[XX]*dt1;
	p1->u_vel[YY] = p1->vel[YY] + t[YY]; //0.5*p1->acc[YY]*dt1;
	p1->u_vel[ZZ] = p1->vel[ZZ] + t[ZZ]; //0.5*p1->acc[ZZ]*dt1;
	/*
	p1->vel[XX] = p1->vel[XX] + t[XX]; //0.5*p1->acc[XX]*dt1;
	p1->vel[YY] = p1->vel[YY] + t[YY]; //0.5*p1->acc[YY]*dt1;
	p1->vel[ZZ] = p1->vel[ZZ] + t[ZZ]; //0.5*p1->acc[ZZ]*dt1;
	*/
	return;
}

void Engine::computePosition(Planet *p1)
{
	p1->u_x[XX] = p1->x[XX] + p1->u_vel[XX]*dt1;
	p1->u_x[YY] = p1->x[YY] + p1->u_vel[YY]*dt1;
	p1->u_x[ZZ] = p1->x[ZZ] + p1->u_vel[ZZ]*dt1;
	
	return;
}

real Engine::fillSpace(Planet p1, Planet p2)
{
	computeDistance(p1, p2);
	return -1.0/distance;
}

real Engine::internalDistance(real *x1, real *x2)
{
	return (x1[XX] - x2[XX])*(x1[XX] - x2[XX])+
		(x1[YY] - x2[YY])*(x1[YY] - x2[YY])+
		(x1[ZZ] - x2[ZZ])*(x1[ZZ] - x2[ZZ]);
}

void Engine::computeDistance(Planet p1, Planet p2)
{
	/* it is distance^2 */
	distance = internalDistance(p1.x, p2.x);
	return;	
}

void Engine::computeAccellVerlet(Planet *p)
{
	/* Fill matrix space */
	for (int i=0; i < planets; i++)
	{
		for (int j=i+1; j < planets; j++)
		{
			fillSpace(&p[i], &p[j], i, j);
		}
	}
	
	/* Compute acc */
	for (int i=0; i < planets; i++)
	{
		for (int j=0; j < planets; j++)
		{
			computeAccell(&p[i], &p[j], i, j);
		}
	}

	return;
}

void Engine::updateInitVerletStepTwo(Planet *p1)
{
#if 0
	for (int i=0; i < planets; i++)
	{
		p1->x[XX] = p1->u_x[XX];
		p1->x[YY] = p1->u_x[YY];
		p1->x[ZZ] = p1->u_x[ZZ];
	}
#endif

	return;
}
