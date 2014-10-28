#include "ReadsFilter.hh"
#include "KmerScanner.hh"


void ReadsFilter::index_kmers()
{
	for (size_t i = 0; i < jellyfish_db_.entries_ct(); i++)
	{
		uint64_t kmer = jellyfish_db_.get_kmer(i);
		kmers_index_.insert(kmer);
	}
}

void ReadsFilter::filter_reads(std::vector<Read>& reads)
{
	std::vector<uint32_t> keep_reads(reads.size());

	KmerScanner kmer_scanner(kmer_len_);

	#pragma omp parallel
	{
		uint64_t kmer;
		KmerScanner kmer_scanner(kmer_len_);

		#pragma omp for
		for (size_t i = 0; i < reads.size(); i++)
		{
			const Read& r = reads[i];
			kmer_scanner.set_curr_seq(r.seq);

			uint64_t num_kmers_in_index = 0;
			while(kmer_scanner.next_kmer(kmer))
			{
				if (kmer_scanner.ambig_kmer())
					break;

				if (kmers_index_.find(kmer) != kmers_index_.end())
				{
					num_kmers_in_index++;

					if (num_kmers_in_index >= keep_read_threshold_)
						break;
				}
			}

			keep_reads[i] =  (num_kmers_in_index >= keep_read_threshold_);
		}
	}

	size_t num_reads_kept = 0;
	for (size_t i = 0; i < reads.size(); i++)
	{
		if (keep_reads[i])
			reads[num_reads_kept++] = reads[i];
	}
	reads.resize(num_reads_kept);
}
