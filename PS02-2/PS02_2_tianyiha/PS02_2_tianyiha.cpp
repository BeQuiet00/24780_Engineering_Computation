using namespace std;
#include <iostream>
#include <cstdio>
#include <cmath>
#include <ctime>
#include <chrono>

int card[145],cardNum = 0;
int corAns = 0;
double corPerc;

void swap(int &a, int &b) {
	int temp = a;
	a = b;
	b = temp;
}

void shuffle(int* c) {
	srand(time(0));
	for (int i = 1; i <= 144; i++) {
		int k = rand() % 144 + 1;
		swap(c[i], c[k]);
	}
}

int main() {

	for (int i = 1; i <= 144; i++) {
		card[i] = i;
	}

	shuffle(card);

	/*
	for (int i = 1; i <= 144; i++) {
		printf("%d ", card[i]);
	}
	*/
	while (cardNum < 1 || cardNum>144) {
		printf("How many flash cards to work on?\n");
		scanf("%d", &cardNum);
		if (cardNum < 1 || cardNum>144) {
			printf("The number of cards must be between 1 and 144.\n");
		}
	}

	auto start = chrono::steady_clock::now();

	for (int i = 1; i <= cardNum; i++) {
		int card1, card2, ans;
		card1 = card[i] / 12 + 1;
		card2 = card[i] % 12 + 1;
		printf("%d x %d = ", card1, card2);
		scanf("%d", &ans);
		if (ans != card1 * card2) {
			printf("Wrong answer. Correct answer is %d.\n", card1 * card2);
		}
		else {
			corAns++;
		}
	}

	auto end = chrono::steady_clock::now();
	double dur = chrono::duration_cast<chrono::seconds>(end - start).count();

	corPerc = 100 * corAns / cardNum;

	printf("You answered %d problems in %d seconds.\n", cardNum, int(dur));
	printf("You answered %d problems correctly (%d%%).\n", corAns, int(corPerc));

	return 0;
}