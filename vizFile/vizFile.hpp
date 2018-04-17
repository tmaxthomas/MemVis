#pragma once
#include "types.hpp"

class VizFile : public Readable {
public:
    class ByteData {
	public:
        U64 numReads;
        U64 numWrites;
        U64 numExecutes;

		U64 numHits;
		U64 numMisses;

        inline U64 numAccesses() const { return numReads + numWrites + numExecutes; }

        ByteData() : numReads(0), numWrites(0), numExecutes(0), numHits(0), numMisses(0) {}
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

		bool operator==(const Segment &rhs) const { return startAddress == rhs.startAddress; }
		bool operator!=(const Segment &rhs) const { return !(rhs == *this); }
		bool operator<(const Segment &rhs) const { return startAddress < rhs.startAddress; }
		bool operator>(const Segment &rhs) const { return rhs < *this; }
		bool operator<=(const Segment &rhs) const { return !(rhs < *this); }
		bool operator>=(const Segment &rhs) const { return !(*this < rhs); }

		Segment() : startAddress(0), size(0) {}
		virtual ~Segment() {}
	};

	std::vector<Segment> segments;
	std::map<Address, ByteData> bytes;

	VizFile() {}
	virtual ~VizFile() {}

	virtual bool read(std::istream &stream);
	bool readSection(std::istream &stream);
};
