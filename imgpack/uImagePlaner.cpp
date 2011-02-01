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

	int x = 1;
	int y = 1;

	while(true) {	
		if(((x * y) >= sizeXY) || ((x == MAX_TEXTURE_SIZE) && (y == MAX_TEXTURE_SIZE))) {

			calcList = rectList;
			std::sort(calcList.begin(), calcList.end());
			fillRect(0, x, 0, y, calcList.end());

			if(isDone()) {
				outSizeX = x;
				outSizeY = y;
				return;
			}
		}

		if(y <= x)
			y = y * 2;
		else {
			y = y / 2;
			x = x * 2;
		}

		if(x > MAX_TEXTURE_SIZE)
			x = MAX_TEXTURE_SIZE;
		
		if(y > MAX_TEXTURE_SIZE)
			y = MAX_TEXTURE_SIZE;
		
	}

/*

	outSizeX = MAX_TEXTURE_SIZE;
	outSizeY = MAX_TEXTURE_SIZE;

	calcList = rectList;
	std::sort(calcList.begin(), calcList.end());
	fillRect(0, MAX_TEXTURE_SIZE, 0, MAX_TEXTURE_SIZE, calcList.end());

*/

}
