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
           "\nOffer Password (opass)\n\n"
           "Help Summary: the following command line switches can be used:\n\n"
           "  -e    Export  - dump the full list of three letter words and marks\n"
           "  -h    Help    - show this help information\n"
           "  -q    Quick   - just offer a password and no other output\n"
           "  -v    Version - display the version of the program\n"
           "  -w ?  Words   - specify how many three letter words make up "
           "the password (? == digits)\n"
           "  -s ?  Suggest - specify how different passwords to offer (? "
           "== digits)\n");
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