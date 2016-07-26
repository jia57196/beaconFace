

#include "common.h"
#include <boost/regex.hpp>

void print_filePath(const std::string & dir_path) {
  for (auto it : recursive_directory_range(dir_path)) {
    std::cout << it << std::endl;
  }
}

// return the filenames of all files that have the specified extension
// in the specified directory and all subdirectories
void get_all_files(const path& root, const std::string& ext, std::vector<path>& ret) {
  if (!exists(root) || !is_directory(root)) return;

  recursive_directory_iterator it(root);
  recursive_directory_iterator endit;

  while (it != endit) {
    if (is_regular_file(*it) && it->path().extension() == ext) {
      ret.push_back(it->path().relative_path());
    }
    ++it;
  }
}
