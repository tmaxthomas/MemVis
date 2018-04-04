
#include "types.hpp"

class VizFile : public Readable, public Writable {
public:
	class ByteData : public Readable, public Writable {
	public:
		U32 numHits;
		U32 numMisses;

		inline U32 numAccesses() const { return numHits + numMisses; }

		ByteData() : numHits(0), numMisses(0) {}
		virtual ~ByteData() {}

		virtual bool read(std::istream &stream);
		virtual bool write(std::ostream &stream) const;
	};
	class Segment : public Readable, public Writable {
	public:
		Address startAddress;
		Address size;
		std::vector<ByteData> bytes;

		inline Address endAddress() const { return startAddress + size; }

		Segment() : startAddress(0), size(0) {}
		virtual ~Segment() {}

		virtual bool read(std::istream &stream);
		virtual bool write(std::ostream &stream) const;
	};

	std::vector<Segment> segments;

	VizFile() {}
	virtual ~VizFile() {}

	virtual bool read(std::istream &stream);
	virtual bool write(std::ostream &stream) const;
};
