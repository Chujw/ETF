#pragma once

#include "ofMain.h"
#include "Image.h"

#define		COLOR	180
#define		SPEED	1
#define		TIMESTEP_DIR 8
#define		TIMESTEP_COLI 1
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
		void distributeBots();
		bool collide(ofVec2f this_pos, int this_mark);



		Image img;
		ofImage canvas;
		int* marker;
		int mark_count;
		int timestep_dir_count;
		int timestep_coli_count;
		ofImage SrcImage;
		ofImage edge_map;
		ofVec2f* grad_map;
		ofVec2f* ETF_map;
		ofImage ETF_Visual;
		int snapCounter;
		ofVec2f bot_pos;
		ofVec2f last_bot_pos;
		//ofVec2f bot_startpos;
		ofVec2f direction;
		ofVec2f last_direction;
};
