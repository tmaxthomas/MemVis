
#include "vizFile/vizFile.hpp"

void make_segment(VizFile &file, Address start, Address size, int accessMult = RAND_MAX) {
	VizFile::Segment segment;
	segment.startAddress = (Address)(start);
	segment.size = size;

	for (int addr = 0; addr < segment.size; addr ++) {
		VizFile::ByteData data;
		data.numHits = (U32)(rand());
		data.numMisses = (U32)(rand());
		segment.bytes.push_back(std::move(data));
	}

	file.segments.push_back(std::move(segment));
}

int main(int argc, const char **argv) {
	VizFile testFile;

	srand(time(NULL));
	make_segment(testFile, 0x1000, 0x2000);
	//Heap
	make_segment(testFile, 0x1800000, 0x230);
	//System
	make_segment(testFile, 0xbfff8000, 0x2000);
	//Stack
	make_segment(testFile, 0x7fff8000, 0x2000);

	std::ofstream outStream(argv[1]);

	if (!testFile.write(outStream)) {
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
