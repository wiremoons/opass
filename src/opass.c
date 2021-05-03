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

#include <assert.h> /* assert */
#include <ctype.h>  /* isdigit */
#include <errno.h>  /* strerror */
#include <stdio.h>  /* printf, fprintf */
#include <stdlib.h> /* malloc, srandom, random, env */
#include <string.h> /* strncat */
#include <time.h>   /* time */

/*
 * 'set_number_passwords()' set the number of passwords to display as output
 * Get number from user via environment variable 'OPASS_NUM' or uses defined
 * MAX_PASSWORDS as a default
 */
int set_number_passwords(void) {
    int result = getenv("OPASS_NUM") ? (atoi(getenv("OPASS_NUM"))) : MAX_PASSWORDS;
    printf("\ncurrent: %d\n", result);

    if ((isdigit(result) != 0) || (result < 1 || result > 50)) {
        printf("\nreseting: %d\n", result);
        result = MAX_PASSWORDS;
    }
    printf("\nreturning: %d\n", result);
    return result;
}

/*
 * 'set_number_words()' set the number of words to use to create a password string.
 * Get number from user via environment variable 'OPASS_WORDS' or uses defined
 * MAX_WORDS as a default
 */
int set_number_words(void) {
    int result = getenv("OPASS_WORDS") ? (atoi(getenv("OPASS_WORDS"))) : MAX_WORDS;
    printf("\ncurrent: %d\n", result);

    if ((isdigit(result) != 0) || (result < 1 || result > 50)) {
        printf("\nreseting: %d\n", result);
        result = MAX_WORDS;
    }
    printf("\nreturning: %d\n", result);
    return result;
}

/*
 * 'with_spaces()' adds a spaces at every third character position in the
 * 'str_password' string received via a pointer to this function. Returns a pointer to a new
 * string 'str_newpass' with the spaces added.
 */
char *with_spaces(char *str_password) {
    /*
     * get memory for a new string to contain the password plus
     * the spaces needed. All words in the passwords are 3
     * characters in length - so divide password length by 3.
     * Answer will give number of spaces required plus 1 for the
     * '\0' on the end of the new string. No space is added to the
     * end of the string - so the one extra will be used for the '\0'
     */
    size_t length = (strlen(str_password) + (strlen(str_password) / 3));
    char *str_newpass = malloc(sizeof(char) * length);

    if (NULL == str_newpass) {
        fprintf(
                stderr,
                "Error allocating memory in function 'with_spaces()': %snp\n",
                strerror(errno));
        exit(EXIT_FAILURE);
    }

    /* initialise so works with strncat */
    *str_newpass = '\0';

    /* count for 'str_newpass' string for pointer location */
    int snp = 0;
    /* count for 'str_password' string for pointer location */
    int sp = 0;
    /* count position fro inserting a space every three characters */
    int addspc = 1;

    /* loop while we can read the *str_password string passed to this
     * function */
    while (*(str_password + sp) != '\0') {
        /* copy a char from '*str_password' string to the version
         * to include spaces '*str_newpass' */
        *(str_newpass + snp) = *(str_password + sp);

        /* check if at the third character position but not
         * on the last 3 letter word in the whole string */
        if ((addspc == 3) & (snp != length - 2)) {
            /* increment the 'str_newpass' pointer to the next char position */
            snp++;
            /* insert a space at the new char location */
            *(str_newpass + snp) = ' ';
            /* now reset count to add a space */
            addspc = 0;
        }

        /* increment to next char position  */
        snp++;
        sp++;
        /* increment char space count by 1 */
        addspc++;
    }
    /* done - so terminate the new string with a NUL */
    *(str_newpass + snp) = '\0';

    return str_newpass;
}

/*
 * 'get_random_password_str()' gets a string created from randomly selected words
 * from the three letter word array.
 */
char *get_random_password_str(int wordsRequired) {
    /* Allocate a new char pointer (string) to hold the generated
     password that will be allocated on the heap - so will exist
     after function ends. Words are three chars in length times
     number words required + 1 for '\0' */
    char *generated_password = malloc(((sizeof(char) * 3) * wordsRequired) + 1);

    if (NULL == generated_password) {
        fprintf(
                stderr,
                "Error allocating memory in function 'get_random_password_str()': %s\n",
                strerror(errno));
        exit(EXIT_FAILURE);
    }
    /* initialise our new malloc variable so works with strncat */
    *generated_password = '\0';

    for (int x = 1; x <= wordsRequired; x++) {
        /* get a random number constrained by the size of the word array */
        long r = (random() % wordArraySize);
        //printf("DEBUG: word array random number: %ld\n",r);
        /* add the new word to the 'generated_password' variable we allocated on
         * heap earlier */
        strncat(generated_password, *(words + r), strlen(*(words + r)));
    }
    /* return the heap memory address of variable: char *generated_password */
    return generated_password;
}

/* Quick output was requested via command line option '-q' or '--quick' */
void get_quick(int wordsRequired) {
    char *newpass = get_random_password_str(wordsRequired);
    printf("%s\n", newpass);
    free(newpass);
    newpass = NULL;
}

/*-------------------------------*/
/* MAIN - Program starts here    */
/*-------------------------------*/

int main(int argc, char **argv) {

    /* set the number of passwords to provide as output */
    int numPassSuggestions = set_number_passwords();
    printf("\nfinal: %d\n", numPassSuggestions);

    /* number of random words per password */
    int wordsRequired = set_number_words();;
    printf("\nfinal: %d\n", wordsRequired);

    /* get total number of 3 letter words in our array of three letter words */
    wordArraySize = sizeof(words) / sizeof(words[0]);

    /* get total number of mark characters in our array */
    marksArraySize = sizeof(marks) / sizeof(marks[0]);

    /* seed random with the current time in seconds since the
     * Epoch done once - used as is global value for programs life */
    srandom(time(NULL));

    /* obtain any command line args from the user and action them */
    if (argc > 1) {

        if (strcmp(argv[1], "-h") == 0 ||
            strcmp(argv[1], "--help") == 0) {
            show_help();
            return (EXIT_SUCCESS);
        }

        if (strcmp(argv[1], "-q") == 0 ||
            strcmp(argv[1], "--quick") == 0) {
            get_quick(wordsRequired);
            return (EXIT_SUCCESS);
        }

        if (strcmp(argv[1], "-e") == 0 ||
            strcmp(argv[1], "--export") == 0) {
            dump_words();
            return (EXIT_SUCCESS);
        }

        if (strcmp(argv[1], "-v") == 0 ||
            strcmp(argv[1], "--version") == 0) {
            show_version(argv[0], numPassSuggestions, marksArraySize, wordsRequired);
            return (EXIT_SUCCESS);
        }
    }

    printf("Suggested passwords are:\n\n");
    for (int x = 1; x <= numPassSuggestions; x++) {
        char *newpass = get_random_password_str(wordsRequired);
        char *spc_newpass = with_spaces(newpass);
        printf("%s", spc_newpass);
        printf("\t%s", newpass);
        /* random mark  */
        printf("\t%s", marks[(random() % marksArraySize)]);
        /* random number 0 .. 99 display leading 0 */
        printf("\t%02ld\n", (random() % 99));
        /* finished with *newpass and *spc_newpass now - so free memory
         * up */
        free(newpass);
        newpass = NULL;
        free(spc_newpass);
        spc_newpass = NULL;
    }

    return EXIT_SUCCESS;
}
