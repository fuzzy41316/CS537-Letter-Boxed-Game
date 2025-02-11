#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// Letters are loaded from the board, dictionary to compare to, and seen letters for comparisons
char **letters = NULL;					// Letters loaded from the board, by line
int board_line_count = 0;
char **dict = NULL;						// Dictionary containing valid words for the game
int dict_line_count = 0;
bool seen[26] = {false};				// Letters seen in the board
bool letters_used[26] = {false};		// Letters used in the solving of the board
char last_letter_used = '\0';			// Last letter used, to be checked in solution according to rules (default of null)
char *buffer = NULL;
size_t length = 0;
ssize_t read_length = 0;

/*
 * Helper function to free memory: (board and dictionary upon exit)
 */
void free_memory(void) 
{
    if (letters != NULL) 
	{
        for (int i = 0; i < board_line_count; i++) 
		{
            if (letters[i] != NULL) 
			{
                free(letters[i]);  // Free each row of letters
            }
        }
        free(letters);  // Free the array of pointers
    }

    if (dict != NULL) 
	{
        for (int i = 0; i < dict_line_count; i++) 
		{
            if (dict[i] != NULL) 
			{
                free(dict[i]);  // Free each row of dict
            }
        }
        free(dict);  // Free the array of pointers
    }

	if (buffer != NULL)
	{
		free(buffer);
	}
}

/*
 * Helper function for loading the board from cmd-line args and validating the board
 */
void load_board(char *argv[])
{	
	// Open board, read contents, and store in global struct
	FILE *fp = fopen(argv[1], "r");
	if (fp == NULL)
	{
		free_memory();
		exit(1);
	}

	// Grab lines from board until EOF or -1 is returned from getline
	while ((read_length = getline(&buffer, &length, fp)) != EOF)
	{		
		// Check for duplicate letters
		for (int i = 0; i < (int)read_length; i++)
		{
			if (buffer[i] >= 'a' && buffer[i] <= 'z')
			{
				// Map index of seen character: 0 = 'a', 1 = 'b', etc.
				int index = buffer[i] - 'a';

				// Ignore null terminating character
				if (seen[index])
				{
					// Duplicate found
					printf("Invalid board\n");
					free_memory();
					fclose(fp);
					exit(1);
				}
				// Mark letter as seen otherwise
				else seen[index] = true;				
			}
		}

		// Store into letters array, if a letter
		if ((buffer[0] >= 'a') && (buffer[0] <= 'z'))
		{	
			// Dynamically allocate memory to store the buffer
			if (board_line_count == 0) letters = calloc(1, sizeof(char *));
			else letters = realloc(letters, (board_line_count + 1) * sizeof(char *));
			if (letters == NULL)
			{
				free_memory();
				fclose(fp);
				exit(1);
			}

			letters[board_line_count] = calloc((read_length + 1), sizeof(char));
			// If null, exit
			if (letters[board_line_count] == NULL)
			{
				free_memory();
				fclose(fp);
				exit(1);
			}

			// Store buffer into board
			strncpy(letters[board_line_count], buffer, read_length);
			board_line_count++;
		}
	}

	// if size of board (# lines) < 3 sides (lines), then exit program
	if (board_line_count < 3)
	{
		printf("Invalid board\n");
		free_memory();
		fclose(fp);
		exit(1);
	}
	fclose(fp);
	free(buffer);
	buffer = NULL;
}

/*
 * Helper function for loading the dictionary into a global variable for comparisons
 */
void load_dict(char *argv[])
{
	// Load dictionary from file, exit if unsuccessful
	FILE *fp = fopen(argv[2], "r");
	if (fp == NULL)
	{
		free_memory();
		exit(1);
	}

	while ((read_length = getline(&buffer, &length, fp)) != -1)
	{
		// Store into dict array, if a letter
		if ((buffer[0] >= 'a') && (buffer[0] <= 'z'))
		{	
			// Dynamically allocate memory to store the buffer
			if (dict_line_count == 0) dict = calloc(1, sizeof(char *));
			else dict = realloc(dict, (dict_line_count + 1) * sizeof(char *));
			if (dict == NULL)
			{
				free_memory();
				fclose(fp);
				exit(1);
			}

			dict[dict_line_count] = calloc((read_length + 1), sizeof(char));
			// If null, exit
			if (dict[dict_line_count] == NULL)
			{
				free_memory();
				fclose(fp);
				exit(1);
			}

			// Store buffer into dict
			strncpy(dict[dict_line_count], buffer, read_length);
			dict_line_count++;
		}
	}

	// Close file
	fclose(fp);
	free(buffer);
	buffer = NULL;
}
/*
 * Helper function that polls for input from the user, checking if solution is correct
 */
void solution(void)
{
	// Open STDIN, and read words from input	
	while ((read_length = getline(&buffer, &length, stdin)) != EOF)
	{
		// All letters used are to be present on board
		for (int i = 0; i < (int)read_length; i++)
		{
			// Ignore carriage and terminating character
			if (((buffer[i] >= 'a') && (buffer[i] <= 'z')))
			{
				// Align letter with indexing of seen array, to see if it's on the board
				int index = buffer[i] - 'a';

				// If not seen, then the letter isn't apart of the board, invalid solution
				if (!seen[index])
				{
					printf("Used a letter not present on the board\n");
					free_memory();
					exit(0);
				}
			}
		}
		
		// First character of word in solution must match the preceding word's last character
		if (last_letter_used != '\0')	// Check for default character (rule doesn't apply to first word)
		{
			if (last_letter_used != buffer[0] && ((buffer[0] >= 'a') && (buffer[0] <= 'z')))
			{
				printf("First letter of word does not match last letter of previous word\n");
				free_memory();
				exit(0);
			}
			else 
			{
				last_letter_used = buffer[read_length - 2];	// Set new last letter to the current word
			}
		}	
		else last_letter_used = buffer[read_length - 2];	// This is for the first word

		// Letters cannot be used consecutively on same side of board (same line)
		for (int i = 0; i < (int)read_length - 2; i++)
		{
			char letter1 = buffer[i];
			char letter2 = buffer[i + 1];

			for (int line = 0; line < board_line_count; line++)
			{
				// Search for chosen consecutive letters on the same side of the board
				char *pos1 = strchr(letters[line], letter1);
				char *pos2 = strchr(letters[line], letter2);

				// Found in same side (line), check if together in word
				if (pos1 && pos2)
				{
					printf("Same-side letter used consecutively\n");
					free_memory();
					exit(0);
				}
			}
		}

		// Word must be found in dictionary	
		bool letter_found = false;

		// If line is not apart of the alphabet (null, newline, etc.) then ignore
		if ((buffer[0] < 'a') || (buffer[0] > 'z')) letter_found = true;

		// Iterate through dictionary
 		for (int i = 0; i < dict_line_count; i++)
		{
			// Remove new line from words
			buffer[strcspn(buffer, "\n")] = 0;
			dict[i][strcspn(dict[i], "\n")] = 0;

			if (strcmp(buffer, dict[i]) == 0) letter_found = true;
		}
		
		if (!letter_found)
		{
			printf("Word not found in dictionary\n");
			free_memory();
			exit(0);
		}

		// Track letters used in this solution
		for (int i = 0; i < (int)read_length; i++)
		{
			// Only track letters between a and z 
			if (buffer[i] >= 'a' && buffer[i] <= 'z')
			{
				int index = buffer[i] - 'a';	// Adapt index to 0 = 'a', 1 = 'b', etc.
				letters_used[index] = true;
			}
		}

		// Determine if solution is correct...
		bool completed_solution = true;
		for (int i = 0; i < 26; i++)
		{
			// If letters used in the solution matches all of the letters seen in the board, then solution was correct
			if (seen[i] && !letters_used[i]) completed_solution = false;
		}
		if (completed_solution)
		{
			printf("Correct\n");
			free_memory();
			exit(0);
		}
	}
	// Must use all letters on the board
	printf("Not all letters used\n");
	free_memory();
	exit(0);
}

/*
 * Board takes two command line arguments: a board file and dictionary file in .txt format,
 * then it reads words from standard input (STDIN) until the board is solved or an invalid
 * solution is attempted
 * EX: ./letter-boxed.c board1.txt dict.txt
 *
 */
int main(int argc, char *argv[])
{
	// Access board file if arguments are board.txt, dict.txt, otherwise exit with error code 1
	if (argc < 3) exit(1);

	// Load board, and determine if it's valid or not
	load_board(argv);

	// Load the dictionary
	load_dict(argv);

	// Check for solutions
	solution();
}	
