#ifndef ELIB_OPTIONS_OPTION_HPP
#define ELIB_OPTIONS_OPTION_HPP

# include <elib/options/fwd.hpp>
# include <elib/options/value.hpp>
# include <elib/aux.hpp>

# include <string>
# include <map>
# include <memory>
# include <vector>

namespace elib { namespace options
{
    ////////////////////////////////////////////////////////////////////////////
    //
    struct option
    {
        std::string string_key;
        int position_key{-1};
        
        std::vector< std::string > value;
        std::vector< std::string > original_tokens;
        
        bool unregistered{true};
    };
    
    ////////////////////////////////////////////////////////////////////////////
    //
    class option_description
    {
    public:
        option_description() {}
        
        option_description(
            std::string const & name, value_semantic && s
        );
        
        option_description(
            std::string const & name, value_semantic && s
          , std::string desc
        );
        
        virtual ~option_description();
        
        match_result match(std::string const & opt, bool aprox);
        
        std::string const & key(std::string const & opt) const;
        
        std::string canonical_name() const;
        
        bool has_short_name()  const { return !m_short_name.empty(); }
        bool has_long_name()   const { return !m_long_name.empty(); }
        bool has_description() const { return !m_desc.empty(); }
        
        std::string const & long_name()   const { return m_long_name; }
        std::string const & short_name()  const { return m_short_name; }
        std::string const & description() const { return m_desc; }
        
        std::shared_ptr<const value_semantic> semantics() const
        { return m_semantics; }
        
        std::string format_name() const;
        std::string format_parameter() const;
        
    private:
        option_description & set_name(const char *name);
        
        std::string m_short_name, m_long_name, m_desc;
        std::shared_ptr<const value_semantic> m_semantics;
    };  

     ////////////////////////////////////////////////////////////////////////////
    //
    class options_description
    {
    public:
        options_description();
        options_description(std::string const & caption);
        
        void add(std::shared_ptr<option_description> opt);
        
        options_description & add(options_description const &);
        
        option_description_init add_options();
        
        bool has_option(std::string const & name) const;
        
        option_description const & find(std::string const & name) const;
        option_description const * find_nothrow(std::string const & name) const;
        
        std::vector< std::shared_ptr<option_description> > const &
        options() const;
        
    private:
        std::map<std::string, int> m_map_to_index;
        std::vector< std::shared_ptr<option_description> > m_options;
    };
    
    ////////////////////////////////////////////////////////////////////////////
    //
    class option_description_init
    {
    private:
        friend class options_description;
        
        option_description_init(options_description & owner);
        
    public:
        option_description_init &
        operator()(std::string name, std::string desc)
        {
            m_owner.add(std::make_shared<option_description>(
                name, untyped_value{}, desc
            ));
            return *this;
        }
        
        option_description_init &
        operator()(std::string name, value_semantic && s);
        
        option_description_init &
        operator()(std::string name, value_semantic && s, std::string desc);

    private:        
        options_description & m_owner;
    };
    
    
    ////////////////////////////////////////////////////////////////////////////
    //
    class positional_options_description
    {
    public:
        positional_options_description();
        
        positional_options_description &
        add(std::string const & name, int max_count);
        
        unsigned max_total_count() const;
        
        std::string const & name_for_position(unsigned pos) const;

    private:
        std::vector< std::string > m_names;
        std::string m_trailing;
    };
}}                                                          // namespace elib
#endif /* ELIB_OPTIONS_OPTION_HPP */