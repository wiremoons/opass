/**
 * @file opass.c
 * @brief Offer Password (opass) application
 * @details Simple c program to offer password suggestions based on a collection of three
 * letter words.
 * @See https://github.com/wiremoons/opass
 *
 * @license MIT License
 *
 */

#include "opass.h"

#include <stdlib.h> /* malloc, srandom, random, env */
#include <ctype.h>  /* isdigit */
#include <stdio.h>  /* printf, fprintf */
#include <string.h> /* strncat */
#include <time.h>   /* time */
#include <assert.h> /* assert macro */
#include <errno.h>  /* strerror - see function ‘with_spaces’ */

/**
 * @brief Set the total number of passwords to display as output for the user to select from.
 * @param OPASS_NUM : obtained from user set environment variable or uses `MAX_PASSWORDS` as a default.
 * @return int : number of different passwords to generate.
 */
int set_number_passwords(void)
{
    int result =
        getenv("OPASS_NUM") ? (atoi(getenv("OPASS_NUM"))) : MAX_PASSWORDS;

    if ((isdigit(result) != 0) || (result < 1 || result > 50)) {
        result = MAX_PASSWORDS;
    }
    return result;
}

/**
 * @brief Set the number of three words to use to create a password string, effecting the passwords overall length.
 * @param `OPASS_WORDS` : obtained from user set environment variable or uses `MAX_WORDS` as a default.
 * @return `int` : number of different words used to generate the password.
 */
int set_number_words(void)
{
    int result =
        getenv("OPASS_WORDS") ? (atoi(getenv("OPASS_WORDS"))) : MAX_WORDS;

    if ((isdigit(result) != 0) || (result < 1 || result > 50)) {
        result = MAX_WORDS;
    }
    return result;
}

/**
 * @brief Gets a string created from randomly selected three (3) letter words from the `const char *words[]` array.
 * @param wordsRequired : the number of random words to obtain from the `char const *words[]` array.
 * @return a pointer to the heap allocated string of three (3) letter words randomly generated.
 */
char *get_random_password_str(int wordsRequired, int wordArraySize)
{
    /** @note Allocate on the heap a new string sized hold the generated password.
     * Memory sized based on the words being three chars in length times number words required.
     * Then plus one (1) for the C string termination character `\0`.
     */
    char *generated_password = malloc(((sizeof(char) * 3) * wordsRequired) + 1);

    if (NULL == generated_password) {
        fprintf(stderr,
                "Error allocating memory in function 'get_random_password_str()' in file '%s' at line '%d'.\nERROR : %s\n",
                __FILE__, __LINE__, strerror(errno));
        exit(EXIT_FAILURE);
    }
    /** @note initialise newly allocated memory with the C string termination character `\0` so works with `strncat` */
    *generated_password = '\0';

    for (int x = 1; x <= wordsRequired; x++) {
        /* get a random number constrained by the size of the word array */
        long r = (random() % wordArraySize);
        #if DEBUG
        printf("DEBUG: word array random number: %ld\n",r);
        #endif
        /* add the new word to the 'generated_password' variable we allocated on
         * heap earlier
         */
        strncat(generated_password, *(words + r), strlen(*(words + r)));
    }
    /* return the heap memory address of variable: char *generated_password */
    return generated_password;
}

/**
 * @brief Created a new string and adds a spaces at every third character position. New string is then output and freed.
 * @param str_password : the baseline string to be used - copied in memory to a new string that has added spaces.
 * @return no return.
 */
void with_spaces(char *str_password)
{
    /**
     * @note Get heap memory for a new string to contain the existing password `*str_password` plus the spaces needed.
     * All words in the password string are three (3) characters in length - so divide password length by three (3).
     * Result will give number of spaces required. No additional space is added to the end of the password string by
     * this function - so the one extra space that results in the new string length calculation will be used for the
     * C string termination character `\0` instead.
     */
    size_t length = (strlen(str_password) + (strlen(str_password) / 3));
    char *str_newpass = malloc(sizeof(char) * length);

    if (NULL == str_newpass) {
        fprintf(stderr,
                "Error allocating memory in function 'with_spaces()' in file '%s' at line '%d'.\nERROR : %s\n",
                __FILE__, __LINE__, strerror(errno));
        exit(EXIT_FAILURE);
    }

    /** @note initialise newly allocated memory with the C string termination character `\0` so works with `strncat` */
    *str_newpass = '\0';

    /** @var count for `str_newpass` to manage pointer location */
    int snp = 0;
    /** @var count for 'str_password' to manage pointer location */
    int sp = 0;
    /** @var count to track position for inserting a space every three characters */
    int add_space = 1;

    while (*(str_password + sp) != '\0') {
        /* copy a char from '*str_password' to the version that is to also include spaces '*str_newpass' */
        *(str_newpass + snp) = *(str_password + sp);

        /* check if at the third character position but not in the last 3 letter word in the whole string */
        if ((add_space == 3) & (snp != length - 2)) {
            /* increment the '*str_newpass' pointer to the next char position ready for the space to be added*/
            snp++;
            /* insert a space at the current pointer location */
            *(str_newpass + snp) = ' ';
            /* now reset count */
            add_space = 0;
        }

        /* increment the pointer to next char position on each of the password strings */
        snp++;
        sp++;
        /* also increment char space count by 1 to track when three (3) is reached */
        add_space++;
    }
    /* done - so terminate the new string with a NUL */
    *(str_newpass + snp) = '\0';
    /* use the new `*str_newpass` - output for the users benefit and reference */
    show_password(str_newpass);

    /* free up the heap memory allocated to the `*str_newpass` as it is no longer needed */
    free(str_newpass);
    str_newpass = NULL;
}

/**
 * @brief Manipulate the existing heap allocated string to capitalises each three letter words.
 * @param str_password : the baseline string to be used - existing in memory string is altered.
 * @return no return.
 */
void with_capitilised_words(char *str_password)
{
    size_t length = strlen(str_password);

    if (NULL == str_password) {
        fprintf(stderr,
                "Error NULL pointer in function 'with_capitilised_words()' in file '%s' at line '%d'.\nERROR : %s\n",
                __FILE__, __LINE__, strerror(errno));
        exit(EXIT_FAILURE);
    }

    /** @var count for 'str_password' to manage pointer location */
    int sp = 0;
    /** @var count to track position for capitalising a character every three characters */
    int add_space = 0;

    while (*(str_password + sp) != '\0') {

        /* ensure the first letter of the first word is capitalised */
        if (sp == 0) {
            *(str_password + sp) = toupper(*(str_password + sp));
        }

        /* check if at the third character position but not in the last 3 positions in the whole string */
        if ((add_space == 3) & (sp != length - 3)) {
            /* convert to uppercase at the current pointer location */
            *(str_password + sp) = (char)toupper(*(str_password + sp));
               /* now reset count */
            add_space = 0;
        }

        /* increment the pointer to next char position on each of the password strings */
        sp++;
        /* also increment char space count by 1 to track when three (3) is reached */
        add_space++;
    }

}


/**
 * @brief Quick output was requested via command line option '-q' or '--quick'
 * @param wordsRequired : the number of three letter words to include in output
 * @param wordArraySize : the size of the `char const *words[]` array.
 * @return no return
 */
void get_quick(int wordsRequired, int wordArraySize)
{
    char *newpass = get_random_password_str(wordsRequired, wordArraySize);
    printf("%s\n", newpass);
    free(newpass);
    newpass = NULL;
}

/*-------------------------------*/
/* MAIN - Program starts here    */
/*-------------------------------*/
/**
 * @brief Main program start point for `opass`.
 * @param argc : number of arguments included when the program was executed.
 * @param argv : array of user provided command line arguments.
 * @return : int indicating success or failure for programs execution.
 */
int main(int argc, char **argv)
{

    /** @var : set the version using the define from the header */
    char version[] = VERSION;

    /** @var : set the number of passwords to provide as output */
    int const numPassSuggestions = set_number_passwords();

    /** @var : set the number of random three letter words per password */
    int const wordsRequired = set_number_words();

    /** @var : get total number of three letter words in our array contain all the three letter words */
    int const wordArraySize = sizeof(words) / sizeof(words[0]);
    assert(wordArraySize == 1312);

    /** @var : get the total number of mark characters in our array */
    int const marksArraySize = sizeof(marks) / sizeof(int);
    assert(marksArraySize == 10);

    /* seed random with the current time in seconds since the
     * Epoch done once - used as is global value for programs life */
    srandom(time(NULL));

    /** @note obtain any command line args from the user and action them */
    if (argc > 1) {

        if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
            show_help();
            return (EXIT_SUCCESS);
        }

        if (strcmp(argv[1], "-q") == 0 || strcmp(argv[1], "--quick") == 0) {
            get_quick(wordsRequired,wordArraySize);
            return (EXIT_SUCCESS);
        }

        if (strcmp(argv[1], "-e") == 0 || strcmp(argv[1], "--export") == 0) {
            dump_words(wordArraySize, marksArraySize, words, marks);
            return (EXIT_SUCCESS);
        }

        if (strcmp(argv[1], "-v") == 0 || strcmp(argv[1], "--version") == 0) {
            show_version(argv[0], numPassSuggestions, wordsRequired, version, wordArraySize, marksArraySize);
            return (EXIT_SUCCESS);
        }
    }

    /** @section No command line options were provided by the user - so run the default action of
     *  generating and then displaying passwords for the user to select from.
     */
    printf("Suggested passwords are:\n\n");
    for (int x = 1; x <= numPassSuggestions; x++) {
        /* get a base set of words to use as a password string: `*newpass` */
        char *newpass = get_random_password_str(wordsRequired,wordArraySize);

        #if DEBUG
        printf("DEBUG: '*newpass' length: %d\n",(int)strlen(newpass));
        #endif

        /* size_t == length of 'newpass' + mark + random number */
        size_t fullpass_sz = ( (sizeof(char) * strlen(newpass)) + (sizeof(char)) + (sizeof(char) *2 ) + 1);

        #if DEBUG
        printf("DEBUG: '*fullpass' length: %d\n",(int)fullpass_sz);
        #endif

        char *fullpass=malloc(fullpass_sz);

        /** @note Create a `*fullpass` with component parts.
         * The call to random is cast as `int` as Windows uses *rand/srand* alternative
         * that returns `int` whereas other systems all use *srandom/random* from 'stdlib.h' library
         * that returns a `long`. As only random numbers between 0 and 99 are being used, limiting
         * the returned value to `int` from default `long` is safe. */
        snprintf(fullpass,fullpass_sz,"%s%c%02d",newpass,(marks[(random() % marksArraySize)]),(int)(random() % 99));

        /* output a word only version of the password with spaces between the word */
        if ((strlen(newpass) > 0) || (NULL != newpass)) {
            with_spaces(newpass);
        }
        printf("    ");
        /* output a word, mark and random number version of the password */
        show_password(fullpass);
        printf("    ");
        /* output a word, mark and random number version of the password with each word capitalised */
        with_capitilised_words(fullpass);
        show_password(fullpass);
        printf("\n");
        free(newpass);
        free(fullpass);
        newpass = NULL;
        fullpass = NULL;
    }

    return EXIT_SUCCESS;
}
