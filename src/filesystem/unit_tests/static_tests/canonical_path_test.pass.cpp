// REQUIRES: ELIB_FILESYSTEM
#include <elib/filesystem.hpp>
#include "rapid-cxx-test.hpp"
using namespace elib;
using namespace elib::fs;

TEST_SUITE(elib_filesystem_static_canonical_path_test_suite)

TEST_CASE(canonical_test)
{
    const path curr = current_path();
    const path dne("/baz");
    const path base("/");
    TEST_REQUIRE(! fs::exists(dne));
    std::error_code ec;
    // Does not exists
    {
        TEST_CHECK_THROW(filesystem_error, fs::canonical(dne));
        
        TEST_CHECK_NO_THROW(fs::canonical(dne, ec));
        TEST_CHECK(ec);
        
        TEST_CHECK_NO_THROW(fs::canonical(dne, base, ec));
        TEST_CHECK(ec);
    }
    // single dot
    {
        const path p(".");
        path ret;
        TEST_CHECK_NO_THROW(ret = fs::canonical(p));
        TEST_CHECK(ret == curr);
        
        ret = fs::canonical(p, ec);
        TEST_CHECK(!ec);
        TEST_CHECK(ret == curr);
        
        ret = fs::canonical(p, base, ec);
        TEST_CHECK(!ec);
        TEST_CHECK(ret == path("/"));
    }
    // double dot
    {
        const path p("..");
        path ret;
        TEST_CHECK_NO_THROW(ret = fs::canonical(p));
        TEST_CHECK(ret == curr.parent_path());
        
        ret = fs::canonical(p, ec);
        TEST_CHECK(!ec);
        TEST_CHECK(ret == curr.parent_path());
        
        /// NOTE: on mac /tmp's canonical path is not /tmp.
        /// So to make this test path,  we have to find tmp's canonical path.
        path canon_tmp = fs::canonical("/tmp");
        ret = fs::canonical(p, canon_tmp, ec);
        TEST_CHECK(!ec);
        TEST_CHECK(ret == canon_tmp.parent_path());
    }
    // TODO symlink test
}

TEST_SUITE_END()
