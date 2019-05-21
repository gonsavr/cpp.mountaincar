//
// Created by lenzer on 19.05.2019.
//

#include <math.h>
#include <iostream>
#include "common.h"
#include "controller.h"

double Controller::P = 0;
double Controller::Fautomobile = 0;


void drawCircle(float cx, float cy, float r, int num_segments) {
    glBegin(GL_LINE_LOOP);
    for (int ii = 0; ii < num_segments; ii++)   {
        float theta = 2.0f * 3.1415926f * float(ii) / float(num_segments);//get the current angle
        float x = r * cosf(theta);//calculate the x component
        float y = r * sinf(theta);//calculate the y component
        glVertex2f(x + cx, y + cy);//output vertex
    }
    glEnd();
}



Controller::Controller(int m, int P, AbstractCurve *c) {
    GameOver = false;
    this->m = m;
    this->epoch = 0;
    velocity = 0;
    curve = c;
    position = WIDTH / 4;
}

void Controller::draw(GLFWwindow *window) {
    glClearColor ( 1.0f, 1.0f, 1.0f, 1.0f );
    glClear (GL_COLOR_BUFFER_BIT);
    glPointSize(10);
    glLineWidth(2.5);
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_LINE_STRIP);
    for(int i = WIDTH / 2 - 0.9 * WIDTH / 2; i < WIDTH / 2 + 0.9 * WIDTH / 2; i++)
    {
        glVertex2d( i, ((i - WIDTH / 2.) * (i - WIDTH /  2.)) / 128 );
    }
    glEnd();
//        for(int i = WIDTH / 2 - 0.9 * WIDTH / 2; i < WIDTH / 2 + 0.9 * WIDTH / 2; i++)
//        {
//            glVertex2d( i, (degree_4(i - WIDTH / 2.)) / 20000 );
//        }
//        glEnd();

    drawCircle(curve->getCentrePosition_x(position - WIDTH / 2), curve->getValue(position - WIDTH / 2.) + RADIUS, RADIUS, 150);
    std::cout << curve->getCentrePosition_x(position - WIDTH / 2) << std::endl;

    // Swap buffers
    glfwSwapBuffers(window);
    glfwPollEvents();
}

void Controller::updateVelocity() {
    double Ftr = g * mu / sqrt(1 + square(curve->getDerivative(position - WIDTH / 2.0)));
//        double Ftr = 0;
    double Fmg = g / sqrt(1 + square(1.0 / curve->getDerivative(position - WIDTH / 2.0)));
//        double Fmg = 0;
    if(position - WIDTH / 2. >= 0)
        Fmg = -Fmg;
    if(velocity >= 0)
        Ftr = -Ftr;
//        if(position - WIDTH / 2. < 0.1)
//            Fmg = 0;
//        if(position - WIDTH / 2. < 0.1)
//            Ftr = 0;
    velocity += Fautomobile + Ftr + Fmg;
}

void Controller::updateVelocity(action a) {
    double Ftr = g * mu / sqrt(1 + square(curve->getDerivative(position - WIDTH / 2.0)));
//        double Ftr = 0;
    double Fmg = g / sqrt(1 + square(1.0 / curve->getDerivative(position - WIDTH / 2.0)));
//        double Fmg = 0;
    if(position - WIDTH / 2. >= 0)
        Fmg = -Fmg;
    if(velocity >= 0)
        Ftr = -Ftr;
//        if(position - WIDTH / 2. < 0.1)
//            Fmg = 0;
//        if(position - WIDTH / 2. < 0.1)
//            Ftr = 0;
    if(a == LEFT) {
        Fautomobile = -AUTOMOBILE;
    }

    if(a == RIGHT) {
        Fautomobile = AUTOMOBILE;
    }

    if(a == STAY) {
        Fautomobile = 0;
    }
    velocity += Fautomobile + Ftr + Fmg;
}

void Controller::step(action a) {
    updateTime();
//        updateVelocity();
    updateVelocity(a);
    //       std::cout << P << std::endl;
    position += velocity / sqrt(1 + square(curve->getDerivative(position - WIDTH / 2.0))) ;
    if(position >= WINPOS)
        GameOver = true;
}

void Controller::reset() {
    GameOver = false;
    position = WIDTH / 4;
    velocity = 0;

}