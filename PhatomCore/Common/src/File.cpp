#include "File.h"
namespace phatom {
File::File() : 
mFilePtr(nullptr), 
mFileSize(0), 
mBadFlag(false) {}

File::~File() {
    Close();
}

bool File::OpenFile(const char* filePath, const char* mode) {
    if(mFilePtr != nullptr) {
        Close();
    }
    mFilePtr = fopen(filePath, mode);
    if(mFilePtr == nullptr) {
        return false;
    }
    ClearBadFlag();
    Seek(0, SeekDir::End);
    mFileSize = Tell();
    Seek(0, SeekDir::Begin);
    if(IsBad()) {
        Close();
        return false;
    }
    return true;
}

bool File::OpenBinary(const char* filePath) {
    return OpenFile(filePath, "rb");
}

bool File::OpenText(const char* filePath) {
    return OpenFile(filePath, "r");
}

bool File::CreateBinary(const char* filePath) {
    return OpenFile(filePath, "wb");
}

bool File::CreateText(const char* filePath) {
    return OpenFile(filePath, "w");
}

bool File::OpenBinary(const std::string& filePath) {
    return OpenBinary(filePath.c_str());
}

bool File::OpenText(const std::string& filePath) {
    return OpenText(filePath.c_str());
}

bool File::CreateBinary(const std::string& filePath) {
    return CreateBinary(filePath.c_str());
}

bool File::CreateText(const std::string& filePath) {
    return CreateText(filePath.c_str());
}

void File::Close() {
    if(mFilePtr != nullptr) {
        fclose(mFilePtr);
        mFilePtr = nullptr;
        mFileSize = 0;
        mBadFlag = false;
    }
}

bool File::IsOpen() {
    return mFilePtr != nullptr;
}

int64_t File::GetSize() const {
    return mFileSize;
}

bool File::IsBad() const {
    return mBadFlag;
}

void File::ClearBadFlag() {
    mBadFlag = false;
}

bool File::IsEOF() {
    return feof(mFilePtr) != 0;
}

FILE* File::GetFilePointer() const {
    return mFilePtr;
}

bool File::ReadAll(std::vector<char>* buffer) {
    if(buffer == nullptr) {
        return false;
    }
    buffer->resize(mFileSize);
    Seek(0, SeekDir::Begin);
    if(!Read((*buffer).data(), mFileSize)) {
        return false;
    }
    return true;
}

bool File::ReadAll(std::vector<int8_t>* buffer) {
    if(buffer == nullptr) {
        return false;
    }
    buffer->resize(mFileSize);
    Seek(0, SeekDir::Begin);
    if(!Read((*buffer).data(), mFileSize)) {
        return false;
    }
    return true;
}

bool File::ReadAll(std::vector<uint8_t>* buffer) {
    if(buffer == nullptr) {
        return false;
    }
    buffer->resize(mFileSize);
    Seek(0, SeekDir::Begin);
    if(!Read((*buffer).data(), mFileSize)) {
        return false;
    }
    return true;
}

bool File::Seek(int64_t offset, SeekDir origin) {
    if(mFilePtr == nullptr) {
        return false;
    }
    int cOrigin = 0;
    switch (origin) {
        case SeekDir::Begin: {
            cOrigin = SEEK_SET;
            break;
        }
        case SeekDir::Current: {
            cOrigin = SEEK_CUR;
            break;
        }
        case SeekDir::End: {
            cOrigin = SEEK_END;
            break;
        }
        default:
            return false;
    }
    if(fseek(mFilePtr, offset, cOrigin) != 0) {
        mBadFlag = true;
        return false;
    }
    return true;
}

int64_t File::Tell() {
    if(mFilePtr == nullptr) {
        return -1;
    }
    return (int64_t)ftell(mFilePtr);
}
} // namespace phatom
