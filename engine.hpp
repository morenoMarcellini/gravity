#ifndef _ENGINE_
#define _ENGINE_

#include <cmath>
#include <iostream>
#include <functional>

#include "main.hpp"
#include "planet.hpp"

class Engine
{
private:
	const real G = -1.0;
	real acc[4], vel[4];
	real world;
	real ***space;
	real t[NN]; // for swap
	int planets;
	bool verlet = true;
	bool first = false;
	bool second = false;
	
	/* we bind the integrator at constructor level */
	std::function<void(Planet*)> integrator;
	std::function<void(Planet*)> initIntegrator;
	
	/* Kinetic functions */
	real invertMass(real a);
	void computeDistance(Planet p1, Planet p2);
	void computeDistance(Planet *p1, Planet *p2);
	real internalDistance(real *x1, real *x2);
	void computeAccell(Planet *p1, Planet *p2, int row, int col);
	void resetAccell(Planet *p1);
	real fillSpace(Planet p1, Planet p2);
	void fillSpace(Planet *p1, Planet *p2, int row, int col);
	
	/* Euler-Richardson integration */
	void computeHalfStepEulerRichardson(Planet *p1);
	void refillSpace(Planet *p1, Planet *p2, int row, int col);
	void computeAccellMiddlePoint(Planet *p1, Planet *p2, int row, int col);
	void computeHalfStepAccEulerRichardson(Planet *p1);
	void integrateEulerRichardson(Planet *p1);
	//
	void computePosition(Planet *p1);
	void computeVelocityEulerRichardson(Planet *p1);
	void update(Planet p1);
	
	/* Simple Euler integration */
	void integrateEuler(Planet *p1);
	
	/* Euler middle point */
	void integrateEulerMidPoint(Planet *p1);
	
	/* Euler half-step */
	bool initIntegrateEulerHalfStep(Planet *p1);
	void integrateEulerHalfStep(Planet *p1);
	
	/* Simple Euler-Cromer integration */
	void integrateEulerCromer(Planet *p1);
	
	/* Verlet's integration integration */
	void initPositionVerletStepOne(Planet *p1);
	void initPositionVerletStepTwo(Planet *p1);
	void integrateVerlet(Planet *p1);
	void updateInitVerletStepOne(Planet *p1);
	void updateInitVerletStepTwo(Planet *p1);
	void updateVerlet(Planet *p1);
	
	/* Velocity Verlet's methods */
	bool initVelVerlet = false;
	void computeAceVelVerlet(Planet *p);
	void computeAccellVerlet(Planet *p);
	void integrateVelocityVerlet(Planet *p1);
	void updateAceVelVerlet(Planet *p1);
	void updateVelocityVerlet(Planet *p1);

public:
	Engine();
	Engine(const Engine& other);
	Engine(int counts);
	Engine(int counts, int mode);
	~Engine();
	
	real energy[4], kinetic[4], potential[4];
	real distance;
	void integrate(Planet *p1);
	int steps;
	real dt1, dt2;
	
	/* Methods */
	bool setVerlet(bool in);
	bool getVerlet() const;
	void computePotential(Planet *p1, int row, int col);
	void computeKinetic(Planet *p1);
};

#endif
