using namespace std;

#include <cmath>
#include <cstdio>
#include <iostream>
#include <string>

const char allowed_letters[15] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
								   'X', 'x', '/', '-', ' ' };

int scores[21];

int check_input(const string input_string) {
	int string_len = input_string.length();
	if (string_len < 21) {
		printf("Too Short\n");
		return 0;
	}
	for (int i = 0; i < 21; i++) {
		int flag = 0;
		for (int j = 0; j < 15; j++) {
			if (input_string[i] == allowed_letters[j]) flag = 1;
		}
		if (!flag) {
			printf("Wrong Character\n");
			return 0;
		}
	}
	return 1;
}

void interpret_input(const string input_string) {
	for (int i = 0; i < 21; i++) {
		int asc_num = int(input_string[i]);
		if ((48 <= asc_num) && (asc_num <= 57)) {
			scores[i] = asc_num - 48;
			if ((i % 2) && (scores[i] + scores[i-1] == 10)) {   // if is a spare
				scores[i] = -1;
			}
		}
		if ((asc_num == 88) || (asc_num == 120)) {
			scores[i] = 10;
		}
		if (asc_num == 47) {
			scores[i] = -1; // indicates a spare
		}
		if ((asc_num == 32) || (asc_num == 45)) {
			scores[i] = 0;
		}
	}
}

void calculate_scores() {
	int tot_scr = 0;
	for (int i = 0; i < 18; i++) {
		if (scores[i] == 10) {
			int idx = i + 1, num_shots = 0, scr = 10;
			while ((idx < 21) && (num_shots < 2)) {
				if (scores[idx]) {
					scr += 0 < scores[idx] ? scores[idx] : 10 - scores[idx - 1];
					num_shots ++;
				}
				idx++;
			}
			tot_scr += scr;
		}
		else if (scores[i] == -1) {
			int scr = 0 < i ? 10 - scores[i - 1] : 0;
			int idx = i + 1, flag = 1;
			while ((idx < 21) && (flag)) {
				if (scores[idx]) {
					scr += 0 < scores[idx] ? scores[idx] : 10 - scores[idx - 1];
					flag = 0;
				}
				idx++;
			}
			tot_scr += scr;
		}
		else {
			tot_scr += scores[i];
		}
		printf("%d %d\n", i+1, tot_scr);
	}
	for (int i = 18; i < 21; i++) {
		tot_scr += scores[i];
	}
	printf("%d\n", tot_scr);
}

int main() {
	string input_string;
	getline(cin, input_string);
	
	if (!check_input(input_string)) exit(0);

	interpret_input(input_string);

	/*
	for (int i = 0; i < 21; i++) {
		printf("%d ", scores[i]);
	}
	*/

	calculate_scores();

	return 0;
}