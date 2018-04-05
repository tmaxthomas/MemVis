#ifndef MemoryBlock_hpp
#define MemoryBlock_hpp

#include <stdio.h>
#include "ofMain.h"
#include "ofxGui.h"
#include "vizFile.hpp"

class MemoryBlock : public ofxBaseGui {
	friend class ofPanel;

public:
	MemoryBlock();
	~MemoryBlock();
	MemoryBlock* setup(const VizFile* file, ofRectangle bounds);

	void drawRender();
	int getSegmentYStart(const VizFile::Segment &segment) const;
	int getSegmentHeight(const VizFile::Segment &segment) const;
	ofRectangle getSegmentBounds(const VizFile::Segment &segment) const;
	void getPointsForAddress(const VizFile::Segment &segment, Address addr, ofPoint *points) const;

	virtual bool mouseReleased(ofMouseEventArgs & args);
	virtual bool mouseMoved(ofMouseEventArgs & args);
	virtual bool mousePressed(ofMouseEventArgs & args);
	virtual bool mouseDragged(ofMouseEventArgs & args);

	virtual ofAbstractParameter & getParameter();
	virtual bool mouseScrolled(ofMouseEventArgs & args);
	virtual void render();
	virtual bool setValue(float mx, float my, bool bCheckBounds);
	virtual void generateDraw();

	inline int getPixelWidth() const { return mRender.getWidth() / mScale; }
	inline int getPixelHeight() const { return mRender.getHeight() / mScale; }

	void scaleChanged(int &scale);
private:
	ofParameter<int> mScale;
	const VizFile *mFile;
	std::vector<bool> mDisplayedSegments;
	int mHighlightSegment;
	ofImage mRender;
};

#endif /* MemoryBlock_hpp */
