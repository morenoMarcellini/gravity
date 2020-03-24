#include "utilities.hpp"

float Utilities::current_time(void)
{
   struct timeval tv;
#ifdef __VMS
   (void) gettimeofday(&tv, NULL );
#else
   struct timezone tz;
   (void) gettimeofday(&tv, &tz);
#endif
   return (float) tv.tv_sec + tv.tv_usec * 1e-06;
}

//const std::vector<std::string> options = {"Euler", "EulerMiddlePoint", "EulerHalfStep", "EulerCromer", "EulerRichardson", "Verlet", "VelocityVerlet", "t_MAX_INT" };
void Utilities::usage(){
	
	std::cerr << "usage: gravity -f <input file> -n steps -m <mode>\n";
	std::cerr << "       -n and -m are optional\n";
	std::cerr << "       mode: Euler";
	std::cerr << "             EulerMiddlePoint";
	std::cerr << "             EulerHalfStep";
	std::cerr << "             EulerCromer";
	std::cerr << "             EulerRichardson";
	std::cerr << "             Verlet";
	std::cerr << "             VelocityVerlet";
/*
	std::cerr << "       mode: Euler"
	std::cerr << "       mode: Euler"
	std::cerr << "       mode: Euler"
	*/
	std::cerr << "       Euler integration is the default\n";
	return;
}


float Utilities::renderPerSecond(void)
{
	/*
   static int frames = 0;
   static double tRot0 = -1.0, tRate0 = -1.0;
   double dt, t = current_time();

   if (tRot0 < 0.0)
      tRot0 = t;
   dt = t - tRot0;
   tRot0 = t;

   frames++;
   
   if (tRate0 < 0.0)
      tRate0 = t;
   if (t - tRate0 >= 5.0) {
      GLfloat seconds = t - tRate0;
      GLfloat fps = frames / seconds;
      printf("%d frames in %3.1f seconds = %6.3f FPS\n", frames, seconds,
             fps);
      fflush(stdout);
      tRate0 = t;
      frames = 0;
   }
   */
   return -1.0;
}
