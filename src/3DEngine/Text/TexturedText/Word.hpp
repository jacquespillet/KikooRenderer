#pragma once
#include "Util/Common.h"
#include "Character.hpp"

namespace KikooRenderer {
namespace CoreEngine {
namespace Text {

class Word {
private:	
	std::vector<Character> characters;
	double width = 0;
	double fontSize;

public:

	/**
	 * Create a new empty word.
	 * @param fontSize - the font size of the text which this word is in.
	 */
	Word(double fontSize){
		this->fontSize = fontSize;
	}
	
	/**
	 * Adds a character to the end of the current word and increases the screen-space width of the word.
	 * @param character - the character to be added.
	 */
	void addCharacter(Character character){
		characters.push_back(character);
		width += character.getxAdvance() * fontSize;
	}
	
	/**
	 * @return The list of characters in the word.
	 */
	std::vector<Character> getCharacters(){
		return characters;
	}
	
	/**
	 * @return The width of the word in terms of screen size.
	 */
	double getWordWidth(){
		return width;
	}
};


}
}
}