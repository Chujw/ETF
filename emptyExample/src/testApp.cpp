#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	edge_map = img.getEdgeMap();
	SrcImage = img.getSrcImage();
	grad_map = img.getGradMap();
	ETF_map = img.getETFMap(grad_map);
	canvas.allocate(ofGetWidth(),ofGetHeight(),OF_IMAGE_GRAYSCALE);
	for(int i=0;i<ofGetWidth()*ofGetHeight();i++)
		canvas.getPixelsRef()[i]=255;
	marker = new int[ofGetWidth()*ofGetHeight()];
	mark_count = 0;
	snapCounter = 0;
	timestep_dir_count = 0;
	timestep_coli_count = 0;
	distributeBots();
	int index = bot_pos.x+bot_pos.y*ofGetWidth();
	direction = ETF_map[index]; 
	last_direction = direction;
	last_bot_pos = bot_pos;
	//bot_startpos.set(bot_pos);
	

	//canvas.clone(edge_map);
}

//--------------------------------------------------------------
void testApp::update(){
	ofSetBackgroundAuto(false);
}

//--------------------------------------------------------------
void testApp::draw(){
	ofSetColor(255);
	//SrcImage.draw(0,0);
	//ETF_Visual.draw(0,0);

	//visualizedETFMap(ETF_map);
	//ETF_Visual.grabScreen(0,0,ofGetWidth(),ofGetHeight());
	//ETF_Visual.saveImage("ETF.jpg");
	
	ofPolyline line;
	line.addVertex(bot_pos.x,bot_pos.y);
	if(UpdateBot())
	{
		//line.addVertex(bot_pos.x,bot_pos.y);
		//line.draw();
		//ofSetColor(255,0,0);
		//ofEllipse(bot_startpos.x,bot_startpos.y,2,2);
		int index = floor(0.5+bot_pos.x)+floor(0.5+bot_pos.y)*ofGetWidth();
		if(index>=0 && index<ofGetWidth()*ofGetHeight())
		{
			canvas.getPixelsRef()[index]=COLOR;
			marker[index] = mark_count;
		}
		timestep_dir_count++;
		timestep_coli_count++;
	}
	else
	{
		timestep_dir_count=0;
		timestep_coli_count=0;
	}
	canvas.reloadTexture();
	canvas.draw(0,0);
	//edge_map.draw(0,0);
}


bool testApp::UpdateBot()
{
	int index = floor(bot_pos.x+0.5)+floor(bot_pos.y+0.5)*ofGetWidth();
	if(bot_pos.x>=0 && floor(0.5+bot_pos.x)<ofGetWidth() && bot_pos.y>=0 && floor(0.5+bot_pos.y)<ofGetHeight())// NOT out of window yet
	{
		if(timestep_dir_count>TIMESTEP_DIR)	// update direction after ... TIMESTEPs
		{
			timestep_dir_count = 0;
			if(ETF_map[index].length()>0)	// get new direction based on ETF
			{
				last_direction = direction;
				direction = ETF_map[index] + direction;
				direction.normalize();
				if(last_direction.dot(direction)<0)
				{
					direction*=-1;
				}
			}
		}
		last_bot_pos = bot_pos;
		bot_pos += SPEED * direction;	// update position
		if(timestep_coli_count>TIMESTEP_COLI)
		{
			timestep_coli_count = 0;
			if(collide(bot_pos,mark_count))	
			{
				distributeBots();
				return false;
			}
		}
		return true;
	}
	else
	{
		//bot_pos.set(ofRandom(0,ofGetWidth()),ofRandom(0,ofGetHeight()));
		//bot_startpos.set(bot_pos);
		distributeBots();
		return false;
	}
}

bool testApp::collide(ofVec2f this_pos, int this_mark)
{
	int this_index = floor(0.5+this_pos.x) + (floor(0.5+this_pos.y)) * ofGetWidth();
	if(canvas.getPixelsRef()[this_index]==COLOR)	// check its own spot
	{
		if(floor(0.5+last_bot_pos.x) != floor(0.5+bot_pos.x) || floor(0.5+last_bot_pos.y)!=floor(0.5+bot_pos.y))	// if it really moved
			return true;
	}

	for(int dy=-1;dy<=1;dy++)	// check collision in the 8-neighborhood and itself
	{
		for(int dx=-1;dx<=1;dx++)
		{
			int index = floor(0.5+this_pos.x+dx) + (floor(0.5+this_pos.y+dy)) * ofGetWidth();
			if(index>=0 && index<ofGetWidth()*ofGetHeight() && canvas.getPixelsRef()[index]==COLOR)
			{
				if(marker[index]!=this_mark)
					return true;
			}
		}
	}
	return false;
}

// find a start point at the edge map.
void testApp::distributeBots()
{
	int x = ofRandom(0,ofGetWidth());
	int y = ofRandom(0,ofGetHeight());
	int index = x+y*ofGetWidth();
	
	if(edge_map.getPixelsRef()[index] == 0 && !collide(ofVec2f(x,y),-1))
	{
		last_bot_pos = bot_pos;
		bot_pos.set(x,y);	// use the new position
		mark_count++;
		//bot_startpos.set(bot_pos);
	}
	else
		distributeBots();

}

//--------------------------------------------------------------
void testApp::visualizedETFMap(ofVec2f *thisETFMap)
{
	//ofImage visualGrad;	// output image
	//int width = ofGetWidth();
	//int height = ofGetHeight();
	//visualGrad.allocate(width,height,OF_IMAGE_GRAYSCALE);
	////initialization
	//for(int j=0;j<height;j++)
	//{
	//	for(int i=0;i<width;i++)
	//	{
	//		visualGrad.getPixelsRef()[j*width+i] = 255;	// init as white
	//	}
	//}
	//			unsigned char* visualvec = visualGrad.getPixels();
	//

	// mark ETF as black
	int c = 0;
	for(int y=0; y<ofGetHeight(); y++)
	{
		for(int x=0; x<ofGetWidth(); x++)
		{
			
			int index = x + y * ofGetWidth();
			ofVec3f start_etf_vec = ofVec3f(x,y,0);
			ofVec3f end_etf_vec = ofVec3f(x+thisETFMap[index].x,y+thisETFMap[index].y,0);
			//float dist = sqrtf((start_etf_vec.x-end_etf_vec.x)*(start_etf_vec.x-end_etf_vec.x)+(start_etf_vec.y-end_etf_vec.y)*(start_etf_vec.y-end_etf_vec.y));
			if(start_etf_vec!=end_etf_vec)
			{
				ofSetColor(c);
				//ofDrawArrow(start_etf_vec,end_etf_vec);
				ofPolyline etf_line;
				etf_line.addVertex(start_etf_vec.x, start_etf_vec.y);
				etf_line.addVertex(end_etf_vec.x, end_etf_vec.y);
				etf_line.draw();
			}
		}
	}
}




//--------------------------------------------------------------
void testApp::keyPressed(int key){
	switch(key){
	case ' ':
		ofImage saveImage;
		saveImage.grabScreen(0,0,ofGetWidth(),ofGetHeight());
		saveImage.saveImage("snapshot"+ofToString(snapCounter)+".png");
		snapCounter++;
	}
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}


