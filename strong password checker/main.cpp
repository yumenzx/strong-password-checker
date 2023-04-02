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

bool containsLowercaseLetter(std::string s)
{
	for (char c : s)
		if (c >= 'a' && c <= 'z')
			return true;
	return false;
}

bool containsUppercaseLetter(std::string s)
{
	for (char c : s)
		if (c >= 'A' && c <= 'Z')
			return true;
	return false;
}

bool containsDigit(std::string s)
{
	for (char c : s)
		if (c >= '0' && c <= '9')
			return true;
	return false;
}

inline FailedCondition checkCondition1(std::string& s, int& difference)
{
	int length = s.length();
	int nr = 0;

	char a = 'a';
	if (length < 6) {
		nr = 6 - length;
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
		difference = nr;
		return FailedCondition::TOO_FEW_CHARACTERS;
	}

	if (length > 20) {
		nr = abs(20 - length);
		s.resize(20);
		difference = nr;
		return FailedCondition::TOO_MANY_CHARACTERS;
	}

	difference = nr;
	return FailedCondition::NO_FAILED_CONDITION;
}

FailedCondition checkCondition2(std::string& s,int& nrOfChanges)
{
	bool containsLowercaseLetter, containsUppercaseLetter, containsDigit;
	containsLowercaseLetter = containsUppercaseLetter = containsDigit = false;
	int nr = 0;
	std::queue<int> lowercasePossitions;
	std::queue<int> uppercasePossitions;
	std::queue<int> digitPossitions;
	
	for(int i=0;i<s.length();i++){
		char c = s[i];
		if (c >= 'a' && c <= 'z') {
			containsLowercaseLetter = true;
			lowercasePossitions.push(i);
			continue;
		}

		if (c >= 'A' && c <= 'Z') {
			containsUppercaseLetter = true;
			uppercasePossitions.push(i);
			continue;
		}

		if (c >= '0' && c <= '9') {
			containsDigit = true;
			digitPossitions.push(i);
		}
	}

	if (lowercasePossitions.empty()) {
		if (s.length() < 6)
			s += 'a';
		else if (uppercasePossitions.size() >= 2) {
			s.at(uppercasePossitions.front()) = 'a';
			uppercasePossitions.pop();
		}
		else if (digitPossitions.size() >= 2) {
			s.at(digitPossitions.front()) = 'a';
			digitPossitions.pop();
		} else
			s += 'a';
		nr++;
	}

	if (uppercasePossitions.empty()) {
		if (s.length() < 6)
			s += 'A';
		else if (lowercasePossitions.size() >= 2) {
			s.at(lowercasePossitions.front()) = 'A';
			lowercasePossitions.pop();
		}
		else if (digitPossitions.size() >= 2) {
			s.at(digitPossitions.front()) = 'A';
			digitPossitions.pop();
		} else
			s += 'A';
		nr++;
	}

	if (digitPossitions.empty()) {
		if (s.length() < 6)
			s += '0';
		else if (uppercasePossitions.size() >= 2) {
			s.at(uppercasePossitions.front()) = '0';
			uppercasePossitions.pop();
		}
		else if (lowercasePossitions.size() >= 2) {
			s.at(lowercasePossitions.front()) = '0';
			lowercasePossitions.pop();
		} else
			s += '0';
		nr++;
	}
	
	nrOfChanges = nr;

	if (containsLowercaseLetter == false) 
		return FailedCondition::NO_LOWERCASE_LETTER;
	if (containsUppercaseLetter == false)
		return FailedCondition::NO_UPPERCASE_LETTER;
	if (containsDigit == false)
		return FailedCondition::NO_DIGIT;

	return FailedCondition::NO_FAILED_CONDITION;
}

FailedCondition checkCondition3(std::string& s,int &nrOfRepeatingCharsInRow)
{
	int nr = 0;
	std::string::const_iterator end = s.end() - 2;
	for (auto it = s.begin(); it < end; ++it) {
		char c = *it;
		if (c == *(it + 1) && c == *(it + 2)) {
			nr++;
			if (!containsLowercaseLetter(s))
				*(it + 2) = 'a';
			else if (!containsUppercaseLetter(s))
				*(it + 2) = 'A';
			else if (!containsDigit(s))
				*(it + 2) = '1';
			else
				*(it + 2) = c + 1;
		}
	}

	nrOfRepeatingCharsInRow = nr;
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

		condition3Meet = checkCondition3(s, nr);
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