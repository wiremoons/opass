/*
* Offer Password (opass): output.h
*
* opass is a password generation program that uses a pool of three
* letter English words
*
* author:     simon rowe <simon@wiremoons.com>
* license:    open-source released under "MIT License"
* source:     https://github.com/wiremoons/opass
*
* originally created: 05 Jan 2016
* updated significantly: 01 May 2021
*
* The program is licensed under the "MIT License" see
* http://opensource.org/licenses/MIT for more details.
*
*/

#ifndef OPASS_OUTPUT_H
#define OPASS_OUTPUT_H

extern char version[];
extern int wordArraySize;
extern int marksArraySize;
extern const char *words[];
extern const char *marks[];

void dump_words();
void show_help(void);
void show_version(char *program_name, int numPassSuggestions, int marksArraySize, int wordsRequired);


#endif //OPASS_OUTPUT_H
