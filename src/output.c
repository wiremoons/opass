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
#include <ctype.h>
#include <string.h>

/*
 * show_password outputs the generated password using colours
 * unless disabled via the environment variable:
 */
void show_password(char *out_password) {
    if ((strlen(out_password) <= 0) || (NULL == out_password)) {
        printf("\nERROR\n");
        return;
    }
    #if DEBUG
    printf("\nProcessing: '%s' which has length: '%d'\n",out_password,(int) strlen(out_password));
    #endif

    int test_len = 0;

    while (*out_password != '\0') {
        if (isdigit(*out_password)){
            #if DEBUG
            printf(" DIGIT: [%c] ",*out_password);
            #endif
            printf("%c",*out_password);
        } else if (ispunct(*out_password)){
            #if DEBUG
            printf(" MARK: [%c] ",*out_password);
            #endif
            printf("%c",*out_password);
        } else {
            #if DEBUG
            printf(" default: [%c] ",*out_password);
            #endif
            printf("%c",*out_password);
        }
        test_len++;
        out_password++;
    }
    #if DEBUG
    printf("\nDONE PROCESSING  ['%d' chars]\n",test_len);
    #endif
}

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
           "password choices will be offered to the user to select from.\n\n"
           "For Windows 'cmd.exe' use:     set \"OPASS_WORDS=7\" & set \"OPASS_NUM=8\" & opass\n"
           "For Windows 'Powershell' use:  $env:OPASS_WORDS=7 ; $env:OPASS_NUM=8 ; ./opass\n\n\n"
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

    /* Check build flag used when program was compiled */
    #if DEBUG
        char Build_Type[] = "Debug";
    #else
        char Build_Type[] = "Release";
    #endif

    printf("\n'%s' is version: '%s'.\n", program_name, version);
    printf("Compiled on: '%s @ %s' with C source built as '%s'.\n",__DATE__,__TIME__,Build_Type);
    puts("Copyright (c) 2021 Simon Rowe.\n");
    puts("For licenses and further information visit:");

    puts("- https://github.com/wiremoons/opass/");

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
        printf("%d ", *(marks + m));
        m++;
    }
    printf("\n");
}


// TODO: detect compiler type for 'Show_version()' output
//#if defined(_MSC_VER)
//   char Compiler_Type{} = _MSC_VER;
//#elif defined(__clang__)
//    CLANG __clang_major__ __clang_minor__
//#elif defined(__GNUG__)
//    GCC __GNUC__ __GNUC_MINOR__
//#else
//    UNKNOWN 0 0
//#endif
