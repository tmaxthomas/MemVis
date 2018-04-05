#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "vizFile.hpp"

class ofApp : public ofBaseApp{

public:
	void setup();
	void update();
	void draw();

	void exit();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);

	bool loadFile(ofFileDialogResult result);
	void drawRender();

	inline int getPixelWidth() const { return mRender.getWidth() / mScale; }
	inline int getPixelHeight() const { return mRender.getHeight() / mScale; }

	int getSegmentYStart(const VizFile::Segment &segment) const;
	int getSegmentHeight(const VizFile::Segment &segment) const;
	ofRectangle getSegmentBounds(const VizFile::Segment &segment) const;
	void getPointsForAddress(const VizFile::Segment &segment, Address addr, ofPoint *points) const;

	VizFile *mFile;

	std::vector<bool> mDisplayedSegments;
	int mScale;
	int mHighlightSegment;
	ofImage mRender;

//	ofxFloatSlider radius;
//	ofxColorSlider color;
//	ofxVec2Slider center;
//	ofxIntSlider circleResolution;
//	ofxToggle filled;
//	ofxButton twoCircles;
//	ofxButton ringButton;
//	ofxLabel screenSize;
//
//	ofxPanel gui;
//
//	ofSoundPlayer ring;
};


