#include "memDrawing.hpp"
#include <math.h>

void MemDrawing::draw() {
	mDisplayedSegments = std::vector<bool>(mFile->segments.size(), true);
	mSegmentLocations = std::vector<int>(mFile->segments.size(), -1);
	mScale = 1;

	int w = (U32)mSize.width() & ~0xF;
	int h = getSegmentYStart(mFile->segments.size() - 1) + getSegmentHeight(mFile->segments.size() - 1) + 1;
	if (h & 0xF) h += 0x10 - (h & 0xF);
	mSize.setHeight(h);

	printf("Image will be %dx%d pixels\n", w, h);

	QImage img(mSize, QImage::Format_ARGB32_Premultiplied);

	img.fill(Qt::black);

	int count =0;
	for (int j = 0; j < mFile->segments.size(); j ++) {
		U32 maxAccesses = 0;
		for (int i = 0; i < mFile->segments[j].size; i ++) {
			const auto &byte = mFile->segments[j].bytes[i];
			if (byte.numAccesses() > maxAccesses) {
				maxAccesses = byte.numAccesses();
			}
		}

		if (!mDisplayedSegments[j])
			continue;
		for (int i = 0; i < mFile->segments[j].size; i ++) {
			const auto &byte = mFile->segments[j].bytes[i];
			if (byte.numAccesses() == 0) continue;
			float value = (float)byte.numAccesses() / (float)maxAccesses;
			value = sqrtf(sqrtf(value));
			float readRatio = (float)byte.numReads / (float)byte.numAccesses();
			//Convert readRatio to a hue
			float hue = 0.0f /* red */ + (readRatio * 120.0f /* green */);
			float saturation = 255.0f;
			float brightness = value * 255.0f;

			QColor color;
			color.setHsv(hue, saturation, brightness);

			QPoint points[mScale * mScale];
			getPointsForAddress(j, mFile->segments[j].startAddress + i, points);

			for (int k = 0; k < mScale * mScale; k ++) {
				if (points[k].y() >= h) {
					goto done;
				}
				img.setPixelColor(points[k], color);
				count ++;
			}
		}
		printf("Rendered %d / %lu segments\n", j, mFile->segments.size());
	}
done:
	printf("Rendered %d pixels\n", count);

	emit done(img);
	emit cleanup();
}

int MemDrawing::getSegmentYStart(int segmentIndex) const {
	if (segmentIndex == 0) {
		//We're first
		return 0;
	}

	if (mSegmentLocations[segmentIndex] != -1) {
		return mSegmentLocations[segmentIndex];
	}

	int closestHeight = getSegmentHeight(segmentIndex - 1);
	int closestEnd = getSegmentYStart(segmentIndex - 1);

	int y = closestEnd + closestHeight + 4;
	mSegmentLocations[segmentIndex] = y;

	return y;
}

int MemDrawing::getSegmentHeight(int segmentIndex) const {
	return (mFile->segments[segmentIndex].size / getPixelWidth()) + 1; //Round up
}

QRect MemDrawing::getSegmentBounds(int segmentIndex) const {
	return QRect(
		0, getSegmentYStart(segmentIndex) * mScale, getPixelWidth() * mScale, getSegmentHeight(segmentIndex) * mScale
	);
}

void MemDrawing::getPointsForAddress(int segmentIndex, Address addr, QPoint *points) const {
	int w = getPixelWidth();

	Address rel = addr - mFile->segments[segmentIndex].startAddress;

	int sx = rel % w;
	int sy = rel / w;
	sy += getSegmentYStart(segmentIndex);

	int i = 0;
	for (int x = 0; x < mScale; x ++) {
		for (int y = 0; y < mScale; y ++) {
			points[i].setX(sx * mScale + x);
			points[i].setY(sy * mScale + y);
			i ++;
		}
	}
}

