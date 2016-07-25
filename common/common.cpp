

#include "common.h"

void print_filePath(const std::string & dir_path) {
	for (auto it : recursive_directory_range(dir_path)) {
		std::cout << it << std::endl;
	}
}



