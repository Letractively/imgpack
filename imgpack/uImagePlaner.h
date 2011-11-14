// ImagePacker by DAS ( dasannikov@gmail.com )

// Rectangle placer. Usage:
// 1 - Add() all your rectangles in uImagePacker object.
// 2 - Callc() it.
// 3 - uImagePacker::callcList contains output data.

#ifndef _IMAGE_PLANER_H_
#define _IMAGE_PLANER_H_

#include <vector>

class uImagePlaner {
public:

	uImagePlaner() : maxTextureSize(1024), startTextureSize(1) {}

	void add(int x, int y, const std::string imgName);
	int getTextureSize(int imgSize);
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
	int maxTextureSize;
	int startTextureSize;

private:

	uImageRectList rectList;

	int fillRect(int minX, int maxX, int minY, int maxY, uImageRectList::iterator start);
	bool isDone();

};

#endif // _IMAGE_PLANER_H_
