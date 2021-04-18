#include "PMXFile.h"

#include <vector>

namespace phatom {
bool ReadString(PMXFile* pmxFile, std::string* val, File& file) {
    uint32_t bufSize;
    if(!file.Read(&bufSize)) {
        return false;
    }

    if(bufSize > 0) {
        if(pmxFile->mHeader.mEncode == 0) {

        }
    }
}

bool ReadPMXFile(PMXFile* pmxFile, File& file) {

}

bool ReadPMXFile(PMXFile* pmxFile, const char* fileName) {
    File file;
    if(!file.OpenBinary(fileName)) {
        return false;
    }

    if(ReadPMXFile(pmxFile, file)) {
        return false;
    }

    return true;
}
} // namespace phatom
