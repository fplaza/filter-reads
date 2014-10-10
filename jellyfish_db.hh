#ifndef JELLYFISH_DB_HH
#define JELLYFISH_DB_HH

#include <string>
#include <boost/iostreams/device/mapped_file.hpp>
#include <cstring>

class JellyfishDB
{
	private:
		static const char JELLYFISH_DB_FILETYPE[]; 
		static const uint64_t JELLYFISH_DB_FILETYPE_SZ;

		const std::string path_;
		boost::iostreams::mapped_file_source mapped_file_;
		uint64_t kmer_bits_;
		uint64_t kmer_occs_sz_;
		uint64_t entries_ct_;
		uint8_t kmer_len_;
		uint64_t kmer_sz_;
		uint64_t entry_sz_;
		const char* entries_ptr_;

		uint64_t header_sz() const;

	public:
		JellyfishDB(const std::string& path);
		~JellyfishDB();
		const std::string& path() const;
		uint8_t kmer_len() const;
		uint64_t entries_ct() const;
		uint64_t get_kmer(uint64_t i) const;
};

inline JellyfishDB::~JellyfishDB()
{
	mapped_file_.close();
}

inline const std::string& JellyfishDB::path() const
{
	return path_;
}

inline uint64_t JellyfishDB::entries_ct() const
{
	return entries_ct_;
}

inline uint8_t JellyfishDB::kmer_len() const
{
	return kmer_len_;
}

inline uint64_t JellyfishDB::header_sz() const
{
	return 72 + 2 * (4 + 8 * kmer_bits_);
}

inline uint64_t JellyfishDB::get_kmer(uint64_t i) const
{
	uint64_t kmer = 0;
	std::memcpy(&kmer, entries_ptr_+entry_sz_*i, kmer_sz_);
	return kmer;
}

#endif // JELLYFISH_DB_HH


