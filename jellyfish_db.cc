#include "jellyfish_db.hh"
#include <stdexcept>
#include <cstring>
#include <iostream>

const char JellyfishDB::JELLYFISH_DB_FILETYPE[] = "JFLISTDN";
const uint64_t JellyfishDB::JELLYFISH_DB_FILETYPE_SZ = sizeof(JELLYFISH_DB_FILETYPE) - 1;


JellyfishDB::JellyfishDB(const std::string& path)
	:  path_(path)
{
	try
	{
		mapped_file_.open(path_);
	}
	catch(...)
	{
		throw std::invalid_argument("error: " + path_ + " cannot be opened.");
	}


	if (strncmp(JELLYFISH_DB_FILETYPE, mapped_file_.data(), JELLYFISH_DB_FILETYPE_SZ) != 0)
		throw std::invalid_argument("error: " + path_ + " is not a valid Jellyfish database.");

	kmer_bits_ = *(reinterpret_cast<const uint64_t*>(mapped_file_.data()+8));
	kmer_occs_sz_ = *(reinterpret_cast<const uint64_t*>(mapped_file_.data()+16));
	entries_ct_ = *(reinterpret_cast<const uint64_t*>(mapped_file_.data()+48));
	kmer_len_ = kmer_bits_/2;
	kmer_sz_ = (kmer_bits_ + 7)/8;

	entry_sz_ = kmer_sz_ + kmer_occs_sz_;
	entries_ptr_ = mapped_file_.data()+ header_sz(); 
}
