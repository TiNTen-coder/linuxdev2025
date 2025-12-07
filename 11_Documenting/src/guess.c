#include <stdio.h>
#include <err.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <libintl.h>
#include <locale.h>
#include <wchar.h>
#include "roman.h"
#include "config.h"
#include "argp.h"

#define _(STRING) gettext(STRING)

struct arguments {
    int roman;
};

static error_t parse_opt (int key, char *arg, struct argp_state *state) {
    struct arguments *arguments = state->input;
    switch (key) {
        case 'r':
            arguments->roman = 1;
            break;
        case 'v':
            arguments->roman = 1;
            break;
        case ARGP_KEY_END:
            if (state->arg_num != 0)
                argp_usage (state);
            break;
        default:
            return ARGP_ERR_UNKNOWN;
    }
    return 0;
}


/** @page guess
 * Guessing number program
 * @section SYNOPSIS
 * `guess` [ \a -option ]
 * @section DESCRIPTION
 * Generate a fantasy name (or a \a number of names, if \a number is provided) according to \a pattern.
 *
 * @copydetails library
 */
int main(int argc, char **argv) {
    int left = 1, right = 100, middle, ret_code;
    char *buf=0;;
    wchar_t wanswer[2] = {'\0', '\0'};
    char answer[10];
    setlocale(LC_ALL, "");
    bindtextdomain(PACKAGE, LOCALE_PATH);
    textdomain(PACKAGE);

    argp_program_version = VERSION;
    struct argp_option options[] = {
        {"roman", 'r', 0, 0,  _("Use roman numerals in programm"), 0},
        { 0 }
    };

    struct arguments arguments = {0};
    struct argp argp = {options, parse_opt, 0, _("Program to guess numbers"), 0, 0, 0};
    
    textdomain("libc");
    argp_parse(&argp, argc, argv, 0, 0, &arguments);
    textdomain(PACKAGE);

    if (arguments.roman)
        printf(_("Pick a number between I and C\n"));
    else
        printf(_("Pick a number between 1 and 100\n"));

    for (;;) {
        middle = (right - left) / 2 + left;
        to_roman(middle, &buf);
        if (arguments.roman)
            printf(_("Is number bigger %s, y/n?\n"), buf);
        else
            printf(_("Is number bigger %d, y/n?\n"), middle);
        ret_code = wscanf(L" %lc", &wanswer[0]);
        if (ret_code == EOF) {
            exit(0);
        }
        if (ret_code == 0) {
            printf(_("Repeat your answer\n"));
            continue;
        }
        if (ret_code == 1) {
            wcstombs(answer, wanswer, 10);
            if (!strcmp(answer, _("y"))) {
                left = middle;
                if ((right - left) == 1) {
                    to_roman(right, &buf);
                    if (arguments.roman)
                        printf(_("Your number is %s\n"), buf);
                    else
                        printf(_("Your number is %d\n"), right);
                    exit(0);
                }
                continue;
            }
            if (!strcmp(answer, _("n"))) {
                right = middle;
                if ((right - left) == 1) {
                    if (arguments.roman)
                        printf(_("Your number is %s\n"), buf);
                    else
                        printf(_("Your number is %d\n"), middle);
                    exit(0);
                }
                continue;
            }
            printf(_("Repeat your answer\n"));
            for (;;) {
                ret_code = wscanf(L"%lc", &wanswer[0]);
                if (wanswer[0] == L'\n')
                    break;
            }
        }

    }
    free(*buf);
}