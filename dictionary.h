#pragma once

// This file includes code for importing and containing raw dictionary file from disk, as well for storing word connection weights.

#include <map>
#include <random>
#include <functional>
#include <ctime>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

namespace randPhrase {
	struct generator {
		std::default_random_engine theGenerator;
		std::uniform_int_distribution<int> distribution;
	};

	struct wordLink {
		int linkedindex;
		double weight; // 0 - 1, 2 for special
	};
	struct word {
		std::string theWord;
		std::vector<wordLink> wordLink;
	};
	class dictionary {
	private:
		std::map<int, word> _wordContainer;
		generator randGen;

		double lengthFactor;

		void changeWeight(int index, int i, double weight);
		void shiftWeight(int linkIndex, int index, double weight);
		int findWordIndex(std::string word);

		std::string sanitizeWord(std::string word);

	public:
		dictionary(std::string file);

		void loadDictionary(std::string file);
		std::string makePhrase();
		void forcePhrase(std::string phrase);
		void writeWordsWithNumChar(int numChars);

		int makeRandInt();
		double makeRandDouble();

		//~dictionary();
	};
}