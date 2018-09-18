#include "dictionary.h"

// Impliments dictionary file functions.

//std::map<int, word> _wordContainer;

void randPhrase::dictionary::changeWeight(int index, int i, double weight) {
	double tempWeight = (_wordContainer.at(index).wordLink[i].weight + weight) / 2.0;//(makeRandDouble() * 2);
	if (tempWeight > 1) {
		tempWeight = 1.0;
	}
	else if (tempWeight < 0) {
		tempWeight = 0.0;
	}
	_wordContainer.at(index).wordLink[i].weight = tempWeight;
}

void randPhrase::dictionary::shiftWeight(int linkIndex, int index, double weight) {
	int i;
	int savIndex;
	for (i = 0; i < _wordContainer.at(index).wordLink.size(); i++) {
		if (_wordContainer.at(index).wordLink[i].linkedindex == linkIndex) {
			break;
		}
	}
	if (i != _wordContainer.at(index).wordLink.size()) {
		changeWeight(index, i, weight);
		savIndex = i;
		for (i = 0; i < _wordContainer.at(index).wordLink.size(); i++) {
			if (i == savIndex) {
				continue;
			}
			//changeWeight(index, i, -(weight/100));
		}
	}
}

int randPhrase::dictionary::findWordIndex(std::string word) {
	int retIndex = -1;
	for (int i = 0; i < _wordContainer.size(); i++) {
		if (_wordContainer.at(i).theWord == word) {
			retIndex = i;
			break;
		}
	}
	return retIndex;
}

std::string randPhrase::dictionary::sanitizeWord(std::string word) {
	int attributeStart = 0;
	for (int i = 0; i < word.length(); i++) {
		if (word[i] == '/') {
			attributeStart = i;
			break;
		}
	}

	if (attributeStart == 0) {
		return word;
	}

	word.resize(attributeStart);

	for (int i = 0; i < word.length(); i++ ) {
		std::transform(word.begin(), word.end(), word.begin(), ::tolower);
	}
	return word;
}

randPhrase::dictionary::dictionary(std::string file) {
	loadDictionary(file);
	randGen.theGenerator.seed(time(NULL));
	lengthFactor = 5.5;
}

void randPhrase::dictionary::loadDictionary(std::string file) {
	std::ifstream dictFile;
	dictFile.open(file);
	word tempWord;
	int index = 0;
	// Start code
	tempWord.theWord = "$START_CODE";
	_wordContainer.insert(std::pair<int, word>(index, tempWord));
	do {
		index++;
		dictFile >> tempWord.theWord;
		tempWord.theWord = sanitizeWord(tempWord.theWord);
		_wordContainer.insert(std::pair<int, word>(index, tempWord));
	} while (!dictFile.eof());

	// End codes
	index++;
	tempWord.theWord = "$END_CODE";
	_wordContainer.insert(std::pair<int, word>(index, tempWord));
	return;
}

std::string randPhrase::dictionary::makePhrase() {
	int randIndex = 0;
	int lastIndex = 0;
	int realWords = 0;
	int numWords = 0;
	int numLearned = 0;
	std::string retString = "";

	while (true) {
		wordLink maxLastWeight = { -1, 0 };
		double randWeight;

		while (true) {
			randWeight = makeRandDouble();
			randIndex = makeRandInt() % _wordContainer.size();
			if (randIndex == 0 || randIndex == (_wordContainer.size())) {
				continue;
			}
			if (randWeight <= pow((1.0 / _wordContainer.at(randIndex).theWord.length()), lengthFactor) * 10000) {
				break;
			}
		}
		if (_wordContainer.at(randIndex).theWord.size() > lengthFactor) {
			lengthFactor += 2.5;
		}
		else {
			lengthFactor -= lengthFactor <= 0 ? -0.5 : 2.5;
		}
		randWeight = makeRandDouble() < 0.3 ? 2.0 : makeRandDouble();
		bool lastIndexInList = false;
		for (int i = 0; i < _wordContainer.at(lastIndex).wordLink.size(); i++) {
			lastIndexInList = lastIndexInList || _wordContainer.at(lastIndex).wordLink[i].linkedindex == randIndex;
		}
		if (!lastIndexInList) {
			for (int i = 0; i < _wordContainer.at(lastIndex).wordLink.size(); i++) {
				if (_wordContainer.at(lastIndex).wordLink[i].weight > maxLastWeight.weight) {
					maxLastWeight.linkedindex = _wordContainer.at(lastIndex).wordLink[i].linkedindex;
					maxLastWeight.weight = _wordContainer.at(lastIndex).wordLink[i].weight;
				}
			}
			if (maxLastWeight.weight >= randWeight/5) {
				randIndex = maxLastWeight.linkedindex;
				numLearned++;
			}
			else {
				wordLink tempLink;
				tempLink.linkedindex = randIndex;
				tempLink.weight = 0;
				_wordContainer.at(lastIndex).wordLink.push_back(tempLink);
			}
		}
		shiftWeight(randIndex, lastIndex, randWeight == 2.0 ? makeRandDouble() : randWeight);
		if (randIndex == _wordContainer.size()) {
			break;
		}
		lastIndex = randIndex;
		retString += _wordContainer.at(randIndex).theWord;
		numWords++;
		if (makeRandDouble() >= pow((1.0 / numWords), 2) * 10) {
			retString += ", ";
			realWords += numWords;
			numWords = 0;
		}
		else {
			retString += " ";
		}
		double endWeight = (makeRandDouble() / 10) * (realWords);
		if (endWeight > randWeight && endWeight > maxLastWeight.weight) {
			break;
		}
	}

	int percentage = (numLearned * 1.0 / realWords) * 100;
	if (retString[retString.length() - 2] == ',') {
		retString.resize(retString.length() - 1);
	}
	retString[retString.length() - 1] = (makeRandDouble() < 0.3 ? '?' : '.');
	retString += " (" + std::to_string(retString.length()) + ", " + std::to_string(realWords + 1) + ", " + std::to_string(percentage);
	retString += ") ";
	for (int i = 0; i < retString.length(); i++) {
		retString[i] = toupper(retString[i]);
	}
	lengthFactor = 5.5;
	//return std::to_string(percentage) + " - ";
	return retString;
}

void randPhrase::dictionary::forcePhrase(std::string phrase) {
	std::vector<std::string> tempList;
	std::vector<int> tempIndexes;
	std::string tempWord = "";
	for (int i = 0; i < phrase.length() + 1; i++) {
		if (phrase[i] == ' ' || i == phrase.length()) {
			tempList.push_back(tempWord);
			tempWord = "";
			continue;
		}
		tempWord += phrase[i];
	}
	for (int i = 0; i < tempList.size(); i++) {
		tempIndexes.push_back(findWordIndex(tempList[i]));
	}
	tempIndexes.push_back(_wordContainer.size());
	for (int i = 0; i < tempList.size(); i++) {
		wordLink tempLink = { tempIndexes[i + 1], 2 };
		_wordContainer.at(tempIndexes[i]).wordLink.push_back(tempLink);
	}
}

void randPhrase::dictionary::writeWordsWithNumChar(int numChars) {
	std::vector<std::string> listWords;
	for (int i = 0; i < _wordContainer.size(); i++) {
		if (_wordContainer.at(i).theWord.length() == numChars) {
			listWords.push_back(_wordContainer.at(i).theWord);
		}
	}
	for (int i = 0; i < listWords.size(); i++) {
		std::cout << listWords[i] << std::endl;
	}
	return;
}

int randPhrase::dictionary::makeRandInt() {
	return randGen.distribution(randGen.theGenerator);
}

double randPhrase::dictionary::makeRandDouble() {
	return (makeRandInt() * 1.0) / INT_MAX;
}