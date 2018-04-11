#pragma once
#include "types.hpp"

class VizFile : public Readable {
public:
    class ByteData {
	public:
        U64 numReads;
        U64 numWrites;
        U64 numExecutes;

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
    std::map<Address, ByteData> bytes;

	VizFile() {}
	virtual ~VizFile() {}

	virtual bool read(std::istream &stream);
};
