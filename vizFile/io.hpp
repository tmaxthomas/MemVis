//------------------------------------------------------------------------------
// Copyright (c) 2015 Glenn Smith
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//  * Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//  * Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//  * Neither the name of the project nor the
//    names of its contributors may be used to endorse or promote products
//    derived from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//------------------------------------------------------------------------------

#ifndef io_hpp
#define io_hpp

#include "types.hpp"
#include <type_traits>
#include <map>
#include <iostream>
#include <functional>
#include <assert.h>

//Define this to activate stream debug read/write logging
#if 0
#define PRINT_DEBUG_INFO
#endif

class IO {
public:
#ifdef PRINT_DEBUG_INFO
	//Print out information about the data being read / written to streams.
	// READCHECK and WRITECHECK will automatically append the type to name, normal
	// raw IO::read/write methods won't.
	template <typename T>
	static inline void debug_print(std::istream &stream, T &value, const std::string &name) {
		std::istream::pos_type pos = stream.tellg();
		//Basic information about the data's name / offset
		printf("Read %s at offset %lu (0x%lX)\n", name.c_str(), size_t(pos), size_t(pos));
	}
	template <typename T>
	static inline void debug_print(std::ostream &stream, const T &value, const std::string &name) {
		std::istream::pos_type pos = stream.tellp();
		//Basic information about the data's name / offset
		printf("Write %s at offset %lu (0x%lX)\n", name.c_str(), size_t(pos), size_t(pos));
	}
	template <typename T>
	static inline void debug_print_value(std::istream &stream, T &value, const std::string &name) {
		std::istream::pos_type pos = stream.tellg();
		//Basic information about the data's name / offset
		printf("Read %s at offset %lu (0x%lX): 0x%X / %u\n", name.c_str(), size_t(pos) - sizeof(T), size_t(pos) - sizeof(T), value, value);
	}
	template <typename T>
	static inline void debug_print_value(std::ostream &stream, const T &value, const std::string &name) {
		std::istream::pos_type pos = stream.tellp();
		//Basic information about the data's name / offset
		printf("Write %s at offset %lu (0x%lX): 0x%X / %u\n", name.c_str(), size_t(pos), size_t(pos), value, value);
	}
	static inline void debug_print_value(std::istream &stream, F32 &value, const std::string &name) {
		std::istream::pos_type pos = stream.tellg();
		//Basic information about the data's name / offset
		printf("Read %s at offset %lu (0x%lX): 0x%08X / %f\n", name.c_str(), size_t(pos) - sizeof(F32), size_t(pos) - sizeof(F32), value, value);
	}
	static inline void debug_print_value(std::ostream &stream, const F32 &value, const std::string &name) {
		std::istream::pos_type pos = stream.tellp();
		//Basic information about the data's name / offset
		printf("Write %s at offset %lu (0x%lX): 0x%08X / %f\n", name.c_str(), size_t(pos), size_t(pos), value, value);
	}

	//Print out information about the data being read / written to streams.
	// READCHECK and WRITECHECK will automatically append the type to name, normal
	// raw IO::read/write methods won't.
	template <typename T>
	static inline void debug_error(std::istream &stream, T &value, const std::string &name) {
		std::istream::pos_type pos = stream.tellg();
		//Basic information about the data's name / offset
		std::cout << "[Error] Read " << name << " at offset " << pos << " (0x" << std::hex << pos << ")" << std::dec << std::endl;
	}
	template <typename T>
	static inline void debug_error(std::ostream &stream, const T &value, const std::string &name) {
		std::istream::pos_type pos = stream.tellp();
		//Basic information about the data's name / offset
		std::cout << "[Error] Write " << name << " at offset " << pos << " (0x" << std::hex << pos << ")" << std::dec << std::endl;
	}
#else
	//Use a macro here so that absolutely no code is generated.
#define debug_print(stream, value, name)
#define debug_print_value(stream, value, name)
	//Use a macro here so that absolutely no code is generated.
#define debug_error(stream, value, name)
#endif

	//Read primitive types from a std::istream
	template <typename T, bool=true>
	struct read_impl {
		static inline bool read(std::istream &stream, T &value, const std::string &name) {
			if (stream.eof()) {
				debug_error(stream, value, name);
				return false;
			}
			stream.read(reinterpret_cast<char *>(&value), sizeof(value));
			debug_print_value(stream, value, name);
			bool success = stream.good();
			if (!success) {
				debug_error(stream, value, name);
			}
			return success;
		}
	};
	//Read structures from a std::istream
	template <typename T>
	struct read_impl<T, false> {
		static inline bool read(std::istream &stream, T &value, const std::string &name) {
			debug_print(stream, value, name);
			value.fileOffset = stream.tellg();
			bool success = (value.read(stream) && stream.good());
			if (!success) {
				debug_error(stream, value, name);
			} else {
				value.fileSize = int(stream.tellg()) - value.fileOffset;
			}
			return success;
		}
	};

	/**
	 * Read types from a stream
	 * @param stream The stream from which the data is read
	 * @param value A reference into which the data will be read
	 * @param name A string containing the name of the variable (for debugging)
	 * @return If the operation was successful
	 */
	template <typename T>
	static inline bool read(std::istream &stream, T &value, const std::string &name) {
		//This will select one of the two read_impls above based on whether or not
		// T is a struct or a primitive type.
		return read_impl<T, std::is_fundamental<T>::value>::read(stream, value, name);
	}

    /**
     * Read a vector from a stream
     * @param stream The stream from which the data is read
     * @param value A reference into which the data will be read
     * @param name A string containing the name of the variable (for debugging)
     * @return If the operation was successful
     */
    template <typename T, int size>
    static inline bool read(std::istream &stream, T value[size], const std::string &name) {
        //Read all the objects
        for (U32 i = 0; i < size; i ++) {
            //Make sure the read succeeds
            if (!read(stream, value[i], name + " obj"))
                return false;
        }

        return true;
    }

	/**
	 * Read a vector from a stream
	 * @param stream The stream from which the data is read
	 * @param value A reference into which the data will be read
	 * @param name A string containing the name of the variable (for debugging)
	 * @return If the operation was successful
	 */
	template <typename T>
	static inline bool read(std::istream &stream, std::vector<T> &value, const std::string &name) {
		//Read the size of the vector
		U32 size;
		if (!read(stream, size, name + " size"))
			return false;
		//Reserve some space
		value.reserve(size);

		//Read all the objects
		for (U32 i = 0; i < size; i ++) {
			T obj;
			//Make sure the read succeeds
			if (read(stream, obj, name + " obj"))
				value.push_back(obj);
			else
				return false;
		}

		return true;
	}

	/**
	 * Read a string from a stream
	 * @param stream The stream from which the data is read
	 * @param value A reference into which the data will be read
	 * @param name A string containing the name of the variable (for debugging)
	 * @return If the operation was successful
	 */
	static inline bool read(std::istream &stream, std::string &value, const std::string &name) {
		//How long is the string
		U8 length;
		if (!read(stream, length, name + " length"))
			return false;
		//Empty the string
		value = std::string();
		//Read each byte of the string
		for (U32 i = 0; i < length; i ++) {
			//If we can read the byte, append it to the string
			U8 chr;
			if (read(stream, chr, name + " chr"))
				value += chr;
			else
				return false;
		}

		return true;
	}

	/**
	 * Read a dictionary from a stream
	 * @param stream The stream from which the data is read
	 * @param value A reference into which the data will be read
	 * @param name A string containing the name of the variable (for debugging)
	 * @return If the operation was successful
	 */
	static inline bool read(std::istream &stream, Dictionary &value, const std::string &name) {
		//How long is the map
		U32 length;
		if (!read(stream, length, name + " length"))
			return false;

		//Empty the map
		value = Dictionary();

		//Read the map strings
		for (U32 i = 0; i < length; i ++) {
			std::string key, val;

			//Make sure we can read it
			if (!read(stream, key, name + " key") ||
			    !read(stream, val, name + " val")) {
				return false;
			}

			//Insert the pair
			value.push_back(Dictionary::value_type(key, val));
		}

		return true;
	}

	/**
	 * Read a vector from a stream, but use a given method for reading instead of 
	 * the standard call to read().
	 * @param stream The stream from which the data is read
	 * @param value A reference into which the data will be read
	 * @param passed_method A function which will read the object's fields from the stream.
	 *                        Arguments are (T &object, std::istream &stream)
	 * @param name A string containing the name of the variable (for debugging)
	 * @return If the operation was successful
	 */
	template <typename T>
	static inline bool read_with(std::istream &stream, std::vector<T> &value, std::function<bool(T&, std::istream &)> passed_method, const std::string &name) {
		//Read the size of the vector
		U32 size;
		if (!read(stream, size, name + " size"))
			return false;
		//Reserve some space
		value.reserve(size);

		//Read all the objects
		for (U32 i = 0; i < size; i ++) {
			T obj;
			//Make sure the read succeeds
			if (passed_method(obj, stream))
				value.push_back(obj);
			else
				return false;
		}

		return true;
	}

	//Write primitive types to a std::ostream
	template <typename T, bool=true>
	struct write_impl {
		static inline bool write(std::ostream &stream, const T &value, const std::string &name) {
			debug_print_value(stream, value, name);
			stream.write(reinterpret_cast<const char *>(&value), sizeof(value));
			return stream.good();
		}
	};
	//Write structures to a std::ostream
	template <typename T>
	struct write_impl<T, false> {
		static inline bool write(std::ostream &stream, const T &value, const std::string &name) {
			debug_print(stream, value, name);
			return value.write(stream);
		}
	};

	/**
	 * Write types to a stream
	 * @param stream The stream to which the data is written
	 * @param value The value of the data to write
	 * @param name A string containing the name of the variable (for debugging)
	 * @return If the operation was successful
	 */
	template <typename T>
	static inline bool write(std::ostream &stream, const T &value, const std::string &name) {
		//This will select one of the two write_impls above based on whether or not
		// T is a struct or a primitive type.
		return write_impl<T, std::is_fundamental<T>::value>::write(stream, value, name);
	}

	/**
	 * Write a vector to a stream
	 * @param stream The stream to which the data is written
	 * @param value The vector to write
	 * @param name A string containing the name of the variable (for debugging)
	 * @return If the operation was successful
	 */
	template <typename T>
	static inline bool write(std::ostream &stream, const std::vector<T> &value, const std::string &name) {
		//Write the vector's size first
		if (!write(stream, static_cast<U32>(value.size()), name + " size"))
			return false;
		//Write all of the objects in the vector
		for (size_t i = 0; i < value.size(); i ++) {
			if (!write(stream, value[i], name + " value"))
				return false;
		}
		return true;
	}

	/**
	 * Write a string to a stream
	 * @param stream The stream to which the data is written
	 * @param value The string to write
	 * @param name A string containing the name of the variable (for debugging)
	 * @return If the operation was successful
	 */
	static inline bool write(std::ostream &stream, const std::string &value, const std::string &name) {
		//How long is the string
		if (!write(stream, static_cast<U8>(value.length()), name + " length"))
			return false;
		//Write each byte of the string
		for (U32 i = 0; i < value.length(); i ++) {
			if (!write(stream, value[i], name + " char"))
				return false;
		}

		return true;
	}

	/**
	 * Write a dictionary to a stream
	 * @param stream The stream to which the data is written
	 * @param value The map to write
	 * @param name A string containing the name of the variable (for debugging)
	 * @return If the operation was successful
	 */
	static inline bool write(std::ostream &stream, const Dictionary &value, const std::string &name) {
		//How long is the map
		if (!write(stream, static_cast<U32>(value.size()), name + " length"))
			return false;
		
		//Write each string in the map
		for (auto pair : value) {
			if (!write(stream, pair.first, name + " name") ||
			    !write(stream, pair.second, name + " value"))
				return false;
		}

		return true;
	}
};

//Slightly hacky read function so we don't get weird memory errors when trying to
// read a U32 into a U8. Also no reinterpret_cast because those are screwy
template <typename T, typename F>
inline bool __cast_read(std::istream &stream, F &thing, const std::string &name) {
	//Temp of the type we want
	T temp{};
	//Make sure the read actually works
	if (!IO::read(stream, temp, name))
		return false;
	//And then just copy into our variable
	thing = F(temp);
	return true;
}

//Write function in the same idea as the read function, just backwards
template <typename T, typename F>
inline bool __cast_write(std::ostream &stream, const F &thing, const std::string &name) {
	//Literally just cast and write
	T temp(thing);
	return IO::write(stream, temp, name);
}

//Macros to speed up file reading/writing
#ifdef PRINT_DEBUG_INFO
	#define READCHECK(name, type)  { if (!__cast_read<type, decltype(name)> (stream, name, #name " as " #type)) return false; }
	#define WRITECHECK(name, type) { if (!__cast_write<type, decltype(name)>(stream, name, #name " as " #type)) return false; }
#else
	#define READCHECK(name, type)  { if (!__cast_read<type, decltype(name)> (stream, name, "")) return false; }
	#define WRITECHECK(name, type) { if (!__cast_write<type, decltype(name)>(stream, name, "")) return false; }
#endif

#endif
