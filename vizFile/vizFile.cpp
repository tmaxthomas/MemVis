
#include "vizFile.hpp"
#include "io.hpp"

bool VizFile::read(std::istream &stream) {
	//Header: version
    char header[4];
    if (!IO::read<char, 4>(stream, header, "header")) return false;
    if (strncmp(header, "vzfh", 4) != 0) return false;

    U32 version;
    U32 numReadWrite;
    U32 numExec;
    READCHECK(version, U32);
    READCHECK(numReadWrite, U32);
    READCHECK(numExec, U32);

    {
        char section_header[16];
        if (!IO::read<char, 16>(stream, section_header, "section header")) return false;
        if (strncmp(section_header, "read-write", 16) != 0) return false;

        for (U32 i = 0; i < numReadWrite; i ++) {
            Address address;
            U32 reads;
            U32 writes;

            READCHECK(address, Address);
            READCHECK(reads, U32);
            READCHECK(writes, U32);

            bytes[address].numReads = reads;
            bytes[address].numWrites = writes;
        }
    }
    {
        char section_header[16];
        if (!IO::read<char, 16>(stream, section_header, "section header")) return false;
        if (strncmp(section_header, "exec", 16) != 0) return false;

        for (U32 i = 0; i < numExec; i ++) {
            Address address;
            U64 execs;

            READCHECK(address, Address);
            READCHECK(execs, U64);

            bytes[address].numExecutes = execs;
         }
    }

	return true;
}
