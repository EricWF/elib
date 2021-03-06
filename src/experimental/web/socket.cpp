#include <elib/experimental/web/socket.hpp>
#include <elib/config.hpp>
#include <elib/assert.hpp>
#include <elib/aux/move.hpp>

#define ELIB_WEB_HANDLE_OR_THROW_ERROR(...) \
    ELIB_CATCH_AND_RETHROW(detail::handle_or_throw_error(__VA_ARGS__);)

namespace elib { namespace web
{
    namespace detail
    {
        // Forward declarations are provided to satify clang's "-Wmissing-prototype"
        bool handle_or_throw_bad_sock(std::string msg, socket const & s, std::error_code *ec);
        void handle_error(std::error_code & ec) noexcept;
        void handle_or_throw_error(std::string msg, std::error_code *ec);
        
        ////////////////////////////////////////////////////////////////////////
        //
        bool handle_or_throw_bad_sock(std::string msg, socket const & s
                                    , std::error_code *ec)
        {
            std::error_code m_ec{};
            if (s.fail())
                m_ec = s.error_code();
            else if (!s.is_open())
                m_ec = std::error_code{EBADF, std::system_category()};
            else
                return false;
                
            if (!ec)
            {
                ELIB_THROW_EXCEPTION(
                    socket_error(elib::move(msg), elib::move(m_ec))
                  );
            }
            // else
            *ec = elib::move(m_ec);
            return true;
        }
        
        ////////////////////////////////////////////////////////////////////////
        //
        void handle_error(std::error_code & ec) noexcept
        {
            ELIB_ASSERT(errno != 0);
            ec = std::error_code{errno, std::system_category()};
        }
        
        ////////////////////////////////////////////////////////////////////////
        //
        void handle_or_throw_error(std::string msg
                                 , std::error_code *ec)
        {
            ELIB_ASSERT(errno != 0);
            std::error_code m_ec{errno, std::system_category()};
            
            if (!ec)
            {
                ELIB_THROW_EXCEPTION(socket_error(
                    elib::move(msg), elib::move(m_ec)
                ));
            }
            // else
            *ec = elib::move(m_ec);
        }
    }                                                       // namespace detail
    
    ////////////////////////////////////////////////////////////////////////////
    //
    void socket::open(sock_domain d, sock_type t, int protocol) noexcept
    {
        reset();
        m_fd = ::socket(static_cast<int>(d), static_cast<int>(t), protocol);
        if (m_fd < 0)
        {
            detail::handle_error(m_ec);
            m_fd = -1;
        }
    }
    
    ////////////////////////////////////////////////////////////////////////////
    //
    void socket::shutdown(sock_shut s) noexcept
    {
        if (! is_open()) return;
        if (-1 == ::shutdown(m_fd, static_cast<int>(s)))
            detail::handle_error(m_ec);
    }
    
    ////////////////////////////////////////////////////////////////////////////
    //
    void socket::close() noexcept
    {
        if (! is_open()) return;
        if (-1 == ::close(m_fd))
            detail::handle_error(m_ec);
        m_fd = -1;
    }
    
    ////////////////////////////////////////////////////////////////////////////
    //
    bool socket::listen(int backlog) noexcept
    {
        if (!good()) return false;
        if (-1 == ::listen(m_fd, backlog))
        {
            detail::handle_error(m_ec);
            return false;
        }
        return true;
    }
    
    ////////////////////////////////////////////////////////////////////////////
    //
    bool socket::m_bind(const sockaddr *addr, socklen_t len) noexcept
    {
        if (!good()) return false;
        if (-1 == ::bind(m_fd, addr, len))
        {
            detail::handle_error(m_ec);
            return false;
        }
        return true;
    }
    
    ////////////////////////////////////////////////////////////////////////////
    //
    bool socket::m_connect(const sockaddr *addr, socklen_t len) noexcept
    {
        if (!good()) return false;
        if (-1 == ::connect(m_fd, addr, len))
        {
            detail::handle_error(m_ec);
            return false;
        }
        return true;
    }
    
    namespace detail
    {
        ////////////////////////////////////////////////////////////////////////
        //
        socket accept_impl(socket const & s, sockaddr *addr, socklen_t *len
                         , std::error_code *ec)
        {
            if (ec) ec->clear();
            socket m_sock{};
            
            ELIB_RETHROW_BLOCK_BEGIN()
            {
                if (detail::handle_or_throw_bad_sock(
                    "bad socket passed to accept", s, ec))
                    return m_sock;
            } ELIB_RETHROW_BLOCK_END()
                
            int fd = ::accept(s.raw_socket(), addr, len);
            if (fd >= 0)
                m_sock.m_fd = fd;
            else
                ELIB_WEB_HANDLE_OR_THROW_ERROR("accept failed", ec);
            return m_sock;
        }
                   
        ////////////////////////////////////////////////////////////////////////
        //
        ssize_t receive_impl(socket const & s, std::vector<char> & v
                           , msg_flags f, std::error_code *ec)
        {
            if (ec) ec->clear();
                
            ELIB_RETHROW_BLOCK_BEGIN()
            {
                if (detail::handle_or_throw_bad_sock(
                    "bad socket passed to receive", s, ec))
                    return -1;
            } 
            ELIB_RETHROW_BLOCK_END()
            
            ssize_t ret = ::recv(s.raw_socket(), static_cast<void*>(&v[0])
                                , v.size(), static_cast<int>(f));
                                
            if (-1 == ret)
                ELIB_WEB_HANDLE_OR_THROW_ERROR("receive failed", ec);
            return ret;
        }
                  
        ////////////////////////////////////////////////////////////////////////
        //
        ssize_t receive_msg_impl(socket const & s, message_t & m, msg_flags f
                               , std::error_code *ec)
        {
            if (ec) ec->clear();
            
            ELIB_RETHROW_BLOCK_BEGIN() 
            {
                if (detail::handle_or_throw_bad_sock(
                    "bad socket passed to receive_msg", s, ec))
                    return -1;
            } 
            ELIB_RETHROW_BLOCK_END()
                
            ssize_t ret = ::recvmsg(s.raw_socket(), &m, static_cast<int>(f));
            
            if (-1 == ret)
                ELIB_WEB_HANDLE_OR_THROW_ERROR("receive_msg failed", ec);
            return ret;
        }
              
        ////////////////////////////////////////////////////////////////////////
        //
        ssize_t send_impl(socket const & s, std::vector<char> const & v
                        , msg_flags f, std::error_code *ec)
        {
            if (ec) ec->clear();
            
            ELIB_RETHROW_BLOCK_BEGIN()
            {
                if (detail::handle_or_throw_bad_sock(
                    "bad socket passed to send", s, ec))
                    return -1;
            }
            ELIB_RETHROW_BLOCK_END()
                
            ssize_t ret = ::send(s.raw_socket(), static_cast<const void*>(&v[0])
                                , v.size(), static_cast<int>(f));
            if (-1 == ret)
            {
                ELIB_WEB_HANDLE_OR_THROW_ERROR("send failed", ec);
            }
            return ret;
        }
        
        ////////////////////////////////////////////////////////////////////////
        //
        ssize_t send_msg_impl(socket const & s, message_t const & m
                            , msg_flags f, std::error_code *ec)
        {
            ELIB_ASSERT(!errno);
            if (ec) ec->clear();
                
            ELIB_RETHROW_BLOCK_BEGIN()
            {
                if (detail::handle_or_throw_bad_sock(
                    "bad socket passed to send_msg", s, ec))
                    return -1;
            }
            ELIB_RETHROW_BLOCK_END()
                
            ssize_t ret = ::sendmsg(s.raw_socket(), &m, static_cast<int>(f));
            
            if (-1 == ret)
            {
                ELIB_WEB_HANDLE_OR_THROW_ERROR("send_msg failed", ec);
            }
            return ret;
        }
                  
        ////////////////////////////////////////////////////////////////////////
        //
        ssize_t send_to_impl(socket const & s, std::vector<char> const & v
                           , msg_flags f
                           , const sockaddr *dest_addr, socklen_t len
                           , std::error_code *ec)
        {
            if (ec) ec->clear();
                
            ELIB_RETHROW_BLOCK_BEGIN()
            {
                if (detail::handle_or_throw_bad_sock(
                    "bad socket passed to send_to", s, ec))
                    return -1;
            } 
            ELIB_RETHROW_BLOCK_END()
            
            ssize_t ret = ::sendto(s.raw_socket(), static_cast<const void*>(&v[0])
                                  , v.size(), static_cast<int>(f)
                                  , dest_addr, len);
               
            if (-1 == ret)
            {
                ELIB_WEB_HANDLE_OR_THROW_ERROR("send_to failed", ec);
            }
            return ret;
        }
        
#if defined(__GNUC__) && !defined(__clang__)
# pragma GCC diagnostic push
# pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif
        ::sockaddr_in get_peer_name_impl(socket const & s, std::error_code *ec)
        {
            if (ec) ec->clear();
            
            ::sockaddr_in in;
            
            ELIB_RETHROW_BLOCK_BEGIN()
            {
                if (detail::handle_or_throw_bad_sock(
                    "bad socket passed to get_peer_name", s, ec))
                    return in;
            }
            ELIB_RETHROW_BLOCK_END()
            
            ::socklen_t len = sizeof(::sockaddr_in);
            int ret = ::getpeername(
                s.raw_socket()
              , reinterpret_cast<::sockaddr*>(&in)
              , &len
              );
            
            if (ret == -1) 
            {
                ELIB_WEB_HANDLE_OR_THROW_ERROR("get_peer_name failed", ec);
                return in;
            }
            // TODO this is wrong
            ELIB_ASSERT(len <= sizeof(::sockaddr_in));
            return in;
        }
#if defined(__GNUC__) && !defined(__clang__)
# pragma GCC diagnostic pop
#endif
        
        
#if defined(__GNUC__) && !defined(__clang__)
# pragma GCC diagnostic push
# pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif
        ::sockaddr_in get_sock_name_impl(socket const & s, std::error_code *ec)
        {
            if (ec) ec->clear();
              
            ::sockaddr_in in;
            
            ELIB_RETHROW_BLOCK_BEGIN()
            {
                if (detail::handle_or_throw_bad_sock(
                    "bad socket passed to get_sock_name", s, ec))
                    return in;
            }
            ELIB_RETHROW_BLOCK_END()
            
            ::socklen_t len = sizeof(::sockaddr_in);
            int ret = ::getsockname(
                s.raw_socket()
              , reinterpret_cast<::sockaddr*>(&in)
              , &len
              );
            
            if (ret == -1)
            {
                ELIB_WEB_HANDLE_OR_THROW_ERROR("get_sock_name failed", ec);
                return in;
            }
            
            ELIB_ASSERT(len <= sizeof(::sockaddr_in));
            return in;
        }
#if defined(__GNUC__) && !defined(__clang__)
# pragma GCC diagnostic pop
#endif

    }                                                       // namespace detail
}}                                                       // namespace elib::web
