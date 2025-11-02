#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <rhash.h>

#ifdef HAVE_READLINE
#include <readline/readline.h>
#include <readline/history.h>
#endif

#define MAX_LINE 1024
#define HASH_COUNT 3

typedef struct {
    const char *name;
    unsigned int id;
} HashAlgo;

static const HashAlgo algorithms[] = {
    {"MD5", RHASH_MD5},
    {"SHA1", RHASH_SHA1},
    {"TTH", RHASH_TTH}
};

int hash_string(const char *str, unsigned int hash_id,
                char *output, int use_base64) {
    unsigned char digest[64];
    int digest_size = rhash_get_digest_size(hash_id);

    if (rhash_msg(hash_id, str, strlen(str), digest) < 0) {
        return -1;
    }

    if (use_base64) {
        int flags = RHPR_BASE64;
        rhash_print_bytes(output, digest, digest_size, flags);
    } else {
        int flags = RHPR_HEX;
        rhash_print_bytes(output, digest, digest_size, flags);
    }

    return 0;
}

int hash_file(const char *filename, unsigned int hash_id,
              char *output, int use_base64) {
    unsigned char digest[64];
    int digest_size = rhash_get_digest_size(hash_id);

    if (rhash_file(hash_id, filename, digest) < 0) {
        return -1;
    }

    if (use_base64) {
        int flags = RHPR_BASE64;
        rhash_print_bytes(output, digest, digest_size, flags);
    } else {
        int flags = RHPR_HEX;
        rhash_print_bytes(output, digest, digest_size, flags);
    }

    return 0;
}

void process_command(const char *algo_name, const char *input) {
    unsigned int hash_id = 0;
    int found = 0;
    int use_base64 = 0;

    if (islower((unsigned char)algo_name[0])) {
        use_base64 = 1;
    }

    for (int i = 0; i < HASH_COUNT; i++) {
        if (strcasecmp(algo_name, algorithms[i].name) == 0) {
            hash_id = algorithms[i].id;
            found = 1;
            break;
        }
    }

    if (!found) {
        fprintf(stderr, "Unknown algorithm: %s\n", algo_name);
        return;
    }

    int is_string = 0;
    char processed_input[MAX_LINE];

    if (input[0] == '"') {
        is_string = 1;
        strncpy(processed_input, input + 1, MAX_LINE - 1);
        processed_input[MAX_LINE - 1] = '\0';

        size_t len = strlen(processed_input);
        if (len > 0 && processed_input[len - 1] == '"') {
            processed_input[len - 1] = '\0';
        }
    } else {
        is_string = 0;
        strncpy(processed_input, input, MAX_LINE - 1);
        processed_input[MAX_LINE - 1] = '\0';
    }

    char output[256];
    int result;

    if (is_string) {
        result = hash_string(processed_input, hash_id, output, use_base64);
        if (result < 0) {
            fprintf(stderr, "Error hashing string\n");
            return;
        }
    } else {
        result = hash_file(processed_input, hash_id, output, use_base64);
        if (result < 0) {
            fprintf(stderr, "Error reading file: %s\n", processed_input);
            return;
        }
    }

    printf("%s\n", output);
}

void execute_line(const char *line) {
    char algo[64];
    char input[MAX_LINE];

    if (line[0] == '\0' || line[0] == '\n' || line[0] == '#') {
        return;
    }

    char line_copy[MAX_LINE];
    strncpy(line_copy, line, MAX_LINE - 1);
    line_copy[MAX_LINE - 1] = '\0';

    char *token = strtok(line_copy, " \t");
    if (token == NULL) {
        return;
    }
    strncpy(algo, token, 63);
    algo[63] = '\0';

    token = strtok(NULL, "");
    if (token == NULL) {
        fprintf(stderr, "Missing input for algorithm: %s\n", algo);
        return;
    }

    while (*token == ' ' || *token == '\t') {
        token++;
    }

    strncpy(input, token, MAX_LINE - 1);
    input[MAX_LINE - 1] = '\0';

    process_command(algo, input);
}

int main(int argc, char *argv[]) {
    rhash_library_init();

    if (argc > 1) {
        if (argc >= 3) {
            process_command(argv[1], argv[2]);
        } else {
            fprintf(stderr, "Usage: %s <algorithm> <input>\n", argv[0]);
            fprintf(stderr, "   or: %s (for interactive mode)\n\n", argv[0]);
            fprintf(stderr, "Algorithms: MD5, SHA1, TTH (or md5, sha1, tth for Base64)\n\n");
            fprintf(stderr, "Input format:\n");
            fprintf(stderr, "  - String: starts with \" (e.g., \"hello\")\n");
            fprintf(stderr, "  - File:   anything else (e.g., file.txt)\n\n");
            fprintf(stderr, "Examples:\n");
            fprintf(stderr, "  %s MD5 '\"Hello, World!\"'  # Hash string, HEX output\n", argv[0]);
            fprintf(stderr, "  %s md5 '\"Hello, World!\"'  # Hash string, Base64 output\n", argv[0]);
            fprintf(stderr, "  %s SHA1 file.txt         # Hash file, HEX output\n", argv[0]);
            fprintf(stderr, "  %s sha1 file.txt         # Hash file, Base64 output\n", argv[0]);
            return 1;
        }
    } else {
#ifdef HAVE_READLINE
        char *line;
        printf("RHash Calculator (type 'exit' to quit)\n");
        printf("Format: <ALGORITHM> <input>\n");
        printf("  - Uppercase algorithm (MD5) = HEX output\n");
        printf("  - Lowercase algorithm (md5) = Base64 output\n");
        printf("  - String starts with \": \"hello\"\n");
        printf("  - File: filename.txt\n\n");

        while ((line = readline("rhasher> ")) != NULL) {
            if (strlen(line) > 0) {
                add_history(line);
            }

            if (strcmp(line, "exit") == 0 || strcmp(line, "quit") == 0) {
                free(line);
                break;
            }

            execute_line(line);
            free(line);
        }
        printf("\n");
#else
        char buffer[MAX_LINE];
        printf("RHash Calculator (type 'exit' to quit)\n");
        printf("Format: <ALGORITHM> <input>\n");
        printf("  - Uppercase algorithm (MD5) = HEX output\n");
        printf("  - Lowercase algorithm (md5) = Base64 output\n\n");
        printf("rhasher> ");
        fflush(stdout);

        while (fgets(buffer, MAX_LINE, stdin) != NULL) {
            buffer[strcspn(buffer, "\n")] = 0;

            if (strcmp(buffer, "exit") == 0 || strcmp(buffer, "quit") == 0) {
                break;
            }

            execute_line(buffer);
            printf("rhasher> ");
            fflush(stdout);
        }
        printf("\n");
#endif
    }

    return 0;
}
