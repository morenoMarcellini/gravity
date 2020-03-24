#include <sys/time.h>
#include <unistd.h>
#include <iostream>

class Utilities 
{
	public:
		static float current_time(void);
		static float renderPerSecond(void);
		static void usage(void);
};
