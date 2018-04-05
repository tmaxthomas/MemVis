#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
	ofSetVerticalSync(true);

	ofSetWindowShape(1280, 720);

	float scale;

	ofAppBaseWindow *window = ofGetWindowPtr();
	ofAppGLFWWindow *glfwWindow = dynamic_cast<ofAppGLFWWindow *>(window);
	if (glfwWindow != nullptr) {
		scale = glfwWindow->getPixelScreenCoordScale();
		ofSetWindowShape(1280 * scale, 720 * scale);
	} else {
		scale = 1.0f;
	}

	scale *= 4.0f;

	//Load file
	mFile = nullptr;
	ofFileDialogResult result = ofSystemLoadDialog("Select a .vzf");
	if (!result.bSuccess) {
		ofSystemAlertDialog("Need a vzf file");
		ofExit();
		return;
	}
	if (!loadFile(result)) {
		ofSystemAlertDialog("Error loading vzf");
		ofExit();
		return;
	}

	mMemoryBlock.setup(mFile, ofRectangle(100, 100, 800, 800));

//	// we add this listener before setting up so the initial circle resolution is correct
//	circleResolution.addListener(this, &ofApp::circleResolutionChanged);
//	ringButton.addListener(this, &ofApp::ringButtonPressed);
//
//	gui.setup(); // most of the time you don't need a name
//	gui.add(filled.setup("fill", true));
//	gui.add(radius.setup("radius", 140, 10, 300));
//	gui.add(center.setup("center", ofVec2f(ofGetWidth()*.5, ofGetHeight()*.5), ofVec2f(0, 0), ofVec2f(ofGetWidth(), ofGetHeight())));
//	gui.add(color.setup("color", ofColor(100, 100, 140), ofColor(0, 0), ofColor(255, 255)));
//	gui.add(circleResolution.setup("circle res", 5, 3, 90));
//	gui.add(twoCircles.setup("two circles"));
//	gui.add(ringButton.setup("ring"));
//	gui.add(screenSize.setup("screen size", ofToString(ofGetWidth())+"x"+ofToString(ofGetHeight())));
//
//	bHide = false;
//
//	ring.load("ring.wav");
}

//--------------------------------------------------------------
void ofApp::exit(){
	if (mFile) {
		delete mFile;
	}
//	ringButton.removeListener(this, &ofApp::ringButtonPressed);
}

bool ofApp::loadFile(ofFileDialogResult result) {
	mFile = new VizFile();
	std::ifstream stream(result.getPath());

	if (!mFile->read(stream)) {
		delete mFile;
		mFile = nullptr;
		return false;
	}

	return true;
}

//--------------------------------------------------------------
void ofApp::update(){
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackground(255, 255, 255);

	mMemoryBlock.draw();
	
//	ofSetColor(ofColor::white);
//	ofDrawRectangle(0, 0, mRender.getWidth() + 2, mRender.getHeight() + 2);
//	if(filled){
//		ofFill();
//	}else{
//		ofNoFill();
//	}
//
//	ofSetColor(color);
//	if(twoCircles){
//		ofDrawCircle(center->x-radius*.5, center->y, radius );
//		ofDrawCircle(center->x+radius*.5, center->y, radius );
//	}else{
//		ofDrawCircle((ofVec2f)center, radius );
//	}
//
//	// auto draw?
//	// should the gui control hiding?
//	if(!bHide){
//		gui.draw();
//	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if(key == '1'){
		mMemoryBlock.getParameter().cast<int>()++;
	}
	if(key == '2'){
		mMemoryBlock.getParameter().cast<int>()--;
	}
//	else if(key == 's'){
//		gui.saveToFile("settings.xml");
//	}
//	else if(key == 'l'){
//		gui.loadFromFile("settings.xml");
//	}
//	else if(key == ' '){
//		color = ofColor(255);
//	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}


