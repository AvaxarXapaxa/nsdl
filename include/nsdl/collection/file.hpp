#pragma once

#include <nsdl/collection/includes.hpp>
#include <nsdl/collection/misc.hpp>


namespace nk {
	// Writes a binary string to a file, returns if it went successful
	inline bool writeToFile(const std::wstring file_name, const std::vector<U8>& data) {
		#ifdef _WIN32
		std::ofstream fout{ file_name, std::ios::out | std::ios::binary };
		#else
		std::ofstream fout{ nk::wstringToUtf8(file_name), std::ios::out | std::ios::binary };
		#endif
		if (fout.is_open()) {
			fout.write((char*)(&data[0]), data.size());
			fout.close();
			return fout.good();
		}
		return false;
	}

	// Reads the binary content of a file and returns it, it'll return an empty vector if it didn't went well 
	inline std::vector<U8> readFromFile(const std::wstring file_name, bool* success = nullptr) {
		#ifdef _WIN32
		std::ifstream fin{ file_name, std::ios::in | std::ios::binary };
		#else
		std::ifstream fin{ nk::wstringToUtf8(file_name), std::ios::in | std::ios::binary };
		#endif
		std::vector<U8> content;
		if (fin.is_open()) {
			U32 it = 0;
			while (1) {
				char byte;
				fin.read(&byte, 1);
				if (fin.eof()) break;
				content.push_back(byte);
				it++;
			}
			if (success != nullptr)
				*success = true;
			return content;
		}
		if (success != nullptr)
			*success = false;
		return content;
	}

	// Makes a vector of bytes from a byte pointer
	inline std::vector<U8> createByteVectorFromPointer(void* data, size_t size) {
		std::vector<U8> vec;
		for (size_t i = 0; i < size; i++)
			vec.push_back(((U8*)data)[i]);
		return vec;
	}
}
