
#include "vizFile/vizFile.hpp"

int main(int argc, const char **argv) {
	VizFile testFile;

	srand((unsigned int)time(NULL));

	//Create some random segments
	for (int i = 0; i < 10; i ++) {
		VizFile::Segment segment;
		segment.startAddress = (Address)(i * 0x10000);
		segment.size = 0x10000;

		for (int addr = 0; addr < segment.size; addr ++) {
			VizFile::ByteData data;
			data.numHits = (U32)(addr % 16 == 0 ? 100 : 0);
			data.numMisses = (U32)(addr % 16 == 0 ? 100 : 0);
			segment.bytes.push_back(std::move(data));
		}

		testFile.segments.push_back(std::move(segment));
	}

	std::ofstream outStream(argv[1]);

	if (!testFile.write(outStream)) {
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
