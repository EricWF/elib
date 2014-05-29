#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include <elib/filesystem.hpp>
#include <system_error>
#include <cstdint>
#include "../dynamic_test_helper.hpp"
using namespace elib::fs;

constexpr const std::uintmax_t bad_count = static_cast<std::uintmax_t>(-1);

BOOST_AUTO_TEST_SUITE(elib_filesystem_dynamic_remove_all_test_suite)

BOOST_AUTO_TEST_CASE(dne_test)
{
    scoped_test_env env;
    path const file = make_env_path("dne");
    
    {
        std::error_code ec;
        BOOST_REQUIRE(0 == remove_all(file, ec));
        BOOST_REQUIRE(not ec);
    }
    {
        BOOST_REQUIRE(0 == remove_all(file));
    }
}

BOOST_AUTO_TEST_CASE(empty_dir_test)
{
    scoped_test_env env;
    path const file = make_env_path("dir1");
    
    python_create_dir(file);
    
    std::error_code ec;
    BOOST_REQUIRE(1 == remove_all(file, ec));
    BOOST_REQUIRE(not ec);
}

BOOST_AUTO_TEST_CASE(file_test)
{
   scoped_test_env env;
    path const file = make_env_path("file1");
    
    python_create_file(file);
    
    std::error_code ec;
    BOOST_REQUIRE(1 == remove_all(file, ec));
    BOOST_REQUIRE(not ec); 
}

BOOST_AUTO_TEST_CASE(non_empty_non_recursive_dir_test)
{
    scoped_test_env env;
    path const file = make_env_path("dir1");
    path const f1 = file / "f1";
    path const f2 = file / "f2";
    
    python_create_dir(file);
    python_create_file(f1);
    python_create_file(f2);
    
    std::error_code ec;
    BOOST_REQUIRE(3 == remove_all(file, ec));
    BOOST_REQUIRE(not ec);
}

BOOST_AUTO_TEST_CASE(non_empty_recursive_dir_test)
{
    scoped_test_env env;
    path const file = make_env_path("dir1");
    path const f1 = file / "f1";
    path const f2 = file / "f2";
    path const dir1 = file / "dir1";
    path const f3 = dir1 / "f3";
    path const f4 = dir1 / "f4";
    
    python_create_dir(file);
    python_create_dir(dir1);
    python_create_file(f1);
    python_create_file(f2);
    python_create_file(f3);
    python_create_file(f4);
    
    std::error_code ec;
    BOOST_REQUIRE(6 == remove_all(file, ec));
    BOOST_REQUIRE(not ec);
}

BOOST_AUTO_TEST_SUITE_END()