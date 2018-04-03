
#include "vizFile.hpp"
#include "io.hpp"

bool VizFile::read(std::istream &stream) {
	//Header: version
	U8 version;
	READCHECK(version, U8);
	if (version != 0) {
		return false;
	}

	READCHECK(segments, std::vector<Segment>);
	return true;
}

bool VizFile::write(std::ostream &stream) const {
	//Header: version
	U8 version = 0;
	WRITECHECK(version, U8);

	WRITECHECK(segments, std::vector<Segment>);
	return true;
}


bool VizFile::Segment::read(std::istream &stream) {
	READCHECK(startAddress, Address);
	READCHECK(size, Address);

	READCHECK(bytes, std::vector<ByteData>);
	return true;
}

bool VizFile::Segment::write(std::ostream &stream) const {
	WRITECHECK(startAddress, Address);
	WRITECHECK(size, Address);

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

