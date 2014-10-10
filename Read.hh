#ifndef READ_HH
#define READ_HH

#include <string>
#include <iostream>

struct Read
{
	std::string name;
	std::string seq;
};

std::ostream& operator<<(std::ostream& os, const Read& r);

#endif // READ_HH
