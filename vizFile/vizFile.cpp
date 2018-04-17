
#include "vizFile.hpp"
#include "io.hpp"

bool VizFile::read(std::istream &stream) {
	segments.clear();
	bytes.clear();

	//Header: version
	while (!stream.eof()) {
		char header[4];
		if (!IO::read<char, 4>(stream, header, "header")) return false;
		if (strncmp(header, "vzfh", 4) != 0) return false;

		U32 version;
		READCHECK(version, U32);

		if (version != 1) return false;

		char section_header[17]{};
		if (!IO::read<char, 16>(stream, section_header, "section header")) return false;
		//Which fn is this
		if (std::string(section_header) == "read-write") {
			if (!readSection(stream)) return false;
		} else if (std::string(section_header) == "exec") {
			if (!readSection(stream)) return false;
			break;
		} else {
			return false;
		}
	}
	
	//Generate segments
	Address epsilon = 0x1000;
	
	Segment *current = NULL;
	Address last = 0x0;
	for (const auto &pair : bytes) {
		Address next = pair.first;
		if (next - last > epsilon) {
			//New segment
			current = NULL;
		}
		if (current == NULL) {
			segments.push_back(Segment());
			current = &(segments.back());
			current->startAddress = next - (next % epsilon);
			current->size = 0;
		}
		
		last = next;
		
		current->addByte(next, pair.second);
	}

	std::sort(segments.begin(), segments.end());

	return true;
}

bool VizFile::readSection(std::istream &stream) {
	U64 count;
	READCHECK(count, U64);

	for (U64 i = 0; i < count; i ++) {
		Address address;

		U32 numReads;
		U32 numWrites;
		U64 numExecutes;
		U32 numHits;
		U32 numMisses;

		READCHECK(address, Address);
		READCHECK(numReads, U32);
		READCHECK(numWrites, U32);
		READCHECK(numExecutes, U64);
		READCHECK(numHits, U32);
		READCHECK(numMisses, U32);

		bytes[address].numReads += numReads;
		bytes[address].numWrites += numWrites;
		bytes[address].numExecutes += numExecutes;
		bytes[address].numHits += numHits;
		bytes[address].numMisses += numMisses;
	}
	return true;
}

void VizFile::Segment::addByte(Address address, const ByteData &byte) {
	//Expand to fill for this address
	assert(address >= startAddress);
	assert(address >= startAddress + size);
	
	int offset = address - startAddress;
	bytes.resize(offset + 1);
	bytes[offset] = byte;
	
	size = offset;
}
