#include "SequenceFileParser.hh"

SequenceFileParser::SequenceFileParser(const std::string& filepath)
	: filepath_(filepath)
{
	fp = gzopen(filepath.c_str(), "r");
	kseq = kseq_init(fp);
}


SequenceFileParser::~SequenceFileParser()
{
	kseq_destroy(kseq);
	gzclose(fp);
}

bool SequenceFileParser::next_read(Read& r)
{
    if (kseq_read(kseq) >= 0)
	{
		r.name.assign(kseq->name.s, kseq->name.l);
		r.seq.assign(kseq->seq.s, kseq->seq.l);

		return true;
	}
	else
	{
		return false;
	}

}

bool SequenceFileParser::next_chunk(std::vector<Read>& reads, size_t chunk_size)
{
	reads.resize(chunk_size);
	Read r;
	size_t num_reads = 0;

	while (num_reads < chunk_size && next_read(r))
	{
		reads[num_reads++] = r;
	}

	reads.resize(num_reads);

	return !reads.empty();

}
