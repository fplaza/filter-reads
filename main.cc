#include <iostream>
#include <new>
#include <cstdlib>
#include <fstream>
#include "TimeProfiler.hh"
#include "Parameters.hh"
#include "SequenceFileParser.hh"
#include "ReadsFilter.hh"

void no_more_memory()
{
	std::cerr << "error: memory allocation failed." << std::endl;
	std::exit (1);
}

int main(int argc, char *argv[])
{
	std::set_new_handler(no_more_memory);

	try
	{
		TimeProfiler time_profiler;
		time_profiler.start_new_timer("Total");

		const Parameters& pars = Parameters::parse(argc, argv);

		ReadsFilter reads_filter(pars.jellyfish_db_file, pars.keep_read_threshold);
		std::cout << "Indexing all " << reads_filter.kmer_len() << "-mers..." << std::endl;
		time_profiler.start_new_timer("Indexing k-mers");
		reads_filter.index_kmers();
		time_profiler.stop_last_timer();
		std::cout << "Done.\n"  << std::endl;
		
		std::ofstream ofs;
		ofs.open(pars.output_file.c_str());		

		std::cout << "Filtering reads..." << std::endl;
		time_profiler.start_new_timer("Filtering reads");
		size_t num_reads_kept = 0;
		size_t num_reads = 0;

		std::vector<Read> reads_chunk;
		size_t chunk_size = 2000000;
		SequenceFileParser file_parser(pars.input_file);

		while(file_parser.next_chunk(reads_chunk, chunk_size))
		{
			num_reads += reads_chunk.size();
			reads_filter.filter_reads(reads_chunk);
			num_reads_kept += reads_chunk.size();

			for (size_t curr_read = 0; curr_read < reads_chunk.size(); curr_read++)
				ofs << reads_chunk[curr_read].name << '\n';
		}
		time_profiler.stop_last_timer();
		std::cout << "Done. " <<  num_reads << " reads processed, " << num_reads_kept << " kept. " << std::endl;

		ofs.close();

		time_profiler.stop_last_timer();
		std::cout << time_profiler << std::endl;
		
		std::exit(0);
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		std::exit(-1);
	}
}
