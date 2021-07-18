/**
 * @file output.h
 * @brief Offer Password (opass): opass is a password generation program that uses a pool of three
 * letter English words
 *
 * @author:     simon rowe <simon@wiremoons.com>
 * @license:    open-source released under "MIT License"
 * @source:     https://github.com/wiremoons/opass
 *
 * @date originally created: 05 Jan 2016
 * @date updated significantly: 01 May 2021
 *
 * The program is licensed under the "*MIT License*" see
 * http://opensource.org/licenses/MIT for more details.
 *
 */

#ifndef OPASS_OUTPUT_H
#define OPASS_OUTPUT_H

void dump_words(int wordArraySize, int marksArraySize, char const *words[], int const marks[]);
void show_help(void);
void show_version(const char *program_name, int numPassSuggestions, int wordsRequired, const char *version, int wordArraySize, int  marksArraySize);
void show_password(char *out_password);


#endif //OPASS_OUTPUT_H
