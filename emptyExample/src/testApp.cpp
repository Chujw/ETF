#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	img.getEdgeMap();
	SrcImage = img.getSrcImage();
	grad_map = img.getGradMap();
	ETF_map = img.getETFMap(grad_map);
	snapCounter = 0;
	direction.set(1,0); // point to right
	bot_pos.set(0,100);
	bot_startpos.set(bot_pos);
}

//--------------------------------------------------------------
void testApp::update(){
	ofSetBackgroundAuto(false);
}

//--------------------------------------------------------------
void testApp::draw(){
	ofSetColor(0);
	//SrcImage.draw(0,0);
	//ETF_Visual.draw(0,0);

	//visualizedETFMap(ETF_map);
	//ETF_Visual.grabScreen(0,0,ofGetWidth(),ofGetHeight());
	//ETF_Visual.saveImage("ETF.jpg");
	
	ofPolyline line;
	line.addVertex(bot_pos.x,bot_pos.y);
	if(UpdateBot())
	{
		line.addVertex(bot_pos.x,bot_pos.y);
		line.draw();
		
	}
}


bool testApp::UpdateBot()
{
	int index = floor(bot_pos.x+0.5)+floor(bot_pos.y+0.5)*ofGetWidth();
	if(index>=0 && index<ofGetWidth()*ofGetHeight())
	{
		if(ETF_map[index].length()>0)	// change direction based on ETF
		{
			direction = ETF_map[index] + direction;
			direction.normalize();
		}
		bot_pos += SPEED * direction;
		if(int(bot_startpos.x)==int(bot_pos.x) && int(bot_startpos.y) == int(bot_pos.y))	// back to start
		{
			bot_pos.set(ofRandom(0,ofGetWidth()),ofRandom(0,ofGetHeight()));
			bot_startpos.set(bot_pos);
			return false;
		}
		return true;
	}
	else
	{
		bot_pos.set(ofRandom(0,ofGetWidth()),ofRandom(0,ofGetHeight()));
		bot_startpos.set(bot_pos);
		return false;
	}
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


