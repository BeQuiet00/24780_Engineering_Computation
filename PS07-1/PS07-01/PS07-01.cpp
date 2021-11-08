using namespace std;
#include <cstdio>
#include <iostream>
#include <cmath>
#include <thread>
#include <chrono>
#include <string>
#include <fstream>

#include "fssimplewindow.h"

int colormap_r[8] = { 0, 0, 255, 255, 0, 0, 255, 255 };
int colormap_g[8] = { 0, 0, 0, 0, 255, 255, 255, 255 };
int colormap_b[8] = { 0, 255, 0, 255, 0, 255, 0, 255 };

class CharBitmap {

public:

    int wid, hei;
    char* pix;

    CharBitmap() {
        // cout << "create bitmap\n";
        pix = nullptr;
        wid = 0; hei = 0;
    }

    ~CharBitmap() {
        delete[] pix;
        pix = NULL;
        // cout << "deleted bitmap\n";
    }

    void CleanUp() {
        if (pix != nullptr) {
            delete[] pix;
            pix = nullptr;
        }
        wid = 0; hei = 0;
    }

    void Create(int w, int h) {
        wid = w; hei = h;
        if (pix != nullptr) {
            delete[] pix;
            pix = nullptr;
        }
        pix = new char[w * h];
        for (int i = 0; i < hei; i++) {
            for (int j = 0; j < wid; j++) {
                pix[i * wid + j] = '0';
            }
        }
    }

    void SetPixel(int x, int y, char p) {
        if ((0 <= x) && (x <= wid - 1) && (0 <= y) && (y <= hei - 1)) {
            pix[(y * wid + x)] = p;
            printf("The %d %d pixel set to %c\n", x, y, p);
        }
    }

    char GetPixel(int x, int y) {
        if ((0 <= x) && (x <= wid - 1) && (0 <= y) && (y <= hei - 1)) {
            return pix[(y * wid + x)];
        }
        return 0;
    }

    void Draw() {
        for (int i = 0; i < hei; i++) {
            for (int j = 0; j < wid; j++) {
                glColor3ub(colormap_r[pix[i * wid + j] - '0'], colormap_g[pix[i * wid + j] - '0'], colormap_b[pix[i * wid + j] - '0']);
                glBegin(GL_QUADS);
                glVertex2i(i * 20, j * 20);
                glVertex2i(i * 20 + 19, j * 20);
                glVertex2i(i * 20 + 19, j * 20 + 19);
                glVertex2i(i * 20, j * 20 + 19);
                glEnd();
            }
        }
    }

    void Save(string filename) {
        ofstream File(filename);

        File << wid << ' ' << hei << endl;
        for (int i = 0; i < hei; i++) {
            for (int j = 0; j < wid; j++) {
                File << pix[i * wid + j];
            }
            File << endl;
        }

        File.close();
    }
};

int w = 0, h = 0;

int getInput() {
    printf("Enter Dimension>\n");
    
    // prasing input
    string buffer;
    getline(cin, buffer);
    string delimiter = " ";

    size_t pos = 0;
    std::string token;
    while ((pos = buffer.find(delimiter)) != string::npos) {
        token = buffer.substr(0, pos);
        // cout << token << endl;
        w = stoi(token);
        buffer.erase(0, pos + delimiter.length());
    }
    // cout << buffer << endl;
    h = stoi(buffer);

    if ((w <= 0) || (w > 64) || (h <= 0) || (h > 64)) return 1;

    return 0;
}

CharBitmap bm;

void process_pix(int p) {
    int lb, mb, rb, mx, my;
    int clickState = 0;

    auto key = FsInkey();
    auto mState = FsGetMouseEvent(lb, mb, rb, mx, my);

    int pix_x = my / 20;
    int pix_y = mx / 20;
    
    bm.SetPixel(pix_x, pix_y, char(p + '0'));
}

void saveFile() {
    string buffer;
    getline(cin, buffer);
    string filename = buffer + ".txt";
    bm.Save(filename);
}

void loadFile() {
    string buffer;
    getline(cin, buffer);
    string filename = buffer + ".txt";
}

int main() {
    
    while (getInput()) {}
    
    FsOpenWindow(16, 16, w * 20, h * 20, 1); 

    bm.Create(w, h); 

    // bm.SetPixel(5, 5, '1'); // testing

    auto key = FsInkey();

    for (;;)
    {
        FsPollDevice();
        
        if (FSKEY_ESC == key){
            break;
        }
        if ((0 <= key - FSKEY_0) && (key - FSKEY_0 <= 7)){
            process_pix(key - FSKEY_0);
        }
        if (FSKEY_S == key) {
            saveFile();
        }
        if (FSKEY_L == key) {
            loadFile();
        }

        key = FsInkey();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        bm.Draw();
        FsSwapBuffers();
        FsSleep(10);
    }

	return 0;
}