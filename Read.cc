#include "Read.hh"

std::ostream& operator<<(std::ostream& os, const Read& r)
{
	os << '>' << r.name << '\n' << r.seq;

	return os;
}
