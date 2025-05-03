//
// Created by Liubov Udalova on 03.05.25.
//

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include "symbol_table.h"

static SymbolEntry *head = NULL;

void assign_variable(const char *name, double value) {
    SymbolEntry *current = head;
    while (current) {
        if (strcmp(current->name, name) == 0) {
            current->value = value;
            return;
        }
        current = current->next;
    }

    SymbolEntry *new_entry = malloc(sizeof(SymbolEntry));
    if (!new_entry) {
        perror("assign_variable: malloc failed");
        exit(1);
    }
    new_entry->name = strdup(name);
    new_entry->value = value;
    new_entry->next = head;
    head = new_entry;
}

SymbolLookupResult lookup_variable(const char *name) {
    SymbolEntry *current = head;
    while (current) {
        if (strcmp(current->name, name) == 0) {
            return (SymbolLookupResult){.found = true, .value = current->value};
        }
        current = current->next;
    }
    return (SymbolLookupResult){.found = false, .value = 0.0};
}

void free_symbol_table(void) {
    SymbolEntry *current = head;
    while (current) {
        SymbolEntry *next = current->next;
        free((char *) current->name);
        free(current);
        current = next;
    }
    head = NULL;
}
