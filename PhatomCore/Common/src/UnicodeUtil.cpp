#include "UnicodeUtil.h"

#include <stdexcept>

namespace phatom {
    std::wstring ToWString(const std::string& utf8Str) {
        std::wstring wStr;
        if(!TryToWString(utf8Str, wStr)) {
            throw std::invalid_argument("Faild to convert wstring.");
        }
        return wStr;
    }
    
} // namespace phatom
