#pragma once

#include <unordered_map>

#define TRANSLATE(STR) ts->Translate(STR)
#define C_TRANSLATE(STR) ts->Translate(STR).c_str()

class Translator {
private:
    std::unordered_map<std::string, std::string> map;

public:
    
    void Load();

    std::string Translate(const std::string& _msg) {
        if (map[_msg].length() == 0) {
            return _msg;
        }

        return map[_msg];
    }

public:
    static Translator* GetSingleton() {
        static Translator singleton;
        return std::addressof(singleton);
    }
private:
    Translator() {}
    Translator(const Translator&) = delete;
    Translator(Translator&&) = delete;

    ~Translator() = default;

    Translator& operator=(const Translator&) = delete;
    Translator& operator=(Translator&&) = delete;
};