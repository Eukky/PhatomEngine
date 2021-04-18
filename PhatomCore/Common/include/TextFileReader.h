#ifndef PHATOM_TEXTFILEREADER_H
#define PHATOM_TEXTFILEREADER_H

#include "File.h"

namespace phatom {
class TextFileReader {
public:
    TextFileReader() = default;
    explicit TextFileReader(const char* filePath);
    explicit TextFileReader(const std::string& filePath);

    bool Open(const char* filePath);
    bool Open(const std::string& filePath);
    void Close();
    bool IsOpen();

    std::string ReadLine();
    void ReadAllLines(std::vector<std::string>& lines);
    std::string ReadAll();
    bool IsEOF();
private:
    File mFile;
};  
} // namespace phatom


#endif //PHATOM_TEXTFILEREADER_H