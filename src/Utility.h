#pragma once

namespace Utility {
    const std::string delimiter{";##;"};

    std::vector<std::string> Split(const std::string& _str, const std::string& _delimiter) {
        std::vector<std::string> result;
        std::string str = _str;

        while (str.length() != 0) {
            int index = str.find(_delimiter);
            if (index == std::string::npos) {
                result.push_back(str);
                break;
            }

            std::string found = str.substr(0, index);
            str.erase(0, index + _delimiter.length());

            result.push_back(found);
        }

        return result;
    }

    bool to_bool(const std::string& _str) {
        return _str == "1";
    }
}