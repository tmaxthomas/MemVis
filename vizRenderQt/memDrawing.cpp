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
            float accessRatio = (float)byte.numAccesses() / (float)maxAccesses;
            accessRatio = sqrtf(sqrtf(accessRatio));
			float readRatio = (float)byte.numReads / (float)byte.numAccesses();
            float hitRatio = (float)byte.numHits / (float)byte.numAccesses();
            float hue, saturation, brightness;

            //Hue switch (0.0f low bound, 120.0f high bound)
            switch(settings.hueAxis) {
            case MemDrawing::DrawSettings::Axis::Accesses:
                hue = 0.0f + (accessRatio * 120.0f);
                break;
            case MemDrawing::DrawSettings::Axis::HitsVsMisses:
                hue = 0.0f + (hitRatio * 120.0f);
                break;
            case MemDrawing::DrawSettings::Axis::ReadsVsWrites:
                hue = 0.0f + (readRatio * 120.0f);
                break;
            }

            saturation = 255.0f;

            //Brightness switch
            switch(settings.brightnessAxis) {
            case MemDrawing::DrawSettings::Axis::Accesses:
                 brightness = accessRatio * 255.0f;
                break;
            case MemDrawing::DrawSettings::Axis::HitsVsMisses:
                brightness = hitRatio * 255.0f;
                break;
            case MemDrawing::DrawSettings::Axis::ReadsVsWrites:
                brightness = readRatio * 255.0f;
                break;
            }

			QColor color;
			color.setHsv(hue, saturation, brightness);

			QPoint point;
			getPointForAddress(j, mFile->segments[j].startAddress + i, point);

			if (point.y() >= h) {
				goto done;
			}
			img.setPixelColor(point, color);
			count ++;
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
		0, getSegmentYStart(segmentIndex), getPixelWidth(), getSegmentHeight(segmentIndex)
	);
}

void MemDrawing::getPointForAddress(int segmentIndex, Address addr, QPoint &point) const {
	int w = getPixelWidth();

	Address rel = addr - mFile->segments[segmentIndex].startAddress;

	int sx = rel % w;
	int sy = rel / w;
	sy += getSegmentYStart(segmentIndex);

	point.setX(sx);
	point.setY(sy);
}

Address MemDrawing::getAddressForPoint(QPointF point) const {
	QPoint pt((int)floor(point.x()), (int)floor(point.y()));
	//Step 1: what segment contains this point
	for (int i = 0; i < mFile->segments.size(); i ++) {
		if (!mDisplayedSegments[i])
			continue;
		if (getSegmentBounds(i).contains(pt)) {
			//Now translate it back

			int dx = pt.x();
			int dy = pt.y() - getSegmentYStart(i);

			Address offset = dx + (dy * getPixelWidth());

			return mFile->segments[i].startAddress + offset;
		}
	}
	return 0;
}

