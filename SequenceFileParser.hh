#ifndef SEQUENCE_FILE_PARSER_HH
#define SEQUENCE_FILE_PARSER_HH

#include <string>
#include <vector>
#include <zlib.h>
#include "kseq.h"
#include "Read.hh"


class SequenceFileParser
{
	private:
		const std::string filepath_;
		gzFile fp;
		kseq_t *kseq;
	public:
		SequenceFileParser(const std::string& filepath);
		~SequenceFileParser();
		bool next_read(Read& r);
		bool next_chunk(std::vector<Read>& reads, size_t chunk_size);

};

#endif // SEQUENCE_FILE_PARSER_HH
