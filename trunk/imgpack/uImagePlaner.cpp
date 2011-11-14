// ImagePacker by DAS ( dasannikov@gmail.com )

#include <algorithm>
#include "uImagePlaner.h"

///////////////////////////////////////////////////////////////////////////

void uImagePlaner::add(int x, int y, const std::string imgName)
{
	uImageRect img;
	img.sizeX = x;
	img.sizeY = y;
	img.name = imgName;

	rectList.push_back(img);

	if((x > startTextureSize) || (y > startTextureSize))
		startTextureSize = getTextureSize( (x > y) ? x : y );

	if((x > 1024) || (y > 1024))
		maxTextureSize = 2048;

//	if((x > 2048) && (y > 2048))
//		ERROR

}

///////////////////////////////////////////////////////////////////////////

int uImagePlaner::getTextureSize(int imgSize)
{
	int i = 1;
	while(i < imgSize)
		i *= 2;

	return i;
}

///////////////////////////////////////////////////////////////////////////

int uImagePlaner::fillRect(int minX, int maxX, int minY, int maxY, uImageRectList::iterator start)
{
	start--;

	int rectW = maxX - minX;
	int rectH = maxY - minY;

	if(rectH < calcList[0].sizeY) return maxY;
	if(rectW < calcList[0].sizeX) return maxY;

	for(uImageRectList::iterator i = start; i != calcList.begin(); i--)
		if((!i->done) && (i->sizeY <= rectH) && (i->sizeX <= rectW))  {
			i->done = true;
			i->rectMinX = minX;
			i->rectMinY = minY;
			i->rectMaxX = minX + i->sizeX;
			i->rectMaxY = minY + i->sizeY;

			fillRect(i->rectMaxX, maxX, minY, i->rectMaxY, i);
			fillRect(minX, maxX, i->rectMaxY, maxY, i);
			
			return i->rectMaxY;
		}
	
	if(start == calcList.begin())
		if((!start->done) && (start->sizeY <= rectH) && (start->sizeX <= rectW)) {
			start->done = true;
			start->rectMinX = minX;
			start->rectMinY = minY;
			start->rectMaxX = minX + start->sizeX;
			start->rectMaxY = minY + start->sizeY;
	}

	return -1;
}

///////////////////////////////////////////////////////////////////////////

bool uImagePlaner::isDone()
{
	for(uImageRectList::iterator i = calcList.begin(); i != calcList.end(); i++)
		if(i->done)
			return true;

	return false;
}

///////////////////////////////////////////////////////////////////////////

void uImagePlaner::calc()
{

	int sizeXY = 0;
	for(uImageRectList::iterator i = rectList.begin(); i != rectList.end(); i++)
		sizeXY += i->sizeX * i->sizeY;

	int x = startTextureSize;
	int y = startTextureSize;

	while(true) {	
		if(((x * y) >= sizeXY) || ((x == maxTextureSize) && (y == maxTextureSize))) {

			calcList = rectList;
			std::sort(calcList.begin(), calcList.end());
			fillRect(0, x, 0, y, calcList.end());

			if(isDone()) {
				outSizeX = x;
				outSizeY = y;
				return;
			}
		}

		y = x = x * 2;

		if(x > maxTextureSize)
			x = maxTextureSize;
		
		if(y > maxTextureSize)
			y = maxTextureSize;
		
	}

/*

	outSizeX = MAX_TEXTURE_SIZE;
	outSizeY = MAX_TEXTURE_SIZE;

	calcList = rectList;
	std::sort(calcList.begin(), calcList.end());
	fillRect(0, MAX_TEXTURE_SIZE, 0, MAX_TEXTURE_SIZE, calcList.end());

*/

}
