// ImagePacker by DAS ( dasannikov@gmail.com )

// Rectangle placer. Usage:
// 1 - Add all your rectangles in uImagePacker object.
// 2 - Callc() it.
// 3 - uImagePacker::callcList contains output data.

#pragma once
#include <vector>

#define MAX_TEXTURE_SIZE 2048

class uImagePlaner {
public:

	void add(int x, int y, const std::string imgName);
	void calc();

	class uImageRect {
	public:
		
		int rectMinX;
		int rectMaxX;
		int rectMinY;
		int rectMaxY;

		int sizeX;
		int sizeY;

		std::string name;
		
		bool done;

		uImageRect() : done(false) { rectMinX = rectMaxX = rectMinY = rectMaxY = sizeX = sizeY = 0; }
		const bool operator < ( const uImageRect& rhs ) const { return sizeY == rhs.sizeY ? sizeX < rhs.sizeX : sizeY < rhs.sizeY; }
	};

	typedef std::vector<uImageRect> uImageRectList;

	uImageRectList calcList;

	int outSizeX;
	int outSizeY;

private:

	uImageRectList rectList;

	int fillRect(int minX, int maxX, int minY, int maxY, uImageRectList::iterator start);
	bool isDone();

};

