#pragma once
#include "Util/Common.h"

namespace KikooRenderer {
namespace CoreEngine {
namespace Text {


class Character {
private:
	int id;
	double xTextureCoord;
	double yTextureCoord;
	double xMaxTextureCoord;
	double yMaxTextureCoord;
	double xOffset;
	double yOffset;
	double sizeX;
	double sizeY;
	double xAdvance;
public:
	/**
	 * @param id
	 *            - the ASCII value of the character.
	 * @param xTextureCoord
	 *            - the x texture coordinate for the top left corner of the
	 *            character in the texture atlas.
	 * @param yTextureCoord
	 *            - the y texture coordinate for the top left corner of the
	 *            character in the texture atlas.
	 * @param xTexSize
	 *            - the width of the character in the texture atlas.
	 * @param yTexSize
	 *            - the height of the character in the texture atlas.
	 * @param xOffset
	 *            - the x distance from the curser to the left edge of the
	 *            character's quad.
	 * @param yOffset
	 *            - the y distance from the curser to the top edge of the
	 *            character's quad.
	 * @param sizeX
	 *            - the width of the character's quad in screen space.
	 * @param sizeY
	 *            - the height of the character's quad in screen space.
	 * @param xAdvance
	 *            - how far in pixels the cursor should advance after adding
	 *            this character.
	 */
	Character(int id, double xTextureCoord, double yTextureCoord, double xTexSize, double yTexSize,
			double xOffset, double yOffset, double sizeX, double sizeY, double xAdvance) {
		this->id = id;
		this->xTextureCoord = xTextureCoord;
		this->yTextureCoord = yTextureCoord;
		this->xOffset = xOffset;
		this->yOffset = yOffset;
		this->sizeX = sizeX;
		this->sizeY = sizeY;
		this->xMaxTextureCoord = xTexSize + xTextureCoord;
		this->yMaxTextureCoord = yTexSize + yTextureCoord;
		this->xAdvance = xAdvance;
	}

	int getId() {
		return id;
	}

	double getxTextureCoord() {
		return xTextureCoord;
	}

	double getyTextureCoord() {
		return yTextureCoord;
	}

	double getXMaxTextureCoord() {
		return xMaxTextureCoord;
	}

	double getYMaxTextureCoord() {
		return yMaxTextureCoord;
	}

	double getxOffset() {
		return xOffset;
	}

	double getyOffset() {
		return yOffset;
	}

	double getSizeX() {
		return sizeX;
	}

	double getSizeY() {
		return sizeY;
	}

	double getxAdvance() {
		return xAdvance;
	}

};

}
}
}
