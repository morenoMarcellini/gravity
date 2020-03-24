/* C++ include */
#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <fstream>
#include <vector>
#include <exception>

/* C include */
#include <unistd.h>
#include <getopt.h>
  
#include "main.hpp"
#include "planet.hpp"
#include "engine.hpp"
#include "open3D.hpp"
#include "utilities.hpp"
#include "integrator.hpp"

#define DEBUG 1

int main(int argc, char** argv){

	real *x, *y, *z, *vx, *vy, *vz, *mass, *radius;
	unsigned int steps = 100;
	int count = 0;
	int model[] = {0, 1, 2, 4, 8, 16, 32, -1};
	int mode = -1;
	
	/* For openGL */
	GLint window;
	GLsizei xSiz, ySiz;
	xSiz = 500;
	ySiz = 500;
	
	std::vector <std::string> src (8);
	
	Planet *p;
	
	if (argc < 2)
	{ // print usage and exit
		Utilities::usage();
		return -1;
	}
	
	/* read the input */
	int c;
//	int digit_optind = 0;
	
	while (1) 
	{
	//	int this_option_optind = optind ? optind : 1;
		int option_index = 0;
		static struct option long_options[] = 
		{
			{"add",     required_argument, 0,  0 },
			{"append",  no_argument,       0,  0 },
			{"delete",  required_argument, 0,  0 },
			{"verbose", no_argument,       0,  0 },
			{"create",  required_argument, 0, 'c'},
			{"file",    required_argument, 0, 'f'},
			{0,         0,                 0,  0 }
		};
	
		c = getopt_long(argc, argv, "f:n:m:",
					long_options, &option_index);
		if (c == -1)
			break;
	
		switch (c) {
			case 0:
				std::cout << "option %s" << long_options[option_index].name;
				if (optarg)
				{
					std::cout << " with arg %s" << optarg;
				}
				std::cout << "\n";
				break;
				/*
			case '0':
			case '1':
			case '2':
				if (digit_optind != 0 && digit_optind != this_option_optind)
				{
					std::cout<<"digits occur in two different argv-elements.\n";
				}
				digit_optind = this_option_optind;
				std::cout << "option " << c << std::endl;
				break;
				*/
			case 'f':
				src.at(0) = (std::string) optarg;
				break;
			case 'n':
				src.at(1) = (std::string) optarg;
				break;
			case 'm':
				src.at(2) = (std::string) optarg;
				break;
			case '?':
			case 'h':
				Utilities::usage();
				return -1;
				break;
			default:
				Utilities::usage();
				return -1;
		}
	}
	
	if (optind < argc) {
		std::cout<<"non-option ARGV-elements: ";
		while (optind < argc)
		{
			std::cout<<argv[optind++]<<" ";
		}
	std::cout<<"\n";
	}

	int k = 0;
	for (std::vector<std::string>::const_iterator t = options.begin(); t < options.end(); t++)
	{
		if (src[2].compare(*t) == 0)
		{
			mode = model[k];
			std::cout << *t << " " << mode << std::endl;
		}
		k++;
	}
	
	if (mode == -1)
	{
		Utilities::usage();
		return -1;
	}
//	exit(-1);
/*	//
	for (int i=1; i < argc; i++)
	{
		std::string arg = argv[i];
		if (arg == "-f")
		{
			i++;
			std::string q = argv[i];
			src.at(0) = q;
		}
	 if (arg == "-n")
		{
			i++;
			std::string q = argv[i];
			src.at(1)=q;
		}		
		if (arg == "-m")
		{
			i++;
			std::string q = argv[i];
			src.at(2)=q;
		}
		if (arg == "-h")
		{
			usage();
			return -1;
		}
		
	}
*/
	std::ifstream read_file(src.at(0));
	
	if (!read_file.is_open())
	{
		std::cout << "File " << src.at(0) << " not existing\n";
		return -1;
	}
	
	if (!src.at(1).empty())
	{
		try
		{
			steps = std::stoi(src.at(1));
		}
		catch (std::invalid_argument& e)
		{
			std::cerr << e.what();
			std::cerr << "Invalid value, bye\n";
			return -1;
		}
		
	} 
	
	/* allocate the memory for coors */
	x = new real [128];
	y = new real [128];
	z = new real [128];
	vx = new real [128];
	vy = new real [128];
	vz = new real [128];
	mass = new real [128];
	radius = new real [128];
	
	while (!read_file.eof())
	{
		read_file >> x[count] >> y[count] >> z[count] >>
		vx[count] >> vy[count] >> vz[count] >>
		mass[count] >> radius[count];
		count++;
	}
	count--;
	
	p = new Planet[count+1];
	if (p == (Planet*) NULL)
	return 1;
	
	for (int i=0; i < count; i++)
	{
		p[i].setIndex(i+1);
		p[i].SetX(x[i]);
		p[i].SetY(y[i]);
		p[i].SetZ(z[i]);
		p[i].SetVx(vx[i]);
		p[i].SetVy(vy[i]);
		p[i].SetVz(vz[i]);
		p[i].SetMass(mass[i]);
		p[i].SetRadius(radius[i]);
	}
	/* deallocate memory for coors */ 
	delete[] x;
	delete[] y;
	delete[] z;
	delete[] vx;
	delete[] vy;
	delete[] vz;
	delete[] mass;
	delete[] radius;
	/* create the engine*/
	Engine en(count, mode);
	//Integrator in(p);
	
	if (src.at(2) == "EuRi")
	{
		en.setVerlet(false);
	}
	
	/* open the 2 output files */
	std::ofstream velocity("velocity.out");	
	std::ofstream energy("energy.out");
	std::ofstream position("position.out");
	if (velocity.is_open())
	{	// I do not remember what is doing
		velocity.setf(std::ios::showpos);
	} 
	else
	{
		std::cerr << "cannot open the velocity file \n";
		return -1;
	}
	
	if (energy.is_open()){
		energy.setf(std::ios::showpos);
	}
	else
	{
		return -1;
	}
	
	if (position.is_open()){
		position.setf(std::ios::showpos);
	}
	else
	{
		return -1;
	}
	
	// glutMainLoop(); // This call controls everything.
	// glutMainLoopEvent();
	// xSiz = glutGet(GLUT_WINDOW_WIDTH);
	// ySiz = glutGet(GLUT_WINDOW_HEIGHT);
	
	/* Open the openGL window */
	window = open3D(argc, argv, xSiz, ySiz);
	gluLookAt(0.0, 0.0, BASE_Z, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_ACCUM_BUFFER_BIT);
	/* render the primitive */
	for (int i=0; i < count; i++)
	{
		renderPrimitive(&p[i]);
	}
	//glLoadIdentity();
	//glTranslatef(0.0, 0.0, -500.0);
	glFinish(); // wait the rendering is over
	glutSwapBuffers(); // swap buffer
	
	/* MAIN LOOP */
	/* std::cerr <<en.getVerlet() << std::endl;
	exit(-1);
	
	if (en.getVerlet() == true)
	{
		en.setVerlet(false);
		en.integrate(p);
		en.setVerlet(true);
	}
	*/
	for (unsigned int i=0; i < (steps+1); i++)
	{
		en.integrate(p);
#if 0		
		for (int j=0; j < count; j++)
		{
			std::cerr << p[j].x[XX] << " " << p[j].x[YY] << " " << p[j].x[ZZ] << " ";
		}
		std::cerr << std::endl;
#endif		
		for (int j=0; j < count; j++)
		{
			position << p[j].x[XX] << " " << p[j].x[YY] << " " << p[j].x[ZZ] << " ";
		}
		position << std::endl;
		
		if ((i % 100) == 0) 
		{
			std::cout << "\r" << "step " << i << std::flush;
		}
		
		if ((i % 100) == 0)
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_ACCUM_BUFFER_BIT);
			camera();
			for (int j=0; j < count; j++)
			{
				// render the spheres
				//renderPrimitive(&p[i], xSiz, ySiz);	
				//renderPrimitive(&p[i]);
			    renderForCamera(&p[j]);
			}
			glFinish();
			glutSwapBuffers();		
#if 0			
			real K = 0.0;
			real U = 0.0;
			
			for (int j=0; j < count; j++)
			{
				en.computeKinetic(&p[j]);
			}
			
			en.computePotential(p, count, count);
			
			for(int j=0; j < count; j++)
			{
				K += p[j].K;
				U += p[j].U;
			}
			energy << "Energy : " << K << " " << U << " " << K+U << "\n";
#endif			
			glutMainLoopEvent(); // execute and reset the queue event
			if (leave == true)
			{
				i = steps+2;
			}
		}
#if DEBUG
		for (int j=0; j < count; j++)
		{
			velocity << p[j].vel[XX] << " " << p[j].vel[YY] << " " << p[j].vel[ZZ] << " " ;
//			velocity << p[j].K << " " << p[j].U << "\n";
		}
		velocity << std::endl;
#endif
	}  
	velocity.close();
	/* ending */
	
	//  cout << "done \n";
	std::ofstream last("last.out");
	if (!last.is_open())
	return -1;
	
	for (int i=0; i< count; i++)
	{
		last << p[i].x[XX] << " " << p[i].x[YY] << " " << p[i].x[ZZ] << " ";
		last << p[i].vel[XX] << " " << p[i].vel[YY] << " " << p[i].vel[ZZ] << " ";
		last << p[i].mass << " " << p[i].radius << "\n";
	}
	
	last.close();
	delete[] p;
	energy.close();
	position.close();
	std::cout << "\n";
	glLoadIdentity();
	utilPrintEnd();
	/* so we can close neatly */
	glutMainLoop();
	return 0;
}
