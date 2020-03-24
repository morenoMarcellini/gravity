#include <cassert>
#include "planet.hpp"

/* Constructors */ 

Planet::Planet()
{
	for (int i=0; i <= NN; i++)
	{
		x[i] = 0.0;
		vel[i] = 0.0;
		acc[i] = 0.0;
	}
	mass = 0.0;
	radius = 0.0;
}

/* Destructor */

Planet::~Planet()
{
}

/* Public */
void Planet::setIndex(int i)
{	
	index = i;
}

int Planet::readIndex() const
{
	return index;
}

void Planet::SetX(real a)
{
  x[XX] = a;
}

void Planet::SetY(real a)
{
  x[YY] = a;
}

void Planet::SetZ(real a)
{
  x[ZZ] = a;
}

void Planet::SetVx(real a)
{
  vel[XX] = a;
}

void Planet::SetVy(real a)
{
  vel[YY] = a;
}

void Planet::SetVz(real a)
{
  vel[ZZ] = a;
}
/*
void Planet::SetAx(real a)
{
  acc[XX] = a;
}

void Planet::SetAy(real a)
{
  acc[YY] = a;
}

void Planet::SetAz(real a)
{
  acc[ZZ] = a;
}
*/
void Planet::SetMass(real a)
{
  mass = a;
}

void Planet::SetRadius(real a)
{
  radius = a;
}

/* move all update to engine */
void Planet::update()
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
*/
	x[XX] = u_x[XX];
	x[YY] = u_x[YY];
	x[ZZ] = u_x[ZZ];
	x[NN] = u_x[NN];

	acc[XX] = 0.0;
	acc[YY] = 0.0;
	acc[ZZ] = 0.0;
	acc[NN] = 0.0;

	return;
}

void Planet::update(bool state)
{
/*
	vel[XX] = u_vel[XX];
	vel[YY] = u_vel[YY];
	vel[ZZ] = u_vel[ZZ];
	vel[NN] = u_vel[NN];
*/
	x_v[XX] = x[XX];
	x_v[YY] = x[YY];
	x_v[ZZ] = x[ZZ];
	x_v[NN] = x[NN];

	x[XX] = u_x[XX];
	x[YY] = u_x[YY];
	x[ZZ] = u_x[ZZ];
	x[NN] = u_x[NN];

	acc[XX] = 0.0;
	acc[YY] = 0.0;
	acc[ZZ] = 0.0;
	acc[NN] = 0.0;

	return;
}
