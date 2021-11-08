using namespace std;
#include <cstdio>
#include <iostream>
#include <cmath>
#include <thread>
#include <chrono>
#include <string>
#include <fstream>

#include "fssimplewindow.h"

const int colormap_r[8] = { 0, 0, 255, 255, 0, 0, 255, 255 };
const int colormap_g[8] = { 0, 0, 0, 0, 255, 255, 255, 255 };
const int colormap_b[8] = { 0, 255, 0, 255, 0, 255, 0, 255 };

template <class T>
class BitmapTemplate {
private:
    int wid, hei;  // Dimension of the bitmap
    T* pix;

public:
    BitmapTemplate() {
        pix = nullptr;
        wid = 0; hei = 0;
    }

    ~BitmapTemplate() {
        delete[] pix;
        pix = NULL;
    }

    void CleanUp() {
        if (pix != nullptr) delete[] pix;
        pix = nullptr;
    }

    void set(const T* incoming_pix) {
        if (incoming_pix != pix) {
            CleanUp();
            pix = new T[wid * hei];
            if (nullptr != incoming_pix) {
                for (int i = 0; i < wid * hei; i++) {
                    pix[i] = incoming_pix[i];
                }
            }
        }
    }

    void copy_from(const BitmapTemplate& incoming) {
        wid = incoming.wid; hei = incoming.hei;
        set(incoming.pix);
    }

    BitmapTemplate(const BitmapTemplate& incoming) {
        pix = nullptr;
        copy_from(incoming);
    }

    BitmapTemplate& operator=(const BitmapTemplate& incoming) {
        copy_from(incoming);
        return *this;
    }

    int GetWidth() {
        return wid;
    }

    int GetHeight() {
        return hei;
    }

    void Create(int w, int h) {
        wid = w; hei = h;
        if (pix != nullptr) {
            delete[] pix;
            pix = nullptr;
        }
        pix = new T[w * h];
        for (int i = 0; i < hei; i++) {
            for (int j = 0; j < wid; j++) {
                pix[i * wid + j] = '0';
            }
        }
    }

    T GetPixel(int x, int y) {
        if ((0 <= x) && (x <= wid - 1) && (0 <= y) && (y <= hei - 1)) {
            return pix[(y * wid + x)];
        }
        return pix[0];
    }

    void SetPixel(int x, int y, T p) {
        if ((0 <= x) && (x <= wid - 1) && (0 <= y) && (y <= hei - 1)) {
            pix[(y * wid + x)] = p;
            // printf("The %d %d pixel set to %c\n", x, y, p);
        }
    }
};

class CharBitmap : public BitmapTemplate <char>{

public:

    void Draw() {
        int wid = GetWidth();
        int hei = GetHeight();
        for (int i = 0; i < hei; i++) {
            for (int j = 0; j < wid; j++) {
                char pix = GetPixel(i, j);
                glColor3ub(colormap_r[pix - '0'], colormap_g[pix - '0'], colormap_b[pix - '0']);
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
        int wid = GetWidth();
        int hei = GetHeight();

        File << wid << ' ' << hei << endl;
        for (int i = 0; i < hei; i++) {
            for (int j = 0; j < wid; j++) {
                char pix = GetPixel(i, j);
                File << pix;
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

const int max_undo = 10;
CharBitmap bm, undo[max_undo];
int undo_step = 0;

void process_pix(int p) {
    int lb, mb, rb, mx, my;
    int clickState = 0;

    auto key = FsInkey();
    auto mState = FsGetMouseEvent(lb, mb, rb, mx, my);

    int pix_x = mx / 20;
    int pix_y = my / 20;

    bm.SetPixel(pix_x, pix_y, char(p + '0'));
}

void saveFile() {
    string buffer;
    getline(cin, buffer);
    string filename = buffer + ".txt";
    bm.Save(filename);
}

void save_undo(CharBitmap incoming) {
    if (undo_step == max_undo-1) {
        for (int i = max_undo-2; i >= 0; i++) {
            undo[i] = undo[i + 1];
        }
        undo_step--;
    }
    undo[undo_step] = incoming;
    undo_step++;
}

CharBitmap load_undo() {
    if (undo_step <= 0) {
        return undo[0];
    }
    undo_step--;
    return undo[undo_step];
}

int main() {

    while (getInput()) {}

    FsOpenWindow(16, 16, w * 20, h * 20, 1);

    bm.Create(w, h);
    for (int i = 0; i < max_undo; i++) undo[i].Create(w, h);

    auto key = FsInkey();

    for (;;) {
        FsPollDevice();
        key = FsInkey();

        if (FSKEY_ESC == key) {
            break;
        }
        if ((0 <= key - FSKEY_0) && (key - FSKEY_0 <= 7)) {
            save_undo(bm);
            process_pix(key - FSKEY_0);
        }
        if (FSKEY_S == key) {
            saveFile();
        }
        if (FSKEY_U == key) {
            bm = load_undo();
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        bm.Draw();
        FsSwapBuffers();
        FsSleep(10);
    }

    return 0;
}