#pragma once

#include "ofMain.h"
#include "ofSSobj.h"

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

private:
	ofTexture texInput_;
	ofFbo fboRender_;


	void loadShaders();
	void doAA();

	unsigned int aaType;
	ofShader shaderSmaa_;
	ofShader shaderFxaa_;
	ofFbo fboAA_;
	ofSSobj  ssObj;
	ofFbo fboSmaaWk_;
	ofTexture texSmaaSearch_;
	ofTexture texSmaaArea_;
};
