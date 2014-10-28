#include "Parameters.hh"
#include <iostream>
#include <fstream>
#include <exception>
#include <boost/program_options.hpp>
namespace po = boost::program_options;

Parameters Parameters::parse(int argc, char* argv[])
{
	Parameters pars;

	// Create options decription
	po::options_description opts_desc("");

	opts_desc.add_options()
		("help,h", "display this help and exit.")
		("input-file,i", po::value<std::string>(&pars.input_file), "")
		("jellyfish-db,j", po::value<std::string>(&pars.jellyfish_db_file), "")
		("output-file,o", po::value<std::string>(&pars.output_file), "")
		("keep-read-threshold,t", po::value<uint64_t>(&pars.keep_read_threshold)->default_value(1), "")
		;


	// Retrieve and parse command line parameters
	po::variables_map vm;
	po::store(po::command_line_parser(argc, argv).options(opts_desc).run(), vm);

	// Print help
	if (argc == 1 || vm.count("help"))
	{
		std::cout << opts_desc << std::endl;
		std::exit(0);
	}

	po::notify(vm);

	check_file_is_readable(pars.input_file);
	check_file_is_readable(pars.jellyfish_db_file);
	check_file_is_writable(pars.output_file);

	return pars;
}

void Parameters::check_file_is_readable(const std::string& filepath)
{
	std::ifstream ifs;
	ifs.open(filepath.c_str());

	if (ifs.good())
	{
		ifs.close();
	}
	else
	{
		throw (std::invalid_argument("error: " + filepath +
					" cannot be opened. Check that the path is valid and that you have read permissions."));
	}
}

void Parameters::check_file_is_writable(const std::string& filepath)
{
	std::ofstream ofs;
	ofs.open(filepath.c_str());

	if (ofs.good())
	{
		ofs.close();
	}
	else
	{
		throw (std::invalid_argument("error: " + filepath +
					" cannot be created. Check that the path is valid and that you have write permissions."));
	}
}

template <typename T>
void Parameters::check_val_within_bounds(const std::string& val_name, T value, T lower, T higher)
{
	if(value < lower || value > higher)
	{
		throw (std::invalid_argument("error: " + val_name + " must be a value within range: " +
					'[' + boost::lexical_cast<std::string>(lower) + ';' + boost::lexical_cast<std::string>(higher) + ']'));
	}
}


