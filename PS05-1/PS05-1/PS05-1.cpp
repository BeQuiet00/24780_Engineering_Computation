using namespace std;
#include <iostream>
#include <cstdio>
#include <chrono>
#include <thread>
#include <cmath>
#include <ctime>

#include "fssimplewindow.h"

const double PI = 3.1415927;

class Position {

public:

    double x, y;

    int ScnX() {
        return (int)x * 10;
    }

    int ScnY() {
        return (int)1000 - y * 10;
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

Shape saber1, saber2;
double TarDeg = 30.0;

Shape obs[21], obs_c[21];
int obs_status[21]; // 1: Active 0: Destroyed
double obs_v[21];
int obs_num = 0, obs_max = 5;

Shape trj[800];
int trj_idx = 0;

void InitSaber() {
    saber1.p[0].x = 10 - 0.1; saber1.p[0].y = 5 + 1.5;
    saber1.p[1].x = 10 + 0.1; saber1.p[1].y = 5 - 1.5;
    saber1.r = 255; saber1.g = 0; saber1.b = 0;
    saber1.fill = 0;
    saber1.shape_type = 2;

    saber2.p[0].x = 50 - 0.1; saber2.p[0].y = 5 + 1.5;
    saber2.p[1].x = 50 + 0.1; saber2.p[1].y = 5 - 1.5;
    saber2.r = 0; saber2.g = 0; saber2.b = 255;
    saber2.fill = 0;
    saber2.shape_type = 2;
}

void ProcessSaber() {
    glEnable(GL_LINE_STIPPLE); glLineStipple(1, 0xcccc);
    saber1.Draw();
    saber2.Draw();
    glDisable(GL_LINE_STIPPLE);
}

void InitObs() {
    for (int i = 1; i <= obs_max; i++) {
        obs_status[i] = 1;

        obs[i].r = 0; obs[i].g = 0; obs[i].b = 0;
        int clr = rand() % 2;
        if (clr == 0) obs[i].r = 255;
        if (clr == 1) obs[i].b = 255;
        int w = 6;
        int h = 6;
        obs[i].p[0].x = rand() % (60 - w); obs[i].p[0].y = 100 - 80 / obs_max * (i - 1);
        obs[i].p[1].x = obs[i].p[0].x + w; obs[i].p[1].y = obs[i].p[0].y - h;
        obs[i].fill = 1;
        obs[i].shape_type = 2;

        obs_c[i].r = 255; obs_c[i].g = 255; obs_c[i].b = 255;
        obs_c[i].rad = 10;
        obs_c[i].p[0].x = obs[i].p[0].x + w / 2; obs_c[i].p[0].y = obs[i].p[0].y - h / 2;
        obs_c[i].fill = 1;
        obs_c[i].shape_type = 3;

        obs_v[i] = -5;
    }
    obs_num = obs_max;
}

void ProcessObs() {
    for (int i = 1; i <= obs_max; i++) {
        if (obs_status[i]) {
            obs[i].p[0].y += obs_v[i] * 0.025;
            obs[i].p[1].y += obs_v[i] * 0.025;
            obs_c[i].p[0].y += obs_v[i] * 0.025;
            obs_v[i] -= 0.025 * 0.5;
            obs[i].Draw();
            obs_c[i].Draw();
        }
    }
    for (int i = 1; i <= obs_max; i++) {
        if (obs_status[i]) {
            if (obs[i].p[0].y <= 0) {
                obs_status[i] = 0;
                obs_num--;
            }
        }
    }
    while (obs_num < obs_max) {
        for (int i = 1; i <= obs_max; i++) 
        if (obs_status[i] == 0){
            obs_status[i] = 1;
            obs[i].r = 0; obs[i].g = 0; obs[i].b = 0;
            int clr = rand() % 2;
            if (clr == 0) obs[i].r = 255;
            if (clr == 1) obs[i].b = 255;
            int w = 6;
            int h = 6;
            obs[i].p[0].x = rand() % (60 - w); obs[i].p[0].y = 100;
            obs[i].p[1].x = obs[i].p[0].x + w; obs[i].p[1].y = obs[i].p[0].y - h;
            obs[i].fill = 1;
            obs[i].shape_type = 2;

            obs_c[i].r = 255; obs_c[i].g = 255; obs_c[i].b = 255;
            obs_c[i].rad = 10;
            obs_c[i].p[0].x = obs[i].p[0].x + w / 2; obs_c[i].p[0].y = obs[i].p[0].y - h / 2;
            obs_c[i].fill = 1;
            obs_c[i].shape_type = 3;
            obs_v[i] = -5;
            obs_num++;
            break;
        }
    }
}

void HitCheck() {
    for (int i = 0; i <= obs_max; i++) {
        if (obs_status[i]) {
            if ((obs[i].p[0].x <= saber1.p[0].x) && (saber1.p[1].x <= obs[i].p[1].x)) {
                if (saber1.p[0].y >= obs[i].p[1].y) {
                    if (saber1.r == obs[i].r) {
                        printf("object destroyed!\n");
                        obs_status[i] = 0;
                        obs_num--;
                    }
                    else {
                        printf("Wrong color!\n");
                        obs_status[i] = 0;
                        obs_num--;
                    }
                }
            }
            else if ((obs[i].p[0].x <= saber2.p[0].x) && (saber2.p[1].x <= obs[i].p[1].x)) {
                if (saber2.p[0].y >= obs[i].p[1].y) {
                    if (saber2.r == obs[i].r) {
                        printf("object destroyed!\n");
                        obs_status[i] = 0;
                        obs_num--;
                    }
                    else {
                        printf("Wrong color!\n");
                        obs_status[i] = 0;
                        obs_num--;
                    }
                }
            }
        }
    }
}

int main() {

    printf("Welcome to Beat Saber Lite :)\n");
    printf("Use A D J L to move around the saber.\n");
    printf("Enjoy the game!\n");

    srand(time(NULL));

    FsOpenWindow(0, 0, 600, 1000, 1);

    InitSaber();

    InitObs();

    while (1) {
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        int key = FsInkey();

        switch (key) {
        case FSKEY_A:
            saber1.p[0].x -= 2; saber1.p[1].x -= 2;
            break;
        case FSKEY_D:
            saber1.p[0].x += 2; saber1.p[1].x += 2;
            break;
        case FSKEY_J:
            saber2.p[0].x -= 2; saber2.p[1].x -= 2;
            break;
        case FSKEY_L:
            saber2.p[0].x += 2; saber2.p[1].x += 2;
            break;
        case FSKEY_ESC:
            return 0;
        }

        ProcessObs();

        ProcessSaber();

        HitCheck();

        FsSwapBuffers();
        FsPollDevice();
        FsSleep(10);
    }

    return 0;
}
