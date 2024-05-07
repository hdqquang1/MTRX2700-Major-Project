#include "headsUp.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

char* randomWord() {
	char *words[] = {
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

    int numWords = sizeof(words) / sizeof(words[0]); // Calculate the number of words in the array
    int randomIndex = rand() % numWords; // Generate a random index within the bounds of the array
    return words[randomIndex]; // Return the randomly chosen word
}
