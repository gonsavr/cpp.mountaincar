//
// Created by lenzer on 19.05.2019.
//

#include "agent.h"
#include "controller.h"

const double alpha = 0.001;

AgentQ::AgentQ() {
    for(int i = 0; i < GRANULARITY; i++) {
        for(int j = 0; j < VEL_GRANUALITY; j++) {
            QSTAY[i][j] = 1.0 / 3;
            QRIGHT[i][j] = 1.0 / 3;
            QLEFT[i][j] = 1.0 / 3;
        }
    }
}

//pos >= 0
int AgentQ::getIndexPosition(double pos) {
    return (int)pos;
}
int AgentQ::getIndexVelocity(double vel) {
    if(vel > 0)
        return 1;
    if(vel <= 0)
        return 0;
//    return 0;
}

action AgentQ::getAction(State state) {
    int indexPos = getIndexPosition(state.position);
    int indexVel = getIndexVelocity(state.velocity);
//    int indexVel = 0;
    int coin = rand() % 1000;

    if(gamma * 1000 > coin) {

        //return maxQ(state)
        double s = QSTAY[indexPos][indexVel];
        double r = QRIGHT[indexPos][indexVel];
        double l = QLEFT[indexPos][indexVel];
        if(s > l) {
            if(s > r)
                return STAY;
            return RIGHT;
        }
        return LEFT;
    }

    action a = action(rand() % 3);
    return a;
}

double AgentQ::getMaxQ(State state) {
    int indexPos = getIndexPosition(state.position);
    int indexVel = getIndexVelocity(state.velocity);
//    int indexVel = 0;
    double s = QSTAY[indexPos][indexVel];
    double r = QRIGHT[indexPos][indexVel];
    double l = QLEFT[indexPos][indexVel];
    if(s > l) {
        if(s > r)
            return s;
        return r;
    }
    return l;
}

void AgentQ::update(int reward, State state, State newState, action a) {
    int pos = getIndexPosition(state.position);
    int vel = getIndexVelocity(state.velocity);
    if(a == STAY) {
        double tmp = QSTAY[pos][vel];
        QSTAY[pos][vel] = (1 - alpha) * QSTAY[pos][vel] + alpha * (reward + gamma * getMaxQ(newState));
        double delta = QSTAY[pos][vel] - tmp;
//        QLEFT[pos][vel] -= delta / 2;
//        QRIGHT[pos][vel] -= delta / 2;
    }
    if(a == LEFT) {
        double tmp = QLEFT[pos][vel];
        QLEFT[pos][vel] = (1 - alpha) * QLEFT[pos][vel] + alpha * (reward + gamma * getMaxQ(newState));
        double delta = QLEFT[pos][vel] - tmp;
//        QSTAY[pos][vel] -= delta / 2;
//        QRIGHT[pos][vel] -= delta / 2;
    }
    if(a == RIGHT) {
        double tmp = QRIGHT[pos][vel];
        QRIGHT[pos][vel] = (1 - alpha) * QRIGHT[pos][vel] + alpha * (reward + gamma * getMaxQ(newState));
        double delta = QRIGHT[pos][vel] - tmp;
//        QSTAY[pos][vel] -= delta / 2;
//        QLEFT[pos][vel] -= delta / 2;
    }
}

void AgentQ::setGamma(double gamma) {
    this->gamma = gamma;
}
AgentQ::~AgentQ() {}