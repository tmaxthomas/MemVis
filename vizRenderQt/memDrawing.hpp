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

	MemDrawing(VizFile *file, int width) : mFile(file), mSize(width, 0) {}

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

	inline int getPixelWidth() const { return mSize.width() / mScale; }
	inline int getPixelHeight() const { return mSize.height() / mScale; }

};

#endif /* MemScene_hpp */
