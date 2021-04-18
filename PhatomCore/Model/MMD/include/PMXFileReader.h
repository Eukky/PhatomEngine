#ifndef PHATOM_PMXFILEREADER_H
#define PHATOM_PMXFILEREADER_H

#include "PMXFile.h"

namespace phatom {
class PMXFileReader {
public:
    PMXFileReader();
    ~PMXFileReader();

    PMXFile* getFile();
    bool ReadPMXFile(const char* fileName);
private:


private:
    PMXFile* mFile;

};
} // namespace phatom


#endif //PHATOM_PMXFILEREADER_H