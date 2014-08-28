#ifndef SIM_H
#define SIM_H

#include <utility>
typedef std::pair<double, double> SimPoint;
std::vector<SimPoint> controlResponse(int iterations, double setpoint, double kp, double ki, double kd);

#define ARRAY_SIZE(a) ((int)(sizeof(a)/sizeof(a[0])))

#endif // SIM_H
