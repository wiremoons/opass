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
           "  -e    Export  - dump the full list of three letter words\n"
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
void show_version(char *program_name, int numPassSuggestions, int marksArraySize, int wordsRequired) {
    printf("%s is: %s\n", program_name, version);

    /* display some stats about passwords being generated */
    printf("Application Stats:\n");
    printf("  - Number of three letter words available: ");
    printf("%d\n", wordArraySize);
    printf("  - Number of marks (#..@) available: ");
    printf("%d\n", marksArraySize);
    printf("  - Number of words per suggested password: ");
    printf("%d\n", wordsRequired);
    printf("  - Password character length will be: ");
    printf("%d\n", (wordsRequired * 3));
    printf("  - Number of password suggestions to offer: ");
    printf("%d\n\n", numPassSuggestions);
}


/*
 * dump out the complete list of words held in our words array
 */
void dump_words() {
    int i = 0;

    while (i < wordArraySize) {
        printf("%s ", *(words + i));
        i++;
    }
    printf("\n");
}