#include "fssimplewindow.h"
#include "yssimplesound.h"

#include <iostream>
#include <cstdio>
#include <string>
#include <fstream>
#include <chrono>
#include <cmath>

using namespace std;

const int window_wid = 800;
const int resolution = 8;
const int max_data_len = window_wid / resolution;
const double PI = 3.14159;

class wave {
public:
	int freq;	  // frequency and magnitude of the spectrum

	int r, g, b;
	int alpha;

	double a = 1 - 1e-5;
	double pre_mag = 0;

	void setColor(int inp_r, int inp_g, int inp_b, int inp_alpha) {
		r = inp_r; g = inp_g; b = inp_b;
		alpha = inp_alpha;
	}

	void Draw(int len, int data[]) {
		glColor4ub(r, g, b, alpha);
		glBegin(GL_TRIANGLE_STRIP);

		for (int i = 0; i < len; i++) {
			glVertex2f(i * resolution, 300.0 + double(data[i]) * (1.0 - double(abs(i - len / 2) * abs(i - len / 2)) / ((len / 2) * (len / 2))));
			glVertex2f(i * resolution, 300.0 - double(data[i]) * (1.0 - double(abs(i - len / 2) * abs(i - len / 2)) / ((len / 2) * (len / 2))));
		}

		glEnd();
	}
};
 
void blackBackground() {
	glColor3ub(10, 10, 10);
	glBegin(GL_QUADS);
	glVertex2i(0,0);
	glVertex2i(0,600-1);
	glVertex2i(window_wid-1,600-1);
	glVertex2i(window_wid*10-1,0);
	glEnd();
}

wave waveR, waveG, waveB, waveW;

void visualizeSpectrum(int len, int sample_l[], int sample_r[]) {
	// set up color blending
	glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// red wave
	waveR.setColor(255, 58, 140, 100);
	int sample_red[max_data_len];
	for (int i = 0; i < len / 2; i++) {
		sample_red[i] = sample_l[i];
	}
	waveR.Draw(len/2, sample_red);

	// green wave
	waveR.setColor(83, 255, 150, 100);
	int sample_grn[max_data_len];
	for (int i = 0; i < len / 2; i++) {
		sample_grn[i] = sample_r[i + len / 6];  // manully implement a phase shift
	}
	waveR.Draw(len/2, sample_grn);

	// blue wave
	waveR.setColor(65, 70, 255, 100);
	int sample_ble[max_data_len];
	for (int i = 0; i < len / 2; i++) {
		sample_ble[i] = (sample_l[i + len / 6] + sample_r[i + len / 6]) / 2;
	}
	waveR.Draw(len/2, sample_ble);

	// while wave

	waveW.setColor(255, 255, 255, 200);
	int sample_wht[max_data_len];
	for (int i = 0; i < len / 2; i++) {
		sample_wht[i] = (sample_l[i + len / 3] + sample_r[i + len / 3]) / 4;
	}
	waveW.Draw(len / 2, sample_wht);

	// turn off color blending
	glDisable(GL_BLEND);
}

void playSound(const char file_name[]) {
	YsSoundPlayer player;
	YsSoundPlayer::SoundData wav;
	player.MakeCurrent();
	player.Start();

	if (YSOK != wav.LoadWav(file_name)) {
		printf("Load error!");
		return;
	}

	int play_back_rate = wav.PlayBackRate();

	player.Stop(wav);
	player.PlayOneShot(wav);

	auto begin = chrono::high_resolution_clock::now();

	long cur_len = 0;
	int cur_sample_l[max_data_len * 2], cur_sample_r[max_data_len * 2];
	long cur_num_sample = 0;
	
	while (YSTRUE == player.IsPlaying(wav)) {
		FsPollDevice();
		int key = FsInkey();
		if (key == FSKEY_ESC) {
			break;
		}

		auto cur = chrono::high_resolution_clock::now();
		auto dur = cur - begin;
		double dur_s = (double)std::chrono::duration_cast<std::chrono::milliseconds>(dur).count()/1000.0;

		cur_num_sample = wav.SecToNumSample(dur_s, play_back_rate);
		cur_len = (max_data_len * 2 < cur_num_sample) ? max_data_len * 2 : cur_num_sample;

		for (int i = 0; i < cur_len; i++) {
			int idx = cur_num_sample - cur_len + i + 1;
			cur_sample_l[i] = wav.GetSignedValueRaw(0, idx) / 100;
			cur_sample_r[i] = wav.GetSignedValueRaw(1, idx) / 100;
		}

		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		blackBackground();
		visualizeSpectrum(cur_len, cur_sample_l, cur_sample_r);
		FsSwapBuffers();
		player.KeepPlaying();
		FsSleep(80);
	}
	player.End();
}

int main(void){

	FsOpenWindow(0, 0, window_wid, 600, 1);

	playSound("Chopin-ballade-no-1-in-g-minor-op-23.wav");

	return 0;
}



