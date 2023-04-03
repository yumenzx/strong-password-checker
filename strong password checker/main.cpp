#include<iostream>
#include<string>
#include<queue>


enum class FailedCondition {
	NO_FAILED_CONDITION, // all condtions are meet

	TOO_FEW_CHARACTERS, // from condition 1
	TOO_MANY_CHARACTERS,

	NO_LOWERCASE_LETTER, // from condition 2
	NO_UPPERCASE_LETTER,
	NO_DIGIT,

	REPEATING_CHARACTERS // from condition 3
};


class Condition {
private:
	FailedCondition condition;

public:

	Condition() : condition(FailedCondition::NO_FAILED_CONDITION) {
	};

	Condition(FailedCondition c) : condition(c) {
	};

	void operator=(FailedCondition otherCondition) {
		condition = otherCondition;
	};

	bool operator==(FailedCondition otherCondition) {
		return condition == otherCondition;
	};


	bool noFailedCondition() {
		return condition == FailedCondition::NO_FAILED_CONDITION;
	};

	friend std::ostream& operator<<(std::ostream& os, const Condition& c)
	{
		switch (c.condition)
		{
		case FailedCondition::NO_FAILED_CONDITION:
			os << "NO_FAILED_CONDITION";
			break;
		case FailedCondition::TOO_FEW_CHARACTERS:
			os << "TOO_FEW_CHARACTERS";
			break;
		case FailedCondition::TOO_MANY_CHARACTERS:
			os << "TOO_MANY_CHARACTERS";
			break;
		case FailedCondition::NO_LOWERCASE_LETTER:
			os << "NO_LOWERCASE_LETTER";
			break;
		case FailedCondition::NO_UPPERCASE_LETTER:
			os << "NO_UPPERCASE_LETTER";
			break;
		case FailedCondition::NO_DIGIT:
			os << "NO_DIGIT";
			break;
		case FailedCondition::REPEATING_CHARACTERS:
			os << "REPEATING_CHARACTERS";
			break;
		default:
			os << "ERROR";
			break;
		}
		return os;
	}
};

/*
* function returns true if the string contains at least one lowercase letter
*/
bool containsLowercaseLetter(std::string s)
{
	for (char c : s)
		if (c >= 'a' && c <= 'z')
			return true;
	return false;
}

/*
* function returns true if the string contains at least one uppercase letter
*/
bool containsUppercaseLetter(std::string s)
{
	for (char c : s)
		if (c >= 'A' && c <= 'Z')
			return true;
	return false;
}

/*
* function returns true if the string contains at least one digit
*/
bool containsDigit(std::string s)
{
	for (char c : s)
		if (c >= '0' && c <= '9')
			return true;
	return false;
}


/*
* this function checks the condition 1
* if the first condition is not meet, we have 2 cases: too few chars and too many chars
*	too few chars -> we append to the string chars until the length is 6
*				and we try to append chars to meet the second condition:
*					if s not contains lowercase letter then append one lowercase letter
* 					if s not contains uppercase letter then append one uppercase letter
* 					if s not contains digit then append one digit
*	too many chars -> we resize the string to 20
*/
inline FailedCondition checkCondition1(std::string& s, int& nrOfChanges)
{
	int length = s.length();
	int nr = 0;

	char a = 'a';
	if (length < 6) {
		nr = 6 - length;
		// append chars until the length is 6
		for (int i = 0; i < nr; i++) {
			if (!containsLowercaseLetter(s))
				s += 'a';
			else if (!containsUppercaseLetter(s))
				s += 'A';
			else if (!containsDigit(s))
				s += '0';
			else
				s += ++a;
		}
		nrOfChanges = nr;
		return FailedCondition::TOO_FEW_CHARACTERS;
	}

	if (length > 20) {
		nr = abs(20 - length);
		s.resize(20);
		nrOfChanges = nr;
		return FailedCondition::TOO_MANY_CHARACTERS;
	}

	nrOfChanges = nr;
	return FailedCondition::NO_FAILED_CONDITION;
}


/*
* this function checks the second condition
* if the second condition is not meet, we have 3 cases: the string does not contain at least one lowercase letter or uppercase letter or digit
*  if the condition is not meet then we have 3 cases: no lowercase letter, no uppercase letter or no digit
*		no lowercase letter -> if the length is smaller then 6 then we append a lowercase letter
*								else if we have at least 2 uppercase letters or digits, then we change to a lowercase letter
*		no uppercase letter & no no digit -> the same way
*/
FailedCondition checkCondition2(std::string& s,int& nrOfChanges)
{
	bool containsLowercaseLetter, containsUppercaseLetter, containsDigit;
	containsLowercaseLetter = containsUppercaseLetter = containsDigit = false;
	int nr = 0;
	std::queue<int> lowercasePossitions;
	std::queue<int> uppercasePossitions;
	std::queue<int> digitPossitions;
	
	for(int i=0;i<s.length();i++){  // for each char in string
		char c = s[i];
		if (c >= 'a' && c <= 'z') {  // if is lowercase letter
			containsLowercaseLetter = true; // mark that it contains at least one lowercase letter
			lowercasePossitions.push(i);  // save the index of the lowercase letter
			continue;
		}

		if (c >= 'A' && c <= 'Z') { // if is uppercase letter
			containsUppercaseLetter = true; // mark that it contains at least one uppercase letter
			uppercasePossitions.push(i); // save the index of the uppercase letter
			continue;
		}

		if (c >= '0' && c <= '9') { // if is digit
			containsDigit = true;  // mark that it contains at least one digit
			digitPossitions.push(i); // save the index of the digit
		}
	}

	if (lowercasePossitions.empty()) { // if the string does not contain at least one lowercase letter
		if (s.length() < 6)		// if the legth is smaller then 6, then append a lowercase letter
			s += 'a';
		else if (uppercasePossitions.size() >= 2) { // else if we have at least 2 uppercase letters then change the first uppercase letter to a lowercase letter
			s.at(uppercasePossitions.front()) = 'a';
			uppercasePossitions.pop();
		}
		else if (digitPossitions.size() >= 2) { // else if we have at least 2 digits then change the first digit to a lowercase letter
			s.at(digitPossitions.front()) = 'a';
			digitPossitions.pop();
		} else  // else just append one lowercase letter
			s += 'a';
		nr++;
	}

	if (uppercasePossitions.empty()) { // if the string does not contain at least one uppercase letter
		if (s.length() < 6) // if the legth is smaller then 6, then append a uppercase letter
			s += 'A';
		else if (lowercasePossitions.size() >= 2) { // else if we have at least 2 lowercase letters then change the first lowercase letter to a uppercase letter
			s.at(lowercasePossitions.front()) = 'A';
			lowercasePossitions.pop();
		}
		else if (digitPossitions.size() >= 2) { // else if we have at least 2 digits then change the first digit to a uppercase letter
			s.at(digitPossitions.front()) = 'A';
			digitPossitions.pop();
		} else // else just append one uppercase letter
			s += 'A';
		nr++;
	}

	if (digitPossitions.empty()) { // if the string does not contain at least one digit
		if (s.length() < 6) // if the legth is smaller then 6, then append a digit
			s += '0';
		else if (uppercasePossitions.size() >= 2) { // else if we have at least 2 uppercase letters then change the first uppercase letter to a digit
			s.at(uppercasePossitions.front()) = '0';
			uppercasePossitions.pop();
		}
		else if (lowercasePossitions.size() >= 2) { // else if we have at least 2 lowercase letters then change the first lowercase letter to a digit
			s.at(lowercasePossitions.front()) = '0';
			lowercasePossitions.pop();
		} else // else just append one digit
			s += '0';
		nr++;
	}
	
	nrOfChanges = nr;
	// return a status
	if (containsLowercaseLetter == false) 
		return FailedCondition::NO_LOWERCASE_LETTER;
	if (containsUppercaseLetter == false)
		return FailedCondition::NO_UPPERCASE_LETTER;
	if (containsDigit == false)
		return FailedCondition::NO_DIGIT;

	return FailedCondition::NO_FAILED_CONDITION;
}


/* 
* this function checks the third condition
* if the condition is not meet, then we have 3 repeating characters in a row
*		go through the string and we verify the following 2 characters
*		if they are equal then we change the third character in this way
*			if the string does not contain at least one lowercase letter then we change to a lowercase letter
*			if the string does not contain at least one uppercase letter then we change to a uppercase letter
*			if the string does not contain at least one digit then we change to a digit
* 
*/
FailedCondition checkCondition3(std::string& s,int &nrOfChanges)
{
	int nr = 0;
	std::string::const_iterator end = s.end() - 2;
	for (auto it = s.begin(); it < end; ++it) { // for each character
		char c = *it;
		if (c == *(it + 1) && c == *(it + 2)) { // if the character is equal to the following 2
			nr++;
			if (!containsLowercaseLetter(s)) // if not contains at least one lowercase letter
				*(it + 2) = 'a';	// change to a lowercase letter
			else if (!containsUppercaseLetter(s)) // if not contains at least one uppercase letter
				*(it + 2) = 'A';	// change to a uppercase letter
			else if (!containsDigit(s))  // if not contains at least one digit
				*(it + 2) = '1'; // change to a digit
			else
				*(it + 2) = c + 1; // change the character to next character in the alphabet
		}
	}

	nrOfChanges = nr;
	if (nr)
		return FailedCondition::REPEATING_CHARACTERS;

	return FailedCondition::NO_FAILED_CONDITION;
}

int minimumNrOfChanges(std::string& s)
{
	Condition condition1Meet, condition2Meet, condition3Meet;

	bool cond1Meet, cond2Meet, cond3Meet;

	int nrOfChanges = 0;
	bool passNotStrong;
	do {
		passNotStrong = false;
		int nr;

		condition3Meet = checkCondition3(s, nr); // we check the third condition first because it can make the string to meet the second condition
		nrOfChanges += nr;
		condition1Meet = checkCondition1(s, nr);
		nrOfChanges += nr;
		condition2Meet = checkCondition2(s, nr);
		nrOfChanges += nr;

		cond1Meet = condition1Meet.noFailedCondition();
		cond2Meet = condition2Meet.noFailedCondition();
		cond3Meet = condition3Meet.noFailedCondition();

		if (!cond1Meet) {
			std::cout << "Failed condition 1: " << condition1Meet << std::endl;
			passNotStrong = true;
		}
		if (!cond2Meet) {
			std::cout << "Failed condition 2: " << condition2Meet << std::endl;
			passNotStrong = true;
		}
		if (!cond3Meet) {
			std::cout << "Failed condition 3: " << condition3Meet << std::endl;
			passNotStrong = true;
		}

	} while (passNotStrong);

	return nrOfChanges;
}

int main()
{
	std::string s;
	
	std::cin >> s;
	while (s != "exit") {
		int nrOfChanges = minimumNrOfChanges(s);
		std::cout << "Minimum nr of changes: " << nrOfChanges << std::endl;
		std::cout << "New password: " << s << std::endl;
		
		std::cin >> s;
	}

	return 0;
}