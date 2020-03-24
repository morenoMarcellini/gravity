#ifndef _MAIN_
#define _MAIN_

#include <string>
#include <vector>

typedef float real;
#define XX	0
#define YY	1
#define ZZ	2
#define NN	3

#define STEPS 50000

const std::vector<std::string> options = {"Euler", "EulerMiddlePoint", "EulerHalfStep", "EulerCromer", "EulerRichardson", "Verlet", "VelocityVerlet", "t_MAX_INT" };
#endif
