#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
	ofSetVerticalSync(true);

	ofSetWindowShape(1280, 720);

	ofAppBaseWindow *window = ofGetWindowPtr();
	ofAppGLFWWindow *glfwWindow = dynamic_cast<ofAppGLFWWindow *>(window);
	if (glfwWindow != nullptr) {
		mScale = glfwWindow->getPixelScreenCoordScale();
		ofSetWindowShape(1280 * mScale, 720 * mScale);
	} else {
		mScale = 1.0f;
	}

	mScale *= 4.0f;
	mHighlightSegment = -1;

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

	mDisplayedSegments = std::vector<bool>(mFile->segments.size(), true);

	return true;
}

void ofApp::drawRender() {
	int w = ofGetViewportWidth() & ~0xF;
	int h = ofGetViewportHeight() & ~0xF;
	mRender.setImageType(OF_IMAGE_COLOR);
	mRender.allocate(w, h, OF_IMAGE_COLOR);

	U32 maxAccesses = 0;
	for (int j = 0; j < mFile->segments.size(); j ++) {
		if (!mDisplayedSegments[j])
			continue;
		for (int i = 0; i < mFile->segments[j].size; i ++) {
			VizFile::ByteData &byte = mFile->segments[j].bytes[i];
			if (byte.numAccesses() > maxAccesses) {
				maxAccesses = byte.numAccesses();
			}
		}
	}

	ofPixelsRef pixels = mRender.getPixels();
	pixels.setColor(ofColor::black);

	for (int j = 0; j < mFile->segments.size(); j ++) {
		if (!mDisplayedSegments[j])
			continue;
		for (int i = 0; i < mFile->segments[j].size; i ++) {
			VizFile::ByteData &byte = mFile->segments[j].bytes[i];
			float value = (float)byte.numAccesses() / (float)maxAccesses;
			float hitRatio = (float)byte.numHits / (float)byte.numAccesses();
			//Convert hitRatio to a hue
			float hue = 0.0f /* red */ + (hitRatio * 120.0f /* green */);
			float saturation = 255.0f;
			float brightness = value * 255.0f;

			ofColor color;
			color.setHsb(hue, saturation, brightness);

			ofPoint points[mScale * mScale];
			getPointsForAddress(mFile->segments[j], mFile->segments[j].startAddress + i, points);

			for (int k = 0; k < mScale * mScale; k ++) {
				if (points[k].y >= h) {
					break;
				}
				pixels.setColor(points[k].x, points[k].y, color);
			}
		}
	}
	mRender.update();
}

int ofApp::getSegmentYStart(const VizFile::Segment &segment) const {
	//Find closest segment before this
	int closest = -1;

	for (int i = 0; i < mFile->segments.size(); i ++) {
		if (!mDisplayedSegments[i])
			continue;
		const auto &other = mFile->segments[i];

		Address end = other.endAddress();
		if (end < segment.startAddress && (closest == -1 || end > mFile->segments[closest].endAddress())) {
			closest = i;
		}
	}

	if (closest == -1) {
		//We're first
		return 0;
	}

	int closestEnd = getSegmentYStart(mFile->segments[closest]);
	int closestHeight = getSegmentHeight(mFile->segments[closest]);

	return closestEnd + closestHeight + 4;
}

int ofApp::getSegmentHeight(const VizFile::Segment &segment) const {
	return (segment.size / getPixelWidth()) + 1; //Round up
}

ofRectangle ofApp::getSegmentBounds(const VizFile::Segment &segment) const {
	return ofRectangle(
		0, getSegmentYStart(segment) * mScale, getPixelWidth() * mScale, getSegmentHeight(segment) * mScale
	);
}

void ofApp::getPointsForAddress(const VizFile::Segment &segment, Address addr, ofPoint *points) const {
	int w = getPixelWidth();

	Address rel = addr - segment.startAddress;

	int sx = rel % w;
	int sy = rel / w;
	sy += getSegmentYStart(segment);

	int i = 0;
	for (int x = 0; x < mScale; x ++) {
		for (int y = 0; y < mScale; y ++) {
			points[i].x = sx * mScale + x;
			points[i].y = sy * mScale + y;
			i ++;
		}
	}
}

//--------------------------------------------------------------
void ofApp::update(){
	if (!mRender.isAllocated()) {
		drawRender();
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofSetColor(255, 255, 255, 255);
	ofBackground(0, 0, 0);
//	ofSetColor(ofColor::white);
//	ofDrawRectangle(0, 0, mRender.getWidth() + 2, mRender.getHeight() + 2);
	int rx = 0;
	int ry = 0;
	if (ofGetViewportWidth() & 0xF) {
		rx = 1;
	}
	if (ofGetViewportHeight() & 0xF) {
		ry = 1;
	}
	mRender.draw(rx, ry);

	if (mHighlightSegment != -1) {
		const auto &segment = mFile->segments[mHighlightSegment];
		ofRectangle bounds = getSegmentBounds(segment);

		ofSetColor(255, 255, 255, 100);
		ofDrawRectangle(bounds);
	}

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
		mScale ++;
		mRender.clear();
	}
	if(key == '2'){
		mScale --;
		mRender.clear();
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
	mHighlightSegment = -1;
	for (int i = 0; i < mFile->segments.size(); i ++) {
		const auto &segment = mFile->segments[i];
		ofRectangle bounds = getSegmentBounds(segment);
		if (bounds.inside(x, y)) {
			mHighlightSegment = i;
		}
	}
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
	mRender.clear();
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}


