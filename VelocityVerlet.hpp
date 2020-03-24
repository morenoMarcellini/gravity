#ifndef _VELOCITY_VERLET_
#define _VELOCITY_VERLET_

//#include <cmath>
//#include <iostream>

#include "main.hpp"
#include "planet.hpp"

class VelocityVerlet
{
	private:
	
	public:
		/* Velocity Verlet's methods */
		bool precomputeAcceleration(Planet *p);
		void computeAccellVerlet(Planet *p);
		void computVelocityVerletPos(Planet *p1);
		void updateAccelVerlet(Planet *p1);
		void updateVelocityVerlet(Planet *p1);

}
