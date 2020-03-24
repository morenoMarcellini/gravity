#include "integrator.hpp"

#include <iostream>
#include <cmath>

Integrator::Integrator(Planet *p)
{
	while (p->readIndex() > 0){
		count = (count > p->readIndex()) ? count : p->readIndex();
		p++;
	}
	return;
}

Integrator::~Integrator(){}

void Integrator::update(Planet *p)
{
/*
	vel[XX] = u_vel[XX];
	vel[YY] = u_vel[YY];
	vel[ZZ] = u_vel[ZZ];
	vel[NN] = u_vel[NN];

	x_v[XX] = x[XX];
	x_v[YY] = x[YY];
	x_v[ZZ] = x[ZZ];
	x_v[NN] = x[NN];

	x[XX] = u_x[XX];
	x[YY] = u_x[YY];
	x[ZZ] = u_x[ZZ];
	x[NN] = u_x[NN];
*/
	for (int i=0; i < count; i++){
		p[i].acc[XX] = 0.0;
		p[i].acc[YY] = 0.0;
		p[i].acc[ZZ] = 0.0;
		p[i].acc[NN] = 0.0;
	}
	
	return;
}

void Integrator::update(Planet *p, bool state)
{

	for (int i=0; i < count; i++){
		p[i].vel[XX] = p[i].u_vel[XX];
		p[i].vel[YY] = p[i].u_vel[YY];
		p[i].vel[ZZ] = p[i].u_vel[ZZ];
		p[i].vel[NN] = p[i].u_vel[NN];
	
		p[i].x_v[XX] = p[i].x[XX];
		p[i].x_v[YY] = p[i].x[YY];
		p[i].x_v[ZZ] = p[i].x[ZZ];
		p[i].x_v[NN] = p[i].x[NN];
	
		p[i].x[XX] = p[i].u_x[XX];
		p[i].x[YY] = p[i].u_x[YY];
		p[i].x[ZZ] = p[i].u_x[ZZ];
		p[i].x[NN] = p[i].u_x[NN];
	
		p[i].acc[XX] = 0.0;
		p[i].acc[YY] = 0.0;
		p[i].acc[ZZ] = 0.0;
		p[i].acc[NN] = 0.0;
	}

	return;
}

