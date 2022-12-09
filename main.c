//
//  Author: Arwa Mazher
//  System: Replit
//  Description: This game works just like NY Times Wordle, however the computer guesses the same way a real person would.
//               The computer will guess each word in 15 guesses or less.
//  Links to wordle dictionary words at:
//    https://www.reddit.com/r/wordle/comments/s4tcw8/a_note_on_wordles_word_list/

#include <stdio.h>    // for printf(), scanf()
#include <stdlib.h>   // for exit( -1)
#include <string.h>   // for strcpy
#include <assert.h>   // for assert() sanity checks
#include <ctype.h>    // for toupper()
#include <time.h>     // for time()

// Declare globals
#define WORD_LENGTH 5     // All words have 5 letters, + 1 NULL at the end when stored
#define WORDS_FILE_NAME "wordsLarge.txt"
//#define WORDS_FILE_NAME  "wordsTiny.txt"
#define MAX_NUMBER_OF_WORDS 12947   // Number of words in the full set of words file
#define true 1   // Make boolean logic easier to understand
#define false 0  // Make boolean logic easier to understand

typedef struct wordCount wordCountStruct;
struct wordCount{
    char word[ WORD_LENGTH + 1];   // The word length plus NULL
    int score;                     // Score for the word
};


//-----------------------------------------------------------------------------------------
// Read in words from file into array.  We've previously read the data file once to
// find out how many words are in the file.
void readWordsFromFile(
            char fileName[],        // Filename we'll read from
            wordCountStruct *words, // Array of words where we'll store words we read from file
            int *wordCount)          // How many words.  Gets updated here and returned
{
    FILE *inFilePtr  = fopen(fileName, "r");  // Connect logical name to filename
    assert( inFilePtr != NULL);               // Ensure file open worked correctly

    // Read each word from file and store into array, initializing the score for that word to 0.
    char inputString[ 6];
    *wordCount = 0;
    while( fscanf( inFilePtr, "%s", inputString) != EOF) {
        strcpy( words[ *wordCount].word, inputString);
        words[ *wordCount].score = 0;
        (*wordCount)++;
    }

    // Close the file
    fclose( inFilePtr);
} // end readWordsFromFile(..)


//-----------------------------------------------------------------------------------------
// Comparator for use in built-in qsort(..) function.  Parameters are declared to be a
// generic type, so they will match with anything.
// This is a two-part comparison.  First the scores are compared.  If they are the same,
// then the words themselves are also compared, so that the results are in descending
// order by score, and within score they are in alphabetic order.
int compareFunction( const void * a, const void * b) {
    // Before using parameters we have cast them into the actual type they are in our program
    // and then extract the numerical value used in comparison
    int firstScore = ((wordCountStruct *) a)->score;
    int secondScore = ((wordCountStruct *) b)->score;

    // If scores are different, then that's all we need for our comparison.
    if (firstScore != secondScore) {
        // We reverse the values, so the result is in descending vs. the otherwise ascending order
        // return firstScore - secondScore;   // ascending order
        return secondScore - firstScore;      // descending order
    }
    else {
        // Scores are equal, so check words themselves, to put them in alphabetical order
        return strcmp( ((wordCountStruct *)a)->word,  ((wordCountStruct *)b)->word );
    }
} //end compareFunction(..)

// -----------------------------------------------------------------------------------------
// Find a secret word
void findSecretWord(
        wordCountStruct allWords[],    // Array of all the words
        int wordCount,                  // How many words there are in allWords
        char secretWord[])              // The word to be guessed
{
    char computerGuess[6];  // Allocate space for the computer guess
		wordCountStruct allWordsCopy[wordCount];
		for (int i = 0; i < wordCount; i++) {
			allWordsCopy[i] = allWords[i];
		}
    
    printf("Trying to find secret word: \n");
    // Display secret word with a space between letters, to match the guess words below.
    printf("       ");
    for( int i=0; i<WORD_LENGTH; i++) {
        printf("%c ", secretWord[ i]);
    }
    printf("\n");
    printf("\n");
    
    // Loop until the word is found
    int guessNumber = 1;
		strcpy(computerGuess, "arise");
	
    //while (true) {
			while (guessNumber != 16) {
			int scores[5] = {0, 0, 0, 0, 0};
  		int flags[5] = {0, 0, 0, 0, 0};
			
			printf("%5d. ", guessNumber);
			char guessCopy[6];
			char secretCopy[6];
			char displayChars[5] = {' ', ' ', ' ', ' ', ' '};
			strcpy(guessCopy, computerGuess);
			strcpy(secretCopy, secretWord);

			for (int i = 0; i < 5; i++) {
				if (computerGuess[i] == secretWord[i]) {
					printf("%c ", toupper(guessCopy[i]));
					guessCopy[i] = ' ';
					secretCopy[i] = ' ';
					displayChars[i] = ' ';
					scores[i] = 3;
					flags[i] = 1;
					// take out words that don't have letter in same spot
					for (int j = 0; j < wordCount; j++) {
						if (allWordsCopy[j].word[i] != secretWord[i]) {
							strcpy(allWordsCopy[j].word, "");
						}
					}
				}
				// look for letters that may be in the wrong position
				else {
					printf("%c ", computerGuess[i]);
					if (scores[i] == 0) {
						for (int j = 0; j < 5; j++) {
							if (computerGuess[i] == secretWord[j] && flags[j] == 0) {
								guessCopy[i] = ' ';
								secretCopy[j] = ' ';
								displayChars[i] = '*';
								scores[i] = 1;
          			flags[j] = 1;
								break;
							}
					}
					}
				} // end else {}
			} // end for(int i...)

			// if guessed the correct word	
			if (strcmp(computerGuess, secretWord) == 0) {
				printf("\n");
				printf("Got it!\n");
				break;
			}

			// display spaces and asterisk information
			printf("\n");
			printf("       ");
			for (int i = 0; i < 5; i++) {
        printf("%c ", displayChars[i]);
    	}
    	printf("\n");

			// take out words that have letters that don't exist in secretWord
			for (int i = 0; i < wordCount; i++) {
				for (int j = 0; j < 5; j++) {
					if (allWordsCopy[i].word[j] == computerGuess[j] && (scores[j] == 0 || scores[j] == 1)) {
						strcpy(allWordsCopy[i].word, "");
					}

					// take out words that have letters in incorrect spots
					for (int k = 0; k < 5; k++) {
						if (allWordsCopy[i].word[j] == computerGuess[k] && (scores[k] == 0)) {
							int flag = false;
							for(int m = 0; m < 5; m++){
								if(computerGuess[k] == computerGuess[m] && m != k){
									flag = true;
								}
							}
								
							if(flag == false){
								strcpy(allWordsCopy[i].word, "");
							}
						}
					}
				} // end fot (int j...)
			} // end for (int i...)

			// set the new computerGuess
			for (int i = 0; i < wordCount; i++) {
				if (strcmp(allWordsCopy[i].word, "") != 0) {
					strcpy(computerGuess, allWordsCopy[i].word);
					break;
				}
			}
			// Update guess number
			guessNumber++;
    } //end while(...)
	for(int i = 0; i < wordCount; i++){
		if(strcmp(allWords[i].word,secretWord) == 0){
			allWords[i].score = guessNumber;
		}
	}
} //end findSecretWord


// -----------------------------------------------------------------------------------------
int main() {
    char wordsFileName[81];                   // Stores the answers file name
    strcpy(wordsFileName, WORDS_FILE_NAME);   // Set the filename, defined at top of program.
    srand( (unsigned) time( NULL));           // Seed the random number generator to be current time
    // Declare space for all the words, of a maximum known size.
    wordCountStruct allWords[ MAX_NUMBER_OF_WORDS];
    // Start out the wordCount to be the full number of words.  This will decrease as
    //    play progresses each time through the game.
    int wordCount = 0;
    // The secret word that the computer will try to find, plus the return character from fgets.
    char secretWord[ WORD_LENGTH + 1];
    char userInput[ 81];                // Used for menu input of secret word

    // Read in words from file, update wordCount and display information
    readWordsFromFile( wordsFileName, allWords, &wordCount);
    printf("Using file %s with %d words. \n", wordsFileName, wordCount);

    // Run the word-guessing game three times
    for( int i=0; i<3; i++) {
        // Reset secret Word
        strcpy( secretWord, "");
        // Display prompt
        printf("-----------------------------------------------------------\n");
        printf("\n");
        printf("Enter a secret word or just r to choose one at random: ");
        scanf(" %s", userInput);
        // Eliminate the return character at end or userInput if it is there
        int length = (int) strlen( userInput);
        if( userInput[ length] == '\n') {
            userInput[ length] = '\0';
        }
        strcpy( secretWord, userInput);   // Store the secret word from user input
                
        // If input was 'r' then choose a word at random.
        if( strlen( secretWord) <= 1) {
            // Randomly select a secret word to be guessed.
            int randomIndex = rand() % wordCount;
            strcpy( secretWord, allWords[ randomIndex].word);
        }
        
        // Run the game once with the current secret word
        findSecretWord( allWords, wordCount, secretWord);
    }
	
    printf("Done\n");
    printf("\n");
    return 0;
} // end main()