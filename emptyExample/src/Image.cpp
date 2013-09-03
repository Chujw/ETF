#include "Image.h"

bool Image::InitImage()
{
	
	if(!srcImage.loadImage("eagle.jpg"))
	{
		cout<<"Error: Cannot load image."<<endl;
		cout<<"Initializing images..."<<endl;
	}
	srcImage.resize(ofGetWidth(),ofGetHeight());
	srcImage.setImageType(OF_IMAGE_GRAYSCALE);
	if(srcImage.isAllocated())
		return true;
	else
		return false;
}

// clear the image
void Image::DelImage()
{
	srcImage.clear();
}

// get the source image
ofImage Image::getSrcImage()	
{
	if(srcImage.isAllocated())
		return srcImage;
	else
	{
		InitImage();
		return srcImage;
	}
}

// compute the edgemap
ofImage Image::getEdgeMap()
{
	if(InitImage())
	{
		cout<<"Computing Gaussian Map..."<<endl;
		edgemap = srcImage;
		edgemap = gau.GaussianBlur(srcImage,0.8);	// smooth out noise
		cout<<"Computing Sobel Map..."<<endl;
		edgemap = sobel.outputImage(edgemap);	// get sobel filtered image
		return edgemap;
	}
	else
	{
		cout<<"Error: Initialize failed."<<endl;
		return false;
	}
}

ofVec2f* Image::getGradMap()
{
	if(srcImage.isAllocated())
		return sobel.getGradient(srcImage);	// get gradient vector map
	else
		cout<<"Error: Please initialize source image first."<<endl;
}

ofVec2f* Image::getETFMap(ofVec2f *GradMap)
{
	cout<<"Computing ETF..."<<endl;
	return ETF.iterETF(GradMap,srcImage.width,srcImage.height,ITER_ETF);
}

double* Image::getMagMap()	
{
	return ETF.GetMagMap();
}

