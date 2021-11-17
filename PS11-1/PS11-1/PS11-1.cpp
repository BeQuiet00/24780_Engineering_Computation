#include <cstdio>   
#include <iostream>
#include <cmath>

using namespace std;

const int maxn = 4;

template <class T, const int NR, const int NC>
class MatrixTemplate {
private:
    T data[maxn * maxn];

public:
    void Set(int r, int c, T inp_data) {
        int pos = NC * (r - 1) + c - 1;
        data[pos] = inp_data;
    }

    T Get(int r, int c) {
        int pos = NC * (r - 1) + c - 1;
        return data[pos];
    }
};

class Matrix4x4 : public MatrixTemplate <double, 4, 4> {
public:
    void Transpose() {
        for (int i = 1; i <= 3; i++) {
            for (int j = i + 1; j <= 4; j++) {
                double tmp = Get(i, j);
                Set(i, j, Get(j, i));
                Set(j, i, tmp);
            }
        }
    }

    void Print() {
        for (int i = 1; i <= 4; i++) {
            for (int j = 1; j <= 4; j++) {
                printf("%.2lf ", Get(i, j));
            }
            printf("\n");
        }
    }

    void Invert() {
        double inv[16], det;

        inv[0] = Get(2, 2) * Get(3, 3) * Get(4, 4) -
                 Get(2, 2) * Get(3, 4) * Get(4, 3) -
                 Get(3, 2) * Get(2, 3) * Get(4, 4) +
                 Get(3, 2) * Get(2, 4) * Get(4, 3) +
                 Get(4, 2) * Get(2, 3) * Get(3, 4) -
                 Get(4, 2) * Get(2, 4) * Get(3, 3);

        inv[4] = -Get(2, 1) * Get(3, 3) * Get(4, 4) +
                 Get(2, 1) * Get(3, 4) * Get(4, 3) +
                 Get(3, 1) * Get(2, 3) * Get(4, 4) -
                 Get(3, 1) * Get(2, 4) * Get(4, 3) -
                 Get(4, 1) * Get(2, 3) * Get(3, 4) +
                 Get(4, 1) * Get(2, 4) * Get(3, 3);

        inv[8] = Get(2, 1) * Get(3, 2) * Get(4, 4) -
                 Get(2, 1) * Get(3, 4) * Get(4, 2) -
                 Get(3, 1) * Get(2, 2) * Get(4, 4) +
                 Get(3, 1) * Get(2, 4) * Get(4, 2) +
                 Get(4, 1) * Get(2, 2) * Get(3, 4) -
                 Get(4, 1) * Get(2, 4) * Get(3, 2);

        inv[12] = -Get(2, 1) * Get(3, 2) * Get(4, 3) +
                   Get(2, 1) * Get(3, 3) * Get(4, 2) +
                   Get(3, 1) * Get(2, 2) * Get(4, 3) -
                   Get(3, 1) * Get(2, 3) * Get(4, 2) -
                   Get(4, 1) * Get(2, 2) * Get(3, 3) +
                   Get(4, 1) * Get(2, 3) * Get(3, 2);

        inv[1] = -Get(1, 2) * Get(3, 3) * Get(4, 4) +
                   Get(1, 2) * Get(3, 4) * Get(4, 3) +
                   Get(3, 2) * Get(1, 3) * Get(4, 4) -
                   Get(3, 2) * Get(1, 4) * Get(4, 3) -
                   Get(4, 2) * Get(1, 3) * Get(3, 4) +
                   Get(4, 2) * Get(1, 4) * Get(3, 3);

        inv[5] = Get(1, 1) * Get(3, 3) * Get(4, 4) -
                 Get(1, 1) * Get(3, 4) * Get(4, 3) -
                 Get(3, 1) * Get(1, 3) * Get(4, 4) +
                 Get(3, 1) * Get(1, 4) * Get(4, 3) +
                 Get(4, 1) * Get(1, 3) * Get(3, 4) -
                 Get(4, 1) * Get(1, 4) * Get(3, 3);

        inv[9] = -Get(1, 1) * Get(3, 2) * Get(4, 4) +
                  Get(1, 1) * Get(3, 4) * Get(4, 2) +
                  Get(3, 1) * Get(1, 2) * Get(4, 4) -
                  Get(3, 1) * Get(1, 4) * Get(4, 2) -
                  Get(4, 1) * Get(1, 2) * Get(3, 4) +
                  Get(4, 1) * Get(1, 4) * Get(3, 2);

        inv[13] = Get(1, 1) * Get(3, 2) * Get(4, 3) -
                  Get(1, 1) * Get(3, 3) * Get(4, 2) -
                  Get(3, 1) * Get(1, 2) * Get(4, 3) +
                  Get(3, 1) * Get(1, 3) * Get(4, 2) +
                  Get(4, 1) * Get(1, 2) * Get(3, 3) -
                  Get(4, 1) * Get(1, 3) * Get(3, 2);

        inv[2] = Get(1, 2) * Get(2, 3) * Get(4, 4) -
                 Get(1, 2) * Get(2, 4) * Get(4, 3) -
                 Get(2, 2) * Get(1, 3) * Get(4, 4) +
                 Get(2, 2) * Get(1, 4) * Get(4, 3) +
                 Get(4, 2) * Get(1, 3) * Get(2, 4) -
                 Get(4, 2) * Get(1, 4) * Get(2, 3);

        inv[6] = -Get(1, 1) * Get(2, 3) * Get(4, 4) +
                  Get(1, 1) * Get(2, 4) * Get(4, 3) +
                  Get(2, 1) * Get(1, 3) * Get(4, 4) -
                  Get(2, 1) * Get(1, 4) * Get(4, 3) -
                  Get(4, 1) * Get(1, 3) * Get(2, 4) +
                  Get(4, 1) * Get(1, 4) * Get(2, 3);

        inv[10] = Get(1, 1) * Get(2, 2) * Get(4, 4) -
                  Get(1, 1) * Get(2, 4) * Get(4, 2) -
                  Get(2, 1) * Get(1, 2) * Get(4, 4) +
                  Get(2, 1) * Get(1, 4) * Get(4, 2) +
                  Get(4, 1) * Get(1, 2) * Get(2, 4) -
                  Get(4, 1) * Get(1, 4) * Get(2, 2);

        inv[14] = -Get(1, 1) * Get(2, 2) * Get(4, 3) +
                   Get(1, 1) * Get(2, 3) * Get(4, 2) +
                   Get(2, 1) * Get(1, 2) * Get(4, 3) -
                   Get(2, 1) * Get(1, 3) * Get(4, 2) -
                   Get(4, 1) * Get(1, 2) * Get(2, 3) +
                   Get(4, 1) * Get(1, 3) * Get(2, 2);

        inv[3] = -Get(1, 2) * Get(2, 3) * Get(3, 4) +
                  Get(1, 2) * Get(2, 4) * Get(3, 3) +
                  Get(2, 2) * Get(1, 3) * Get(3, 4) -
                  Get(2, 2) * Get(1, 4) * Get(3, 3) -
                  Get(3, 2) * Get(1, 3) * Get(2, 4) +
                  Get(3, 2) * Get(1, 4) * Get(2, 3);

        inv[7] = Get(1, 1) * Get(2, 3) * Get(3, 4) -
                 Get(1, 1) * Get(2, 4) * Get(3, 3) -
                 Get(2, 1) * Get(1, 3) * Get(3, 4) +
                 Get(2, 1) * Get(1, 4) * Get(3, 3) +
                 Get(3, 1) * Get(1, 3) * Get(2, 4) -
                 Get(3, 1) * Get(1, 4) * Get(2, 3);

        inv[11] = -Get(1, 1) * Get(2, 2) * Get(3, 4) +
                   Get(1, 1) * Get(2, 4) * Get(3, 2) +
                   Get(2, 1) * Get(1, 2) * Get(3, 4) -
                   Get(2, 1) * Get(1, 4) * Get(3, 2) -
                   Get(3, 1) * Get(1, 2) * Get(2, 4) +
                   Get(3, 1) * Get(1, 4) * Get(2, 2);

        inv[15] = Get(1, 1) * Get(2, 2) * Get(3, 3) -
                  Get(1, 1) * Get(2, 3) * Get(3, 2) -
                  Get(2, 1) * Get(1, 2) * Get(3, 3) +
                  Get(2, 1) * Get(1, 3) * Get(3, 2) +
                  Get(3, 1) * Get(1, 2) * Get(2, 3) -
                  Get(3, 1) * Get(1, 3) * Get(2, 2);

        det = Get(1, 1) * inv[0] + Get(1, 2) * inv[4] + Get(1, 3) * inv[8] + Get(1, 4) * inv[12];

        if (det == 0)
            return;

        det = 1.0 / det;

        for (int i = 1; i <= 4; i++) {
            for (int j = 1; j <= 4; j++) {
                Set(j, i, det * inv[4 * (i - 1) + j - 1]);
            }
        }
    }
};


int main() {
    Matrix4x4 mat;     
    const double v[] = {
        6.0,1.0,4.0,9.0,         
        9.0,8.0,6.0,1.0,         
        7.0,2.0,9.0,4.0,
        1.0,7.0,5.0,9.0
    };  

    for (int i = 0; i < 16; ++i) {
        const int r = 1 + i / 4;         
        const int c = 1 + i % 4;
        mat.Set(r, c, v[i]);
    }     
    mat.Print();
    mat.Transpose();
    printf("\n");     
    mat.Print();
    // If you go for extra credit, uncomment the following three lines.     
    printf("\n");     
    mat.Invert();     
    mat.Print();     
    return 0; 
} 