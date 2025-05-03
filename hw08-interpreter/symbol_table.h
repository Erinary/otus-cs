//
// Created by Liubov Udalova on 03.05.25.
//

#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <stdbool.h>

typedef struct SymbolEntry {
    const char *name;
    double value;
    struct SymbolEntry *next;
} SymbolEntry;

typedef struct {
    bool found;
    double value;
} SymbolLookupResult;

void assign_variable(const char *name, double value);
SymbolLookupResult lookup_variable(const char *name);
void free_symbol_table(void);

#endif //SYMBOL_TABLE_H
