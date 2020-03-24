#ifndef _PLANET_
#define _PLANET_

#include "main.hpp"

class Planet
{
 private:
  int index = -1;
  
 public:
  real x[4], u_x[4], x_v[4];
  real vel[4], u_vel[4];
  real acc[4], u_acc[4];
  real mass;
  real radius;
  real K, U;

  Planet();
  ~Planet();
  void setIndex(int i); 
  int  readIndex(void) const;
  void SetX(real a);
  void SetY(real a);
  void SetZ(real a);
  void SetVx(real a);
  void SetVy(real a);
  void SetVz(real a);
  /*
  void SetAx(real a);
  void SetAy(real a);
  void SetAz(real a);
  */
  void SetMass(real a);
  void SetRadius(real a);
  void update(void);
  void update(bool state);
  
};

#endif
