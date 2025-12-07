#include "roman.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "regex.h"

typedef struct {
    int value;
    char * letter;
} roman_pair;


const roman_pair roman_map[] = {{1000, "M"}, {900, "CM"}, {500, "D"}, {400, "CD"}, {100, "C"}, {90, "XC"}, {50, "L"}, {40, "XL"}, {10, "X"}, {9, "IX"}, {5, "V"}, {4, "IV"}, {1, "I"}}; /**< roman letters mapping */

/** Convert arabic value @p val to string @p buf 
 *
 * @param val arabic value
 * @param buf buffer that holds string
 * 
 * @p buf can be passed a zero to initialize a buffer enough to hold roman value
 * however you should free this memory after use
 */
void to_roman(int val, char **buf) {
    if ((val < 1) || (val >= 5000))
        return NULL;

    if (*buf == 0) {
        *buf = malloc(sizeof(char) * 17);
    }
    memset(*buf, 0, 17);
    for(int i = 0; i < sizeof(roman_map)/sizeof(roman_map[0]); i++) {
        int integer = roman_map[i].value;
        char *numeral = roman_map[i].letter;
        while (val >= integer) {
            strcat(*buf, numeral);
            val -= integer;
        }
    }
}

/** Checks @p str to be a valid roman string in range (1..5000)
 *
 * @param str string containing roman value
 * @return arabic value returns -1 if fails to match
 */
int check_roman(const char *str) {
    regex_t regex;
    static char roman_pattern[] = "^M{0,4}(CM|CD|D?C{0,3})(XC|XL|L?X{0,3})(IX|IV|V?I{0,3})$";
    regcomp(&regex, roman_pattern, REG_EXTENDED);
    if (regexec(&regex, str, 0, NULL, 0) == REG_NOMATCH) {
        return -1;
    }
    return 1;
}

/**  Gets slice of string @p str from @p start to @p end
 *
 * @param str string to slice
 * @param start start
 * @param end end
 */
void slice(char* str, char* result, size_t start, size_t end) {
    strncpy(result, str + start, end - start);
}

/** Converts roman string @p str to arabic value 
 *
 * @param str string containing roman value
 * @return arabic value returns -1 if fails to match
 */
int from_roman(const char *str) {
    static char buf[17];
    static char sliced[17];
    strncpy(buf, str, 17);
    for(int i=0; i < 17; i++)
        buf[i] = toupper((unsigned char) buf[i]);
    
    if (check_roman(str) == -1) {
        return -1;
    }

    int result = 0;
    int index = 0;
    for(int i = 0; i < sizeof(roman_map)/sizeof(roman_map[0]); i++) {
        int integer = roman_map[i].value;
        char *numeral = roman_map[i].letter;
        slice(buf, sliced, index, index + strlen(numeral));
        while (strcmp(sliced, numeral) == 0) {
            result += integer;
            index += strlen(numeral);
        }
    }
    return result;
}