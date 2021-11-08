using namespace std;
#include <iostream>
#include <cstdio>
#include <cmath>
#include <ctime>
#include <chrono>
#include <cstdlib>

class FlashCard {
public:
	int a, b;

	void present() {
		printf("%dx%d=", a, b);
	}

	int solution() {
		return a * b;
	}
};

class FlashCardSet {
public:
	FlashCard card[145];

	int nCardToWork = 0;

	void Shuffle() {
		for (int i = 1; i <= 144; i++) {
			int j = rand() % 144 + 1;
			swap(card[i], card[j]);
		}
	}
};

int corAns = 0, ans;
double corPerc;
char buffer[256];
FlashCardSet CardSet1;

void swap(FlashCard &card1, FlashCard &card2) {
	FlashCard temp = card1;
	card1 = card2;
	card2 = temp;
}

int main() {
	srand(time(0));

	for (int i = 1; i <= 144; i++) {
		CardSet1.card[i].a = i / 12 + 1;
		CardSet1.card[i].b = i % 12 + 1;
	}

	CardSet1.Shuffle();

	/*
	for (int i = 1; i <= 144; i++) {
		printf("%dx%d ", CardSet1.card[i].a, CardSet1.card[i].b);
	}
	*/

	while (CardSet1.nCardToWork < 1 || CardSet1.nCardToWork>144) {
		printf("How many flash cards to work on?\n");
		// scanf("%d", &CardSet1.nCardToWork);
		fgets(buffer, 256, stdin);
		CardSet1.nCardToWork = atoi(buffer);
		if (CardSet1.nCardToWork < 1 || CardSet1.nCardToWork>144) {
			printf("The number of cards must be between 1 and 144.\n");
		}
	}

	auto start = chrono::steady_clock::now();

	for (int i = 1; i <= CardSet1.nCardToWork; i++) {
		printf("%d x %d = ", CardSet1.card[i].a, CardSet1.card[i].b);
		// scanf("%d", &ans);
		fgets(buffer, 256, stdin);
		ans = atoi(buffer);
		if (ans != CardSet1.card[i].solution()) {
			printf("Wrong answer. Correct answer is %d.\n", CardSet1.card[i].solution());
		}
		else {
			corAns++;
		}
	}

	auto end = chrono::steady_clock::now();
	double dur = chrono::duration_cast<chrono::seconds>(end - start).count();

	corPerc = 100 * corAns / CardSet1.nCardToWork;

	printf("You answered %d problems in %d seconds.\n", CardSet1.nCardToWork, int(dur));
	printf("You answered %d problems correctly (%d%%).\n", corAns, int(corPerc));

	return 0;
}