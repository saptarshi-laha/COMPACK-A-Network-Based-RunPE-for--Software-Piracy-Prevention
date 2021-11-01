#include<stdio.h>

int main(int argc, char** argv) {

	char primeNumber[] = "is a prime number.";
	char notAprimeNumber[] = "is not a prime number.";

	for (int i = 0; i < argc; i++) {
		printf("%s", argv[i]);
	}

	for (int i = 2; i < 50000; i++) {
		int prime = 0;
		for (int j = 2; j <= i; j++) {
			if (i % j == 0) {
				prime = prime + 1;
			}
		}
		if (prime >= 1) {
			printf("%d %s\n", i, notAprimeNumber);
		}
	}


	getch();
}