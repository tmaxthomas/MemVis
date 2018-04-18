#include "memDrawing.hpp"
#include <math.h>

void MemDrawing::draw() {
	//Clear locations before we draw so they recalculate
	mSegmentLocations = std::vector<int>(mFile->segments.size(), -1);

	//Width/height parameters
	int w = (U32)mSize.width() & ~0xF;
	int h = getSegmentYStart(mFile->segments.size() - 1) + getSegmentHeight(mFile->segments.size() - 1) + 1;
	if (h & 0xF) h += 0x10 - (h & 0xF);
	mSize.setHeight(h);

	printf("Image will be %dx%d pixels\n", w, h);

	//Create a new image to render into
	QImage img(mSize, QImage::Format_ARGB32_Premultiplied);
	img.fill(Qt::black);

	int count =0;
	for (int i = 0; i < mFile->segments.size(); i ++) {
		if (!mDisplayedSegments[i])
			continue;

		const VizFile::Segment &segment = mFile->segments[i];

		//Find the byte with the max # addresses so we have something to compare #addresses to
		U32 maxAccesses = 0;
		for (int j = 0; j < segment.size; j ++) {
			const auto &byte = segment.bytes[j];
			if (byte.numAccesses() > maxAccesses) {
				maxAccesses = byte.numAccesses();
			}
		}

		for (int j = 0; j < segment.size; j ++) {
			const auto &byte = segment.bytes[j];
			if (byte.numAccesses() == 0) continue;
            float accessRatio = (float)byte.numAccesses() / (float)maxAccesses;
            accessRatio = sqrtf(sqrtf(accessRatio)); //Makes it look better
			float readRatio = (float)byte.numReads / (float)byte.numAccesses();
			float writeRatio = (float)byte.numWrites / (float)byte.numAccesses();
			float execRatio = (float)byte.numExecutes / (float)byte.numAccesses();
            float hitRatio = (float)byte.numHits / (float)byte.numAccesses();
            float hue, saturation, brightness;

            //Hue switch (0.0f low bound, 120.0f high bound)
            switch(settings.hueAxis) {
            case MemDrawing::DrawSettings::Axis::Accesses:
                hue = 0.0f + (accessRatio * 120.0f);
                break;
            case MemDrawing::DrawSettings::Axis::HitAccessRatio:
                hue = 0.0f + (hitRatio * 120.0f);
                break;
            case MemDrawing::DrawSettings::Axis::ReadAccessRatio:
                hue = 0.0f + (readRatio * 120.0f);
                break;
            case MemDrawing::DrawSettings::Axis::WriteAccessRatio:
                hue = 0.0f + (writeRatio * 120.0f);
                break;
            case MemDrawing::DrawSettings::Axis::ExecAccessRatio:
                hue = 0.0f + (execRatio * 120.0f);
                break;
			case MemDrawing::DrawSettings::Axis::Constant:
	            hue = 60.0f;
	            break;
            }

            saturation = 255.0f;

            //Brightness switch
            switch(settings.brightnessAxis) {
            case MemDrawing::DrawSettings::Axis::Accesses:
            	brightness = accessRatio * 255.0f;
                break;
            case MemDrawing::DrawSettings::Axis::HitAccessRatio:
                brightness = hitRatio * 255.0f;
                break;
            case MemDrawing::DrawSettings::Axis::ReadAccessRatio:
                brightness = readRatio * 255.0f;
                break;
            case MemDrawing::DrawSettings::Axis::WriteAccessRatio:
                brightness = writeRatio * 255.0f;
                break;
            case MemDrawing::DrawSettings::Axis::ExecAccessRatio:
                brightness = execRatio * 255.0f;
                break;
			case MemDrawing::DrawSettings::Axis::Constant:
            	brightness = 255.0f;
            	break;
            }

			QColor color;
			color.setHsv(hue, saturation, brightness);

			//Where we at
			QPoint point;
			getPointForAddress(i, segment.startAddress + j, point);

			//Error checking! This should never happen
			if (point.y() >= h) {
				printf("Too far!\n");
				goto done;
			}
			img.setPixelColor(point, color);
			count ++;
		}
		printf("Rendered %d / %lu segments\n", i, mFile->segments.size());
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

	//We have cache
	if (mSegmentLocations[segmentIndex] != -1) {
		return mSegmentLocations[segmentIndex];
	}

	//Our start = prev seg's end = prev start + prev height
	int closestHeight = getSegmentHeight(segmentIndex - 1);
	int closestEnd = getSegmentYStart(segmentIndex - 1);

	int y = mDisplayedSegments[segmentIndex] ? closestEnd + closestHeight + 4 : closestEnd;
	mSegmentLocations[segmentIndex] = y;

	return y;
}

int MemDrawing::getSegmentHeight(int segmentIndex) const {
	if (!mDisplayedSegments[segmentIndex]) {
		return 0;
	}
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
			//Step 2: Now translate it back
			int dx = pt.x();
			int dy = pt.y() - getSegmentYStart(i);

			Address offset = dx + (dy * getPixelWidth());

			return mFile->segments[i].startAddress + offset;
		}
	}
	return 0;
}

