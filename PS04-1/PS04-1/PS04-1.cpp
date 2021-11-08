using namespace std;
#include <iostream>
#include <cstdio>
#include <chrono>
#include <thread>

#include "fssimplewindow.h"

class Position {

public:

    int x, y;

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

        printf("Line from %d, %d to %d, %d\n", p[0].x, p[0].y, p[1].x, p[1].y);

        glVertex2i(p[0].x, p[0].y);
        glVertex2i(p[1].x, p[1].y);
        glEnd();
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

        printf("Rectangle from %d, %d to %d, %d\n", p[0].x, p[0].y, p[1].x, p[1].y);

        glVertex2i(p[0].x, p[0].y);
        glVertex2i(p[1].x, p[0].y);
        glVertex2i(p[1].x, p[1].y);
        glVertex2i(p[0].x, p[1].y);
        glEnd();
    }
};

void VisualizeArrayContents(int n, int x[]) { 

    Shape l[20];

    for (int i = 0; i < 20; i++) {
        
        l[i].p[0].y = i * 20; l[i].p[0].x = 0;
        l[i].p[1].y = i * 20 + 20; l[i].p[1].x = x[i]*20;

        l[i].r = 0; l[i].g = 0; l[i].b = 255;
        l[i].fill = 1;

        l[i].DrawRect();
    }
}

void SwapInt(int& a, int& b) { 

    int temp = a;
    a = b; 
    b = temp;
}

void BubbleSort(int n, int x[]) { 
    int i, j;
    for (i = 0; i < n; i++) { 
        for (j = i + 1; j < n; j++) { 
            if (x[i] > x[j]) { 
                SwapInt(x[i], x[j]); 
            } 
        }

        FsPollDevice();
        while (FSKEY_NULL == FsInkey()) { 
            glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
            VisualizeArrayContents(n, x);
            FsPollDevice();
            FsSwapBuffers();
            FsSleep(10);
        } 
    } 
}

int main(){

    int x[20] = { 17,10,13,14,15,6,3,2,9,8,18,7,12,11,16,4,1,20,5,19 };
    FsOpenWindow(16, 16, 400, 400, 1);
    BubbleSort(20, x);
    return 0;
}
