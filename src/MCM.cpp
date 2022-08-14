#include "MCM.h"

using namespace MCM;

Setting& Setting::GetSingleton() noexcept {
    static Setting instance;
    return instance;
}