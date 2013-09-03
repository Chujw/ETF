#pragma once

#include "ofMain.h"
#include "Image.h"

#define		SPEED	2

class testApp : public ofBaseApp{
	public:
		void setup();
		void update();
		void draw();
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
	
		void visualizedETFMap(ofVec2f *ETFMap);	// draw out the gradient map
		bool UpdateBot();


		Image img;
		ofImage SrcImage;
		ofVec2f* grad_map;
		ofVec2f* ETF_map;
		ofImage ETF_Visual;
		int snapCounter;
		ofVec2f bot_pos;
		ofVec2f bot_startpos;
		ofVec2f direction;
};
