using namespace std;
#include <iostream>
#include <cstdio>
#include <chrono>
#include <thread>

#include "fssimplewindow.h"

class Position {

public:
    
    double x, y;
    
    int Phys_x() {
        return int((x * 100) + 100);
    }

    int Phys_y() {
        return int(-(y * 100) + 300);
    }
};


class Shape {

public:

    int shape_type; // 0: line, 1: triangle, 2: rectangle, 3: circle
    int fill; // 0: hollow, 1: filled
    Position p[10];
    int rad; //for circle
    int r, g, b;

    void DrawLine() {
        FsPollDevice();
        glColor3ub(r, g, b);
        glBegin(GL_LINES);

        printf("Line from %d, %d to %d, %d\n", p[0].Phys_x(), p[0].Phys_y(), p[1].Phys_x(), p[1].Phys_y());

        glVertex2i(p[0].Phys_x(), p[0].Phys_y());
        glVertex2i(p[1].Phys_x(), p[1].Phys_y());
        glEnd();
        glFlush();
    }

    void DrawRect() {
        FsPollDevice();
        glColor3ub(r, g, b);
        if (fill) {
            glBegin(GL_QUADS);
        }
        else {
            glBegin(GL_LINE_LOOP);
        }

        printf("Rectangle from %d, %d to %d, %d\n", p[0].Phys_x(), p[0].Phys_y(), p[1].Phys_x(), p[1].Phys_y());

        glVertex2i(p[0].Phys_x(), p[0].Phys_y());
        glVertex2i(p[1].Phys_x(), p[0].Phys_y());
        glVertex2i(p[1].Phys_x(), p[1].Phys_y());
        glVertex2i(p[0].Phys_x(), p[1].Phys_y());
        glEnd();
        glFlush();
    }
};

void DrawWall() {
    Shape l[13];
    
    for (int i = 0; i <= 12; i++) {
        l[i].r = 255;
        l[i].g = 0;
        l[i].b = 0;
    }

    l[0].p[0].x = 0; l[0].p[0].y = -3;
    l[0].p[1].x = 0; l[0].p[1].y = 3;
    l[0].DrawLine();

    for (int i = 1; i <= 12; i++) {
        l[i].p[0].x = 0; l[i].p[0].y = 3 - (i - 1) * .5;
        l[i].p[1].x = -.5; l[i].p[1].y = 3 - (i - 1) * .5 - .5;
        l[i].DrawLine();
    }
}

void DrawSpring(double L, double h) {
    Shape l[9];

    for (int i = 0; i <= 9; i++) {
        l[i].r = 0;
        l[i].g = 0;
        l[i].b = 255;
    }

    l[0].p[0].x = 0; l[0].p[0].y = 0;
    l[0].p[1].x = 1.0 * L / 3; l[0].p[1].y = 0;
    l[0].DrawLine();

    l[8].p[0].x = 2.0 * L / 3; l[8].p[0].y = 0;
    l[8].p[1].x = 3.0 * L / 3; l[8].p[1].y = 0;
    l[8].DrawLine();

    double start_x = 1.0 * L / 3, end_x = start_x + 1.0 * L / 36;
    double start_y = 0, end_y = - h / 2;

    for (int i = 1; i <= 7; i++) {
        if (i != 1) {
            start_x = end_x;
            end_x = end_x + 1.0 * L / 18;
            start_y = end_y;
            end_y = -end_y;
        }
        if (i == 7) {
            end_x = end_x - 1.0 * L / 36;
            end_y = 0;
        }
        l[i].p[0].x = start_x; l[i].p[0].y = start_y;
        l[i].p[1].x = end_x; l[i].p[1].y = end_y;
        l[i].DrawLine();
    }

}

void DrawMass(double L, double h) {
    Shape M;
    
    M.r = 0;
    M.g = 0;
    M.b = 0;

    M.p[0].x = L; M.p[0].y = h / 2;
    M.p[1].x = L+h; M.p[1].y = -h / 2;

    M.fill = 1;

    M.DrawRect();
}

int main() {

    int L, h;

    printf("24-780 Engineering Computation Problem Set 2-2–Mass-Spring System\n");
    printf("Enter L and h:\n");
    scanf("%d %d", &L, &h);

	FsOpenWindow(0, 0, 800, 600, 0);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (;;) {

		auto key = FsInkey();
		if (FSKEY_ESC == key)
		{
			break;
		}

        DrawWall();

        DrawSpring(L, h);

        DrawMass(L, h);
    }

    return 0;
}
