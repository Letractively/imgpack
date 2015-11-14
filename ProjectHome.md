# Image packaging tool #

Command line tool for **images and/or textures packaging (atlas)**.
Can be useful for all kind of development (web, opengl, directx)

## How to use ##

```

imgpack [directory]

```

  * Put your images in one directory (curently work only with PNG)
  * Run tool as shown
  * You get packed image or image set
  * You get original image position descriptors (simple txt format)
  * Output image/images will be 2048x2048 pixels max or less.

Here the some output samples (game development & web development)

![http://imgpack.googlecode.com/files/imgpack.png](http://imgpack.googlecode.com/files/imgpack.png)
![http://imgpack.googlecode.com/files/imgpack02.png](http://imgpack.googlecode.com/files/imgpack02.png)

  * _Game sprites from http://www.thetinybangstory.com/_
  * _Icons from http://ipapun.deviantart.com/_

### Output descriptor file format ###


```


// ImgPack
// FILTER: iGame\*.png

"Arrow" = ("iGame01", 256, 1536, 512, 1792);
"Blocked" = ("iGame01", 0, 1536, 256, 1792);
"Briefcase" = ("iGame01", 1792, 1280, 2048, 1536);

OriginalImageName = (AtlasName, leftX, upY, rightX, buttomY);

```


## Download ##
  * Current Win32 **binaries [download link](http://imgpack.googlecode.com/files/imgpack_20110201.zip)**
  * You can use SVN to get latest **source code** (Win, MSVC9, need [DevIL](http://openil.sourceforge.net/) )
  * **Contact me** if you want participate.

## Links ##
  * [Practical Texture Atlases](http://www.gamasutra.com/view/feature/2530/practical_texture_atlases.php)
  * [CSS Sprites: Image Slicing’s Kiss of Death](http://www.alistapart.com/articles/sprites)