
#include <vector>
#include <string.h>
#include <stdlib.h>
#include <algorithm>

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

int main(int argc, const char **argv) {
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
					unsigned char *memptr = &memories[sx + x + ((sy + y) * width)];
					(*memptr)++;
					(*memptr)++;
					(*memptr)++;
					(*memptr)++;
					(*memptr)++;
					(*memptr)++;
					(*memptr)++;
					(*memptr)++;
					(*memptr)++;
					(*memptr)++;
					(*memptr)++;
					(*memptr)++;
					(*memptr)++;
					(*memptr)++;
					(*memptr)++;
					(*memptr)++;
					(*memptr)++;
					(*memptr)++;
					(*memptr)++;
					(*memptr)++;
					(*memptr)++;
					(*memptr)++;
					(*memptr)++;
					(*memptr)++;
					(*memptr)++;
					(*memptr)++;
					(*memptr)++;
					(*memptr)++;
					(*memptr)++;
					(*memptr)++;
					(*memptr)++;
					(*memptr)++;
					(*memptr)++;
					(*memptr)++;
					(*memptr)++;
					(*memptr)++;
					(*memptr)++;
					(*memptr)++;
					(*memptr)++;
					(*memptr)++;
					(*memptr)++;
					(*memptr)++;
					(*memptr)++;
					(*memptr)++;
					(*memptr)++;
					(*memptr)++;
					(*memptr)++;
					(*memptr)++;
					(*memptr)++;
					(*memptr)++;
					(*memptr)++;
					(*memptr)++;
					(*memptr)++;
					(*memptr)++;
					(*memptr)++;
					(*memptr)++;
				}
			}
		}

		lead += bits.width + 1;
	}

	delete [] memories;

	return EXIT_SUCCESS;
}
