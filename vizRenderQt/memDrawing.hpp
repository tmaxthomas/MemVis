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

public:
	MemDrawing(VizFile *file, int width) : mFile(file), mSize(width, 0) {}

public slots:
	void draw();

signals:
	void done(QImage);
	void cleanup();

private:
	int getSegmentYStart(const VizFile::Segment &segment) const;
	int getSegmentHeight(const VizFile::Segment &segment) const;
	QRect getSegmentBounds(const VizFile::Segment &segment) const;
	void getPointsForAddress(const VizFile::Segment &segment, Address addr, QPoint *points) const;

	inline int getPixelWidth() const { return mSize.width() / mScale; }
	inline int getPixelHeight() const { return mSize.height() / mScale; }

};

#endif /* MemScene_hpp */
