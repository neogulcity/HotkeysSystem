#pragma once

inline bool to_bool(std::string _string)
{
	return _string == "TRUE" || _string == "True" || _string == "1";
}

std::string GetStringFromKeycode(int32_t _key);