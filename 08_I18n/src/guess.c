#include <libintl.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define _(string) gettext(string)

int main() {
    setlocale(LC_ALL, "");
    bindtextdomain("guess", "locale");
    textdomain("guess");
    bind_textdomain_codeset("guess", "UTF-8");

    int low = 1;
    int high = 100;
    int mid = (low + high) / 2;

    printf(_("Pick a number between 1 and 100\n"));

    while (low != high) {
        printf(_("Is it greater than %d (y/n)?\n"), mid);

        size_t answer_size = 0;
        char* answer = 0;

        int err = 0;
        while ((err = getline(&answer, &answer_size, stdin)) != -1) {
            if (strncmp(answer, _("y"), strlen(answer) - 1) == 0) {
                low = mid + 1;
                break;
            } else if (strncmp(answer, _("n"), strlen(answer) - 1) == 0) {
                high = mid;
                break;
            } else {
                printf(_("Invalid answer, use (y/n)\n"));
            }
        }

        free(answer);

        if (err == -1) {
            printf(_("Failed reading answer\n"));
            break;
        }

        mid = (low + high) / 2;
    }

    printf(_("You picked %d\n"), mid);

    return 0;
}