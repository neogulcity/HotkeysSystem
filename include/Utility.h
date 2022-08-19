#pragma once

inline bool to_bool(std::string _string)
{
	return _string == "TRUE" || _string == "True";
}