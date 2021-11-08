using namespace std;
#include <cstdio>
#include <iostream>
#include <cmath>
#include <thread>
#include <chrono>
#include <string>
#include <fstream>

#include "fssimplewindow.h"

class screen_cord {
public:
    int x, y;
};

class Line2D {
protected:
    screen_cord p[2];
    screen_cord init_p[2];
    int r, g, b;

public:
    Line2D() {
        p[0].x = 0; p[0].y = 0;
        p[1].x = 0; p[1].y = 0;
        r = 0; g = 0; b = 0;
    }
    
    void Draw() {
        glColor3ub(r, g, b);
        glBegin(GL_LINES);
        // printf("Line from %d, %d to %d, %d\n", p[0].x, p[0].y, p[1].x, p[1].y);
        glVertex2i(p[0].x, p[0].y);
        glVertex2i(p[1].x, p[1].y);
        glEnd();
    }

    void MakeFromString(string inp) {
        int inp_num[7];
        int inp_idx = 0;

        string delimiter = " ";
        size_t pos = 0;
        std::string token;
        while ((pos = inp.find(delimiter)) != string::npos) {
            token = inp.substr(0, pos);
            inp_num[inp_idx] = stoi(token);
            inp_idx++;
            inp.erase(0, pos + delimiter.length());
        }
        inp_num[inp_idx] = stoi(inp);
        p[0].x = inp_num[0]; p[0].y = 600 - inp_num[1];
        p[1].x = inp_num[2]; p[1].y = 600 - inp_num[3];
        r = inp_num[4]; g = inp_num[5]; b = inp_num[6];
        init_p[0] = p[0]; init_p[1] = p[1];
    }

    void Shift(int shf_x, int shf_y) {
        p[0].x += shf_x; p[0].y += shf_y;
        p[1].x += shf_x; p[1].y += shf_y;
    }

    void Reset() {
        p[0] = init_p[0];
        p[1] = init_p[1];
    }
};

class Drawing2D {
protected:
    Line2D* lines;
    int num_lines;
    int pre_x, pre_y;

public:
    Drawing2D() {
        lines = nullptr;
        num_lines = 0;
        pre_x = 0; pre_y = 0;
    }

    ~Drawing2D() {
        delete[] lines;
        lines = NULL;
    }

    void Draw() {
        for (int i = 0; i < num_lines; i++) {
            lines[i].Draw();
        }
    }

    void ReadFile(string file_name) {
        ifstream inputFile(file_name);
        string buffer;
        getline(inputFile, buffer);
        num_lines = stoi(buffer);

        lines = new Line2D[num_lines];

        for (int i = 0; i < num_lines; i++) {
            getline(inputFile, buffer);
            lines[i].MakeFromString(buffer);
        }
    }

    void Move() {
        int lb, mb, rb, mx, my;
        int shf_x, shf_y;
        auto mouse_state = FsGetMouseEvent(lb, mb, rb, mx, my);

        if (FSMOUSEEVENT_LBUTTONDOWN == mouse_state) {
            pre_x = mx;
            pre_y = my;
            // cout << "mouse lb down" << endl;
        }

        if (lb != 0) {
            if (FSMOUSEEVENT_MOVE == mouse_state) {
                shf_x = mx - pre_x; shf_y = my - pre_y;
                // cout << "shift" << shf_x << ' ' << shf_y << endl;
                for (int i = 0; i < num_lines; i++) {
                    lines[i].Shift(shf_x, shf_y);
                }
                pre_x = mx; pre_y = my;
            }
        }
    }

    void Reset() {
        for (int i = 0; i < num_lines; i++) {
            lines[i].Reset();
        }
    }
};

string file_name;


int main() {
    printf("Enter File Name>\n");
    cin >> file_name;
    FsOpenWindow(16, 16, 800, 600, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    Drawing2D D2D;
    D2D.ReadFile(file_name);
    D2D.Draw();
    glFlush();
    for (;;) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        auto key = FsInkey();
        if (FSKEY_ESC == key)
        {
            break;
        }
        if (FSKEY_SPACE == key)
        {
            D2D.Reset();
        }

        D2D.Move();
        D2D.Draw();
        glFlush();
        FsPollDevice();
        FsSleep(10);
    }
    return 0;
}