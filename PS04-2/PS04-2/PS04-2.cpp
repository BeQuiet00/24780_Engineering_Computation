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
        return (int)599 - y * 10;
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
            } else { 
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
            } else { 
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

Shape Canon1, Canon2;
double TarDeg = 30.0;

Shape ball;
int ball_stp;
int ball_status = 0;
double ball_vx, ball_vy;

Shape obs[6];
int obs_status[6]; // 1: Active 0: Destroyed
int obs_v[6];

Shape trj[800];
int trj_idx = 0;

void InitCan() {
    Canon1.p[0].x = 5 - 0.5; Canon1.p[0].y = 5 + 0.5;
    Canon1.p[1].x = 5 + 0.5; Canon1.p[1].y = 5 - 0.5;
    Canon1.r = 0; Canon1.g = 0; Canon1.b = 255;
    Canon1.fill = 1;
    Canon1.shape_type = 2;

    Canon2.p[0].x = 5; Canon2.p[0].y = 5;
    Canon2.p[1].x = 5 + 2 * cos(TarDeg /180.0 * PI); Canon2.p[1].y = 5 + 2 * sin(TarDeg / 180.0 * PI);
    Canon2.r = 0; Canon2.g = 0; Canon2.b = 255;
    Canon2.shape_type = 0;
}

void ProcessCan() {
    Canon2.p[1].x = 5 + 2 * cos(TarDeg / 180.0 * PI); Canon2.p[1].y = Canon1.p[0].y - 0.5 + 2 * sin(TarDeg / 180.0 * PI);
    Canon1.Draw();
    Canon2.Draw();
}

void InitBall() {
    ball_status = 1;
    ball.p[0].x = Canon1.p[0].x + 0.5; 
    ball.p[0].y = Canon1.p[0].y - 0.5;
    ball.r = 255; ball.g = 0; ball.b = 0;
    ball.rad = 5;
    ball.fill = 1;
    ball.shape_type = 3;

    ball_stp = 0;
    ball_vx = 40 * cos(TarDeg / 180.0 * PI);
    ball_vy = 40 * sin(TarDeg / 180.0 * PI);
}

void ProcessBall() {
    if (ball_status) {
        ball_stp++;
        ball_vy -= 9.81 * 0.025;
        ball.p[0].x += ball_vx * 0.025;
        ball.p[0].y += ball_vy * 0.025;
        ball.Draw();
    }
    if (ball.p[0].x >= 80 || ball.p[0].y <= 0) {
        ball_status = 0;
        trj_idx = 0;
    }
}

void InitObs() {
    for (int i = 1; i <= 5; i++) {
        obs_status[i] = 1;
        obs[i].r = 0; obs[i].g = 255; obs[i].b = 0;
        int w = rand() % 8 + 8;
        int h = rand() % 8 + 8;
        obs[i].p[0].x = rand() % (80 - w); obs[i].p[1].y = rand() % (60 - h);
        obs[i].p[1].x = obs[i].p[0].x + w; obs[i].p[0].y = obs[i].p[1].y + h;
        obs[i].fill = 1;
        obs[i].shape_type = 2;
        obs_v[i] = rand() % 11 - 5;
    }
    obs_status[0] = 1;
    obs[0].r = 255; obs[0].g = 0; obs[0].b = 0;
    obs[0].p[0].x = 75; obs[0].p[0].y = 60;
    obs[0].p[1].x = 80; obs[0].p[1].y = 55;
    obs[0].fill = 1;
    obs[0].shape_type = 2;
    obs_v[0] = -10;
}

void ProcessObs() {
    for (int i = 1; i <= 5; i++) {
        if (obs_status[i]) {
            obs[i].p[0].y += obs_v[i] * 0.025;
            obs[i].p[1].y += obs_v[i] * 0.025;

            obs[i].Draw();
        }
    }
    for (int i = 1; i <= 5; i++) {
        if (obs_status[i]) {
            if ((obs[i].p[0].y >= 60) || (obs[i].p[1].y <= 0)) {
                obs_v[i] = -obs_v[i];
            }
        }
    }
    obs[0].p[0].y += obs_v[0] * 0.025;
    obs[0].p[1].y += obs_v[0] * 0.025;
    obs[0].Draw();
    if (obs[0].p[0].y == 0) {
        obs[0].p[0].y = 60;
        obs[0].p[1].y = 55;
    }
}

void InitTrj() {
    for (int i = 0; i <= 799; i++) {
        trj[i].r = 255; trj[i].g = 0; trj[i].b = 0;
        trj[i].shape_type = 0;
    }
}

void ProcessTrj() {
    if (ball_status) {
        trj[trj_idx].p[1].x = ball.p[0].x;
        trj[trj_idx].p[1].y = ball.p[0].y;
        if (trj_idx > 0) {
            trj[trj_idx].p[0].x = trj[trj_idx - 1].p[1].x;
            trj[trj_idx].p[0].y = trj[trj_idx - 1].p[1].y;
        }
        for (int i = 1; i <= trj_idx-1; i++) {
            trj[i].Draw();
        }
        trj_idx++;
    }
}

void HitEffect() {
    int px = ball.p[0].x;
    int py = ball.p[0].y;
    Shape loop;
    for (int i = 1; i <= 80; i++) {
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        loop.p[0].x = px; loop.p[0].y = py;
        loop.fill = 0;
        loop.rad = i * 10;
        loop.r = rand() % 256; loop.g = rand() % 256; loop.b = rand() % 256;
        loop.shape_type = 3;
        loop.Draw();
        FsSwapBuffers();
        FsPollDevice();
        FsSleep(100);
    }
}

int HitCheck() {
    for (int i = 0; i <= 5; i++) {
        if (obs_status[i]) {
            if ((obs[i].p[0].x <= ball.p[0].x) && (ball.p[0].x <= obs[i].p[1].x)) {
                if ((obs[i].p[0].y >= ball.p[0].y) && (ball.p[0].y >= obs[i].p[1].y)) {
                    printf("object %d destroyed!\n", i);
                    obs_status[i] = 0;
                    ball_status = 0;
                    trj_idx = 0;
                    if (i == 0) {
                        HitEffect();
                    }
                    return i;
                }
            }
        }
    }
    return -1;
}

int main(){

    srand(time(NULL)); 

    FsOpenWindow(0, 0, 800, 600, 1);
    
    InitCan();

    InitObs();

    InitTrj();

    while (HitCheck() != 0) {
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        
        int key = FsInkey();

        switch (key) {
        case FSKEY_SPACE:
            if (!ball_status) InitBall();
            break;
        case FSKEY_LEFT:
            TarDeg++;
            break;
        case FSKEY_RIGHT:
            TarDeg--;
            break;
        case FSKEY_UP:
            Canon1.p[0].y++; Canon1.p[1].y++;
            Canon2.p[0].y++; Canon2.p[1].y++;
            break;
        case FSKEY_DOWN:
            Canon1.p[0].y--; Canon1.p[1].y--;
            Canon2.p[0].y--; Canon2.p[1].y--;
            break;
        }

        ProcessObs();

        ProcessBall();

        ProcessCan();

        ProcessTrj();

        FsSwapBuffers();
        FsPollDevice();
        FsSleep(25);
    }

    return 0;
}
