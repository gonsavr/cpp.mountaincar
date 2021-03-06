//
// Created by lenzer on 19.05.2019.
//

#ifndef UNTITLED_COMMON_H
#define UNTITLED_COMMON_H


#define EPOCHCOUNTER 300000
#define ITR 300
#define WINPOS 650
#define ITR 400
#define WINPOS 700
#define HEIGHT 800
#define WIDTH 800
#define RADIUS 10
#define MASS 10
#define POWER 60
#define AUTOMOBILE 3
#define  REWARD -1
#define GRANULARITY 1000
#define VEL_GRANUALITY 3
#define MAX_VLOCITY 100
#define DEVIDE 10


enum action{ STAY, LEFT, RIGHT };


struct State {
    double velocity;
    double position;
};


class AbstractCurve
{
public:
    AbstractCurve() {}
    virtual ~AbstractCurve() {}

    virtual double getValue(double x) { return x; }
    virtual double getDerivative(double x) { return 1; }
    virtual double getCentrePosition_x(double x) { return x; }
    virtual double getCentrePosition_y(double x) { return getValue(x) + RADIUS; }
};

inline double square(double z) { return z * z; }
inline double degree_4(double z) { return z * z * z * z; }

#endif //UNTITLED_COMMON_H
