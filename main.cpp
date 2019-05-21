#include <iostream>
#include <GLFW/glfw3.h>
#include <cmath>
#include <unistd.h>
#include <conio.h>
#include <windows.h>
#include <cstdlib>
#include "common.h"
#include "controller.h"
#include "agent.h"
#include <fstream>


class Parabola: AbstractCurve
{
    double c;
public:
    Parabola(double coef): c(coef) {}
    double getValue(double x) { return c * x * x; }
    double getDerivative(double x) override { return 2 * c * x; }
//    double getCentrePosition_x(double x)
//    {
//        double A,B,C,D,x1,x2; // Объявляем переменные с плавающей точкой.
//        D = B * B - 4 * A * C;// Рассчитываем дискриминант
//        if(D > 0)
//    }
//    double getCentrePosition_y(double x)
};

class Polinom_4: AbstractCurve
{
    double c;
public:
    Polinom_4(double coef): c(coef) {}
    double getValue(double x) { return c * degree_4(x); }
    double getDerivative(double x) { return 4 * c * x * x * x; }
};


static void error_callback(int error, const char* description)
{
    fputs(description, stderr);
}

int epoch(int itr, Controller& controller, AbstractAgent* agent) {
    int reward = 0;
    for(int i = 0; i < itr; i++) {
        State state = controller.getState();
        action action = agent->getAction(state);
        controller.step(action);
        if(controller.isGameOver()) {
            reward +=  itr;
            agent->update(reward, state, controller.getState(), action);
            return reward;
        }
        reward--;
        agent->update(reward, state, controller.getState(), action);
    }
    return reward;
}

using namespace std;


int main() {
    auto curve = (AbstractCurve*) new Parabola(1.0 / 128);
//    auto curve_4 = (AbstractCurve*) new Polinom_4(1.0 / 20000);
    Controller controller(MASS, 0, curve);
//    Controller controller(MASS, 0, curve_4);
    auto agent = new AgentQ();


    // Main loop
    int epochCounter = 1;
    double gamma = 0.5;
    double sum = 0;
    int last = 0;
    double averegest[100];
    ofstream fout;
    fout.open("values.txt");
    while (epochCounter < 300000) {
        double r = epoch(10000, controller, (AbstractAgent*)agent);
        agent->setGamma(gamma);
        gamma = 1.0 / (1 + 0.1 * epochCounter);

        //        cout << r << endl;

        //calculating averege
        if(epochCounter < 101) {
            averegest[epochCounter - 1] = r;
            sum += r;
            cout << sum / epochCounter << endl;
            fout << sum / epochCounter << endl;

        }
        else {
            sum -= averegest[last];
            sum += r;
            averegest[last] = r;
            last = (last + 1) % 100;

            if(epochCounter % 100 == 0) {
                double averege = sum / 100;
                cout << averege << endl;
                fout << averege << endl;
            }
        }
        controller.reset();
        epochCounter++;
    }


    GLFWwindow *window;

    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", NULL, NULL);
    if (!window) {
        fprintf(stderr, "Failed to open GLFW window\n");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }


    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    // set up view
    glViewport(0, 0, WIDTH, HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // see https://www.opengl.org/sdk/docs/man2/xhtml/glOrtho.xml
    glOrtho(0.0, HEIGHT, 0.0, WIDTH, 0.0, 1.0); // this creates a canvas you can do 2D drawing on

    glfwSetKeyCallback(window, Controller::key_callback);

    fout.close();
    agent->setGamma(0);
    while (!glfwWindowShouldClose(window)) {
        for(int i = 0; i < 400; i++) {
            controller.draw(window);
            State state = controller.getState();
            action action = agent->getAction(state);
            controller.step(action);
            usleep(200);
        }
        controller.reset();
    }

    // Terminate GLFW
    glfwTerminate();

    // Exit program
    exit(EXIT_SUCCESS);
}
