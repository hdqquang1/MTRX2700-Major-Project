#include "headsUp.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM_WORDS 100 // Total number of words in the array

char* randomWord() {
    static char* words[NUM_WORDS]; // Static array to store words
    static int usedIndices[NUM_WORDS] = {0}; // Static array to track used indices
    static int wordsCount = 0; // Count of words added to the array

    // Check if all words have been used, reset if needed
    if (wordsCount == NUM_WORDS) {
        wordsCount = 0;
        memset(usedIndices, 0, sizeof(usedIndices));
    }

	char *allWords[] = {
	        "airplane", "boat", "baby", "ears", "scissors", "cough",
	        "cold", "phone", "laugh", "blink", "hairbrush", "sneeze",
	        "spin", "hammer", "book", "phone", "toothbrush", "jump",
	        "clap", "slap", "archer", "ghost", "balance", "shoelaces", "sick", "balloon",
	        "banana peel", "monster", "hiccup", "stomp", "hurt", "hungry",
	        "slip", "karate", "ladder", "scare", "fishing", "dizzy",
	        "read", "hot", "birthday", "president", "apartment", "cradle", "coffee", "trampoline",
	        "waterfall", "window", "proud", "stuck-up", "flashlight", "marry",
	        "overwhelm", "judge", "shadow", "halo", "measure", "clown",
	        "chomp", "slither", "Whale", "Snake", "Elephant", "Giraffe", "Dog", "Cat",
	        "Ant", "Rabbit", "Groundhog", "Hyena", "Kangaroo", "Shark",
	        "Fish", "Polar Bear", "Caterpillar", "Cockroach", "Ram", "Monkey",
	        "Jaguar", "Rooster", "Cycling", "Watching TV", "Swimming", "Running",
	        "Flying in a plane", "Sleeping", "Flipping pancakes", "Listening to music",
	        "Driving a car", "Writing a letter", "Skiing", "Giving a haircut",
	        "Drinking", "Typing", "Eating", "Having a snowball fight", "Playing the guitar",
	        "Surfing", "Yawning", "Playing golf", "Praying", "Brushing teeth",
	        "Showering", "Meditating", "Singing"
	};

    int numWords = sizeof(allWords) / sizeof(allWords[0]); // Calculate the number of words in the array

    int randomIndex;
    do {
        randomIndex = rand() % numWords; // Generate a random index within the bounds of the array
    } while (usedIndices[randomIndex]); // Check if the word at this index has been used

    usedIndices[randomIndex] = 1; // Mark this index as used
    words[wordsCount++] = allWords[randomIndex]; // Add the word to the array

    return words[wordsCount - 1]; // Return the last added word
}
