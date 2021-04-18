#include "TextFileReader.h"

namespace phatom {
TextFileReader::TextFileReader(const char* filePath) {
    Open(filePath);
}

TextFileReader::TextFileReader(const std::string& filePath) {
    Open(filePath);
}

bool TextFileReader::Open(const char* filePath) {
    return mFile.OpenText(filePath);
}

bool TextFileReader::Open(const std::string& filePath) {
    return Open(filePath.c_str());
}

void TextFileReader::Close() {
    mFile.Close();
}

bool TextFileReader::IsOpen() {
    return mFile.IsOpen();
}

std::string TextFileReader::ReadLine() {
    if(!IsOpen() || IsEOF()) {
        return "";
    }
    std::string line;
    auto outputIt = std::back_inserter(line);
    int ch;
    ch = fgetc(mFile.GetFilePointer());
    while(ch != EOF && ch != '\r' && ch != '\n') {
        line.push_back(ch);
        ch = fgetc(mFile.GetFilePointer());
    }
    if(ch != EOF) {
        if(ch == '\r') {
            ch = fgetc(mFile.GetFilePointer());
            if(ch != EOF && ch != '\n') {
                ungetc(ch, mFile.GetFilePointer());
            }
        } else {
            ch = fgetc(mFile.GetFilePointer());
            if(ch != EOF) {
                ungetc(ch, mFile.GetFilePointer());
            }
        }
    }
    return line;
}

void TextFileReader::ReadAllLines(std::vector<std::string>& lines) {
    lines.clear();
    if(!IsOpen() || IsEOF()) {
        return;
    }
    while(!IsEOF()) {
        lines.emplace_back(ReadLine());
    }
}

std::string TextFileReader::ReadAll() {
    std::string all;
    if(IsOpen()) {
        int ch = fgetc(mFile.GetFilePointer());
        while(ch != EOF) {
            all.push_back(ch);
            ch = fgetc(mFile.GetFilePointer());
        }
    }
    return all;
}

bool TextFileReader::IsEOF() {
    if(!IsOpen()) {
        return false;
    }
    return mFile.IsEOF();
}
    
} // namespace phatom
