#pragma once
#include "types.hpp"

class VizFile : public Readable {
public:
    class ByteData {
	public:
        U64 numReads;
        U64 numWrites;
        U64 numExecutes;

		U64 numRWHits;
		U64 numRWMisses;

		U64 numExecHits;
		U64 numExecMisses;

        inline U64 numAccesses() const { return numReads + numWrites + numExecutes; }

        ByteData() : numReads(0), numWrites(0), numExecutes(0) {}
		virtual ~ByteData() {}

        virtual bool operator<(const ByteData &other) const {
            if (numReads != other.numReads)
                return numReads < other.numReads;
            if (numWrites != other.numWrites)
                return numWrites < other.numWrites;
            if (numExecutes != other.numExecutes)
                return numExecutes < other.numExecutes;
            return false;
        }
    };

	class Segment {
	public:
		Address startAddress;
		Address size;
		std::vector<ByteData> bytes;

		inline Address endAddress() const { return startAddress + size; }

		void addByte(Address address, const ByteData &byte);

		Segment() : startAddress(0), size(0) {}
		virtual ~Segment() {}
	};

	std::vector<Segment> segments;
	std::map<Address, ByteData> bytes;

	VizFile() {}
	virtual ~VizFile() {}

	virtual bool read(std::istream &stream);
	bool readRWAccessSection(std::istream &stream, U32 count);
	bool readRWCacheSection(std::istream &stream, U32 count);
	bool readExecAccessSection(std::istream &stream, U32 count);
	bool readExecCacheSection(std::istream &stream, U32 count);
};
