#include "Menu.h"
#include "pugixml.hpp"
#include <iostream>
#include "cSceneUtils.h"

Menu::Menu()
{}


Menu::~Menu()
{}

Menu* Menu::instance = 0;
Menu* Menu::getInstance()
{
	if (instance == 0)
	{
		instance = new Menu();
	}
	return instance;
}

void Menu::loadLanguageFromXml(std::string inputFile)
{
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(inputFile.c_str());

	pugi::xml_node_iterator it = doc.children().begin();
	for (it; it != doc.children().end(); it++)
	{
		//we should find language children here
		pugi::xml_node_iterator it2 = it->children().begin();
		for (it2; it2 != it->children().end(); it2++)
		{
			std::cout << "it name: " << it->name() << std::endl;
			std::cout << "it2 name: " << it2->name() << std::endl; //CTRL + KK

			pugi::xml_node_iterator it3 = it2->children().begin();
			for (it3; it3 != it2->children().end(); it3++)
			{

				_map_languages[it2->first_attribute().value()].push_back(it3->child_value());
			}

		}
	}
}

void Menu::renderSelectedMenu()
{
	std::map < std::string, std::vector<std::string>>::iterator it = _map_languages.begin();
	std::vector<std::string> selected_strings;

	switch (_current_language)
	{
	case ENGLISH:
		selected_strings = _map_languages["EN"];
		break;
	case FRENCH:
		selected_strings = _map_languages["FR"];
		break;
	case SPANISH:
		selected_strings = _map_languages["SP"];
		break;
	case GERMAN:
		selected_strings = _map_languages["GE"];
		break;
	case ITALIAN:
		selected_strings = _map_languages["IT"];
		break;
	case INSTRUCTIONS:
		selected_strings = _map_languages["IN"];
		break;
	default:
		break;
	}

	float sx = 2.0f / 1024;
	float sy = 2.0f / 600;
	GLfloat yoffset = 50.0f;
	GLfloat xoffset = 8 * sx;

	for (std::vector<std::string>::iterator it = selected_strings.begin();
		it != selected_strings.end(); it++)
	{
		std::string tmp = *it;
		//std::cout << tmp << std::endl;
		cSceneUtils::getInstance()->render_text(tmp.c_str(), -1 + xoffset, 1 - yoffset * sy, sx, sy);
		yoffset += 50.0f;
	}
}