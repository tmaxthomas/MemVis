#include <stdlib.h>

struct Bits {
	int width;
	int height;
	const char *layout;
};
static struct Bits chars[256] = {
	{}, // 0
	{}, // 1
	{}, // 2
	{}, // 3
	{}, // 4
	{}, // 5
	{}, // 6
	{}, // 7
	{}, // 8
	{}, // 9
	{}, // 10
	{}, // 11
	{}, // 12
	{}, // 13
	{}, // 14
	{}, // 15
	{}, // 16
	{}, // 17
	{}, // 18
	{}, // 19
	{}, // 20
	{}, // 21
	{}, // 22
	{}, // 23
	{}, // 24
	{}, // 25
	{}, // 26
	{}, // 27
	{}, // 28
	{}, // 29
	{}, // 30
	{}, // 31
	{2, 1,
		"  "},
	{1, 5,
		"."
		"."
		"."
		" "
		"."},
	{}, // 34
	{}, // 35
	{}, // 36
	{}, // 37
	{}, // 38
	{}, // 39
	{}, // 40
	{}, // 41
	{}, // 42
	{}, // 43
	{2, 2,
		" ."
		". "},
	{}, // 45
	{}, // 46
	{}, // 47
	{}, // 48
	{}, // 49
	{}, // 50
	{}, // 51
	{}, // 52
	{}, // 53
	{}, // 54
	{}, // 55
	{}, // 56
	{}, // 57
	{}, // 58
	{}, // 59
	{}, // 60
	{}, // 61
	{}, // 62
	{}, // 63
	{}, // 64
	{}, // 65
	{}, // 66
	{}, // 67
	{}, // 68
	{}, // 69
	{}, // 70
	{}, // 71
	{}, // 72
	{}, // 73
	{}, // 74
	{}, // 75
	{}, // 76
	{}, // 77
	{}, // 78
	{}, // 79
	{}, // 80
	{}, // 81
	{}, // 82
	{}, // 83
	{}, // 84
	{}, // 85
	{}, // 86
	{}, // 87
	{}, // 88
	{}, // 89
	{}, // 90
	{}, // 91
	{}, // 92
	{}, // 93
	{}, // 94
	{4, 1,
		"...."},
	{}, // 96
	{4, 5,
		" .. "
		"   ."
		" ..."
		".  ."
		" ..."},
	{4, 5,
		".   "
		".   "
		"... "
		".  ."
		"... "},
	{4, 4,
		" ..."
		".   "
		".   "
		" ..."},
	{4, 5,
		"   ."
		"   ."
		" ..."
		".  ."
		" ..."},
	{4, 5,
		" .. "
		".  ."
		"...."
		".   "
		" ..."},
	{4, 5,
		"  .."
		" .  "
		"...."
		" .  "
		" .  "},
	{4, 5,
		" .. "
		".  ."
		" ..."
		"   ."
		" .. "},
	{4, 5,
		".   "
		".   "
		"...."
		".  ."
		".  ."},
	{2, 5,
		". "
		"  "
		". "
		". "
		" ."},
	{3, 5,
		"  ."
		"   "
		"  ."
		"  ."
		".. "},
	{3, 5,
		".  "
		"..."
		". ."
		".. "
		". ."},
	{2, 5,
		". "
		". "
		". "
		". "
		" ."},
	{5, 4,
		".... "
		". . ."
		". . ."
		". . ."},
	{4, 4,
		"... "
		".  ."
		".  ."
		".  ."},
	{4, 4,
		" .. "
		".  ."
		".  ."
		" .. "},
	{3, 5,
		"..."
		". ."
		"..."
		".  "
		".  "},
	{3, 5,
		"..."
		". ."
		"..."
		"  ."
		"  ."},
	{4, 4,
		". .."
		"..  "
		".   "
		".   "},
	{3, 5,
		"..."
		".  "
		"..."
		"  ."
		"..."},
	{4, 5,
		" .  "
		"...."
		" .  "
		" .  "
		"  .."},
	{4, 4,
		".  ."
		".  ."
		".  ."
		"... "},
	{5, 4,
		".   ."
		".   ."
		" . . "
		"  .  "},
	{5, 4,
		".   ."
		". . ."
		". . ."
		" . . "},
	{3, 3,
		". ."
		" . "
		". ."},
	{4, 5,
		".  ."
		".  ."
		" ..."
		"   ."
		"... "},
	{4, 4,
		"...."
		"  . "
		" .  "
		"...."},
	{4, 5,
		"  .."
		" .  "
		"..  "
		" .  "
		"  .."},
	{}, // 124
	{4, 5,
		"..  "
		"  . "
		"  .."
		"  . "
		"..  "},
	{}, // 126
	{}, // 127

};

int main(int argc, const char **argv) {
	static const char *str = "flag{youll_probably_remember_this_one}";
	static int length = sizeof(str) / sizeof(const char);

	//Find length of str
	int width = 0;
	int height = 0;
	for (int i = 0; i < length; i ++) {
		struct Bits *bits = &(chars[(int)str[i]]);
		width += bits->width + 1;
		if (bits->height > height)
			height = bits->height;
	}
	width = 2048;
	//Build memory accesses for str
	unsigned char *memories = malloc(sizeof(unsigned char) * width * height);

	int lead = 0;
	for (int i = 0; i < length; i ++) {
		struct Bits *bits = &(chars[(int)str[i]]);

		int sy = height - bits->height;
		int sx = lead;

		for (int y = 0; y < bits->height; y ++) {
			for (int x = 0; x < bits->width; x ++) {
				if (bits->layout[x + y * bits->width] != ' ') {
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

		lead += bits->width + 1;
	}

	free(memories);

	return EXIT_SUCCESS;
}
