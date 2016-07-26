
#pragma once


#include <boost/filesystem.hpp>
#include <string>

using namespace boost::filesystem;

struct recursive_directory_range {
  typedef recursive_directory_iterator iterator;
  recursive_directory_range(path p) : p_(p) {}

  iterator begin() { return recursive_directory_iterator(p_); }
  iterator end() { return recursive_directory_iterator(); }

  path p_;
};


void print_filePath(const std::string & dir_path);

// return the filenames of all files that have the specified extension
// in the specified directory and all subdirectories
void get_all_files(const boost::filesystem::path& root, const std::string& ext,
                   std::vector<boost::filesystem::path>& ret);
