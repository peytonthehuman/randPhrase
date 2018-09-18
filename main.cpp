#include "dictionary.h"
#include <stdlib.h>
#include <queue>

#ifdef _WIN32
#include <windows.h>

void sleep(unsigned milliseconds) {
	Sleep(milliseconds);
}
#else
#include <unistd.h>

void sleep(unsigned milliseconds) {
	usleep(milliseconds * 1000); // takes microseconds
}
#endif

int main() {

	randPhrase::dictionary newDict("C:\\Users\\pvanhook\\source\\usa2.txt");
	newDict.forcePhrase("are you okay");
	newDict.forcePhrase("who am i");
	newDict.forcePhrase("who are you");
	newDict.forcePhrase("how did i get here");
	newDict.forcePhrase("let the day go by");
	newDict.forcePhrase("there is water underground");
	newDict.forcePhrase("here is a television that looks like an apple");
	newDict.forcePhrase("get up off of that thing");
	newDict.forcePhrase("hello i am a computer");

	std::queue<std::string> tempWords;
	for (int i = 0; i < 11; i++) {
		tempWords.push(newDict.makePhrase());
	}
	std::string bufferStr = "";
	while (true) {
		bufferStr = tempWords.front();
		tempWords.pop();
		tempWords.push(newDict.makePhrase());
		for (int i = 0; i < bufferStr.length(); i++) {
			std::cout << bufferStr[i];
			sleep(bufferStr[i] == ' ' ? 60 : 100 + (newDict.makeRandInt() % 80 - 40));
		}
		sleep(500 + (newDict.makeRandInt() % 200));
	}
	return 0;
}

/* Ideas to modify:

split up randPhrase generation code into clause constructor, word picker, grammar picker, etc.

Make rand generator into proper wrapper object which can be formed seperately and given to the the object.

Maybe break the dictionary class up between a dictionary holder for the raw words, a tree holder for saved grammar trees,
and a generator class for price generator.

Use parse trees to use randomly choosen grammars to generate sentences on, rather than raw random word picking.*/