//smaa example by
//https://github.com/iryoku/smaa
//fxaa example by
//http://www.glge.org/demos/fxaa/

#include "testApp.h"
#include "ofShaderHelper.h"
#include "SearchTex.h"
#include "AreaTex.h"



//--------------------------------------------------------------
void testApp::setup(){
	//disable MSAA
	ofDisableAntiAliasing();

	texInput_.allocate(1024,768,GL_RGBA,false);
	ofImage img ;
	img.loadImage("test.png");
	texInput_.loadData(img.getPixelsRef());

	
	ofFbo::Settings defaultSetting;
	defaultSetting.numSamples=0;
	defaultSetting.textureTarget=GL_TEXTURE_2D;
	defaultSetting.height=768;
	defaultSetting.internalformat=GL_RGBA;
	defaultSetting.useDepth=false;
	defaultSetting.useStencil=false;
	defaultSetting.width=1024;
	defaultSetting.wrapModeHorizontal=GL_CLAMP_TO_EDGE;
	defaultSetting.wrapModeVertical=GL_CLAMP_TO_EDGE;
	defaultSetting.numSamples=false;

	fboRender_.allocate(defaultSetting);

	//Setup AA...
	aaType=0;//smaa

	fboAA_.allocate(defaultSetting);
	fboSmaaWk_.allocate(defaultSetting);

	texSmaaArea_.allocate(AREATEX_WIDTH,AREATEX_HEIGHT,GL_RG8,false);
	texSmaaArea_.loadData(areaTexBytes,AREATEX_WIDTH,AREATEX_HEIGHT,GL_RG);
	texSmaaArea_.setTextureMinMagFilter(GL_LINEAR,GL_LINEAR);

	texSmaaSearch_.allocate(SEARCHTEX_WIDTH,SEARCHTEX_HEIGHT,GL_R8,false);
	texSmaaSearch_.loadData(searchTexBytes,SEARCHTEX_WIDTH,SEARCHTEX_HEIGHT,GL_RED);
	texSmaaSearch_.setTextureMinMagFilter(GL_NEAREST,GL_NEAREST);

	loadShaders();
}



//--------------------------------------------------------------
void testApp::update(){
	fboRender_.begin();{
		//drawing target

		texInput_.draw(0,0);

		ofSetColor(255,0,0);
		ofCircle(640,700,20);

		ofSetColor(0,0,255);
		ofPushMatrix();
		ofTranslate(700,700);
		ofRotateZ(ofGetElapsedTimef()*25);
		ofRect(-20,-20,40,40);
		ofPopMatrix();
		
		ofSetColor(0,0,0);
		ofDrawBitmapString("FPS  : "+ ofToString(ofGetFrameRate()),50,670);
		ofDrawBitmapString("[F1] : NOAA "+ ofToString(aaType==0?"*":""),50,700);
		ofDrawBitmapString("[F2] : SMAA "+ ofToString(aaType==1?"*":""),50,720);
		ofDrawBitmapString("[F3] : FXAA "+ ofToString(aaType==2?"*":""),50,740);

		ofSetColor(255,255,255);
	
	}fboRender_.end();
	
	//do anti-aliusing
	if(aaType>0)doAA();
}

//--------------------------------------------------------------
void testApp::draw(){
	
	if(aaType>0){
		fboAA_.draw(0,0);
	}else{
		fboRender_.draw(0,0);
	}
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	if(key=='r')loadShaders();

	switch (key)
	{
	case OF_KEY_F1:
		aaType=0;
		break;
	case OF_KEY_F2:
		aaType=1;
		break;
	case OF_KEY_F3:
		aaType=2;
		break;
	default:
		break;
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


//--------------------------------------------------------------

void testApp::loadShaders(){
	puts("loading shaders...");
	loadShader(shaderFxaa_,"shaders//common/basicPlate.vert.glsl", "shaders/AA/fxaa.fs.glsl");
	loadShader(shaderSmaa_,"shaders/AA/smaa.vert.glsl", "shaders/AA/smaa.frag.glsl");
	
}

void testApp::doAA(){
	if(aaType==1){
		glDisable(GL_BLEND);
		glClearColor(0,0,0,0);

		shaderSmaa_.begin();
		fboAA_.begin(false);
		glColorMask(GL_TRUE,GL_TRUE,GL_FALSE,GL_FALSE);
		glClear(GL_COLOR_BUFFER_BIT);
		shaderSmaa_.setUniform1i("uStep",0);
		shaderSmaa_.setUniformTexture("uInpTex",fboRender_,0);
		ssObj.draw();
		glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
		fboAA_.end();
		
		fboSmaaWk_.begin(false);
		glClear(GL_COLOR_BUFFER_BIT);
		shaderSmaa_.setUniform1i("uStep",1);
		shaderSmaa_.setUniformTexture("uInpTex",fboAA_,0);
		shaderSmaa_.setUniformTexture("uInpTex2",texSmaaArea_,1);
		shaderSmaa_.setUniformTexture("uInpTex3",texSmaaSearch_,2);
		ssObj.draw();
		fboSmaaWk_.end();
		
		fboAA_.begin(false);
		shaderSmaa_.setUniform1i("uStep",2);
		shaderSmaa_.setUniformTexture("uInpTex",fboRender_,0);
		shaderSmaa_.setUniformTexture("uInpTex2",fboSmaaWk_,1);
		ssObj.draw();
		fboAA_.end();
		shaderSmaa_.end();
	}else if(aaType==2){
		fboAA_.begin(false);
		shaderFxaa_.begin();
		shaderFxaa_.setUniformTexture("uInpTex",fboRender_,0);
		ssObj.draw(); 
		shaderFxaa_.end();
		fboAA_.end();
	}
}