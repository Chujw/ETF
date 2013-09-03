#include "ofMain.h"
#include "Gaussian.h"
#include "sob.h"
#include "EdgeTangentFlow.h"

#define ITER_ETF 3	// try twice for now
//#define PATH "data/star.bmp"

class Image{

private:
	ofImage srcImage;
	ofImage edgemap;
	Gaussian gau;
	sob sobel;
	EdgeTangentFlow ETF;
	//double* mag;	//magnitude map
public:
	bool InitImage();	// Initiate image
	void DelImage();
	ofImage getSrcImage();	// get the source image
	ofImage DoG();	//compute the DoG map
	ofImage getEdgeMap();	// compute the edgemap
	ofVec2f* getGradMap();	// get the gradient vector map
	ofVec2f* getETFMap(ofVec2f *GradMap);
	double* getMagMap();
};