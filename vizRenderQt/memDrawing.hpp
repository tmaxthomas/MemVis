#ifndef MemScene_hpp
#define MemScene_hpp

#include <stdio.h>
#include <QGraphicsScene>
#include "vizFile/vizFile.hpp"

class MemDrawing : public QObject {
	Q_OBJECT
	VizFile *mFile;
	QSize mSize;
	int mScale;
	std::vector<bool> mDisplayedSegments;
	mutable std::vector<int> mSegmentLocations; 

public:
	struct DrawSettings {
		enum Axis {
			ReadsVsWrites,
			HitsVsMisses,
			Accesses
		};
		Axis hueAxis = ReadsVsWrites;
		Axis brightnessAxis = Accesses;
	};
	DrawSettings settings;

	MemDrawing(VizFile *file, int width) : mFile(file), mSize(width, 0) {}

public slots:
	void draw();

signals:
	void done(QImage);
	void cleanup();

private:
	int getSegmentYStart(int segmentIndex) const;
	int getSegmentHeight(int segmentIndex) const;
	QRect getSegmentBounds(int segmentIndex) const;
	void getPointsForAddress(int segmentIndex, Address addr, QPoint *points) const;

	inline int getPixelWidth() const { return mSize.width() / mScale; }
	inline int getPixelHeight() const { return mSize.height() / mScale; }

};

#endif /* MemScene_hpp */
