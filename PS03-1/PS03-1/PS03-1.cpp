#include <iostream>
#include <cstdio>
#include <ctime>
#include <chrono>
#include <thread>
#include "fssimplewindow.h"

int main()
{
    FsOpenWindow(16, 16, 800, 600, 0);

    int x1 = 0, x2 = 799, y1 = 0, y2 = 99;
    int r = 0, g = 0, b = 0;

	for (int i = 0; i < 7; i++){
        y1 = i * 100;
        y2 = i * 100 + 100;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		FsPollDevice();

		glColor3ub(r, g, b);
		glBegin(GL_QUADS);
		glVertex2i(x1, y1);
		glVertex2i(x2, y1);
		glVertex2i(x2, y2);
		glVertex2i(x1, y2);
		glEnd();

		glFlush();

		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}

    return 0;
}
