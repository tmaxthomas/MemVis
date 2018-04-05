#include "MemoryBlock.hpp"

MemoryBlock::MemoryBlock() {

}

MemoryBlock::~MemoryBlock() {
	
}



void MemoryBlock::drawRender() {
	int w = (U32)getWidth() & ~0xF;
	int h = (U32)getHeight() & ~0xF;
	mRender.setImageType(OF_IMAGE_COLOR);
	mRender.allocate(w, h, OF_IMAGE_COLOR);

	U32 maxAccesses = 0;
	for (int j = 0; j < mFile->segments.size(); j ++) {
		if (!mDisplayedSegments[j])
			continue;
		for (int i = 0; i < mFile->segments[j].size; i ++) {
			const auto &byte = mFile->segments[j].bytes[i];
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
			const auto &byte = mFile->segments[j].bytes[i];
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

int MemoryBlock::getSegmentYStart(const VizFile::Segment &segment) const {
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

int MemoryBlock::getSegmentHeight(const VizFile::Segment &segment) const {
	return (segment.size / getPixelWidth()) + 1; //Round up
}

ofRectangle MemoryBlock::getSegmentBounds(const VizFile::Segment &segment) const {
	return ofRectangle(
		0, getSegmentYStart(segment) * mScale, getPixelWidth() * mScale, getSegmentHeight(segment) * mScale
	);
}

void MemoryBlock::getPointsForAddress(const VizFile::Segment &segment, Address addr, ofPoint *points) const {
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


MemoryBlock* MemoryBlock::setup(const VizFile* file, ofRectangle bounds) {
	setName("");
	b = bounds;

	mFile = file;
	mScale = 1;
	mDisplayedSegments = std::vector<bool>(mFile->segments.size(), true);
	mHighlightSegment = -1;

	mScale.addListener(this, &MemoryBlock::scaleChanged);

	registerMouseEvents();
	return this;
}

void MemoryBlock::scaleChanged(int &scale) {
	mRender.clear();
	setNeedsRedraw();
}

bool MemoryBlock::mouseReleased(ofMouseEventArgs & args) {
	return false;
}
bool MemoryBlock::mouseMoved(ofMouseEventArgs & args) {
	mHighlightSegment = -1;
	for (int i = 0; i < mFile->segments.size(); i ++) {
		const auto &segment = mFile->segments[i];
		ofRectangle bounds = getSegmentBounds(segment);
		if (bounds.inside(args - b.getPosition())) {
			mHighlightSegment = i;
			return true;
		}
	}
	return false;
}
bool MemoryBlock::mousePressed(ofMouseEventArgs & args) {
	return false;
}
bool MemoryBlock::mouseDragged(ofMouseEventArgs & args) {
	return false;
}
bool MemoryBlock::mouseScrolled(ofMouseEventArgs & args) {
	return false;
}
ofAbstractParameter & MemoryBlock::getParameter() {
	return mScale;
}
void MemoryBlock::render() {
	ofSetColor(255, 255, 255, 255);
	mRender.draw(b);
	if (mHighlightSegment != -1) {
		const auto &segment = mFile->segments[mHighlightSegment];
		ofRectangle bounds = getSegmentBounds(segment);

		ofSetColor(255, 255, 255, 100);
		ofDrawRectangle(bounds + b.getPosition());
	}
}
bool MemoryBlock::setValue(float mx, float my, bool bCheckBounds) {
	return false;
}
void MemoryBlock::generateDraw() {
	drawRender();
}

