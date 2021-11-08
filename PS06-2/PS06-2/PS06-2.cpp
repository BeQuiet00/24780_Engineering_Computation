using namespace std;
#include <iostream>
#include <cstdio>
#include <chrono>
#include <thread>

#include "fssimplewindow.h"

const double PI = 3.14159;
const double neu_len = 3.0, spring_k = 1.0, damp_coe = 0.5;

class Position {

public:

    double x, y;

    int ScnX() {
        return int((x * 100) + 100);
    }

    int ScnY() {
        return int(-(y * 100) + 300);
    }

};

class Shape {

public:

    int shape_type;    // 0: line, 1: triangle, 2: rectangle, 3: circle
    int fill;          // 0: hollow, 1: filled
    Position p[5];
    double rad;           // for circle radius
    int r, g, b;

    void Draw() {
        if (shape_type == 0) {
            glColor3ub(r, g, b);
            glBegin(GL_LINES);

            // printf("Line from %d, %d to %d, %d\n", p[0].ScnX(), p[0].ScnY(), p[1].ScnX(), p[1].ScnY());

            glVertex2i(p[0].ScnX(), p[0].ScnY());
            glVertex2i(p[1].ScnX(), p[1].ScnY());
            glEnd();
        }

        if (shape_type == 1) {
            glColor3ub(r, g, b);
            if (0 != fill) {
                glBegin(GL_TRIANGLES);
            }
            else {
                glBegin(GL_LINE_LOOP);
            }
            for (int i = 0; i < 3; i++) {
                glVertex2i(p[i].ScnX(), p[i].ScnY());
            }
            glEnd();
        }

        if (shape_type == 2) {
            glColor3ub(r, g, b);
            if (fill) {
                glBegin(GL_QUADS);
            }
            else {
                glBegin(GL_LINE_LOOP);
            }

            // printf("Rectangle from %d, %d to %d, %d\n", p[0].ScnX(), p[0].ScnY(), p[1].ScnX(), p[1].ScnY());

            glVertex2i(p[0].ScnX(), p[0].ScnY());
            glVertex2i(p[1].ScnX(), p[0].ScnY());
            glVertex2i(p[1].ScnX(), p[1].ScnY());
            glVertex2i(p[0].ScnX(), p[1].ScnY());
            glEnd();
        }

        if (shape_type == 3) {
            glColor3ub(r, g, b);
            if (0 != fill) {
                glBegin(GL_POLYGON);
            }
            else {
                glBegin(GL_LINE_LOOP);
            }
            for (int i = 0; i < 64; i++) {
                double angle = (double)i * 2 * PI / 64.0;
                double px = p[0].ScnX() + cos(angle) * rad;
                double py = p[0].ScnY() + sin(angle) * rad;
                glVertex2d(px, py);
            }
            glEnd();
        }
    }
};

Shape W[13];

void DrawWall() {    
    for (int i = 0; i <= 12; i++) {
        W[i].r = 255; W[i].g = 0; W[i].b = 0;
        W[i].shape_type = 0;
    }

    W[0].p[0].x = 0; W[0].p[0].y = -3;
    W[0].p[1].x = 0; W[0].p[1].y = 3;
    W[0].Draw();

    for (int i = 1; i <= 12; i++) {
        W[i].p[0].x = 0; W[i].p[0].y = 3 - (i - 1) * .5;
        W[i].p[1].x = -.5; W[i].p[1].y = 3 - (i - 1) * .5 - .5;
        W[i].Draw();
    }
}

Shape S[9];

void DrawSpring(double L, double h) {
    for (int i = 0; i < 9; i++) {
        S[i].r = (int(abs(L - neu_len) * 255) <= 255) ? int(abs(L - neu_len) * 255) : 255;
        S[i].g = 0; S[i].b = 0;
        S[i].shape_type = 0;
    }

    S[0].p[0].x = 0; S[0].p[0].y = 0;
    S[0].p[1].x = 1.0 * L / 3; S[0].p[1].y = 0;
    S[0].Draw();

    S[8].p[0].x = 2.0 * L / 3; S[8].p[0].y = 0;
    S[8].p[1].x = 3.0 * L / 3; S[8].p[1].y = 0;
    S[8].Draw();

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
        S[i].p[0].x = start_x; S[i].p[0].y = start_y;
        S[i].p[1].x = end_x; S[i].p[1].y = end_y;
        S[i].Draw();
    }
}

Shape M;

void DrawMass(double L, double h) {    
    M.r = 0; M.g = 0; M.b = 0;
    M.shape_type = 2;

    M.p[0].x = L; M.p[0].y = h / 2;
    M.p[1].x = L + h; M.p[1].y = -h / 2;

    M.fill = 1;

    M.Draw();
}

double L, h, mass = 1, v = 0;
double cur_force;

int main() {
    printf("24-780 Engineering Computation Problem Set 2-2–Mass-Spring System\n");
    printf("Enter L and h:\n");
    scanf("%lf %lf", &L, &h);

	FsOpenWindow(0, 0, 800, 600, 1);

    auto pre_time = chrono::system_clock::now();

    for (;;) {

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		auto key = FsInkey();
		if (FSKEY_ESC == key)
		{
			break;
		}

        DrawWall();

        DrawSpring(L, h);

        DrawMass(L, h);

        auto cur_time = chrono::system_clock::now();
        chrono::duration<double> time_step = cur_time - pre_time;
        pre_time = cur_time;

        cur_force = -(L - neu_len) * spring_k - v * damp_coe;
        v = v + cur_force / mass * time_step.count();
        L = L + v * time_step.count();

        FsSwapBuffers();
        FsPollDevice();
        FsSleep(10);
    }

    return 0;
}
