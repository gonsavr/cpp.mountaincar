#include <iostream>
#include <GLFW/glfw3.h>
#include <cmath>
#include <unistd.h>
#include <conio.h>
#include <windows.h>

#define HEIGHT 800
#define WIDTH 800
#define RADIUS 30
#define MASS 10
#define POWER 60
#define AUTOMOBILE 3

typedef int action;

double square(double z)
{
    return z * z;
}
void drawCircle(float, float, float, int);

class AbstractCurve
{
public:
    AbstractCurve() {}
    virtual ~AbstractCurve() {}

    virtual double getValue(double x) { return x; }
    virtual double getDerivative(double x) { return 1; }
};


class Controller {
    int m = 10000;
    const double g = 5;
    bool GameOver;
    double mu = 0.2;
    double velocity;
    uint64_t epoch;
    AbstractCurve* curve;
    double position = WIDTH / 2;
public:

    static double P;
    static double Fautomobile;
    Controller(int m, int P, AbstractCurve *c) {
        GameOver = false;
        this -> m = m;
//        this -> P = P;
        this -> epoch = 0;
        velocity = 0.1;
        curve = c;
    }

    ~Controller() {

    }

    bool isGameOver() { return GameOver; }

    void draw(GLFWwindow *window) {
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

        drawCircle(position, curve->getValue(position - WIDTH / 2.) + RADIUS, RADIUS, 150);
        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    void updateTime() {
        epoch++;
    }
    void updateVelocity() {
        double Ftr = g * mu / sqrt(1 + square(curve->getDerivative(position - WIDTH / 2.0)));
        double Fmg = g / sqrt(1 + square(1.0 / curve->getDerivative(position - WIDTH / 2.0)));

        if(curve->getDerivative(position - WIDTH / 2.) >= 0)
            Fmg = -Fmg;
        if(velocity >= 0)
            Ftr = -Ftr;
        velocity += Fautomobile + Ftr + Fmg;

    }

    void step(action a) {
        updateTime();
        updateVelocity();
        std::cout << P << std::endl;
        position += velocity / sqrt(1 + square(curve->getDerivative(position - WIDTH / 2.0))) ;
        if(position >= 680)
            GameOver = true;
    }

    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
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

double Controller::P = 0;
double Controller::Fautomobile = 0;


class Parabola: AbstractCurve
{
    double c;
public:
    Parabola(double coef): c(coef) {}
    double getValue(double x) { return c * x * x; }
    double getDerivative(double x) override { return 2 * c * x; }
};


static void error_callback(int error, const char* description)
{
    fputs(description, stderr);
}

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


using namespace std;


int main() {
    auto curve = (AbstractCurve*) new Parabola(1.0 / 128);
    Controller controller(MASS, 0, curve);
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
    // Main loop
    while (!glfwWindowShouldClose(window) && !controller.isGameOver() ) {
        // Draw gears
        controller.draw(window);
        controller.step(0);
        usleep(200);

    }

    // Terminate GLFW
    glfwTerminate();

    // Exit program
    exit(EXIT_SUCCESS);
}
