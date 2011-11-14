// ImagePacker by DAS ( dasannikov@gmail.com )

#include <string>
#include <iostream>
#include <windows.h>

#include <IL/il.h>
#include <boost/program_options.hpp>

#include "uImagePlaner.h"

///////////////////////////////////////////////////////////////////////////

namespace po = boost::program_options;

///////////////////////////////////////////////////////////////////////////

struct Image {
	int x, y, bpp, format;
	ILuint id;
	unsigned char * data;
	std::string name;
	bool done;
};

///////////////////////////////////////////////////////////////////////////

std::vector<Image> images;

int newImageSizeX = 0;
int newImageSizeY = 0;

unsigned char * newImageData = 0;

std::string newImageName = "Data.png";
std::string newScriptName = "Data.us";

std::string	filter = "[none]";
std::string	dir = "";

FILE *f;
///////////////////////////////////////////////////////////////////////////

void loadImage(const char * filename)
{
	std::string name = dir + "\\" + filename;

	Image img;
	ilGenImages(1, &img.id);

	ilBindImage(img.id);
	ILboolean success = ilLoadImage(name.c_str());

	if(success) {
		
		img.x = ilGetInteger(IL_IMAGE_WIDTH);
		img.y = ilGetInteger(IL_IMAGE_HEIGHT);
		img.bpp = ilGetInteger(IL_IMAGE_BPP);
		img.format = ilGetInteger(IL_IMAGE_FORMAT);
		img.data = ilGetData();
		img.done = false;
		img.name = filename;

		if(img.format != IL_RGBA) {
			ilDeleteImages(1, &img.id);
			printf("ERROR: %s not RGBA \n\n", name.c_str());
		} else
			images.push_back(img);


	} else
	
		ilDeleteImages(1, &img.id);

}

///////////////////////////////////////////////////////////////////////////

void unloadImages()
{
	for(int i = 0; i < (int)images.size(); i++)
		ilDeleteImages(1, &images[i].id);
}

///////////////////////////////////////////////////////////////////////////

Image* findImageData(std::string name)
{
	for(int i = 0; i < (int)images.size(); i++)
		if(images[i].name == name) {
			return &images[i];
		}

	return 0;
}

///////////////////////////////////////////////////////////////////////////

void putImage(uImagePlaner::uImageRect& imgRect)
{
	if(!imgRect.done)
		return;

	Image* img = findImageData(imgRect.name);
	if((imgRect.rectMaxX - imgRect.rectMinX) != img->x)
		printf("IMAGEPACK ERROR: Image size != ImageRect size \n");

	// Copy Old ( Try IL)
	for(int y = 0; y < img->y; y++)
		for(int x = 0; x < img->x; x++) {
			int newX = imgRect.rectMinX + x;
			int newY = imgRect.rectMinY + y;
			int sizeX = newImageSizeX;
			newImageData[(newY * sizeX + newX) * 4 + 0] = img->data[(y * img->x + x) * 4 + 0];
			newImageData[(newY * sizeX + newX) * 4 + 1] = img->data[(y * img->x + x) * 4 + 1];
			newImageData[(newY * sizeX + newX) * 4 + 2] = img->data[(y * img->x + x) * 4 + 2];
			newImageData[(newY * sizeX + newX) * 4 + 3] = img->data[(y * img->x + x) * 4 + 3];
		}

//	ilOverlayImage(img->id, imgRect.rectMinX, imgRect.rectMinY, 0);
		
	img->done = true;
}

///////////////////////////////////////////////////////////////////////////

void generateImage()
{
	printf("\n[%s] Image generation... ", newImageName.c_str());
	uImagePlaner planer;

	for(int i = 0; i < (int)images.size(); i++)
		if(!images[i].done)
			planer.add(images[i].x, images[i].y, images[i].name);

	planer.calc();
	printf("DONE [size:%dx%d]\n", planer.outSizeX, planer.outSizeY);

	if((planer.outSizeX * planer.outSizeY) > planer.maxTextureSize * planer.maxTextureSize) {
		printf("Output image too big. Terminate! \n");
		return;
	}

	// Fill new image

	newImageData = new unsigned char [planer.outSizeX * planer.outSizeY * 4];
	memset(newImageData, 0, planer.outSizeX * planer.outSizeY * 4);

	newImageSizeX = planer.outSizeX;
	newImageSizeY = planer.outSizeY;

	// new by IL
	// ilTexImage(newImageSizeX, newImageSizeY, 0, 4, IL_RGBA, IL_UNSIGNED_BYTE, NULL);

	for(uImagePlaner::uImageRectList::iterator i = planer.calcList.begin(); i != planer.calcList.end(); i++)
		putImage(*i);

	printf("[%s] Writing output image... ", newImageName.c_str());


	// Save IL
	ILuint imageId;

	ilGenImages(1, &imageId);
	ilBindImage(imageId);

	ilLoadDataL(newImageData, newImageSizeX * newImageSizeY * 4, newImageSizeX, newImageSizeY, 1, 4);

	std::string tmpName = newImageName + ".png";
	ilSaveImage(tmpName.c_str());

	//tmpName = newImageName + ".jp2";
	//ilSaveImage(tmpName.c_str());

	ilDeleteImages(1, &imageId);

	delete newImageData;
	printf("DONE\n");

	// Script generation

	printf("[%s] Script generation... ", newImageName.c_str());
   
	for(uImagePlaner::uImageRectList::iterator i = planer.calcList.begin(); i != planer.calcList.end(); i++)
		if(i->done) {
			Image* img = findImageData(i->name);
			char temp[100];
			temp[img->name.copy(temp, img->name.length() - 4)] = 0x0;
			fprintf(f, "\"%s\" = (\"%s\", %d, %d, %d, %d);\n", temp, newImageName.c_str(), i->rectMinX, i->rectMinY, i->rectMaxX, i->rectMaxY);
		}

	printf("DONE\n");
}

///////////////////////////////////////////////////////////////////////////

void loadDir( )
{
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;

	printf("Target files is %s\n\n", filter.c_str());
	hFind = FindFirstFile(filter.c_str(), &FindFileData);

	if (hFind == INVALID_HANDLE_VALUE) {
		printf ("Invalid File Handle. GetLastError reports %d\n", GetLastError ());
		return;
	} 
	else {
		printf ("File found: %s\n", FindFileData.cFileName);
		loadImage(FindFileData.cFileName);
		
		while(FindNextFile(hFind, &FindFileData)) {
			printf ("File found: %s\n", FindFileData.cFileName);
			loadImage(FindFileData.cFileName);
		}

		FindClose(hFind);
		return;
	}
}

///////////////////////////////////////////////////////////////////////////

void genNames(int i)
{
	char tmp[100];
	sprintf(tmp, "_%02d", i);

	newImageName = dir + std::string(tmp);
}

///////////////////////////////////////////////////////////////////////////

bool allDone()
{
	for(int i = 0; i < (int)images.size(); i++)
		if(!images[i].done)
			return false;

	return true;
}

///////////////////////////////////////////////////////////////////////////

int main(int argc, char *argv[ ])
{
	printf("ImagePacker Tool.\n");

/*	// Declare the supported options.
	po::options_description desc("Allowed options");
	desc.add_options()
		("help", "Produce help message")
		("size", po::value<int>(), "Set max image size (Default = 1024)");

	po::positional_options_description p;
	p.add("input-file", 1);


	po::variables_map vm;
	po::store(po::command_line_parser(ac, av).options(desc).positional(p).run(), vm);
	po::notify(vm);    

	if(vm.count("help")) {
		std::cout << desc << "\n";
		return 1;
	}

	if(vm.count("size")) {
		std::cout << "Max image size set to " << vm["size"].as<int>() << ".\n";
	} else {
		std::cout << "Max image size set to default.\n";
	}

	if(vm.count("input-file"))
		std::cout << "Input are: " << vm["input-file"].as< vector<string> >() << "\n";

*/

	if(argc == 1) {
		printf("Work with only PNG images.\n");
		printf("Usage: imgpack [directory]\n");
		return 1;
	}

	dir = argv[1];
	filter = dir + "\\*.png";
	newScriptName = dir + ".utx";

	ilInit();
	ilEnable(IL_FILE_OVERWRITE);

	loadDir();
	printf("Image loading... DONE\n");

	fopen_s(&f, newScriptName.c_str(), "w");
	fprintf(f, "// ImgPack\n");
	fprintf(f, "// FILTER: %s\n\n", filter.c_str());

	int i = 1;
	genNames(i);
	generateImage();

	while(!allDone()) {
		i++;
		genNames(i);
		generateImage();
	}

	fclose(f);

	unloadImages();

	printf("\nImagePack DONE!\n");

	return 0;
}