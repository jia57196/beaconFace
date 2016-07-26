
#include "common/common.h"
#include <gtest/gtest.h>
#include <iostream>
#include <boost/foreach.hpp>

using namespace boost::filesystem;

TEST(list_folder_files, recursivly_go_though_folder) {
	std::string  dir_path("../image");
	print_filePath(dir_path);
	std::cout << "file is listed in above." << std::endl;
}


TEST(list_folder_files, recursivly_go_though_folder_with_filter) {
	std::string  dir_path("../image");
	std::vector<path> ret;
	get_all_files(dir_path, ".jpg", ret);

	BOOST_FOREACH(path const & filename, ret) {
		std::cout << "file: " << filename << std::endl;
	}
}
