
#include "vizFile.hpp"
#include "io.hpp"

bool VizFile::read(std::istream &stream) {
	//Header: version
	U32 version;
	READCHECK(version, U32);
	if (version != 0) {
		return false;
	}

	//16-byte align
	U32 dummy;
	READCHECK(dummy, U32);
	READCHECK(dummy, U32);

	READCHECK(segments, std::vector<Segment>);
	return true;
}

bool VizFile::write(std::ostream &stream) const {
	//Header: version
	U32 version = 0;
	WRITECHECK(version, U32);

	//16-byte align
	U32 dummy = 0;
	WRITECHECK(dummy, U32);
	WRITECHECK(dummy, U32);

	WRITECHECK(segments, std::vector<Segment>);
	return true;
}


bool VizFile::Segment::read(std::istream &stream) {
	READCHECK(startAddress, Address);
	READCHECK(size, Address);

	//16-byte align
	U32 dummy;
	READCHECK(dummy, U32);

	READCHECK(bytes, std::vector<ByteData>);
	return true;
}

bool VizFile::Segment::write(std::ostream &stream) const {
	WRITECHECK(startAddress, Address);
	WRITECHECK(size, Address);

	//16-byte align
	U32 dummy = 0;
	WRITECHECK(dummy, U32);

	WRITECHECK(bytes, std::vector<ByteData>);
	return true;
}


bool VizFile::ByteData::read(std::istream &stream) {
	READCHECK(numHits, U32);
	READCHECK(numMisses, U32);
	return true;
}

bool VizFile::ByteData::write(std::ostream &stream) const {
	WRITECHECK(numHits, U32);
	WRITECHECK(numMisses, U32);
	return true;
}

