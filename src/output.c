/*
 * Offer Password (opass): output.c
 *
 * Simple c program to offer password suggestions based on a collection of three
 * letter words See: https://github.com/wiremoons/opass
 *
 * MIT License
 *
 */

#include "output.h"

#include <stdio.h> /* printf */


/*
 * show_help displays a summary of the command line switches
 * available in the program.
 */
void show_help() {
    printf(""
           "\nOffer Password (opass) Help.\n\n"
           "A number of command line flags can be specified which are listed below.\n"
           "Additionally environment variables can be used to control offered password configuration:\n\n"
           " - OPASS_WORDS=<number of three letter words to include>\n"
           " - OPASS_NUM=<number of passwords to offer>\n\n"
           "These can be set in the shell, or just given when needed on the command line.\n\n\n"
           "Example usage 1:  ./opass\n\n"
           "Shows default usage provides the equivalent command of:  OPASS_WORDS=3 OPASS_NUM=5 ./opass\n\n"
           "Example usage 2:  OPASS_WORDS=7 OPASS_NUM=8 ./opass\n\n"
           "Usage ensures a minimum of seven random three letter words are included and eight different\n"
           "password choices will be offered to the user to select from.\n\n\n"
           "Help Summary: the following command line switches can be used:\n\n"
           "  -e, --export     Dump the full list of three letter words and marks.\n"
           "  -h, --help       Show this help information.\n"
           "  -q, --quick      Just offer a password and no other output.\n"
           "  -v, --version    Display the version of the program and password stats.\n");
}


/*
 * show_version displays a summary of the version information
 * available in the program.
 */
void show_version(char *program_name, int numPassSuggestions, int wordsRequired) {
    printf("%s is: %s\n", program_name, version);

    /* display some stats about passwords being generated */
    printf("\nApplication Password Stats:\n");
    printf("  - Number of three letter words available: ");
    printf("%d\n", wordArraySize);
    printf("  - Number of marks (#..@) available: ");
    printf("%d\n", marksArraySize);
    printf("  - Number of different password suggestions to offer: ");
    printf("%d\n", numPassSuggestions);
    printf("  - Number of words per suggested password: ");
    printf("%d\n", wordsRequired);
    printf("  - Number 0f three letter words total length will be: ");
    printf("%d\n", (wordsRequired * 3));
    printf("  - Total offered password length will be: ");
    printf("%d\n\n", ((wordsRequired * 3)) + 3);

}

/*
 * dump out the complete list of words and marks held in our arrays
 */
void dump_words() {
    int i = 0;
    int m = 0;

    printf("Words used:\n");
    while (i < wordArraySize) {
        printf("%s ", *(words + i));
        i++;
    }
    printf("\n");

    printf("Marks used:\n");
    while (m < marksArraySize) {
        printf("%s ", *(marks + m));
        m++;
    }
    printf("\n");
}