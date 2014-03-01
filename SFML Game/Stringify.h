#pragma once

#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>

#define STRINGIFY_PRECISION 3

template<typename Type> inline std::string Stringify(Type const& x)
{
	std::ostringstream out;
	out << x;
	return out.str();
}

template<> inline std::string Stringify<bool>(bool const& x)
{
	std::ostringstream out;
	out << std::boolalpha << x;
	return out.str();
}

template<> inline std::string Stringify<double>(double const& x)
{
	std::ostringstream out;
	out << std::setprecision(STRINGIFY_PRECISION) << x;
	return out.str();
}

template<> inline std::string Stringify<float>(float const& x)
{
	std::ostringstream out;
	out << std::setprecision(STRINGIFY_PRECISION) << x;
	return out.str();
}
