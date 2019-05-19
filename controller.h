//
// Created by lenzer on 19.05.2019.
//

#ifndef UNTITLED_CONTROLLER_H
#define UNTITLED_CONTROLLER_H

#include <GLFW/glfw3.h>


void drawCircle(float, float, float, int);

class Controller {
    int m = 10000;
    const double g = 5;
    bool GameOver;
    double mu = 0.3;
    double velocity;
    uint64_t epoch;
    AbstractCurve* curve;
    double position = WIDTH / 2;
public:
    struct State getState() {
        struct State state;
        state.velocity = velocity;
        state.position = position;
        return state;
    }

    static double P;
    static double Fautomobile;
    Controller(int m, int P, AbstractCurve *c);

    ~Controller() {}

    bool isGameOver() { return GameOver; }

    void draw(GLFWwindow *window);

    void updateTime() {
        epoch++;
    }
    void updateVelocity();

    void updateVelocity(action a);

    void step(action a);

    void reset();

    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GL_TRUE);

        if(key == GLFW_KEY_A && action == GLFW_PRESS)
            Fautomobile = - AUTOMOBILE;
        if(key == GLFW_KEY_D && action == GLFW_PRESS)
            Fautomobile = AUTOMOBILE;
        if((key == GLFW_KEY_A || key == GLFW_KEY_D) && action == GLFW_RELEASE)
            Fautomobile = 0;
    }
};


#endif //UNTITLED_CONTROLLER_H
