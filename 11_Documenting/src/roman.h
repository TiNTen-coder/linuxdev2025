/** @mainpage Guessing program
 * @copydetails library
 */
/** @page library Guessing program library 
 * This is free and unencumbered software released into the public domain.
 * 
 * Usage: guess [OPTION...]
 *
 *  - -r, -\-roman                - Use roman numerals in programm
 *  - -?, -\-help                 - Give this help list
 *  -     -\-usage                - Give a short usage message
 *  -  -V,-\-version              - Print program version
 *
 * Option -r/-\-roman allows to play game when numbers are roman
 *
 * In the start of the game you should pick a number and answer to program's questions
 * whether it is bigger or smaller 
 *
 */

void to_roman(int val, char **buf);

int from_roman(const char *str);