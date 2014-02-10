#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include "proxy/fwd.hpp"
#include "proxy/http.hpp"

#include <elib/aux.hpp>
#include <elib/enumeration.hpp>
#include <elib/lexical_cast.hpp>

#include <iostream>

using namespace prox;
namespace enums = elib::enumeration;

BOOST_AUTO_TEST_SUITE(prox_http_test_suite)

BOOST_AUTO_TEST_CASE(prox_http_version_test)
{
    http_version zero = http_version::ONE_ZERO;
    http_version one = http_version::ONE_ONE;
    
    BOOST_CHECK(std::string{"HTTP/1.0"} == version_to_string(zero));
    BOOST_CHECK(std::string{"HTTP/1.1"} == version_to_string(one));
    
    BOOST_CHECK(zero == string_to_version("HTTP/1.0"));
    BOOST_CHECK(one == string_to_version("HTTP/1.1"));
}

#define TEST_STR_MAP(Name) \
    BOOST_CHECK(enums::enum_cast<std::string>(Name) == #Name)
    
BOOST_AUTO_TEST_CASE(prox_http_method_test)
{
    auto OPTIONS = http_method::OPTIONS;
    auto GET = http_method::GET;
    auto HEAD = http_method::HEAD;
    auto POST = http_method::POST;
    auto PUT = http_method::PUT;
    auto DELETE = http_method::DELETE;
    auto TRACE = http_method::TRACE;
    auto CONNECT = http_method::CONNECT;
    
    using traits = enums::enum_traits<http_method>;
    using btraits = enums::basic_enum_traits<http_method>;
    BOOST_CHECK(traits::has_first_value && traits::first_value == OPTIONS);
    BOOST_CHECK(traits::has_last_value && traits::last_value == CONNECT);
    BOOST_CHECK(traits::has_is_contigious && traits::is_contigious);
    BOOST_CHECK(enums::has_name_map<http_method>::value
            && btraits::name_map.size() == enums::size<http_method>());
    
    BOOST_CHECK(enums::has_constexpr_range<http_method>::value);
    
    TEST_STR_MAP(OPTIONS);
    TEST_STR_MAP(GET);
    TEST_STR_MAP(HEAD);
    TEST_STR_MAP(POST);
    TEST_STR_MAP(PUT);
    TEST_STR_MAP(DELETE);
    TEST_STR_MAP(TRACE);
    TEST_STR_MAP(CONNECT);
}
#undef TEST_STR_MAP

BOOST_AUTO_TEST_CASE(prox_http_status_test)
{    
    BOOST_CHECK(enums::has_name_map<http_status>::value);
    BOOST_CHECK(enums::has_range<http_status>::value);
    BOOST_CHECK(!enums::has_constexpr_range<http_status>::value);
 
#define TEST_E(V, N)                                                                             \
    BOOST_CHECK(N == enums::underlying_cast(http_status::V));                                    \
    BOOST_CHECK(http_status::V == enums::enum_cast<http_status>(N));                             \
    BOOST_CHECK(http_status::V == enums::enum_cast<http_status>(#V));                            \
    BOOST_CHECK(std::string{ #V } == enums::enum_cast<std::string>(http_status::V));             \
    BOOST_CHECK(#N == elib::lexical_cast<std::string>(enums::underlying_cast(http_status::V)) ); \
    BOOST_CHECK( FN(V) )

#define FN(V) status_is_information(http_status::V)
    TEST_E(CONTINUE, 100);
    TEST_E(SWITCH_PROTOCOL, 101);
#undef FN
#define FN(V) status_is_success(http_status::V)
    TEST_E(OK, 200);
    TEST_E(CREATED, 201);
    TEST_E(ACCEPTED, 202);
    TEST_E(NON_AUTH_INFORMATION, 203);
    TEST_E(NO_CONTENT, 204);
    TEST_E(RESET_CONTENT, 205);
    TEST_E(PARTIAL_CONTENT, 206);
#undef FN
#define FN(V) status_is_redirect(http_status::V)
    TEST_E(REDIRECT_CHOICE, 300);
    TEST_E(REDIRECT_MOVED_PERM, 301);
    TEST_E(REDIRECT_FOUND, 302);
    TEST_E(REDIRECT_SEE_OTHER, 303);
    TEST_E(REDIRECT_NOT_MODIFIED, 304);
    TEST_E(REDIRECT_USE_PROXY, 305);
    //UNUSED 306
    TEST_E(REDIRECT_TEMP, 307);
#undef FN
#define FN(V) status_is_client_error(http_status::V) && status_is_error(http_status::V)
    TEST_E(BAD_REQUEST, 400);
    TEST_E(UNAUTHORIZED, 401);
    TEST_E(PAYMENT_REQUIRED, 402);
    TEST_E(FORBIDDEN, 403);
    TEST_E(NOT_FOUND, 404);
    TEST_E(METHOD_NOT_ALLOWED, 405);
    TEST_E(NOT_ACCEPTABLE, 406);
    TEST_E(PROXY_AUTH_REQUIRED, 407);
    TEST_E(REQUEST_TIMEOUT, 408);
    TEST_E(CONFLICT, 409);
    TEST_E(GONE, 410);
    TEST_E(LENGTH_REQUIRED, 411);
    TEST_E(PRECONDITION_FAILED, 412);
    TEST_E(REQUEST_ENTITY_TOO_LARGE, 413);
    TEST_E(REQUEST_URI_TOO_LARGE, 414);
    TEST_E(UNSUPPORTED_MEDIA_TYPE, 415);
    TEST_E(REQUEST_RANGE_NOT_SATISFIABLE, 416);
    TEST_E(EXPECTATION_FAILED, 417);
#undef FN
#define FN(V) status_is_server_error(http_status::V) && status_is_error(http_status::V)
    TEST_E(INTERNAL_SERVER_ERROR, 500);
    TEST_E(NOT_IMPLEMENTED, 501);
    TEST_E(BAD_GATEWAY, 502);
    TEST_E(SERVICE_UNAVAILABLE, 503);
    TEST_E(GATEWAY_TIMEOUT, 504);
    TEST_E(HTTP_VERSION_UNSUPPORTED, 505);
#undef FN
#undef TEST_E
}

bool operator==(http_data const & lhs, std::string const & rhs)
{
    std::string lhs_s = std::string(lhs.begin(), lhs.end());
    return lhs_s == rhs;
}


std::ostream & operator <<(std::ostream & out, http_data const & dt)
{
    out << std::string{dt.begin(), dt.end()};
    return out;
}

BOOST_AUTO_TEST_CASE(generate_http_field_test)
{
    auto v = generate_http_field(http_field{"Hello", "Value"});
    BOOST_CHECK( v == "Hello: Value\r\n" );
    
    v = generate_http_field(http_field{"Hello-World", "this \"is_my\" value"});
    BOOST_CHECK( v == std::string{"Hello-World: this \"is_my\" value\r\n"});
}

BOOST_AUTO_TEST_CASE(generate_http_fields_test)
{
    std::vector<http_field> v =
        {
            {"Hello", "Value"}
          , {"Hello-World", "Tada "}
          , {"third-list", "3.14"}
        };
    
    std::string s =
        "Hello: Value\r\n"
        "Hello-World: Tada \r\n"
        "third-list: 3.14\r\n";

    BOOST_CHECK(generate_http_fields(v) == s);
}

BOOST_AUTO_TEST_CASE(generate_http_request_header_test)
{
    http_request_header h =
        {http_version::ONE_ZERO, http_method::GET, "/index.html"};
        
    std::string s =
        "GET /index.html HTTP/1.0\r\n";
        
    BOOST_CHECK(generate_http_request_header(h) == s);
}

BOOST_AUTO_TEST_CASE(generate_http_response_header_test)
{
    http_response_header h =
        { http_version::ONE_ONE, http_status::OK, "OK"};
    
    std::string s = "HTTP/1.1 200 OK\r\n";
    BOOST_CHECK(generate_http_response_header(h) == s);
}

BOOST_AUTO_TEST_CASE(generate_http_request_test)
{
    http_request h =
        { 
            {http_version::ONE_ONE, http_method::GET, "/home/index.html"}
          , {{"Hello", "World"}, {"Hello-World", "Value Test"}}
          , {'h', 'e', 'l', 'l', 'o'}
        };
        
    std::string s =
        "GET /home/index.html HTTP/1.1\r\n"
        "Hello: World\r\n"
        "Hello-World: Value Test\r\n"
        "\r\n"
        "hello";
        
    BOOST_CHECK(generate_http_request(h) == s);
}

BOOST_AUTO_TEST_CASE(generate_http_response_test)
{
    http_response h =
        {
            {http_version::ONE_ZERO, http_status::NOT_FOUND, "Page Not Found"}
          , {{"Hello", "\"World\""}, {"My-Test", " 'test' 123"}} 
          , {'t', '\r', '\n', 't', 'e', 's', 't'}
        };
        
    std::string s =
        "HTTP/1.0 404 Page Not Found\r\n"
        "Hello: \"World\"\r\n"
        "My-Test:  'test' 123\r\n"
        "\r\n"
        "t\r\n"
        "test";
        
    BOOST_CHECK(generate_http_response(h) == s);
}
    
BOOST_AUTO_TEST_CASE(parse_http_field_test)
{
    std::string s_ = "Hello-World: Value\r\n";
    std::vector<char> s{s_.begin(), s_.end()};
    auto begin = s.begin();
    auto end = s.end();
    http_field f;
    
    BOOST_CHECK(parse_http_field(begin, end, f) == end);
    BOOST_CHECK(f.key == "Hello-World");
    BOOST_CHECK(f.value == "Value");
}

BOOST_AUTO_TEST_CASE(parse_http_fields_test)
{
    std::string s_ = 
        "Hello-World:  Value\r\n"
        "this: is 'a' test\r\n"
        "This-World-Is:  Going Down!\r\n";
    std::vector<char> s{s_.begin(), s_.end()};
    auto begin = s.begin();
    auto end = s.end();
        
    std::vector<http_field> v =
        {
            {"Hello-World", " Value"}
          , {"this", "is 'a' test"}
          , {"This-World-Is", " Going Down!"}
        };
    
    std::vector<http_field> r;
    BOOST_CHECK(parse_http_fields(begin, end, r) == end);
    BOOST_REQUIRE(r.size() == v.size());
    
    auto vb = v.begin();
    for (const auto & f : r)
    {
        BOOST_CHECK(vb->key == f.key);
        BOOST_CHECK(vb->value == f.value);
        ++vb;
    }
}

BOOST_AUTO_TEST_CASE(parse_http_request_header_test)
{
    std::string s_ = "GET \"this-value/index.html\" HTTP/1.1\r\n";
    std::vector<char> s{s_.begin(), s_.end()};
    http_request_header h;
    
    BOOST_CHECK(parse_http_request_header(s.begin(), s.end(), h) == s.end());
    BOOST_CHECK(h.version == http_version::ONE_ONE);
    BOOST_CHECK(h.method == http_method::GET);
    BOOST_CHECK(h.request == "\"this-value/index.html\"");
}

BOOST_AUTO_TEST_CASE(parse_http_response_header_test)
{
    std::string s_ = "HTTP/1.0 404 Page Not Found\r\n";
    std::vector<char> s{s_.begin(), s_.end()};
    http_response_header h;
    
    BOOST_CHECK(parse_http_response_header(s.begin(), s.end(), h) == s.end());
    BOOST_CHECK(h.version == http_version::ONE_ZERO);
    BOOST_CHECK(h.status == http_status::NOT_FOUND);
    BOOST_CHECK(h.description == "Page Not Found");
}

BOOST_AUTO_TEST_SUITE_END()