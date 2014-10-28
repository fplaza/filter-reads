#ifndef READS_FILTER_HH
#define READS_FILTER_HH

#include <string>
#include <vector>
#include <sparsehash/sparse_hash_set>
#include "jellyfish_db.hh"
#include "Read.hh"

class ReadsFilter
{
	private:
		const JellyfishDB jellyfish_db_;
		const size_t kmer_len_;
		google::sparse_hash_set<uint64_t> kmers_index_;
		const uint64_t keep_read_threshold_;
	public:
		ReadsFilter(const std::string& jellyfish_db_file, const uint64_t keep_read_threshold);
		size_t kmer_len() const;
		void index_kmers();
		void filter_reads(std::vector<Read>& reads);
};

inline ReadsFilter::ReadsFilter(const std::string& jellyfish_db_file, const uint64_t keep_read_threshold)
	: jellyfish_db_(jellyfish_db_file), kmer_len_(jellyfish_db_.kmer_len()), kmers_index_(jellyfish_db_.entries_ct()), keep_read_threshold_(keep_read_threshold)
{};

inline size_t ReadsFilter::kmer_len() const
{
	return kmer_len_;
}

#endif // READS_FILTER_HH
