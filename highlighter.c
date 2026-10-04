#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <stdbool.h>



/*
This is the implementation of the highlighter program.
Please read the documentation in the README before you start working on this file.
*/

// Color codes to use in your program. Print a color first, then your text, then the reset color.
static const char* COLOR_RED = "\033[0;31m";
static const char* COLOR_GREEN = "\033[0;32m";
static const char* COLOR_BLUE = "\033[0;34m";
static const char* COLOR_YELLOW = "\033[1;33m";
static const char* COLOR_MAGENTA = "\033[0;35m";
static const char* COLOR_CYAN = "\033[0;36m";
static const char* COLOR_WHITE = "\033[1;37m";
static const char* COLOR_RESET_COLOR = "\033[0m";


// These function prototypes / definitions are suggestions but not required to implement.
// typedef struct Settings with fields FILE*/handle input_stream, output_stream; strings search_text, prefix, postfix; and boolean no_color;
// void print_help()
// void print_error(const char* error_message)
// void output_final_result(int count, const char* search_text)
// const char* get_color_code(const char* color_str)
// int process_args(int argc, char* argv[], Settings* settings)
// int parse_line(Settings* settings, char* line)
// ...


//setting up the structure
typedef struct{
	FILE *input_stream;
	FILE *output_stream;
	char *input_name;
	char *output_name;
	char *search_text;
	char *prefix;
	char *postfix;
	char *color;
	bool no_color;
}Settings; 

//declaring the functions
void print_help();
void print_error(const char* error_message);
void output_final_result(int count, const char* search_text);
const char* get_color_code(const char* color_str);
int process_args(int argc, char* argv[], Settings* settings);
int parse_line(Settings* settings, char* line);

//print the usage, CLI options and examples
void print_help(){
	printf("usage: ./highlighter [-h] [-i INPUT] [-o OUTPUT] [-c {RED,GREEN,BLUE,YELLOW,MAGENTA,CYAN,WHITE}] [--no-color] [--prefix PREFIX] [--postfix POSTFIX] text\n\n");
	printf("highlighter\n\n");
	printf("positional arguments:\n");
	printf(" text                  text to match and highlight\n\n");
	printf("options:\n");
	printf("  -h, --help            show this help message and exit\n");
	printf("  -i INPUT, --input INPUT\n");
	printf("                        input file (default: stdin)\n");
	printf("  -o OUTPUT, --output OUTPUT\n");
	printf("                        output file (default: stdout)\n");
	printf("  -c {RED,GREEN,BLUE,YELLOW,MAGENTA,CYAN,WHITE}, --color {RED,GREEN,BLUE,YELLOW,MAGENTA,CYAN,WHITE}\n");
	printf("                        color option (default: RED)\n");
	printf("  --no-color            do not print color\n");
	printf("  --prefix PREFIX       print prefix before highlighted text (default: empty string)\n");
	printf("  --postfix POSTFIX     print postfix after highlighted text (default: empty string)\n\n");
	printf("examples:\n");
	printf("  ./highlighter -i file.txt \"error\" (highlights \"error\" in file.txt in red via STDOUT)\n");
	printf("  ./highlighter -c GREEN \"warning\" (highlights \"warning\" in green from STDIN to STDOUT)\n");
	printf("  ./highlighter --prefix \">>\" --postfix \"<<\" \"keyword\" -o out.txt (highlights \"keyword\" with prefix \">>\" and postfix \"<<\" in red from STDIN to out.txt)\n");
}

//printing an error message to stderr
void print_error(const char* error_message){
	fprintf(stderr, "Error: %s\n", error_message);
}

//printing how many highlighted matched the search text
void output_final_result(int count, const char* search_text){
	fprintf(stderr, "Highlighted %d matches.\n", count);
}

//returning the color code to the corresponding color
const char* get_color_code(const char* color_str){
	if(strcmp(color_str, "RED") == 0){
		return COLOR_RED;	
	}
	if(strcmp(color_str, "GREEN") == 0){
		return COLOR_GREEN;
	}
	if(strcmp(color_str, "BLUE") == 0){
                return COLOR_BLUE;
        }
	if(strcmp(color_str, "YELLOW") == 0){
                return COLOR_YELLOW;
        }
	if(strcmp(color_str, "MAGENTA") == 0){
                return COLOR_MAGENTA;
        }
	if(strcmp(color_str, "CYAN") == 0){
                return COLOR_CYAN;
        }
	if(strcmp(color_str, "WHITE") == 0){
                return COLOR_WHITE;
        }
	//invalid color input
	return NULL;
}


//parase CLI argument and inputing the setting structure 
int process_args(int argc, char* argv[], Settings* settings){
    	//initializing all the settings
	settings -> input_stream = stdin;
	settings -> output_stream = stdout;
	settings -> search_text = NULL;
	settings -> color = "RED";
	settings -> no_color = false;
	settings -> prefix = "";
	settings -> postfix = "";
	
	//loop though the arguments
	//start with 1 to skip the program name
	for(int i = 1; i < argc; i++){
		//parse arguments for -h or --help
		if(strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0){
			print_help();
			exit(0);
		}
		//parse arguments for input file
		else if(strcmp(argv[i], "-i") == 0 || strcmp(argv[i], "--input") == 0){
			//exceed argc means no argument after 
			if(i + 1 >= argc){
				print_error("Missing file argument after -i");
				return -1;
			}		
			//store the input file
			settings -> input_name = argv[++i];
		}
		//parse arguments for output file
		else if(strcmp(argv[i], "-o") == 0 || strcmp(argv[i], "--output") == 0){
			//exceed argc means no argument after
			if(i + 1 >= argc){
				print_error("Missing file argument after -o");
				return -1;
			}
			//store the output file
			settings -> output_name = argv[++i];
                }
		//parse arguments for color
		else if(strcmp(argv[i], "-c") == 0 || strcmp(argv[i], "--color") == 0){
                        //exceed argc means no argument after
			if(i + 1 >= argc){
				print_error("Missing color after -c");
				return -1;
                        }
			//point to color and check for NULL
            		settings -> color = argv[++i];
			if(settings -> no_color == false && get_color_code(settings -> color) == NULL){
				print_error("Invalid color");
				return -1;
			}
                }
		//parse arguments for no color
                else if(strcmp(argv[i], "--no-color") == 0){
                        settings -> no_color = true;
                }
		//parse arguments for prefix
		else if(strcmp(argv[i], "--prefix") == 0){
			//exceed argc means no argument after
			if(i + 1 >= argc){
				print_error("Missing text after --prefix");
				return -1;
                        }
			settings -> prefix = argv[++i];
                }
		//parse arguments for postfix
		else if(strcmp(argv[i], "--postfix") == 0){
			//exceed argc means no argument after
			if(i + 1 >= argc){
				print_error("Missing text after --postfix");
				return -1;
                        }
            		settings -> postfix = argv[++i];
                }
		//checking for incorrect syntax input by the user
                else if(argv[i][0] == '-' && strcmp(argv[i], "-") != 0){
                        print_error("Invalid option");
                        return -1;
                }
		//parse arguments for search text
		else if(settings -> search_text == NULL){
                        settings -> search_text = argv[i];
                }
		//else multiple arguments given
		else{
			print_error("Multiple positional arguments given");
			return -1;
		}
	}
	//check if it's searching anything
	if(settings -> search_text == NULL){
		print_error("No search text");
		return -1;
	}
	//rejecting empty serach string
	if(strlen(settings -> search_text) == 0){
		print_error("Search text can't be empty");
		return -1;
	}
	//success
	return 0;
}

//searching and retuning the number of matches
int parse_line(Settings* settings, char* line){ 
	char *match;
	char *position = line;
	size_t length = strlen(settings -> search_text);
	int count = 0;
	
	//loop until no more match
	while((match = strstr(position, settings -> search_text)) != NULL){
		//writing the output
		//print everything first 
		fwrite(position, 1, match - position, settings -> output_stream);
		
		//print the prefix
		fprintf(settings -> output_stream, "%s", settings -> prefix);
		//check for no color, if false print the corresponding color code
		if(settings -> no_color == false){
			fprintf(settings -> output_stream, "%s", get_color_code(settings -> color));
		}
		//print the matched text
		fwrite(match, 1, length, settings -> output_stream);
		//reset the color after printing the colored match
		if(settings -> no_color == false){
			fprintf(settings -> output_stream, "%s", COLOR_RESET_COLOR);
		}
		//print the post fix
		fprintf(settings -> output_stream, "%s", settings -> postfix);
		
		//move the point to the next
		count ++;
		position = match + length; 

	}
	//print the remaining line
	fprintf(settings -> output_stream, "%s", position);
	//return the number matches
	return count;
}
int main(int argc, char *argv[]) {
    // TODO: implement the highlighter program
    Settings settings = {0};
    char *line = NULL;
    size_t length = 0;
    ssize_t number_read;
    int total_matches = 0;
	
    //check for invalid and return the error code
    if(process_args(argc, argv, &settings) != 0){
	return 1;
    }

    //open input file if sepcified, else use stdin
    if(settings.input_name != NULL){
    	settings.input_stream = fopen(settings.input_name, "r");
	//exit if input file can't be open
	if(settings.input_stream == NULL){
		print_error("Unable to open input file");
		return 1;
	}
    }
    else{
    	settings.input_stream = stdin; 
    }

    //open output file if sepcified, else use stdout
    if(settings.output_name != NULL){
        settings.output_stream = fopen(settings.output_name, "w");
   	//exit if output file can't be open
   	if(settings.output_stream == NULL){
                print_error("Unable to open output file");
                return 1;
        }
    }
    else{
        settings.output_stream = stdout;
    }
    
    //loop through each line from the input
    number_read = getline(&line, &length, settings.input_stream);
    while(number_read != -1){
	//store the match count
	total_matches += parse_line(&settings, line);
	//move to next line	
	number_read = getline(&line, &length, settings.input_stream);
    }

    //return the output
    output_final_result(total_matches, settings.search_text);

    //close the files and free the memory
    if(settings.input_stream != stdin){
	fclose(settings.input_stream);
    }
    if(settings.output_stream != stdout){
	fclose(settings.output_stream);
    }
    free(line); 
   
    return 0; 
}
