#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define DICE 5
#define CATEGORIES 13
#define PLAYERS 2

/* Category indexes */
enum {
    ONES, TWOS, THREES, FOURS, FIVES, SIXES,
    THREE_KIND, FOUR_KIND, FULL_HOUSE,
    SMALL_STRAIGHT, LARGE_STRAIGHT,
    YAHTZEE, CHANCE
};

/* Function declarations */
void rollDice(int dice[]);
void displayDice(int dice[]);
void countFrequency(int dice[], int freq[]);
int calculateScore(int category, int dice[], int freq[]);
void showScoreMenu(int player, int used[][CATEGORIES], int dice[], int freq[]);
void playTurn(int player, int scores[][CATEGORIES], int used[][CATEGORIES]);
int upperBonus(int scores[]);

int main() {
    int mode;
    int scores[PLAYERS][CATEGORIES] = {0};
    int used[PLAYERS][CATEGORIES] = {0};

    srand(time(0));

    printf("====== YAHTZEE GAME ======\n");
    printf("1. Single Player\n");
    printf("2. Multiplayer (2 Players)\n");
    printf("Select Mode: ");
    scanf("%d", &mode);

    if (mode == 1) {
        for (int round = 0; round < CATEGORIES; round++) {
            printf("\n--- ROUND %d ---\n", round + 1);
            playTurn(0, scores, used);
        }

        int total = 0;
        for (int i = 0; i < CATEGORIES; i++)
            total += scores[0][i];

        total += upperBonus(scores[0]);
        printf("\nFINAL SCORE: %d\n", total);
    }
    else if (mode == 2) {
        for (int round = 0; round < CATEGORIES; round++) {
            printf("\n--- ROUND %d ---\n", round + 1);
            playTurn(0, scores, used);
            playTurn(1, scores, used);
        }

        int total1 = 0, total2 = 0;
        for (int i = 0; i < CATEGORIES; i++) {
            total1 += scores[0][i];
            total2 += scores[1][i];
        }

        total1 += upperBonus(scores[0]);
        total2 += upperBonus(scores[1]);

        printf("\nFINAL SCORES\n");
        printf("Player 1: %d\n", total1);
        printf("Player 2: %d\n", total2);

        if (total1 > total2)
            printf("Player 1 WINS!\n");
        else if (total2 > total1)
            printf("Player 2 WINS!\n");
        else
            printf("DRAW GAME!\n");
    }
    else {
        printf("Invalid choice!\n");
    }

    return 0;
}

/* Roll dice */
void rollDice(int dice[]) {
    for (int i = 0; i < DICE; i++)
        dice[i] = rand() % 6 + 1;
}

/* Display dice */
void displayDice(int dice[]) {
    printf("Dice: ");
    for (int i = 0; i < DICE; i++)
        printf("%d ", dice[i]);
    printf("\n");
}

/* Count frequency */
void countFrequency(int dice[], int freq[]) {
    for (int i = 0; i <= 6; i++)
        freq[i] = 0;

    for (int i = 0; i < DICE; i++)
        freq[dice[i]]++;
}

/* Calculate score */
int calculateScore(int category, int dice[], int freq[]) {
    int sum = 0;
    for (int i = 0; i < DICE; i++)
        sum += dice[i];

    switch (category) {
    case ONES: return freq[1];
    case TWOS: return freq[2] * 2;
    case THREES: return freq[3] * 3;
    case FOURS: return freq[4] * 4;
    case FIVES: return freq[5] * 5;
    case SIXES: return freq[6] * 6;

    case THREE_KIND:
        for (int i = 1; i <= 6; i++)
            if (freq[i] >= 3) return sum;
        return 0;

    case FOUR_KIND:
        for (int i = 1; i <= 6; i++)
            if (freq[i] >= 4) return sum;
        return 0;

    case FULL_HOUSE: {
        int three = 0, two = 0;
        for (int i = 1; i <= 6; i++) {
            if (freq[i] == 3) three = 1;
            if (freq[i] == 2) two = 1;
        }
        return (three && two) ? 25 : 0;
    }

    case SMALL_STRAIGHT:
        if ((freq[1] && freq[2] && freq[3] && freq[4]) ||
            (freq[2] && freq[3] && freq[4] && freq[5]) ||
            (freq[3] && freq[4] && freq[5] && freq[6]))
            return 30;
        return 0;

    case LARGE_STRAIGHT:
        if ((freq[1] && freq[2] && freq[3] && freq[4] && freq[5]) ||
            (freq[2] && freq[3] && freq[4] && freq[5] && freq[6]))
            return 40;
        return 0;

    case YAHTZEE:
        for (int i = 1; i <= 6; i++)
            if (freq[i] == 5) return 50;
        return 0;

    case CHANCE:
        return sum;
    }
    return 0;
}

/* Display scoring menu */
void showScoreMenu(int player, int used[][CATEGORIES], int dice[], int freq[]) {
    const char *names[CATEGORIES] = {
        "Ones", "Twos", "Threes", "Fours", "Fives", "Sixes",
        "Three of a Kind", "Four of a Kind", "Full House",
        "Small Straight", "Large Straight", "Yahtzee", "Chance"
    };

    printf("\n=========== SCORE MENU (Player %d) ===========\n", player + 1);
    printf("No  Category            Status   Score\n");
    printf("----------------------------------------------\n");

    for (int i = 0; i < CATEGORIES; i++) {
        if (used[player][i])
            printf("%2d  %-18s USED     -\n", i, names[i]);
        else
            printf("%2d  %-18s FREE     %d\n", i, names[i],
                   calculateScore(i, dice, freq));
    }
}

/* Player turn */
void playTurn(int player, int scores[][CATEGORIES], int used[][CATEGORIES]) {
    int dice[DICE], freq[7], keep[DICE] = {0};
    int category;

    printf("\n========== PLAYER %d TURN ==========\n", player + 1);

    rollDice(dice);

    for (int roll = 1; roll <= 3; roll++) {
        printf("\nRoll %d:\n", roll);
        displayDice(dice);

        if (roll == 3) break;

        printf("Enter 1 to KEEP dice, 0 to REROLL\n");
        for (int i = 0; i < DICE; i++) {
            printf("Keep Dice %d (%d)? ", i + 1, dice[i]);
            scanf("%d", &keep[i]);
        }

        for (int i = 0; i < DICE; i++)
            if (!keep[i])
                dice[i] = rand() % 6 + 1;
    }

    countFrequency(dice, freq);
    showScoreMenu(player, used, dice, freq);

    do {
        printf("Select category number: ");
        scanf("%d", &category);
    } while (category < 0 || category >= CATEGORIES || used[player][category]);

    scores[player][category] = calculateScore(category, dice, freq);
    used[player][category] = 1;

    printf("Score Earned: %d\n", scores[player][category]);
}

/* Upper bonus */
int upperBonus(int scores[]) {
    int sum = 0;
    for (int i = ONES; i <= SIXES; i++)
        sum += scores[i];
    return (sum >= 63) ? 35 : 0;
}
