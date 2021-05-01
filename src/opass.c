/*
 * Offer Password (opass): opass.c
 *
 * Simple c program to offer password suggestions based on a collection of three
 * letter words See: https://github.com/wiremoons/opass
 *
 * MIT License
 *
 */

#include "opass.h"

#include <ctype.h>  /* isdigit */
#include <errno.h>  /* strerror */
#include <stdio.h>  /* printf, fprintf */
#include <stdlib.h> /* rand, atexit, malloc */
#include <string.h> /* strncat */
#include <time.h>   /* time */
#include <unistd.h> /* getopt */

/*
 * get_cli_args parses the command line arguments provided by the user
 * when the program was run
 */
void get_cli_args(int argc, char **argv)
{
        opterr = 0;
        int c = 0;

        while ((c = getopt(argc, argv, "ehqvw:s:")) != -1) {
                switch (c) {
                /* export of three letter words was requested */
                case 'e':
                        export = 1;
                        break;
                /* provide help summary to user was requested */
                case 'h':
                        help = 1;
                        break;
                /* quick single output was requested */
                case 'q':
                        quick = 1;
                        break;
                /* the version of the application requested */
                case 'v':
                        printf("%s is: %s", argv[0], version);
                        exit(EXIT_SUCCESS);
                /* number of words to include in each password specified */
                case 'w':
                        if (isdigit(*optarg)) {
                                wordsRequired = atoi(optarg);
                        } else {
                                fprintf(stderr,
                                        "ERROR: for -w option please provide "
                                        "numerical digit(s) 1 to 9\n");
                                exit(EXIT_FAILURE);
                        }
                        break;
                /* number of passwords to suggest specified */
                case 's':
                        if (isdigit(*optarg)) {
                                numPassSuggestions = atoi(optarg);
                        } else {
                                fprintf(stderr,
                                        "ERROR: for -s option please provide "
                                        "numerical digit(s) 1 to 9\n");
                                exit(EXIT_FAILURE);
                        }
                        break;
                /* ERROR HANDLING BELOW: command line option
                 * given - but is missing the required data
                 * argument for it */
                case ':':
                        fprintf(stderr,
                                "\nERROR: '%s': option '-%c' requires "
                                "an argument\n",
                                argv[0], optopt);
                        break;
                /* invalid option provided on command line -
                 * also 'default' as the switch fall-thru */
                case '?':
                default:
                        /* invalid option */
                        fprintf(stderr,
                                "\nWARNING: '%s': option '-%c' is "
                                "invalid: ignored\n",
                                argv[0], optopt);
                        break;
                }
        }
}

/*
 * exit_cleanup called when program exits as registed with 'atexit()'
 * in main()
 */

void exit_cleanup()
{
        /* output unless quick (-q) command line option was used */
        if (!quick)
                printf("\nAll is well\n");
}

/*
 * 'with_spaces()' adds spaces at every third character point in the string
 * received via a pointer to this function. Returns a pointer to a new
 * string with the spaces added.
 */

char *with_spaces(char *newpass)
{
        /*
         * get memory for a new string to contain the password plus
         * the spaces needed. All words in the passwords are 3
         * characters in length - so divide password length by 3.
         * Answer will give number of spaces required plus 1 for the
         * '\0' on the end of the new string
         */
        int length = (strlen(newpass) + (strlen(newpass) / 3));
        char *s_newpass = malloc(sizeof(char) * length);

        if (s_newpass == NULL) {
                fprintf(
                    stderr,
                    "Error allocating memory in function 'with_spaces()': %s\n",
                    strerror(errno));
                exit(EXIT_FAILURE);
        }

        /* initialise so works with strncat */
        *s_newpass = '\0';

        /* count for string with spaces for pointer location */
        int s = 0;
        /* count for string with no spaces for pointer location */
        int n = 0;
        /* count to insert a space every three characters */
        int addspc = 1;

        /* loop while we can read the *newpass string passed to this
         * function */
        while (*(newpass + n) != '\0') {
                /* copy a char from '*newpass' string to the version
                 * to include spaces '*s_newpass' */
                *(s_newpass + s) = *(newpass + n);

                /* if we are at the third character as we loop but not
                 * on the last 3 letter word in the string */
                if ((addspc == 3) & (s != length - 2)) {
                        /* increment to the next char position */
                        s++;
                        /* assign a space at the new char location */
                        *(s_newpass + s) = ' ';
                        /* set to zero as will be incremented to 1 on
                         * loop exit - thus reset reset char count so
                         * we can loop back to 3 chars again as
                         * needed */
                        addspc = 0;
                }

                /* increment to next char position  */
                s++;
                n++;
                /* increment char space count by 1 */
                addspc++;
        }
        /* done - so terminate the new string with a NUL */
        *(s_newpass + s) = '\0';

        return s_newpass;
}

/*
 * showBanner displays an on screen a summary of the program name and
 * its current version
 */
void show_banner()
{
        printf("\n\t\t'Offer Password' -  %s\n", version);
        printf("\t\t¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯\n");
}

/*
 * show_help displays on screen a summary of the command line switches
 * available in the program.
 */
void show_help()
{
        show_banner();
        printf(""
               "Help Summary:\n"
               "The following command line switches can be used:\n\n"
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
 * dump out the complete list of words held in our words array
 */
void dump_words()
{
        int i = 0;

        while (i < wordArraySize) {
                printf("%s ", *(words + i));
                i++;
        }
        printf("\n");
}

/*
 * 'get_random()' gets a random password
 *
 *  get a random value within the wordArraySize using recommended:
 *  rand() / (RAND_MAX / N + 1) See C FAQ here:
 *  http://c-faq.com/lib/randrange.html
 *
 */
char *get_random(int wordsRequired)
{
        /* Allocate a new char pointer (string) to hold the generated
         password that will be allocated on the heap - so will exist
         after function ends. Words are three chars in length times
         number words required + 1 for '\0' */
        char *genPass = malloc(((sizeof(char) * 3) * wordsRequired) + 1);

        if (genPass == NULL) {
                fprintf(
                    stderr,
                    "Error allocating memory in function 'get_random()': %s\n",
                    strerror(errno));
                exit(EXIT_FAILURE);
        }
        /* initialise our new malloc variable so works with strncat */
        *genPass = '\0';

        for (int x = 1; x <= wordsRequired; x++) {
                /* get a random number based on the size of the word array */
                int r = rand() / (RAND_MAX / wordArraySize + 1);
                /* add the new word to the 'genPass' variable we allocated on
                 * heap earlier */
                strncat(genPass, *(words + r), strlen(*(words + r)));
        }
        /* return the heap memory address of variable: char *genPass */
        return genPass;
}

/*-------------------------------*/
/* MAIN - Program starts here    */
/*-------------------------------*/

int main(int argc, char **argv)
{
        /* register our atexit() function */
        atexit(exit_cleanup);

        /* get any command line arguments provided by the user and
           then process using getopts() via function below */
        get_cli_args(argc, argv);

        /* Check it it was just help output the user requested? */
        if (help) {
                show_help();
                return EXIT_SUCCESS;
        }

        /* get total number of 3 letter words in our array word *
           [wordArraySize] [4] */
        wordArraySize = sizeof(words) / sizeof(words[0]);

        /* seed random with the current time in seconds since the
         * Epoch done once - used as is global value for programs
         * life */
        srand(time(NULL));

        /* if quick output was requested via command line option '-q'
         * then output a password suggestion and exist. Ignore other
         * options except debug, and password suggestion length */
        if (quick) {
                char *newpass = get_random(wordsRequired);
                printf("%s\n", newpass);
                free(newpass);
                newpass = NULL;
                return EXIT_SUCCESS;
        }

        /* If a dump was requested of the list of words used to
         * generate the passwords was requested using the command line
         * flag -e output it and then exit */
        if (export) {
                dump_words();
                return EXIT_SUCCESS;
        }

        /* output name of app and version */
        show_banner();
        /* display some stats about passwords being generated */
        printf("Application Stats:\n");
        printf("  - Number of three letter words available: ");
        printf("%d\n", wordArraySize);
        printf("  - Number of words per suggested password: ");
        printf("%d\n", wordsRequired);
        printf("  - Password character length will be: ");
        printf("%d\n", (wordsRequired * 3));
        printf("  - Number of password suggestions to offer: ");
        printf("%d\n\n", numPassSuggestions);

        printf("Suggested passwords are:\n\n");
        printf("\tPassword:");
        printf("\tWith spaces:");
        printf("\tRandom no:\n\n");

        for (int x = 1; x <= numPassSuggestions; x++) {
                char *newpass = get_random(wordsRequired);
                char *spc_newpass = with_spaces(newpass);
                printf("\t%s", newpass);
                printf("\t%s", spc_newpass);
                /* get a random number between 0 and 9 twice */
                printf("\t%d%d\n", rand() / (RAND_MAX / 9),
                       rand() / (RAND_MAX / 9));
                /* finished with *newpass and *spc_newpass now - so free memory
                 * up */
                free(newpass);
                newpass = NULL;
                free(spc_newpass);
                spc_newpass = NULL;
        }

        return EXIT_SUCCESS;
}
