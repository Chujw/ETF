#ifndef EDGE_TANGENT_FLOW_H
#define EDGE_TANGENT_FLOW_H

#include "ofMain.h"
#define KERNEL 5

class EdgeTangentFlow
{
private:
	int width, height;
	bool magMapHasGot;
	double *mag;

public:
	EdgeTangentFlow()
	{
		width = 0;
		height = 0;
		magMapHasGot = false;
	};

	~EdgeTangentFlow(){};

	double* GetMagMap()
	{
		return mag;
	};

	ofVec2f* iterETF(ofVec2f *ori_gmap,int w, int h, int iter)	// self-call iter times
	{
		width = w;
		height = h;
		mag = new double[width*height];

		//	initialize the t(x)
		for(int i=0; i<width*height;i++)
		{
			ori_gmap[i].normalize();	// normalize
			if(!magMapHasGot)
				mag[i] = ori_gmap[i].length();	// get magnitude (once)			
			GetPerpendicularVec(&ori_gmap[i]);// Get Perpendicular Vector field (follow the edges tangent direction)
		}
		// iter first time. NOTE: now the ori_gmap is changed to a t(x) tangent map
		cout<<"Iteration: 1"<<endl;
		ofVec2f* iter_new_gmap = new ofVec2f[width*height];
		iter_new_gmap = ETF(ori_gmap);	
		magMapHasGot = true;
		// iter more times
		for(int i=1; i<iter;i++)		
		{
			cout<<"Iteration: "<<i+1<<endl;
			iter_new_gmap = ETF(iter_new_gmap);
		}
		return iter_new_gmap;
	};

private:
	ofVec2f* ETF(ofVec2f *ori_gmap)	
	{
		ofVec2f* new_gmap(ori_gmap);	// output tangent vector map, copy from ori_gmap
		for(int y = KERNEL; y<height-KERNEL;y++)
		{
			for(int x=KERNEL; x<width-KERNEL;x++)
			{
				ofVec2f newVec;
				newVec.set(0,0);
				ofVec2f t_cur_X = ori_gmap[y*width+x];	// t_cur_X is the center vector
				for(int ky=-KERNEL; ky<=KERNEL;ky++)
				{
					for(int kx=-KERNEL; kx<=KERNEL;kx++)
					{
						if((y+ky)*width+x+kx != y*width+x)
						{
							ofVec2f t_cur_Y = ori_gmap[(y+ky)*width+x+kx];	// t_cur_Y is the current neighbor vector
					
							int sign = Sign(t_cur_X,t_cur_Y);
							int ws = WeightSpatial(x,y,x+kx,y+ky);
							double wm = WeightMag(y*width+x,(y+ky)*width+x+kx);
							double wd = WeightDir(t_cur_X,t_cur_Y);
							newVec += double(sign) * t_cur_Y * double(ws) * wm * wd;	// compute formula
						}
					}
				}
				new_gmap[y*width+x] = newVec.normalize();
			}
		}
		return new_gmap;
	};

	void GetPerpendicularVec(ofVec2f* t_init)	//counter-clockwise
	{
		ofVec2f t_new;
		t_new.x = t_init->y;
		if(t_init->x!=0)
			t_new.y = -1*t_init->x;
		else
			t_new.y = t_init->x;
		t_init->set(t_new);
		//vec->set(vec->perpendiculared());
	};

	int WeightSpatial(float X1, float Y1, float X2, float Y2)	//Ws(X,Y)
	{
		float test = sqrt((X1-X2)*(X1-X2)+(Y1-Y2)*(Y1-Y2));

		if(sqrt((X1-X2)*(X1-X2)+(Y1-Y2)*(Y1-Y2))<KERNEL)
		//if(abs(neighborY.x-centerX.x)<KERNEL && abs(neighborY.y-centerX.y)<KERNEL)	//spatial
		{
			return 1;
		}
		return 0;
	};

	double WeightMag(int indexX, int indexY)	// Wm(X,Y)
	{
		//return 0.5*(1+tanh(neighborY.length()-centerX.length()));
		return 0.5*(1+tanh(mag[indexY]-mag[indexX]));
	};

	double WeightDir(ofVec2f centerX, ofVec2f neighborY)	// Wd(X,Y)
	{
		return fabs(centerX.dot(neighborY));
	};

	int Sign(ofVec2f centerX, ofVec2f neighborY)	// sign
	{
		if(centerX.dot(neighborY)>0)
			return 1;
		else
			return -1;
	};


};
#endif