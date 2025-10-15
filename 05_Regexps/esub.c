#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include <stdbool.h>

#define MAX_ERROR_MSG 256
#define MAX_MATCHES 10

void print_regex_error(int errcode, regex_t *compiled) {
    char error_message[MAX_ERROR_MSG];
    regerror(errcode, compiled, error_message, sizeof(error_message));
    fprintf(stderr, "Regex error: %s\n", error_message);
}

char* process_substitution(const char *input, const char *pattern, const char *replacement) {
    regex_t regex;
    regmatch_t matches[MAX_MATCHES];
    int reti;
    
    reti = regcomp(&regex, pattern, REG_EXTENDED);
    if (reti) {
        print_regex_error(reti, &regex);
        return NULL;
    }
    
    size_t input_len = strlen(input);
    size_t repl_len = strlen(replacement);
    size_t result_size = input_len + repl_len * 10 + 100;
    char *result = malloc(result_size);
    if (!result) {
        regfree(&regex);
        return NULL;
    }
    
    result[0] = '\0';
    
    if (regexec(&regex, input, MAX_MATCHES, matches, 0) == 0) {
        strncat(result, input, matches[0].rm_so);
        
        for (size_t i = 0; replacement[i]; i++) {
            if (replacement[i] == '\\' && replacement[i+1] >= '0' && replacement[i+1] <= '9') {
                int group_num = replacement[i+1] - '0';
                if (group_num < MAX_MATCHES && matches[group_num].rm_so != -1) {
                    size_t group_len = matches[group_num].rm_eo - matches[group_num].rm_so;
                    strncat(result, input + matches[group_num].rm_so, group_len);
                }
                i++;
            } else if (replacement[i] == '\\' && replacement[i+1] == '\\') {
                size_t len = strlen(result);
                result[len] = '\\';
                result[len + 1] = '\0';
                i++;
            } else {
                size_t len = strlen(result);
                result[len] = replacement[i];
                result[len + 1] = '\0';
            }
        }
        
        strcat(result, input + matches[0].rm_eo);
    } else {
        strcpy(result, input);
    }
    
    regfree(&regex);
    return result;
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <string> <regexp> <substitution>\n", argv[0]);
        return 1;
    }
    
    const char *input = argv[1];
    const char *pattern = argv[2];
    const char *replacement = argv[3];
    
    char *result = process_substitution(input, pattern, replacement);
    
    if (result) {
        printf("%s\n", result);
        free(result);
        return 0;
    } else {
        return 1;
    }
}
