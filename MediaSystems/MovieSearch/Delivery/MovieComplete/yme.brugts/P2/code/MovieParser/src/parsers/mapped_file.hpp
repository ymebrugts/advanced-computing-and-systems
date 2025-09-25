#pragma once

#include <string>
#include <stdexcept>
#include <cstddef>

#if defined(_WIN32)
#  include <windows.h>
#else
#  include <sys/mman.h>
#  include <sys/stat.h>
#  include <fcntl.h>
#  include <unistd.h>
#endif

class MappedFile {
public:
    MappedFile(const std::string& filename) {
#if defined(_WIN32)
        // Open file
        file_ = CreateFileA(filename.c_str(), GENERIC_READ, FILE_SHARE_READ,
            nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
        if (file_ == INVALID_HANDLE_VALUE) {
            throw std::runtime_error("Failed to open file: " + filename);
        }

        // Get file size
        LARGE_INTEGER size;
        if (!GetFileSizeEx(file_, &size)) {
            CloseHandle(file_);
            throw std::runtime_error("Failed to get file size: " + filename);
        }
        size_ = static_cast<size_t>(size.QuadPart);

        // Create file mapping
        mapping_ = CreateFileMapping(file_, nullptr, PAGE_READONLY, 0, 0, nullptr);
        if (!mapping_) {
            CloseHandle(file_);
            throw std::runtime_error("Failed to create file mapping: " + filename);
        }

        // Map view
        data_ = static_cast<const char*>(
            MapViewOfFile(mapping_, FILE_MAP_READ, 0, 0, 0)
            );
        if (!data_) {
            CloseHandle(mapping_);
            CloseHandle(file_);
            throw std::runtime_error("Failed to map view of file: " + filename);
        }
#else
        // Open file
        fd_ = open(filename.c_str(), O_RDONLY);
        if (fd_ < 0) {
            throw std::runtime_error("Failed to open file: " + filename);
        }

        // Get file size
        struct stat sb;
        if (fstat(fd_, &sb) < 0) {
            close(fd_);
            throw std::runtime_error("Failed to stat file: " + filename);
        }
        size_ = static_cast<size_t>(sb.st_size);

        // Map file
        data_ = static_cast<const char*>(mmap(nullptr, size_, PROT_READ, MAP_PRIVATE, fd_, 0));
        if (data_ == MAP_FAILED) {
            close(fd_);
            throw std::runtime_error("Failed to mmap file: " + filename);
        }
#endif
    }

    ~MappedFile() {
#if defined(_WIN32)
        if (data_) UnmapViewOfFile(data_);
        if (mapping_) CloseHandle(mapping_);
        if (file_ != INVALID_HANDLE_VALUE) CloseHandle(file_);
#else
        if (data_) munmap((void*)data_, size_);
        if (fd_ >= 0) close(fd_);
#endif
    }

    const char* data() const { return data_; }
    size_t size() const { return size_; }
    bool empty() const { return size_ == 0; }

private:
#if defined(_WIN32)
    HANDLE file_ = INVALID_HANDLE_VALUE;
    HANDLE mapping_ = nullptr;
#else
    int fd_ = -1;
#endif
    const char* data_ = nullptr;
    size_t size_ = 0;
};
