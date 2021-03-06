#ifndef MemScene_hpp
#define MemScene_hpp

#include <stdio.h>
#include <QGraphicsScene>
#include "vizFile/vizFile.hpp"

class MemDrawing : public QObject {
	Q_OBJECT
	VizFile *mFile;
	QSize mSize;
	std::vector<bool> mDisplayedSegments;
	mutable std::vector<int> mSegmentLocations; 

public:
	struct DrawSettings {
		enum Axis {
			ReadAccessRatio,
			WriteAccessRatio,
			ExecAccessRatio,
			HitAccessRatio,
			Accesses,
			Constant
		};
		Axis hueAxis = ReadAccessRatio;
		Axis brightnessAxis = Accesses;
	};
	DrawSettings settings;

	void setFile(VizFile *file) {
		mFile = file;
	}
	void setSize(QSize size) {
		mSize = size;
	}
	void setSegmentVisible(int segment, bool vis) {
		mDisplayedSegments[segment] = vis;
	}
	void resetVisible() {
		mDisplayedSegments = std::vector<bool>(mFile->segments.size(), true);
	}

public slots:
	void draw();

signals:
	void done(QImage);
	void cleanup();

public:
	int getSegmentYStart(int segmentIndex) const;
	int getSegmentHeight(int segmentIndex) const;
	QRect getSegmentBounds(int segmentIndex) const;
	void getPointForAddress(int segmentIndex, Address addr, QPoint &point) const;
	Address getAddressForPoint(QPointF point) const;

	inline int getPixelWidth() const { return mSize.width(); }
	inline int getPixelHeight() const { return mSize.height(); }

};

#endif /* MemScene_hpp */
