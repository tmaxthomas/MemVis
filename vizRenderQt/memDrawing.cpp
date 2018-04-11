#include "memDrawing.hpp"
#include <math.h>

void MemDrawing::draw() {
	mDisplayedSegments = std::vector<bool>(mFile->segments.size(), true);
	mScale = 1;

	int w = (U32)mSize.width() & ~0xF;
	int h = getSegmentYStart(mFile->segments.back()) + getSegmentHeight(mFile->segments.back()) + 1;
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
			float hitRatio = 1.0f; //(float)byte.numHits / (float)byte.numAccesses();
			//Convert hitRatio to a hue
			float hue = 0.0f /* red */ + (hitRatio * 120.0f /* green */);
			float saturation = 255.0f;
			float brightness = value * 255.0f;

			QColor color;
			color.setHsv(hue, saturation, brightness);

			QPoint points[mScale * mScale];
			getPointsForAddress(mFile->segments[j], mFile->segments[j].startAddress + i, points);

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

int MemDrawing::getSegmentYStart(const VizFile::Segment &segment) const {
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

int MemDrawing::getSegmentHeight(const VizFile::Segment &segment) const {
	return (segment.size / getPixelWidth()) + 1; //Round up
}

QRect MemDrawing::getSegmentBounds(const VizFile::Segment &segment) const {
	return QRect(
		0, getSegmentYStart(segment) * mScale, getPixelWidth() * mScale, getSegmentHeight(segment) * mScale
	);
}

void MemDrawing::getPointsForAddress(const VizFile::Segment &segment, Address addr, QPoint *points) const {
	int w = getPixelWidth();

	Address rel = addr - segment.startAddress;

	int sx = rel % w;
	int sy = rel / w;
	sy += getSegmentYStart(segment);

	int i = 0;
	for (int x = 0; x < mScale; x ++) {
		for (int y = 0; y < mScale; y ++) {
			points[i].setX(sx * mScale + x);
			points[i].setY(sy * mScale + y);
			i ++;
		}
	}
}

