#pragma once
#include "Util/Common.h"
#include "Character.hpp"

namespace KikooRenderer {
namespace CoreEngine {
namespace Text {


/**
 * Provides functionality for getting the values from a font file.
 * 
 * @author Karl
 *
 */
class MetaFile {
private:
	int PAD_TOP = 0;
	int PAD_LEFT = 1;
	int PAD_BOTTOM = 2;
	int PAD_RIGHT = 3;

	double LINE_HEIGHT =  0.03f;
	int SPACE_ASCII = 32;

	int DESIRED_PADDING = 3;

	std::string SPLITTER = " ";
	std::string NUMBER_SEPARATOR = ",";

	double aspectRatio;

	double verticalPerPixelSize;
	double horizontalPerPixelSize;
	double spaceWidth;
	std::vector<int> padding;
	int paddingWidth;
	int paddingHeight;

    std::ifstream reader;

	std::map<std::string, std::string> values;
	std::map<int, Character*> metaData;

public:
	/**
	 * Opens a font file in preparation for reading.
	 * 
	 * @param file
	 *            - the font file.
	 */
	MetaFile(std::string file, std::string imageFile, double lineHeight, int desiredPadding) {
		LINE_HEIGHT = lineHeight;
		DESIRED_PADDING = desiredPadding;
		// std::cout <<"0" << std::endl;
		padding.resize(4);
		// std::cout <<"1" << std::endl;
		// this.aspectRatio = (double) Display.getWidth() / (double) Display.getHeight();
		this->aspectRatio = 1;
		// std::cout <<"2" << std::endl;
		openFile(file);
		// std::cout <<"3" << std::endl;
		loadPaddingData();
		// std::cout <<"4" << std::endl;
		loadLineSizes();
		// // std::cout <<"5" << std::endl;
		int imageWidth = getValueOfVariable("scaleW");
		std::cout << imageWidth << std::endl;
		// // std::cout <<"6" << std::endl;
		loadCharacterData(imageWidth);
		// std::cout <<"7" << std::endl;
		close();
		// std::cout <<"8" << std::endl;
	}

	~MetaFile() {
		std::map<int, Character*>::iterator it;

		for ( it = metaData.begin(); it != metaData.end(); it++ )
		{
			delete it->second;
		}		
	}

	double getSpaceWidth() {
		return spaceWidth;
	}

	Character *getCharacter(int ascii) {
		return metaData[ascii];
	}

private:
	std::vector<std::string> SplitString(std::string s, std::string delimiter) {
		// std::cout << "0" << std::endl;
		std::vector<std::string> res;
		// std::cout << "1" << std::endl;

		size_t pos = 0;
		std::string token;
		// std::cout << "3" << std::endl;    
		while ((pos = s.find(delimiter)) != std::string::npos) {
			// std::cout << "4" << std::endl;
			token = s.substr(0, pos);
			// std::cout << "5" << std::endl;
			res.push_back(token);
			// std::cout << "6" << std::endl;
			s.erase(0, pos + delimiter.length());
			// std::cout << "7" << std::endl;
		}
		res.push_back(s);
		return res;
	}

	/**
	 * Read in the next line and store the variable values.
	 * 
	 * @return {@code true} if the end of the file hasn't been reached.
	 */
	bool processNextLine() {
		// std::cout << "0" << std::endl;
		values.clear();
		// std::cout << "1" << std::endl;
		if (!reader.eof())  {
			// std::cout << "2" << std::endl;
			std::string line = "";
			// std::cout << "3" << std::endl;
			std::getline(reader, line);

			if(line.size() ==0) return false;
			// std::cout << "4" << std::endl;
			std::vector<std::string> splittedLine = SplitString(line, SPLITTER);
			// std::cout << "5" << std::endl;
			for (int i=0; i<splittedLine.size(); i++) {
				// std::cout << "7" << std::endl;
				std::vector<std::string> valuePairs = SplitString(splittedLine[i], "=");
				// std::cout << "SPLITTED " << splittedLine[i] << "  " << valuePairs.size() << std::endl;
				// std::cout << "8" << std::endl;
				if (valuePairs.size() == 2) {
					// std::cout << "9" << std::endl;
					values[valuePairs[0]]= valuePairs[1];
					// std::cout << valuePairs[0] << " -> " << valuePairs[1] << std::endl;
					// std::cout << "10" << std::endl;
				}
			}
			return true;
		} else {
			return false;
		}
	}

	/**
	 * Gets the {@code int} value of the variable with a certain name on the
	 * current line.
	 * 
	 * @param variable
	 *            - the name of the variable.
	 * @return The value of the variable.
	 */
	int getValueOfVariable(std::string variable) {
		std::string value = values[variable];
		return std::stoi(value);
	}

	/**
	 * Gets the array of ints associated with a variable on the current line.
	 * 
	 * @param variable
	 *            - the name of the variable.
	 * @return The int array of values associated with the variable.
	 */
	std::vector<int> getValuesOfVariable(std::string variable) {
		std::vector<std::string> numbers = SplitString(values[variable], NUMBER_SEPARATOR); 
		std::vector<int>actualValues(numbers.size());
		for (int i = 0; i < actualValues.size(); i++) {
			actualValues[i] = std::stoi(numbers[i]);
		}
		return actualValues;
	}

	/**
	 * Closes the font file after finishing reading.
	 */
	void close() {
    	reader.close();
	}

	/**
	 * Opens the font file, ready for reading.
	 * 
	 * @param file
	 *            - the font file.
	 */
	void openFile(std::string file) {
		reader = std::ifstream(file);
	}

	/**
	 * Loads the data about how much padding is used around each character in
	 * the texture atlas.
	 */
	void loadPaddingData() {
		// std::cout << "0" << std::endl;
		processNextLine();
		// std::cout << "1" << std::endl;
		this->padding = getValuesOfVariable("padding");
		// std::cout << "2" << std::endl;
		this->paddingWidth = padding[PAD_LEFT] + padding[PAD_RIGHT];
		// std::cout << "3" << std::endl;
		this->paddingHeight = padding[PAD_TOP] + padding[PAD_BOTTOM];
		// std::cout << "4" << std::endl;
	}

	/**
	 * Loads information about the line height for this font in pixels, and uses
	 * this as a way to find the conversion rate between pixels in the texture
	 * atlas and screen-space.
	 */
	void loadLineSizes() {
		processNextLine();
		int lineHeightPixels = getValueOfVariable("lineHeight") - paddingHeight;
		verticalPerPixelSize = LINE_HEIGHT / (double) lineHeightPixels;
		horizontalPerPixelSize = verticalPerPixelSize / aspectRatio;
	}

	/**
	 * Loads in data about each character and stores the data in the
	 * {@link Character} class.
	 * 
	 * @param imageWidth
	 *            - the width of the texture atlas in pixels.
	 */
	void loadCharacterData(int imageWidth) {
		processNextLine();
		processNextLine();
		while (processNextLine()) {
			Character* c = loadCharacter(imageWidth);
			if (c != nullptr) {
				this->metaData[c->getId()]= c;
			}
		}
	}

	/**
	 * Loads all the data about one character in the texture atlas and converts
	 * it all from 'pixels' to 'screen-space' before storing. The effects of
	 * padding are also removed from the data.
	 * 
	 * @param imageSize
	 *            - the size of the texture atlas in pixels.
	 * @return The data about the character.
	 */
	Character* loadCharacter(int imageSize) {
		int id = getValueOfVariable("id");
		if (id == SPACE_ASCII) {
			this->spaceWidth = (getValueOfVariable("xadvance") - paddingWidth) * horizontalPerPixelSize;
			return nullptr;
		}
		double xTex = ((double) getValueOfVariable("x") + (padding[PAD_LEFT] - DESIRED_PADDING)) / imageSize;
		double yTex = ((double) getValueOfVariable("y") + (padding[PAD_TOP] - DESIRED_PADDING)) / imageSize;
		int width = getValueOfVariable("width") - (paddingWidth - (2 * DESIRED_PADDING));
		int height = getValueOfVariable("height") - ((paddingHeight) - (2 * DESIRED_PADDING));
		double quadWidth = width * horizontalPerPixelSize;
		double quadHeight = height * verticalPerPixelSize;
		double xTexSize = (double) width / imageSize;
		double yTexSize = (double) height / imageSize;
		double xOff = (getValueOfVariable("xoffset") + padding[PAD_LEFT] - DESIRED_PADDING) * horizontalPerPixelSize;
		double yOff = (getValueOfVariable("yoffset") + (padding[PAD_TOP] - DESIRED_PADDING)) * verticalPerPixelSize;
		double xAdvance = (getValueOfVariable("xadvance") - paddingWidth) * horizontalPerPixelSize;
		return new Character(id, xTex, yTex, xTexSize, yTexSize, xOff, yOff, quadWidth, quadHeight, xAdvance);
	}
};


}
}
}