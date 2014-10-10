#ifndef KMER_SCANNER_HH
#define KMER_SCANNER_HH

class KmerScanner
{
	public:
		KmerScanner(uint8_t kmer_len);
		uint8_t kmer_len() const;
		void set_curr_seq(const std::string& seq);
		bool ambig_kmer();
		bool next_kmer(uint64_t& kmer);

	private:
		const uint8_t kmer_len_;
		const uint64_t kmer_mask_;
		const uint32_t ambig_mask_;
		const uint64_t rkmer_shift_;
		std::string curr_seq_;
		size_t curr_pos_;
		uint64_t curr_kmer_;
		uint64_t curr_rkmer_;
		uint32_t curr_ambig_;
		size_t loaded_nt_;
};

inline KmerScanner::KmerScanner(uint8_t kmer_len)
	: kmer_len_(kmer_len),
	kmer_mask_((~0ULL) >> (sizeof(kmer_mask_)*8 - kmer_len_*2)),
	ambig_mask_((~0) >> (sizeof(ambig_mask_)*8 - kmer_len)),
	rkmer_shift_(2*(kmer_len_-1))
{

	set_curr_seq("");
}

inline uint8_t KmerScanner::kmer_len() const
{
	return kmer_len_;
}

inline void KmerScanner::set_curr_seq(const std::string& seq)
{
	curr_seq_ = seq;
	curr_pos_ = 0;
	curr_kmer_ = 0;
	curr_rkmer_ = 0;
	curr_ambig_ = 0;
	loaded_nt_ = 0;
}

inline bool KmerScanner::ambig_kmer()
{
	return curr_ambig_ != 0;
}

inline bool KmerScanner::next_kmer(uint64_t& kmer)
{
	if (curr_pos_ >= curr_seq_.size())
		return false;

	if (loaded_nt_)
		loaded_nt_--;

	while (loaded_nt_ < kmer_len_) {
		loaded_nt_++;

		curr_kmer_ <<= 2;
		curr_rkmer_ >>= 2;
		curr_ambig_ <<= 1;

		switch (curr_seq_[curr_pos_++]) {
			case 'A': case 'a':
				curr_rkmer_ |= (3ull << rkmer_shift_);
				break;
			case 'C': case 'c':
				curr_kmer_ |= 1;
				curr_rkmer_ |= (2ull << rkmer_shift_);
				break;
			case 'G': case 'g':
				curr_kmer_ |= 2;
				curr_rkmer_ |= (1ull << rkmer_shift_);
				break;
			case 'T': case 't':
				curr_kmer_ |= 3;
				curr_rkmer_ |= (0ull << rkmer_shift_);
				break;
			default:
				curr_ambig_ |= 1;
				break;
		}

		curr_kmer_ &= kmer_mask_;
		curr_ambig_ &= ambig_mask_;
	}

	kmer = std::min(curr_kmer_, curr_rkmer_);

	return true;
}

#endif
