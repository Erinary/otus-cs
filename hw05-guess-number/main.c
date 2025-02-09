#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
//
// Created by Liubov Udalova on 09.02.25.
//
int main(void) {
    int ans;
    srand(time(NULL));
    int number = rand() % 100 + 1;

    printf("I'm thinking of number in a range from 1 to 100, try to guess it!\n");
    while (true) {
        printf("Your guess:");
        scanf("%d", &ans);

        if (ans == number) {
            printf("You're right, my number is %d!", number);
            return 0;
        }

        if (ans < number) {
            printf("My number is greater than yours, try again!\n");
        } else {
            printf("My number is lesser than yours, try again!\n");
        }
    }
}
