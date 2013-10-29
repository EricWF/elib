#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include <elib/fs/path.hpp>
#include <elib/fs/operations.hpp>
#include <elib/fs/filesystem_error.hpp>
#include <system_error>

#include <vector>
#include <string>
#include <fstream>
#include <cstdlib>
#include <functional>

#undef NDEBUG
#include <cassert>

#if defined(DEBUG)
# include <iostream>
# include <boost/filesystem.hpp>
  namespace bfs = boost::filesystem;
# endif


#include <unistd.h>


#ifndef ELIB_CONFIGURE_FS_UNIT_TEST_PATH
# error ELIB_CONFIGURE_FS_UNIT_TEST_PATH must be set to the directory of this file
# endif


#define PYTHON_RUN_STR "python create_test_env.py "

namespace fs = elib::fs;
  
  
void python_run(const std::string& cmd)
{
  std::string s = PYTHON_RUN_STR;
  s += "\"";
  s += cmd;
  s += "\"";
  BOOST_REQUIRE(std::system(s.c_str()) == 0);
}
  
std::vector<std::string> read_list(const std::string& filename)
{

  std::ifstream in(filename);
  if (!in.good())
    std::cout << "file: " << filename << " is not good" << std::endl;
  assert(in.good());
  
  std::vector<std::string> list;
  
  std::string tmp;
  while (in >> tmp)
  {
    if (tmp != "")
      list.push_back(tmp);
  }
  
  return list;
}



const std::string dir_prefix = ELIB_CONFIGURE_FS_UNIT_TEST_PATH;



std::vector<std::string> relative_paths, absolute_paths, canonical_paths,            \
                         dne_paths, directories, character_files, block_files, 
                         regular_files, fifo_files, symlink_to, symlink_from,  
                         empty_files, non_empty_files;
                         
typedef std::vector<std::reference_wrapper<std::vector<std::string>>> vector_list; 
                         
vector_list files_list =
{
  directories, block_files, character_files,  regular_files,  fifo_files, 
  symlink_to, empty_files, non_empty_files
};



BOOST_AUTO_TEST_SUITE(fs_standalone_test_suite)

#define READ_LIST(lname)                                \
  lname = read_list(dir_prefix + "/test_files/" # lname); \
  assert(lname.size() > 0)

BOOST_AUTO_TEST_CASE(fs_standalone_init)
{
  BOOST_REQUIRE( ::chdir( ELIB_CONFIGURE_FS_UNIT_TEST_PATH ) != -1 );
  
  python_run("init_filesystem()");
  
  READ_LIST(relative_paths);
  READ_LIST(absolute_paths);
  READ_LIST(canonical_paths);
  assert(relative_paths.size() == absolute_paths.size());
  assert(canonical_paths.size() == relative_paths.size());
  
  READ_LIST(dne_paths);
  
  READ_LIST(directories);
  READ_LIST(character_files);
  READ_LIST(block_files);
  READ_LIST(regular_files);
  READ_LIST(fifo_files);
  
  READ_LIST(symlink_to);
  READ_LIST(symlink_from);
  assert(symlink_to.size() == symlink_from.size());
  
  READ_LIST(empty_files);
  READ_LIST(non_empty_files);
  
 
}

#undef READ_LIST


BOOST_AUTO_TEST_CASE(test_paths)
{
  auto ait = absolute_paths.begin();
  auto cit = canonical_paths.begin();
  for (auto& s : relative_paths)
  {
    fs::path p {s};
    auto abs = fs::absolute(p);
    auto canon = fs::canonical(p);
        
    BOOST_CHECK(abs == *ait);
    BOOST_CHECK(canon == *cit);
    ++ait;
    ++cit;
  }
  
  for (auto& s : dne_paths)
  {
    fs::path p {s};
    std::error_code ec {};
    
    fs::canonical(p, ec);
    BOOST_CHECK(ec);
    BOOST_CHECK_THROW(fs::canonical(p), fs::filesystem_error);
  }
}

BOOST_AUTO_TEST_CASE(fs_exists_test)
{
  
  for (auto list : files_list)
  {
    for (auto& s : list.get())
    {
      fs::path p {s};
      BOOST_CHECK(fs::exists(p));
      if (!fs::exists(p))
        std::cout << p.native() << std::endl;
    }
  }
  
  for (auto& s : dne_paths)
  {
    fs::path p {s};
    BOOST_CHECK(!fs::exists(p));
  }
}

BOOST_AUTO_TEST_CASE(fs_type_query_test)
{
  for (auto& s : directories)
  {
    fs::path p {s};
    BOOST_CHECK(fs::is_directory(p));
    BOOST_CHECK(!fs::is_character_file(p));
    BOOST_CHECK(!fs::is_block_file(p));
    BOOST_CHECK(!fs::is_regular_file(p));
    BOOST_CHECK(!fs::is_symlink(p));
    BOOST_CHECK(!fs::is_fifo(p));
    BOOST_CHECK(!fs::is_socket(p));
    BOOST_CHECK(!fs::is_other(p));
  }
  
 for (auto& s : block_files)
  {
    fs::path p {s};
    BOOST_CHECK(!fs::is_directory(p));
    BOOST_CHECK(!fs::is_character_file(p));
    BOOST_CHECK(fs::is_block_file(p));
    BOOST_CHECK(!fs::is_regular_file(p));
    BOOST_CHECK(!fs::is_symlink(p));
    BOOST_CHECK(!fs::is_fifo(p));
    BOOST_CHECK(!fs::is_socket(p));
    BOOST_CHECK(fs::is_other(p));
  }
  
  for (auto& s : character_files)
  {
    fs::path p {s};
    BOOST_CHECK(!fs::is_directory(p));
    BOOST_CHECK(fs::is_character_file(p));
    BOOST_CHECK(!fs::is_block_file(p));
    BOOST_CHECK(!fs::is_regular_file(p));
    BOOST_CHECK(!fs::is_symlink(p));
    BOOST_CHECK(!fs::is_fifo(p));
    BOOST_CHECK(!fs::is_socket(p));
    BOOST_CHECK(fs::is_other(p));
  }
  
  for (auto& s : regular_files)
  {
    fs::path p {s};
    BOOST_CHECK(!fs::is_directory(p));
    BOOST_CHECK(!fs::is_character_file(p));
    BOOST_CHECK(!fs::is_block_file(p));
    BOOST_CHECK(fs::is_regular_file(p));
    BOOST_CHECK(!fs::is_symlink(p));
    BOOST_CHECK(!fs::is_fifo(p));
    BOOST_CHECK(!fs::is_socket(p));
    BOOST_CHECK(!fs::is_other(p));
  }
  
  for (auto& s : fifo_files)
  {
    fs::path p {s};
    BOOST_CHECK(!fs::is_directory(p));
    BOOST_CHECK(!fs::is_character_file(p));
    BOOST_CHECK(!fs::is_block_file(p));
    BOOST_CHECK(!fs::is_regular_file(p));
    BOOST_CHECK(!fs::is_symlink(p));
    BOOST_CHECK(fs::is_fifo(p));
    BOOST_CHECK(!fs::is_socket(p));
    BOOST_CHECK(fs::is_other(p));
  }
  
  for (auto& s : symlink_to)
  {
    fs::path p {s};
    BOOST_CHECK(fs::is_symlink(p));
  }
 
}

BOOST_AUTO_TEST_CASE(fs_standalone_clean)
{
  python_run("clean_filesystem()");
}

BOOST_AUTO_TEST_SUITE_END()