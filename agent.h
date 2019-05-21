//
// Created by lenzer on 19.05.2019.
//

#ifndef UNTITLED_AGENT_H
#define UNTITLED_AGENT_H

#include "common.h"
#include "controller.h"
#include <cstdlib>


class AbstractAgent {
public:
    AbstractAgent() {}
    virtual action getAction(State state) { }
    virtual void update(int reward, State state, State newState, action a) {}
    virtual ~AbstractAgent() {};
};

class AgentRandom: AbstractAgent {
public:
    AgentRandom() {}
    action getAction(State state) {
        action a = action(rand() % 3);
        return a;
    }
    ~AgentRandom() {}

};

class AgentQ: AbstractAgent {
    double QSTAY[GRANULARITY][VEL_GRANUALITY];//changed VEL_GRANUALITY
    double QRIGHT[GRANULARITY][VEL_GRANUALITY];
    double QLEFT[GRANULARITY][VEL_GRANUALITY];
    double gamma = 0.99;
    double getMaxQ(State state);
public:
    AgentQ();
    ~AgentQ();

    //pos >= 0
    int getIndexPosition(double pos);
    int getIndexVelocity(double vel);

    action getAction(State state);
    void update(int reward, State state, State newState, action a);
    void setGamma(double gamma);
};

#endif //UNTITLED_AGENT_H
