// REQUIRES: ELIB_WEB
#include <elib/experimental/web/http.hpp>
#include <vector>
#include <string>
#include "rapid-cxx-test.hpp"
using namespace elib::web;
using namespace elib::web::http;

TEST_SUITE(elib_web_http_core_test_suite)

TEST_CASE(version_to_string_test)
{
    {
        auto ret = to_string(version::ONE_ZERO);
        TEST_CHECK(ret == "HTTP/1.0");
    }{
        auto ret = to_string(version::ONE_ONE);
        TEST_CHECK(ret == "HTTP/1.1");
    }
}

TEST_CASE(string_to_version_test)
{
    {
        auto ret = to_version("HTTP/1.0");
        TEST_CHECK(version::ONE_ZERO == ret);
    }{
        auto ret = to_version("HTTP/1.1");
        TEST_CHECK(version::ONE_ONE == ret);
    }{
        TEST_CHECK_THROW(web_error, to_version("BAD"));
    }
}

#define _(M) { method::M, #M }
TEST_CASE(method_test)
{
    struct method_testcase
    {
        method m;
        std::string s;
    };
    
    const std::vector<method_testcase> testcases =
    {
        _(OPTIONS)
      , _(GET)
      , _(HEAD)
      , _(POST)
      , _(PUT)
      , _(DELETE)
      , _(TRACE)
      , _(CONNECT)
    };
    
    for (auto & testcase : testcases) {
        TEST_CHECK(testcase.s == to_string(testcase.m));
        TEST_CHECK(testcase.m == to_method(testcase.s));
    }
}
#undef _

#define _(S, ...) { status::S, #S, __VA_ARGS__}
TEST_CASE(status_test)
{
    struct status_testcase
    {
        status s;
        std::string str;
        bool is_info;
        bool is_success;
        bool is_redirect;
        bool is_client_error;
        bool is_server_error;
    };
    
    const std::vector<status_testcase> testcases
    {
       _(CONTINUE, true, false, false, false, false)
      , _(SWITCH_PROTOCOL, true, false, false, false, false)
      
      , _(OK, false, true, false, false, false)
      , _(CREATED, false, true, false, false, false)
      , _(ACCEPTED, false, true, false, false, false)
      , _(NON_AUTH_INFORMATION, false, true, false, false, false)
      , _(NO_CONTENT, false, true, false, false, false)
      , _(RESET_CONTENT, false, true, false, false, false)
      , _(PARTIAL_CONTENT, false, true, false, false, false)
      
      , _(REDIRECT_CHOICE, false, false, true, false, false)
      , _(REDIRECT_MOVED_PERM, false, false, true, false, false)
      , _(REDIRECT_FOUND, false, false, true, false, false)
      , _(REDIRECT_SEE_OTHER, false, false, true, false, false)
      , _(REDIRECT_NOT_MODIFIED, false, false, true, false, false)
      , _(REDIRECT_USE_PROXY, false, false, true, false, false)
      , _(REDIRECT_TEMP, false, false, true, false, false)
      
      , _(BAD_REQUEST, false, false, false, true, false)
      , _(UNAUTHORIZED, false, false, false, true, false)
      , _(PAYMENT_REQUIRED, false, false, false, true, false)
      , _(FORBIDDEN, false, false, false, true, false)
      , _(NOT_FOUND, false, false, false, true, false)
      , _(METHOD_NOT_ALLOWED, false, false, false, true, false)
      , _(NOT_ACCEPTABLE, false, false, false, true, false)
      , _(PROXY_AUTH_REQUIRED, false, false, false, true, false)
      , _(REQUEST_TIMEOUT, false, false, false, true, false)
      , _(CONFLICT, false, false, false, true, false)
      , _(GONE, false, false, false, true, false)
      , _(LENGTH_REQUIRED, false, false, false, true, false)
      , _(PRECONDITION_FAILED, false, false, false, true, false)
      , _(REQUEST_ENTITY_TOO_LARGE, false, false, false, true, false)
      , _(REQUEST_URI_TOO_LARGE, false, false, false, true, false)
      , _(UNSUPPORTED_MEDIA_TYPE, false, false, false, true, false)
      , _(REQUEST_RANGE_NOT_SATISFIABLE, false, false, false, true, false)
      , _(EXPECTATION_FAILED, false, false, false, true, false)
      
      , _(INTERNAL_SERVER_ERROR, false, false, false, false, true)
      , _(NOT_IMPLEMENTED, false, false, false, false, true)
      , _(BAD_GATEWAY, false, false, false, false, true)
      , _(SERVICE_UNAVAILABLE, false, false, false, false, true)
      , _(GATEWAY_TIMEOUT, false, false, false, false, true)
      , _(HTTP_VERSION_UNSUPPORTED, false, false, false, false, true) 
    };
    
    for (auto const & testcase : testcases) {
        TEST_CHECK(testcase.str == to_string(testcase.s));
        TEST_CHECK(testcase.s == to_status(testcase.str));
        TEST_CHECK(testcase.is_info == status_is_information(testcase.s));
        TEST_CHECK(testcase.is_success == status_is_success(testcase.s));
        TEST_CHECK(testcase.is_redirect == status_is_redirect(testcase.s));
        TEST_CHECK(testcase.is_client_error == status_is_client_error(testcase.s));
        TEST_CHECK(testcase.is_server_error == status_is_server_error(testcase.s));
        TEST_CHECK(
            (testcase.is_client_error || testcase.is_server_error)
            == status_is_error(testcase.s)
          );
    }
}

TEST_CASE(generate_default_request_test)
{
    struct generate_request_testcase
    {
        method m;
        std::string s;
    };
    
    std::vector<generate_request_testcase> const testcases =
    {
        {method::OPTIONS, ""}
      , {method::GET, "/testfile"}
    };
    
    for (auto const & tc : testcases) {
        request ret = generate_default_request(tc.m, tc.s);
        TEST_CHECK(ret.header.http_version == default_version);
        TEST_CHECK(ret.header.code == tc.m);
        TEST_CHECK(ret.header.value == tc.s);
    }
}

TEST_CASE(generate_default_response_test)
{
    struct generate_response_testcase
    {
        status st;
    };
    
    std::vector<generate_response_testcase> const testcases =
    {
        {status::OK}
      , {status::REDIRECT_MOVED_PERM}
    };
    
    for (auto const & tc : testcases) {
        response ret = generate_default_response(tc.st);
        TEST_CHECK(ret.header.http_version == default_version);
        TEST_CHECK(ret.header.code == tc.st);
        TEST_CHECK(ret.header.value == to_string(tc.st));
    }
}


TEST_SUITE_END()