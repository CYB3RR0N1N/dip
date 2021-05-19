#include <iostream>
#include <GL/freeglut.h>

#include <unistd.h>
#include <vector>
#define _USE_MATH_DEFINES
#include <math.h>

using namespace std;

#include "types.h"

#define SCREEN_WIDTH glutGet(GLUT_WINDOW_WIDTH)
#define SCREEN_HEIGHT glutGet(GLUT_WINDOW_HEIGHT)

Point2i Rotate(Point2i p, int angle);

class Primitive
{
public:
    int x;
    int y;
    bool isFilled = false;
    virtual void Draw() = 0;
    virtual void Fill() {} ;
};

class Scene
{
public:
    void DrawScene();
    void AddPrimitive(Primitive *p);
    ~Scene();
//private:
    vector<Primitive *> primitives;
};

Scene::~Scene()
{
    for (auto i = primitives.end() ; i>=primitives.begin() ; --i )
    {
        delete (*i);
        primitives.pop_back();
    }
}
void Scene::AddPrimitive(Primitive *p)
{
    primitives.push_back(p);
}
void Scene::DrawScene()
{
    for (auto i = primitives.begin() ; i!=primitives.end() ; ++i )
        (*i)->Draw();
}

Scene mainScene;

class Polygon : public Primitive
{
public:
    Polygon(int amount=4);
    void Fill() override;
    void Draw() override;
    vector<Point2i> vertexes;
    void Move(int x, int y);
    void Scale(float x, float y);

private:
    Point2i start_point = {200,200};
    Point2i anchor_point;
    bool isOnBorder(int x, int y);
};
Polygon::Polygon(int amount)
{
    start_point = {200,200};
    vertexes.push_back(start_point);
    anchor_point = { start_point.x + 80 + rand() % 40 , start_point.y };
    Point2i tmp = start_point;
    int rest_angle = 360;
    for (int i = 0 ; i < amount-1; i++)
    {
        //Получаем угол
        int angle = 50 + (rand() % rest_angle) % 160;
        rest_angle -= angle;
        //Поворачиваем
        tmp = anchor_point + Rotate(tmp - anchor_point, angle);
        vertexes.push_back(tmp);
    }
}
void Polygon::Draw()
{
    glBegin(GL_LINE_LOOP);
        for (auto i = vertexes.begin() ; i != vertexes.end() ; ++i )
            glVertex2i((*i).x,(*i).y);
        glVertex2i((*vertexes.begin()).x ,(*vertexes.begin()).y);
    glEnd();
}
void Polygon::Move(int x, int y)
{
    for (auto i = vertexes.begin(); i != vertexes.end(); ++i)
    {
        (*i).x += x;
        (*i).y += y;
    }
}

void Polygon::Scale(float x, float y)
{
    for (auto i = vertexes.begin(); i != vertexes.end(); ++i)
    {
        (*i).x *= x;
        (*i).y *= y;
    }
}

Point2i Rotate(Point2i p, int angle)
{
    float angle_rad = (float)angle / 180 * M_PI;
    Point2i np;
    np.x = p.x * cos(angle_rad) - p.y * sin(angle_rad);
    np.y = +p.x * sin(angle_rad) + p.y * cos(angle_rad);
    return np;
}

bool CheckPixel(int x, int y)
{
    uint8_t data = 0;
    glReadPixels(x,y,1,1,GL_BLUE,GL_UNSIGNED_BYTE,&data);
    if (data)
        return false;
    else
        return true;
}
bool CheckPixel(Point2i p)
{
    uint8_t data = 0;
    glReadPixels(p.x,p.y,1,1,GL_BLUE,GL_UNSIGNED_BYTE,&data);
    if (data)
        return false;
    else
        return true;
}
void fillPixel(Point2i p)
{
    uint8_t color = 255;
    glRasterPos2i(p.x,p.y);
    glDrawPixels(1,1,GL_BLUE, GL_UNSIGNED_BYTE, &color);
}
void fillPixel(int x, int y)
{
    uint8_t color = 255;
    glRasterPos2i(x,y);
    glDrawPixels(1,1,GL_BLUE, GL_UNSIGNED_BYTE, &color);
}
void Polygon::Fill()
{   
    Point2i start_point = anchor_point;

    vector<Point2i> stack;
    stack.push_back(start_point);

    while(!stack.empty())
    {
        //Покрасить верхний пиксель
        Point2i tmp = *(stack.end() - 1);
        stack.pop_back();
        if (CheckPixel(tmp))
            fillPixel(tmp);
        //Добавить соседей
        if (CheckPixel(tmp.x + 1, tmp.y))
            stack.push_back({tmp.x + 1, tmp.y});
        if (CheckPixel(tmp.x, tmp.y + 1))
            stack.push_back({tmp.x, tmp.y + 1});
        if (CheckPixel(tmp.x - 1, tmp.y))
            stack.push_back({tmp.x - 1, tmp.y});
        if (CheckPixel(tmp.x, tmp.y - 1))
            stack.push_back({tmp.x , tmp.y - 1});
        //Нарисовать картинку
        glFlush();
    }
    cout << "Filled\n"; 
}

class Line: public Primitive
{
    Point2i begin;
    Point2i end;
public:
    Line(int x1, int y1, int x2, int y2);
    void Draw() override;
};

Line::Line(int x1, int y1, int x2, int y2)
{
    begin = {x1,y1};
    end = {x2,y2};
}
int Sign(int a)
{
    if (a == 0)
        return 0;
    else if (a > 0)
        return 1;
    else if (a < 0)
        return -1;
}
void Line::Draw()
{
    glColor3ub(rand() % 255, rand() % 255, rand() % 255);
    int x = begin.x;
    int y = begin.y;
    int dx = abs(end.x - begin.x);
    int dy = abs(end.y - begin.y);
    int s1 = Sign(end.x - begin.x);
    int s2 = Sign(end.y - begin.y);
    bool is_swap = false;
    if (dy > dx)
    {
        swap(dy,dx);
        is_swap = false;
    }
    int e = 2 * dy - dx;

    for (int i = 0 ; i < dx ; i++)
    {
        glBegin(GL_POINTS);
            glVertex2i(x,y);
        glEnd();

        while (e >=0)
        {
            if (is_swap)
                x += s1;
            else
                y += s2;
            e -= 2*dx;
        }

        if (is_swap)
            y += s2;
        else
            x += s1;

        e+=2*dy;
    }
    glFlush();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glLineWidth(1);
    glColor3ub(rand() % 255, rand() % 255, rand() % 255);

    mainScene.DrawScene();

    glFlush();
}

void mouse(int button, int state, int x, int y)
{
    if (button == GLUT_RIGHT_BUTTON)
    {
        //mainScene.primitives[0]->Fill(); 
    }
    else if ( button == GLUT_LEFT_BUTTON)
    {
        glutPostRedisplay();
        if (state == GLUT_DOWN)
        {
            static int i = 0;
            switch (i)
            {
            case 0:
                dynamic_cast<Polygon*>(mainScene.primitives[0])->Move(100,100);
                glutPostRedisplay();
                i++;
                break;
            case 1:
                dynamic_cast<Polygon*>(mainScene.primitives[0])->Scale(0.5,0.5);
                glutPostRedisplay();
                i++;
                break;
            default:
                i = 0;
                delete mainScene.primitives[0];
                mainScene.primitives[0] = new Polygon(4);
                glutPostRedisplay();
                break;
            }
        }
    }
    cout << "Mouse click " << x   << " " << y << "\n";
}

void resize(int w, int h)
{
    /*    
    glMatrixMode(GL_PROJECTION); // Если раскоментить то размеры фигур не будут 
    glLoadIdentity();            // зависеть от размеров окна
    glOrtho(-SCREEN_WIDTH/2, SCREEN_WIDTH/2, -SCREEN_HEIGHT/2, SCREEN_HEIGHT/2, 0, 1.0); 
    */
    glViewport(0,0,w,h); 
}

int main(int argc, char* argv[])
{
    srand(time(NULL));
    glutInit(&argc,argv); // Инициализация openGL
    glutInitWindowSize(600,600); // Размер окна
    glutInitWindowPosition(100,100); // Позиция окна
    glutInitDisplayMode(GLUT_RGB); // Режим отображения

    glutCreateWindow("OpenGl");
    glutDisplayFunc(display); // Установка callback-функций
    glutMouseFunc(mouse);
    glutReshapeFunc(resize);

    glMatrixMode(GL_PROJECTION); // Задание матрицы отображения
    glLoadIdentity();
    glOrtho(0, SCREEN_WIDTH, 0, SCREEN_HEIGHT, 0, 1.0);
 
    // for (int i = 0 ; i < 100 ; i++)
    // {
    //     Line *line = new Line(rand() % 500,rand() % 500,rand() % 500,rand() % 500);
    //     mainScene.AddPrimitive(line);
    // }
    Polygon *p = new Polygon(4);
    mainScene.AddPrimitive(p);

    glClear(GL_COLOR_BUFFER_BIT);

    glutMainLoop();

    return 0;
}