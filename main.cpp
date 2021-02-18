#include <iostream>
#include <GL/glut.h>

#include <vector>

using namespace std;

#include "types.h"

float frand()
{
    return static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
}

class Polygon
{
public:
    explicit Polygon(int amount=4);

    void StackFill(Color3 color);
    void Draw();

    vector<Point2f> vertexes;
};

Polygon::Polygon(int amount)
{
    Point2f point;
    point.x = 0;
    point.y = 0;
    vertexes.push_back(point);

    for (int i = 1 ; i < amount ; i++)
    {
        point.x = static_cast<float>(vertexes[i-1].x + rand() % 50) / 100.f;
        point.y = static_cast<float>(vertexes[i-1].y + rand() % 50) / 100.f;
        vertexes.push_back(point); 
    }

    //Make it "good"
    // Need to check out an area where dot wanted to generate
}

void Polygon::Draw()
{
    glBegin(GL_LINE_LOOP);
    for (auto i = vertexes.begin() ; i != vertexes.end() ; ++i )
    {
            glVertex2f((*i).x ,(*i).y);
    }
            glVertex2f((*vertexes.begin()).x ,(*vertexes.begin()).y);
    glEnd();
}

void display()
{
    glClearColor(1.f,1.f,1.f,1.f);
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(0.0f, 0.0f, 1.0f);
    glLineWidth(1);

    glLineWidth(2);
    glColor3f(0.0f, 0.0f, 1.0f);
    Polygon polygon;
    polygon.Draw();

    glFlush();
}

int main(int argc, char* argv[])
{
    glutInit(&argc,argv);
    glutInitWindowSize(1200,1200);
    glutInitWindowPosition(100,100);

    glutInitDisplayMode(GLUT_RGB);
    //glutDisplayFunc(display);
    //glutIdleFunc(display);
    glutCreateWindow("OpenGl");
    display();
    glutMainLoop();

    return 0;
}