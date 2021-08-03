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

#include <stdio.h>   /* printf */
#include <stdlib.h>  /* getenv */
#include <ctype.h>   /* toupper */
#include <string.h>


/**
 * @brief Outputs the `out_password` char string provided using ANSI colours, unless disabled via 'NO_COLOR' env variable.
 * @param out_password : the password string to be output to screen
 * @return no return
 */
void show_password(char *out_password) {
    if ((strlen(out_password) <= 0) || (NULL == out_password)) {
        fprintf(stderr,"\nERROR: password to be displayed is zero length or NULL\n");
        return;
    }
    #if DEBUG
    printf("\nProcessing: '%s' which has length: '%d'\n",out_password,(int) strlen(out_password));
    #endif

    /* respect the NO_COLOR environment setting as: https://no-color.org/ */
    if ( getenv("NO_COLOR") ) {
        #if DEBUG
        printf("\n'NO_COLOR' environment setting found.\n");
        #endif
        printf("%s",out_password);
        #if DEBUG
        printf("\nDONE PROCESSING as NO_COLOR ['%d' chars]\n",(int)strlen(out_password));
        #endif
        return;
    }

    int test_len = 0;

    /* specify the different colours that can be used with the output of marks and numbers */
    //char RED[]="\033[1;31m";
    //char BLUE[]="\033[1;34m";
    char CYAN[]="\033[1;36m";
    char GREEN[]="\033[1;32m";
    char RESET[] = "\033[0m";

    while (*out_password != '\0') {
        if (isdigit(*out_password)){
            #if DEBUG
            printf(" DIGIT: [%c] ",*out_password);
            #endif
            printf("%s%c%s",CYAN,*out_password,RESET);
        } else if (ispunct(*out_password)){
            #if DEBUG
            printf(" MARK: [%c] ",*out_password);
            #endif
            printf("%s%c%s",GREEN,*out_password,RESET);
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

/**
 * @brief Displays a summary of the command line switches available and help for controlling outputs for passwords.
 * @param none
 * @return no return
 */
void show_help() {
    printf(""
           "\nOffer Password (opass) Help.\n\n"
           "A number of command line flags can be specified which are listed below.\n"
           "Additionally environment variables can be used to control offered password configuration:\n\n"
           " - OPASS_WORDS=<number of three letter words to include>\n"
           " - OPASS_NUM=<number of passwords to offer>\n\n"
           "These can be set in the shell, or just given when needed on the command line.\n\n\n"
           "Example usage 1:  opass\n\n"
           "Shows default usage provides the equivalent command of:  OPASS_WORDS=3 OPASS_NUM=5 opass\n\n"
           "Example usage 2:  OPASS_WORDS=7 OPASS_NUM=8 opass\n\n"
           "Usage ensures a minimum of seven random three letter words are included and eight different\n"
           "password choices will be offered to the user to select from.\n\n"
           "For Windows 'cmd.exe' use:     set \"OPASS_WORDS=7\" & set \"OPASS_NUM=8\" & opass\n"
           "For Windows 'Powershell' use:  $env:OPASS_WORDS=7 ; $env:OPASS_NUM=8 ; opass\n\n"
           "Output will use ANSI colour by default. The 'NO_COLOR' environment is respected and colour\n"
           "output is disabled if it is set. See: https://no-color.org/\n"
           "This can also be specified on the command line by running commands as shown below:\n\n"
           "For Windows 'cmd.exe' use:            set \"NO_COLOR=1\" & opass\n"
           "For Windows 'Powershell' use:         $env:NO_COLOR=1 ; opass\n"
           "For macOS, Linux, 'Unix shells' use:  NO_COLOR=1 opass\n\n\n"
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
/**
 * @brief Displays a summary of the version information and stats for passwords based on current settings.
 * @param program_name : the name of the application
 * @param numPassSuggestions : current setting for the number of passwords to be shown to the user to select from
 * @param wordsRequired : current settings for the number of three letter words to be used to construct the password
 * @param version : the current opass version (see `opass.h` defined VERSION)
 * @param wordArraySize : the number of three letter words held in the programs `words[]` array
 * @param marksArraySize : the number of marks held in the programs `marks[]` array
 * @return no return
 */
// TODO : use a struct to pass all the variable below?
void show_version(const char *program_name, const int numPassSuggestions, const int wordsRequired, const char *version, const int wordArraySize, const int marksArraySize ) {

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

    if ( getenv("NO_COLOR") ) {
        puts("\n'NO_COLOR' environment exist as: https://no-color.org/");
    }

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
/**
 * @brief Dump out to the screen the complete list of words and marks held in our programs arrays.
 * @param none
 * @return no return
 */
void dump_words(int wordArraySize, int marksArraySize, char const *words[], int const marks[]) {
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
        printf("%c ", *(marks + m));
        m++;
    }
    printf("\n");
}


// TODO: detect compiler type for additional 'Show_version()' output
//#if defined(_MSC_VER)
//   char Compiler_Type{} = _MSC_VER;
//#elif defined(__clang__)
//    CLANG __clang_major__ __clang_minor__
//#elif defined(__GNUG__)
//    GCC __GNUC__ __GNUC_MINOR__
//#else
//    UNKNOWN 0 0
//#endif
