#ifndef PHATOM_FILE_H
#define PHATOM_FILE_H

#include <vector>
#include <string>
#include <cstdio>

namespace phatom {
class File {
public:
    File();
    ~File();

    File(const File&) = delete;
    File& operator = (const File&) = delete;

    bool OpenBinary(const char* filePath);
    bool OpenText(const char* filePath);
    bool CreateBinary(const char* filePath);
    bool CreateText(const char* filePath);

    bool OpenBinary(const std::string& filePath);
    bool OpenText(const std::string& filePath);
    bool CreateBinary(const std::string& filePath);
    bool CreateText(const std::string& filePaht);

    void Close();
    bool IsOpen();
    int64_t GetSize() const;
    bool IsBad() const;
    void ClearBadFlag();
    bool IsEOF();

    FILE* GetFilePointer() const;

    bool ReadAll(std::vector<char>* buffer);
    bool ReadAll(std::vector<uint8_t>* buffer);
    bool ReadAll(std::vector<int8_t>* buffer);

    enum class SeekDir {
        Begin,
        Current,
        End,
    };
    bool Seek(int64_t offset, SeekDir origin);
    int64_t Tell();

    template <typename T>
    bool Read(T* buffer, size_t count = 1) {
        if (buffer == nullptr) {
            return false;
        }
        if (!IsOpen()){
            return false;
        }
        if (fread(buffer, sizeof(T), count, mFilePtr) != count){
            mBadFlag = true;
            return false;
        }
        return true;
    }

    template <typename T> 
    bool Write(T* buffer, size_t count = 1) {
        if (buffer == nullptr) {
            return false;
        }
        if (!IsOpen()){
            return false;
        }
        if (fwrite(buffer, sizeof(T), count, mFilePtr) != count) {
            mBadFlag = true;
            return false;
        }
        return true;
    }

private:
    bool OpenFile(const char* filePath, const char* mode);

private:
    FILE* mFilePtr;
    int64_t mFileSize;
    bool mBadFlag;
    

};
} // namespace phatom


#endif //PHATOM_FILE_H