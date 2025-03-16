#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//
// Created by Liubov Udalova on 09.02.25.
//
struct Cell {
    int set;
    bool rightWall;
    bool downWall;
};

struct Maze {
    int rowLength;
    int rowCount;
    struct Cell *cells; // Pointer to first element of array[][]
};

/**
 * Выводит на экран ряд лабиринта.
 * @param row       массив {@Cell}, представляющий ряд лабиринта
 * @param rowLength длина ряда
 * @param showSet   флаг, выводить ли значение множества
 */
void printRow(struct Cell *row, int rowLength, bool showSet) {
    printf("|");
    for (int i = 0; i < rowLength; i++) {
        if (row[i].downWall) {
            showSet ? printf("_%d_", row[i].set) : printf("___");
        } else {
            showSet ? printf(" %d ", row[i].set) : printf("   ");
        }
        if (row[i].rightWall || i == rowLength - 1) {
            printf("|");
        } else {
            printf(" ");
        }
    }
}

/**
 * Выводит на экран весь лабиринт.
 * @param maze    указатель на {@Maze}
 * @param showSet флаг, выводить ли значение множества
 */
void printMaze(struct Maze *maze, bool showSet) {
    printf("_");
    for (int i = 0; i < maze->rowLength; i++) {
        printf("____");
    }
    printf("\n");

    for (int i = 0; i < maze->rowCount; i++) {
        printRow(maze->cells + i * maze->rowLength, maze->rowLength, showSet);
        printf("\n");
    }
}

/**
 * Инициализирует массив boll, представляющий собой набор уникальных множеств, использованных в ряду
 * лабиринта, где индекс элемента массива - значение множества, а значение элемента массива - флаг,
 * использовано ли множество или нет.
 * @param row        массив {@Cell}, представляющий ряд лабиринта
 * @param rowLength  длина ряда
 * @param uniqueSets массив множеств ячеек
 * @note Соблюдаем контракт, что допустимые значения для множеств ячеек лежат в диапазоне [1, rowLength],
 * иначе можно выйти за границы массива uniqueSets
 */
void initUniqueSets(struct Cell *row, int rowLength, bool *uniqueSets) {
    for (int i = 0; i < rowLength; i++) {
        uniqueSets[i] = false;
    }
    for (int i = 0; i < rowLength; i++) {
        int cellSet = row[i].set;
        if (cellSet != 0) {
            uniqueSets[cellSet - 1] = true;
        }
    }
}

/**
 * Возвращает неиспользованное значение множества и помечает его как использованное.
 * @param uniqueSets массив множеств ячеек
 * @param length     длина массива
 * @return значение свободного множества
 */
int allocUniqueSet(bool *uniqueSets, int length) {
    for (int i = 0; i < length; i++) {
        if (!uniqueSets[i]) {
            uniqueSets[i] = true;
            return i + 1;
        }
    }
    return -1;
}

/**
 * Инициализирует ряд лабиринта с помощью алгоритма Эллера.
 * @param row       массив {@Cell}, представляющий ряд лабиринта
 * @param rowLength длина ряда
 */
void initRow(struct Cell *row, int rowLength) {
    bool *uniqueSets = malloc(sizeof(bool) * rowLength);
    initUniqueSets(row, rowLength, uniqueSets);

    //Присваиваем ячейкам уникальные множества
    for (int i = 0; i < rowLength; i++) {
        if (row[i].set == 0) {
            row[i].set = allocUniqueSet(uniqueSets, rowLength);
        }
    }
    //Создаем границы справа
    for (int i = 0; i < rowLength; i++) {
        if (i + 1 == rowLength) {
            row[i].rightWall = true;
            break;
        }
        if (row[i].set == row[i + 1].set) {
            row[i].rightWall = true;
        } else {
            bool willAddWall = rand() % 2;
            if (willAddWall) {
                row[i].rightWall = true;
            } else {
                row[i + 1].set = row[i].set;
            }
        }
    }
    //Создаем границы снизу
    int countSetItems = 0;
    int countDownWalls = 0;
    int currentSet = 0;
    for (int i = 0; i < rowLength; i++) {
        //При переходе в новое множество
        if (i != 0 && row[i].set != currentSet) {
            if (countDownWalls == countSetItems) {
                row[i - 1].downWall = false;
            }
            countSetItems = 0;
            countDownWalls = 0;
        }
        bool willAddWall = rand() % 2;
        if (willAddWall) {
            row[i].downWall = true;
            countDownWalls++;
        }
        countSetItems++;
        currentSet = row[i].set;
    }
    //Проверка последней ячейки ряда
    if (countDownWalls == countSetItems) {
        row[rowLength - 1].downWall = false;
    }
    free(uniqueSets);
}

/**
 * Генерирует лабиринт с помощью алгоритма Эллера.
 * @param maze указатель на {@Maze}
 * @param seed инициализатор рандомной функции
 */
void generateMaze(struct Maze *maze, unsigned seed) {
    srand(seed);
    for (int i = 0; i < maze->rowLength; i++) {
        maze->cells[i].set = 0;
        maze->cells[i].rightWall = false;
        maze->cells[i].downWall = false;
    }
    //Создаем первую строку
    initRow(maze->cells, maze->rowLength);

    for (int i = 1; i < maze->rowCount; i++) {
        for (int j = 0; j < maze->rowLength; j++) {
            struct Cell *cell = &(maze->cells + i * maze->rowLength)[j];
            *cell = (maze->cells + (i - 1) * maze->rowLength)[j];
            cell->rightWall = false;
            if (cell->downWall) {
                cell->set = 0;
                cell->downWall = false;
            }
        }
        initRow(maze->cells + i * maze->rowLength, maze->rowLength);
    }
    //Завершаем последнюю строку
    struct Cell *lastRow = (maze->cells + (maze->rowCount - 1) * maze->rowLength);
    for (int i = 0; i < maze->rowLength - 1; i++) {
        if (lastRow[i].set != lastRow[i + 1].set) {
            lastRow[i].rightWall = false;
            lastRow[i + 1].set = lastRow[i].set;
        }
        lastRow[i].downWall = true;
    }
    lastRow[maze->rowLength - 1].downWall = true;
}

/**
 * Генерирует лабиринт с помощью алгоритма Эллера.
 * @param maze указатель на {@Maze}
 */
void generateRandomMaze(struct Maze *maze) {
    generateMaze(maze, time(NULL));
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: ./main <rowLength> <rowCount> [<showSet>]\n");
        return -1;
    }

    const int rowLength = atoi(argv[1]);
    const int rowCount = atoi(argv[2]);

    bool showSet = false;
    if (argc > 3) {
        showSet = atoi(argv[3]);
    }

    if (rowLength < 3) {
        printf("Row length must be greater than 2\n");
        return -1;
    }
    if (rowCount < 3) {
        printf("Row count must be greater than 2\n");
        return -1;
    }

    struct Cell *cells = malloc(sizeof(struct Cell) * rowLength * rowCount);
    struct Maze maze = {
        .rowLength = rowLength,
        .rowCount = rowCount,
        .cells = cells
    };
    generateRandomMaze(&maze);
    printMaze(&maze, showSet);
    return 0;
}
