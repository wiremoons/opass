# Summary for `opass`

A small application called 'opass' which is an abbreviation for 'offer
password'. The program suggests passwords using a pool of English
three letter words.

## About

`opass` (short for '*offer password*') is a password creation tool
which uses a pool of three letter english words to generate password
suggestions. The three letter words are randomly selected from the
pool, creating password suggestions and displaying them on the screen
for the user to select from.

The opass program is is written in c, and is a command line /
terminal based program. It should compile and then run on an platform
that supports the c programming language, such as on Windows, Mac and
Linux, FreeBSD, etc.

A three letter English word list of over 1,000 words was acquired for
use in the application, from the Association of British Scrabble
Players (ABSP) web page is here: http://www.absp.org.uk/words/3lw.shtml

## Building the Application

A c compiler will be needed to build the application.

Compile with: `gcc -Wall --std=gnu11 -o opass opass.c` or use the
provided 'Makefile', and run one of the options such as `make opt`.

## License

The program is licensed under the "MIT License" see
http://opensource.org/licenses/MIT for more details.
