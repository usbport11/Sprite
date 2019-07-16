#include "Random.h"

unsigned int RandomChance(unsigned int n) {
	//chance = (100 / n) %
	if(n <= 1) return 1;
	return (rand() % n > n - 2);
}

unsigned int RandomChancePercent(unsigned int n) {
	if(n > 100) return 1;
	if(!n) return 0;
	return (rand() % 100 < n);
}

int RandomValue(int Min, int Max) {
	if(Min > Max) return 0;
	if(Min == Max) return Min;
	return rand() % (Max - Min + 1) + Min;
}
