#ifndef _INTEGRATOR_
#define _INTEGRATOR_

#include "main.hpp"
#include "planet.hpp"

class Integrator{
	private:
	
	public:
		Integrator();
		Integrator(Planet *p);
		Integrator(Integrator &other);
		~Integrator();
		
		int count = -1;
		virtual void update(Planet *p);
		virtual void update(Planet *p, bool state);
};

#endif
