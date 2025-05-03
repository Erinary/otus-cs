#include <stdio.h>
#include <stdbool.h>

#include "ast.h"
#include "generated/lexer.h"
#include "generated/parser.h"

typedef struct {
    bool debug;
    const char **samples;
    int sample_count;
} ParserConfig;

typedef struct {
    bool success;
    double result;
} EvaluationResult;

EvaluationResult evaluate(const char *line, const ParserConfig *config) {
    yy_scan_string(line);
    if (yyparse() == 0 && singleton_ast) {
        if (config->debug) {
            printf("=== AST ===\n");
            print_ast(singleton_ast, 0);
        }

        const double result = execute_ast(singleton_ast);
        free_ast(singleton_ast);
        singleton_ast = NULL;
        return (EvaluationResult){true, result};
    } else {
        return (EvaluationResult){false, 0.0};
    }
}

int main(int argc, char **argv) {
    ParserConfig config = {
        .debug = false,
        .samples = NULL,
        .sample_count = 0
    };

    // Default sample strings
    static const char *default_samples[] = {
        "1 + 2",
    };

    // Parse CLI flags
    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "-d") == 0) {
            config.debug = true;
        } else if (strcmp(argv[i], "-s") == 0) {
            config.samples = default_samples;
            config.sample_count = sizeof(default_samples) / sizeof(default_samples[0]);
        } else {
            fprintf(stderr, "Unknown flag: %s\n", argv[i]);
            return 1;
        }
    }

    // Use samples if provided
    if (config.samples) {
        for (int i = 0; i < config.sample_count; ++i) {
            fprintf(stderr, "Sample #%d: %s\n", i, config.samples[i]);
            const EvaluationResult result = evaluate(config.samples[i], &config);
            if (result.success) {
                fprintf(stderr, "Result: %f\n", result.result);
            } else {
                fprintf(stderr, "Error evaluating command");
            }
        }
    } else {
        // Read from stdin line by line
        char input[2048];
        printf("Enter expressions (Ctrl+D to quit):\n");

        while (fgets(input, sizeof(input), stdin)) {
            const size_t len = strlen(input);
            if (len > 0 && input[len - 1] == '\n') input[len - 1] = '\0';
            if (strlen(input) == 0) continue;

            const EvaluationResult result = evaluate(input, &config);
            if (result.success) {
                fprintf(stderr, "Result: %f\n", result.result);
            } else {
                fprintf(stderr, "Error evaluating command");
            }
        }
    }

    return 0;
}