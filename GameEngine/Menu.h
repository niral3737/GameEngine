#pragma once
#include <string>
#include <map>
#include <vector>

enum eSelectedlanguage
{
	ENGLISH,
	FRENCH,
	SPANISH,
	GERMAN,
	ITALIAN,
	INSTRUCTIONS
};

class Menu
{
public:
	Menu();
	~Menu();
	void loadLanguageFromXml(std::string inputFile);
	void renderSelectedMenu();

	std::map<std::string, std::vector<std::string>> _map_languages;
	eSelectedlanguage _current_language = INSTRUCTIONS;

	static Menu* instance;
	static Menu* getInstance();
};

