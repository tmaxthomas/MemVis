
#include "types.hpp"

class VizFile : public Readable, public Writable {
public:
	class ByteData : public Readable, public Writable {
	public:
		U32 numHits;
		U32 numMisses;

		virtual bool read(std::istream &stream);
		virtual bool write(std::ostream &stream) const;
	};
	class Segment : public Readable, public Writable {
	public:
		Address startAddress;
		Address size;
		std::vector<ByteData> bytes;

		virtual bool read(std::istream &stream);
		virtual bool write(std::ostream &stream) const;
	};

	std::vector<Segment> segments;

	virtual bool read(std::istream &stream);
	virtual bool write(std::ostream &stream) const;
};
