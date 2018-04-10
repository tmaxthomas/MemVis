
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

bool write_random_segments(std::ostream &stream) {
	VizFile testFile;

	srand(time(NULL));
	make_segment(testFile, 0x1000, 0x2000);
	//Heap
	make_segment(testFile, 0x1800000, 0x230);
	//System
	make_segment(testFile, 0xbfff8000, 0x2000);
	//Stack
	make_segment(testFile, 0x7fff8000, 0x2000);

	return testFile.write(stream);
}


struct Bits {
	char ch;
	int width;
	int height;
	const char *layout;
};
static std::vector<Bits> chars{
	{'a', 4, 5,
		" .. "
		"   ."
		" ..."
		".  ."
		" ..."},
	{'c', 4, 4,
		" ..."
		".   "
		".   "
		" ..."},
	{'d', 4, 5,
		"   ."
		"   ."
		" ..."
		".  ."
		" ..."},
	{'e', 4, 5,
		" .. "
		".  ."
		"...."
		".   "
		" ..."},
	{'f', 4, 5,
		"  .."
		" .  "
		"...."
		" .  "
		" .  "},
	{'g', 4, 5,
		" .. "
		".  ."
		" ..."
		"   ."
		" .. "},
	{'i', 2, 5,
		". "
		"  "
		". "
		". "
		" ."},
	{'l', 2, 5,
		". "
		". "
		". "
		". "
		" ."},
	{'n', 4, 4,
		"... "
		".  ."
		".  ."
		".  ."},
	{'o', 4, 4,
		" .. "
		".  ."
		".  ."
		" .. "},
	{'r', 4, 4,
		". .."
		"..  "
		".   "
		".   "},
	{'t', 4, 5,
		" .  "
		"...."
		" .  "
		" .  "
		"  .."},
	{' ', 2, 1,
		"  "},
	{',', 2, 2,
		" ."
		". "},
	{'!', 1, 5,
		"."
		"."
		"."
		" "
		"."},
	{'{', 4, 5,
		"  .."
		" .  "
		"..  "
		" .  "
		"  .."},
	{'}', 4, 5,
		"..  "
		"  . "
		"  .."
		"  . "
		"..  "},
	{'_', 4, 1,
		"...."},
};

const Bits &chBits(char ch) {
	return *std::find_if(chars.begin(), chars.end(), [ch](Bits &bits)->bool {
		return bits.ch == ch;
	});
}

bool write_funny_text(std::ostream &stream) {
	static const char *str = "flag{i_need_a_doctor}";

	//Find length of str
	int width = 0;
	int height = 0;
	for (int i = 0; i < strlen(str); i ++) {
		const Bits &bits = chBits(str[i]);
		width += bits.width + 1;
		height = std::max(height, bits.height);
	}
	if (width & 0xF) width += 0x10 - (width % 0x10);
	//Build memory accesses for str
	unsigned char *memories = new unsigned char[width * height]();

	int lead = 0;
	for (int i = 0; i < strlen(str); i ++) {
		const Bits &bits = chBits(str[i]);

		int sy = height - bits.height;
		int sx = lead;

		for (int y = 0; y < bits.height; y ++) {
			for (int x = 0; x < bits.width; x ++) {
				if (bits.layout[x + y * bits.width] != ' ') {
					memories[sx + x + ((sy + y) * width)] = (x + sx) * 256 / width;
				}
			}
		}

		lead += bits.width + 1;
	}

	VizFile file;
	VizFile::Segment segment;
	segment.startAddress = (Address)(0x0);
	segment.size = width * height;

	for (int i = 0; i < width * height; i ++) {
		VizFile::ByteData data;
		data.numHits = (U32)(memories[i]);
		data.numMisses = (U32)(memories[i] == 0 ? 0 : 256 - memories[i]);
		segment.bytes.push_back(std::move(data));
	}

	file.segments.push_back(std::move(segment));

	delete [] memories;

	return file.write(stream);
}

int main(int argc, const char **argv) {
	std::ofstream f1(argv[1]);
	if (!write_random_segments(f1)) {
		return EXIT_FAILURE;
	}
	std::ofstream f2(argv[2]);
	if (!write_funny_text(f2)) {
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
