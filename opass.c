/*

  Program: opass

  History
  ¯¯¯¯¯¯¯
  Created by Simon Rowe <simon@wiremoons.com>

  Initial version created from the code of a similar (but curses
  based) program called 'sugpass'. Created on: 05 Jan 2016

  22 Jan 2016 - v0.2 with minor updates to output to make consistent
  30 Jan 2016 - v0.3 minor cosmetic updates to output when run
  
  About
  ¯¯¯¯¯¯
  Password creation tool using a pool of three letter English words
  to generate password suggestions.

  Word list acquired from the Association of British Scrabble Players
  (ABSP), from the web page is here:
  http://www.absp.org.uk/words/3lw.shtml

  Compile with: `gcc -Wall --std=gnu11 -o op op.c` or use
  the provided 'Makefile'.

  The program is licensed under the "MIT License" see
  http://opensource.org/licenses/MIT for more details.

*/

#include <stdio.h>	  // for printf() fprintf(stderr)
#include <time.h>	  // for use of time() as seed
#include <stdlib.h>	  // for use of rand() atexit() malloc()
#include <string.h>	  // used for strncat()
#include <unistd.h>	  // used for getopt()
#include <ctype.h>	  // used for isdigit()
#include <errno.h>	  // used for strerror()

/*-----------------------*/
/* SET GLOBAL VARIABLES  */
/*-----------------------*/

/* define the version of the program */
char version[]="version: 0.3";
/* control if debuging output is provided (0 == debug off; 1 == debug on;) */
int debug = 0;
/* control if an export of the three letter words is provided (0 == export off; 1 == export on;) */
int export = 0;
/* control if -q quick output is provided (0 == quick off; 1 == quick on;)*/
int quick = 0;
/* control if -h show help output is required (0 == help off; 1 == help on;) */
int help = 0;
/* number of words in the word array */
int wordArraySize = 0;
/* define how many random words we want to include for each password */
int wordsRequired = 4;
/* define how many password suggestions we want to provide */
int numPassSuggestions = 3;
/* 
   char *words is a pointer to array of strings. 
   Each string in the array contains a three letter word 
*/
char *words[] = { 
	"aah","aal","aas","aba","abb","abo","abs","aby","ace","ach",
	"act","add","ado","ads","adz","aff","aft","aga","age","ago",
	"ags","aha","ahi","ahs","aia","aid","ail","aim","ain","air",
	"ais","ait","aka","ake","ala","alb","ale","alf","all","alp",
	"als","alt","alu","ama","ame","ami","amp","amu","ana","and",
	"ane","ani","ann","ans","ant","any","ape","apo","app","apt",
	"arb","arc","ard","are","arf","ark","arm","ars","art","ary",
	"ash","ask","asp","ass","ate","ats","att","aua","aue","auf",
	"auk","ava","ave","avo","awa","awe","awk","awl","awn","axe",
	"aye","ays","ayu","azo","baa","bac","bad","bag","bah","bal",
	"bam","ban","bap","bar","bas","bat","bay","bed","bee","beg",
	"bel","ben","bes","bet","bey","bez","bib","bid","big","bin",
	"bio","bis","bit","biz","boa","bob","bod","bog","boh","boi",
	"bok","bon","boo","bop","bor","bos","bot","bow","box","boy",
	"bra","bro","brr","bru","bub","bud","bug","bum","bun","bur",
	"bus","but","buy","bye","bys","caa","cab","cad","cag","cam",
	"can","cap","car","cat","caw","cay","caz","cee","cel","cep",
	"cha","che","chi","cid","cig","cis","cit","cly","cob","cod",
	"cog","col","con","coo","cop","cor","cos","cot","cow","cox",
	"coy","coz","cru","cry","cub","cud","cue","cum","cup","cur",
	"cut","cuz","cwm","dab","dad","dae","dag","dah","dak","dal",
	"dam","dan","dap","das","daw","day","deb","dee","def","deg",
	"dei","del","den","dev","dew","dex","dey","dib","did","die",
	"dif","dig","dim","din","dip","dis","dit","div","dob","doc",
	"dod","doe","dof","dog","doh","dol","dom","don","doo","dop",
	"dor","dos","dot","dow","doy","dry","dso","dub","dud","due",
	"dug","duh","dui","dun","duo","dup","dux","dye","dzo","ean",
	"ear","eas","eat","eau","ebb","ech","eco","ecu","edh","eds",
	"eek","eel","een","eff","efs","eft","egg","ego","ehs","eik",
	"eke","eld","elf","elk","ell","elm","els","elt","eme","emo",
	"ems","emu","end","ene","eng","ens","eon","era","ere","erf",
	"erg","erk","erm","ern","err","ers","ess","est","eta","eth",
	"euk","eve","evo","ewe","ewk","ewt","exo","eye","faa","fab",
	"fad","fae","fag","fah","fan","fap","far","fas","fat","faw",
	"fax","fay","fed","fee","feg","feh","fem","fen","fer","fes",
	"fet","feu","few","fey","fez","fib","fid","fie","fig","fil",
	"fin","fir","fit","fix","fiz","flu","fly","fob","foe","fog",
	"foh","fon","fop","for","fou","fox","foy","fra","fro","fry",
	"fub","fud","fug","fum","fun","fur","gab","gad","gae","gag",
	"gak","gal","gam","gan","gap","gar","gas","gat","gau","gaw",
	"gay","ged","gee","gel","gem","gen","geo","ger","get","gey",
	"ghi","gib","gid","gie","gif","gig","gin","gio","gip","gis",
	"git","gju","gnu","goa","gob","god","goe","gon","goo","gor",
	"gos","got","gov","gox","goy","gub","gue","gul","gum","gun",
	"gup","gur","gus","gut","guv","guy","gym","gyp","had","hae",
	"hag","hah","haj","ham","han","hao","hap","has","hat","haw",
	"hay","heh","hem","hen","hep","her","hes","het","hew","hex",
	"hey","hic","hid","hie","him","hin","hip","his","hit","hmm",
	"hoa","hob","hoc","hod","hoe","hog","hoh","hoi","hom","hon",
	"hoo","hop","hos","hot","how","hox","hoy","hub","hue","hug",
	"huh","hui","hum","hun","hup","hut","hye","hyp","ice","ich",
	"ick","icy","ide","ids","iff","ifs","igg","ilk","ill","imp",
	"ing","ink","inn","ins","ion","ios","ire","irk","ish","ism",
	"iso","ita","its","ivy","iwi","jab","jag","jai","jak","jam",
	"jap","jar","jaw","jay","jee","jet","jeu","jew","jib","jig",
	"jin","jiz","job","joe","jog","jol","jor","jot","jow","joy",
	"jud","jug","jun","jus","jut","kab","kae","kaf","kai","kak",
	"kam","kas","kat","kaw","kay","kea","keb","ked","kef","keg",
	"ken","kep","ket","kex","key","khi","kid","kif","kin","kip",
	"kir","kis","kit","koa","kob","koi","kon","kop","kor","kos",
	"kow","kue","kye","kyu","lab","lac","lad","lag","lah","lam",
	"lap","lar","las","lat","lav","law","lax","lay","lea","led",
	"lee","leg","lei","lek","lep","les","let","leu","lev","lew",
	"lex","ley","lez","lib","lid","lie","lig","lin","lip","lis",
	"lit","lob","lod","log","loo","lop","lor","los","lot","lou",
	"low","lox","loy","lud","lug","lum","lur","luv","lux","luz",
	"lye","lym","maa","mac","mad","mae","mag","mak","mal","mam",
	"man","map","mar","mas","mat","maw","max","may","med","mee",
	"meg","meh","mel","mem","men","mes","met","meu","mew","mho",
	"mib","mic","mid","mig","mil","mim","mir","mis","mix","miz",
	"mna","moa","mob","moc","mod","moe","mog","moi","mol","mom",
	"mon","wit","moo","mop","mor","mos","mot","mou","mow","moy",
	"moz","mud","mug","mum","mun","mus","mut","mux","myc","nab",
	"nae","nag","nah","nam","nan","nap","nas","nat","naw","nay",
	"neb","ned","nee","nef","neg","nek","nep","net","new","nib",
	"nid","nie","nil","nim","nip","nis","nit","nix","nob","nod",
	"nog","noh","nom","non","noo","nor","nos","not","now","nox",
	"noy","nth","nub","nun","nur","nus","nut","nye","nys","oaf",
	"oak","oar","oat","oba","obe","obi","obo","obs","oca","och",
	"oda","odd","ode","ods","oes","off","oft","ohm","oho","ohs",
	"oik","oil","ois","oka","oke","old","ole","olm","oms","one",
	"ono","ons","ony","oof","ooh","oom","oon","oop","oor","oos",
	"oot","ope","ops","opt","ora","orb","orc","ord","ore","orf",
	"ors","ort","ose","oud","ouk","oup","our","ous","out","ova",
	"owe","owl","own","owt","oxo","oxy","oye","oys","pac","pad",
	"pah","pal","pam","pan","pap","par","pas","pat","pav","paw",
	"pax","pay","pea","pec","ped","pee","peg","peh","pel","pen",
	"pep","per","pes","pet","pew","phi","pho","pht","pia","pic",
	"pie","pig","pin","pip","pir","pis","pit","piu","pix","plu",
	"ply","poa","pod","poh","poi","pol","pom","poo","pop","pos",
	"pot","pow","pox","poz","pre","pro","pry","psi","pst","pub",
	"pud","pug","puh","pul","pun","pup","pur","pus","put","puy",
	"pya","pye","pyx","qat","qis","qua","qin","rad","rag","rah",
	"rai","raj","ram","ran","rap","ras","rat","rav","raw","rax",
	"ray","reb","rec","red","ree","ref","reg","reh","rei","rem",
	"ren","reo","rep","res","ret","rev","rew","rex","rez","rho",
	"rhy","ria","rib","rid","rif","rig","rim","rin","rip","rit",
	"riz","rob","roc","rod","roe","rok","rom","roo","rot","row",
	"rub","ruc","rud","rue","rug","rum","run","rut","rya","rye",
	"sab","sac","sad","sae","sag","sai","sal","sam","san","sap",
	"sar","sat","sau","sav","saw","sax","say","SAY","saz","sea",
	"sec","sed","see","seg","sei","sel","sen","ser","set","sew",
	"sex","sey","sez","sha","she","shh","shy","sib","sic","sif",
	"sik","sim","sin","sip","sir","sis","sit","six","ska","ski",
	"sky","sly","sma","sny","sob","soc","sod","sog","soh","sol",
	"som","son","sop","sos","sot","sou","sov","sow","sox","soy",
	"soz","spa","spy","sri","sty","sub","sud","sue","sug","sui",
	"suk","sum","sun","sup","suq","sur","sus","swy","sye","syn",
	"tab","tad","tae","tag","tai","taj","tak","tam","tan","tao",
	"tap","tar","tas","tat","tau","tav","taw","tax","tay","tea",
	"tec","ted","tee","tef","teg","tel","ten","tes","tet","tew",
	"tex","the","tho","thy","tic","tid","tie","tig","tik","til",
	"tin","tip","tis","tit","tix","toc","tod","toe","tog","tom",
	"ton","too","top","tor","tot","tow","toy","try","tsk","tub",
	"tug","tui","tum","tun","tup","tut","tux","twa","two","twp",
	"tye","tyg","udo","uds","uey","ufo","ugh","ugs","uke","ule",
	"ulu","umm","ump","ums","umu","uni","uns","upo","ups","urb",
	"urd","ure","urn","urp","use","uta","ute","uts","utu","uva",
	"vac","vae","vag","van","var","vas","vat","vau","vav","vaw",
	"vee","veg","vet","vex","via","vid","vie","vig","vim","vin",
	"vis","vly","voe","vol","vor","vow","vox","vug","vum","wab",
	"wad","wae","wag","wai","wan","wap","war","was","wat","waw",
	"wax","way","web","wed","wee","wem","wen","wet","wex","wey",
	"wha","who","why","wig","win","wis","wit","wiz","woe","wof",
	"wog","wok","won","woo","wop","wos","wot","wow","wox","wry",
	"wud","wus","wye","wyn","xis","yad","yae","yag","yah","yak",
	"yam","yap","yar","yaw","yay","yea","yeh","yen","yep","yes",
	"yet","yew","yex","ygo","yid","yin","yip","yob","yod","yok",
	"yom","yon","you","yow","yug","yuk","yum","yup","yus","zag",
	"zap","zas","zax","zea","zed","zee","zek","zel","zep","zex",
	"zho","zig","zin","zip","zit","ziz","zoa","zol","zoo","zos",
	"zuz","zzz" };


/**-------- FUNCTION: getCLIArgs

   Function called when program starts. Used to parse command line
   options provided by the user using the POSIX compliant getopts()

*/
void getCLIArgs(int argc, char **argv)
{
	opterr = 0;
	int c = 0;
	int index = 0;

  	while ((c = getopt (argc, argv, "dehqvw:s:")) != -1) 
  	{
	    switch (c)
		{
            // debugging output was requested
		case 'd':
			debug = 1;
			break;
            // export of three letter words was requested
		case 'e':
			export = 1;
			break;
            // provide help summary to user was requested
		case 'h':
			help = 1;
			break;
            // quick single output was requested
		case 'q':
			quick = 1;
			break;
			// the version of the application requested
		case 'v':
			printf("%s is: %s",argv[0],version);
			exit(EXIT_SUCCESS);
            // number of words to include in each password specified
		case 'w':
			if (isdigit(*optarg)) {
				wordsRequired = atoi(optarg);
			} else {
				fprintf(stderr,"ERROR: for -w option please provide "
						"numerical digit(s) 1 to 9\n");
				exit(EXIT_FAILURE);
			}
			break;
            // number of passwords to suggest specified
		case 's':
			if (isdigit(*optarg)) {
				numPassSuggestions = atoi(optarg);
			} else {
				fprintf(stderr,"ERROR: for -s option please provide "
						"numerical digit(s) 1 to 9\n");
				exit(EXIT_FAILURE);
			}
			break;
            // ERROR HANDLING BELOW
	        //
        	// command line option given - but is missing the required data argument for it
		case ':':
			fprintf(stderr,"%s: option '-%c' requires an argument\n",argv[0], optopt);
			break;
        	// invalid option provided on command line - also 'default' as the switch fall-thru
		case '?':
		default:
			/* invalid option */
			fprintf(stderr,"%s: option '-%c' is invalid: ignored\n", argv[0], optopt);
			break;
		}
	}
    // if debugging requested - display extra getopt() info
  	if (debug) {
  		printf ("DEBUG optargs():\n\tdebug = %s\n"
				"\tWords per password = %d\n"
				"\tNumber of passwords to suggest = %d\n",
				debug ? "true" : "false",
				wordsRequired, numPassSuggestions);
  		for (index = optind; index < argc; index++) {
		    printf ("\tInvalid option argument(s) seen: %s\n", argv[index]);
        }
    }
}


/**-------- FUNCTION: exitCleanup

   function called when program exits
   Used via registration with 'atexit()' in main()

*/
void exitCleanup()
{
	// output unless quick (-q) command line option was used:
	if (!quick) printf("\nAll is well\n");
}


/**-------- FUNCTION: withSpaces

   function to add spaces at every third character point in the 
   string recieved via a pointer to this function.
   Returns a pointer to a new string with the spaces added.

*/
char *withSpaces(char *newpass)
{
	// can change size_t format type to %zu for gcc (instead of %d)- if get compiler warnings
	if (debug)
	{
		printf("DEBUG: password received by function 'withSpaces' "
			   "is: %s at %d characters length\n",
			   newpass,
			   strlen(newpass) );
	}

	// get memory for a new string to contain the password plus the spaces needed.
	// All words in the passwords are 3 characters in length - so divide password length
	// by 3 answer will give number of spaces required plus 1 for the '\0' on the end of the new string
	int length = ( strlen(newpass) + (strlen(newpass)/3) );
	char *s_newpass = malloc( sizeof(char) * length );
	// if malloc() failed - exit
	if (s_newpass == NULL) 
	{
		fprintf(stderr,"Error allocating memory in function 'withSpaces()': %s\n",strerror(errno));
		exit(EXIT_FAILURE); 
	}
	
	if (debug)
	{
		printf("DEBUG: Calculated length %d "
			   "for allocated string length of: %d in 'withSpaces()' function",
			   length,
			   (strlen(newpass)) + (strlen(newpass)/3));
	}
		
	// initialise our new malloc'd variable so works with strncat
	*s_newpass = '\0';

	// count for string with spaces for pointer location
	int s = 0;
	// count for string with no spaces for pointer location
	int n = 0;
	// count to insert a space every three characters
	int addspc = 1;

	// while we can read the *newpass string passed to this function
	while ( *(newpass+n) != '\0' )
	{
		// copy a char from '*newpass' string to the version to include spaces '*s_newpass'
		*(s_newpass+s) = *(newpass+n);

		if (debug)
		{
			printf("DEBUG: Password char is: %c"
				   " with spaces char: %c @ %d "
				   "and space count at %d\n",
				   *newpass,
				   *(s_newpass+s),s,
				   addspc);
		}

		if (debug)
		{
			printf("DEBUG: char '%c' copied @ string location: %d and count: %d\n",
				   *(newpass+n),
				   s,
				   addspc);
		}

		// if we are at the third character as we loop -
		// but not on the last 3 letter word in the string
		if ( (addspc == 3) & (s != length-2 ) )
		{
			// increment to the next char position
			s++;
			// assign a space at the new char location
			*(s_newpass+s) = ' ';
			// set to zero as will be incremented to 1 on loop exit - thus reset
			if (debug)
			{
				printf("DEBUG: added space @ string location: %d "
					   "and count: %d\n",
					   s,
					   addspc);
			}
			// reset char count so we can loop back to 3 chars again as needed
			addspc = 0;
		}

		// increament to next char position 
		s++; n++;
		// increment char space count by 1
		addspc++;

	}
	// done - so terminate the new string with a NUL
	*(s_newpass+s) = '\0';

	// display the complete password suggesiton created above
	if (debug)
	{
		printf("\nDEBUG: New password suggestion with spaces is: '%s'\n", s_newpass);
	}

	return s_newpass;
}

/**-------- FUNCTION: showBanner

   Show on screen a summary of the program name and its current version

*/
void showBanner()
{
	// display name and version of the program
	printf("\n\t\t'Offer Password' -  %s\n",version);
	printf("\t\t¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯\n");
}

/**-------- FUNCTION: showHelp

   Show on screen a summary of the command line switches available in the
   program.

*/
void showHelp()
{
	showBanner();
	printf(""
		   "Help Summary:\n"
		   "The following command line switches can be used:\n\n"
		   "  -d\tDebug - include addtional debug outputs when run\n"
		   "  -e\tExport - dump the full list of three letter words used and exit\n"
		   "  -h\tHelp - Show this help information\n"
		   "  -q\tQuick - just provide a password offer with no other output\n"
		   "  -v\tVersion - display the version of the program\n"
		   "  -w ?\tWords - specifiy how many three letter words (? == digits 0-9) to include in the password offered\n"
		   "  -s ?\tSuggest - specify how many passwords to offer (? == digits 0-9) to increase available selection\n");
}


/**-------- FUNCTION: dump

   print out the complete list of words held in our words array or three letter
   words, that is used to generate passwords

*/
void dumpWords()
{
	int i = 0;

	while (i < wordArraySize ) 
	{
		printf("%s ", *(words+i));
		i++;
	}
	printf("\n");


	// if debug printout first, middle and last words in our array
	if (debug)
	{
		printf("Words array contains %d words -where:\n", wordArraySize);
		printf("\tFirst word is 'aah'? : %s\n", *(words+0));
		printf("\tMiddle word is %d is : %s\n", (wordArraySize/2), *(words+((wordArraySize/2)-1)) );		
		printf("\tLast word is 'zzz'? : %s\n", *(words+wordArraySize-1));

	}
}

/**-------- FUNCTION: getRandom
 
   gets a random password

   get a random value within the wordArraySize using recommended: 
   rand() / (RAND_MAX / N + 1)
   See C FAQ here: http://c-faq.com/lib/randrange.html

*/
char *getRandom(int wordsRequired)
{
	// Allocate a new char pointer (string) to hold the generated password
	// will be allocated on the heap - so will exist after function ends
	// words are three chars in length times number words required + 1 for '\0'
	char *genPass = malloc( ((sizeof(char)*3) * wordsRequired) + 1 );
	// if malloc() failed - exit
	if (genPass == NULL) 
	{
		fprintf(stderr,"Error allocating memory in function 'getRandom()': %s\n",
				strerror(errno));
		exit(EXIT_FAILURE); 
	}
	// initialise our new malloc variable so works with strncat
	*genPass = '\0';

	for ( int x = 1; x <= wordsRequired; x++ )
	{
		// get a random number based on the size of the word array
		int r = rand() / (RAND_MAX / wordArraySize +1);
		// if debug - print out the randowm number obtained
		// and the corresponding word in the aray at that location
		if (debug) { printf("Random number generated is: %d with word: '%s'\n", r, *(words+r)); }
		// add the new word to the 'genPass' variable we allocated on heap earlier
		strncat( genPass, *(words+r), strlen(*(words+r)) );
	}
	// display the complete password suggesiton created above
	if (debug) { printf("\nNew password suggestion: '%s'\n", genPass); }
	// return the memory address of variable: char *genPass
	return genPass;
}


/**-------------------------------*/
/* MAIN - Program starts here    */
/*-------------------------------*/

int main(int argc, char **argv)
{
	// register our atexit() function
	atexit(exitCleanup);

	// get any command line arguments provided by the user
	// and then process using getopts() via function below
	getCLIArgs(argc,argv);

	// Check it it was just help output the user requested?
	if (help)
	{
		if (debug) { printf("\tDEBUG: User request help output\n"); }
		showHelp();
		return EXIT_SUCCESS;
	}

    // get total number of 3 letter words in our array
	//    *word [wordArraySize] [4]
	wordArraySize = sizeof(words)/sizeof(words[0]);

	// if debug - then print out size of our word array
	if (debug) { printf("\tDEBUG: Word array size is: %d\n", wordArraySize); }

	// seed random with the current time in seconds since the Epoch
	// done once - used as is global value for programs life
	srand(time(NULL));

	// if quick output was requested via command line option '-q' then
	// output a password suggestion and exist. Ignore other options
	// except debug, and password suggestion length
	if (quick){
		if (debug) printf("NB: Quick password requested with '-q' option\n");
		char *newpass = getRandom(wordsRequired);
		printf("%s\n",newpass);
		free(newpass); newpass= NULL;
		return EXIT_SUCCESS;
	}

	// If a dump was requested of the list of words used to generate the
	// passwords was requested using the command line flag -e output it
	// and then exit
	if (export)
	{
		dumpWords();
		return EXIT_SUCCESS;
	}

	// output name of app and version
	showBanner();
	// display some stats about passwords being generated
	printf("Application Stats:\n");
	printf("  - Number of three letter words available: ");
	printf("%d\n", wordArraySize);
	printf("  - Number of words per suggested password: ");
	printf("%d\n",wordsRequired);
	printf("  - Password character length will be: ");
	printf("%d\n", (wordsRequired * 3));
	printf("  - Number of password suggestions to offer: ");
	printf("%d\n\n", numPassSuggestions);

	printf("Suggested passwords are:\n\n");
	printf("\tPassword:");
	printf("\tWith spaces:\n");
	for (int x = 1; x <= numPassSuggestions; x++)
	{
		char *newpass = getRandom(wordsRequired);
		char *spc_newpass = withSpaces(newpass);
		printf("\t%s", newpass);
		printf("\t%s\n", spc_newpass );
		// finished with *newpass and *spc_newpass now - so free memory up
		free(newpass); newpass = NULL;
		free(spc_newpass); spc_newpass = NULL;
	}

	return EXIT_SUCCESS;

}

