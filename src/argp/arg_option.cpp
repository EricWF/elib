/* 
 * Copyright (C) 2013  Eric Fiselier
 * 
 * This file is part of elib.
 *
 * elib is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * elib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with elib.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "elib/argp/arg_option.h"
#include "elib/argp/arg_errors.h"

#include <algorithm>
#include <sstream>
#include <iostream>
#include <cctype>

namespace elib {
namespace argp {
    
    


arg_option::arg_option(arg_type_e arg_type,
               const std::string & name,
               const std::string & cmd_desc,
               const std::string & desc)
    : m_arg_type(arg_type), m_cmd_desc(cmd_desc), m_desc(desc)
{
    if (name != "")
        parse_name_and_set(name);
}


const std::string &
arg_option::command_description() const
{
    return m_cmd_desc;
}

const std::string &
arg_option::description() const
{
    return m_desc;
}

std::string
arg_option::fmt_description() const
{
    std::stringstream ss;
    ss << m_cmd_desc << "    " << m_desc << std::endl;
    return ss.str();
}

void
arg_option::notify(const arg_token & tk)
{
    throw 1;
}

bool 
arg_option::has_short_name() const
{
    return (m_short_name != "");
}

bool 
arg_option::has_long_name() const
{
    return (m_long_name != "");
}
    
const std::string & 
arg_option::short_name() const
{
    return m_short_name;
}

const std::string & 
arg_option::long_name() const
{
    return m_long_name;
}
    
bool 
arg_option::match(const std::string & name) const
{
    std::string tmp;
    return match(name, tmp);
}

bool 
arg_option::match(const std::string & name, std::string & dest) const
{
    if (prefix_is_long_name(name) && has_long_name() &&
        name == m_long_name) {
        
        dest = m_long_name;
        return true;
    }
    
    if (prefix_is_short_name(name) && has_short_name() &&
        name == m_short_name) {
        
        dest = m_short_name;
        return true;
    }
    
    return false;
}
    
bool 
arg_option::match(const arg_option & opt) const
{
    std::string tmp;
    return match(opt, tmp);
}

bool 
arg_option::match(const arg_option & opt, std::string & dest) const
{
    if (has_long_name() && opt.has_long_name() && 
        m_long_name == opt.long_name()) {
        
        dest = m_long_name;
        return true;
    }
    
    if (has_short_name() && opt.has_short_name() &&
        m_short_name == opt.short_name()) {
        
        dest = m_short_name;
        return true;
    }
    
    return false;
}


void 
arg_option::parse_name_and_set(const std::string & s)
{
    auto count = s.find(',');
    try {
        if (count == std::string::npos) {
            handle_name_part(s);
        }
        else {
            handle_name_part(s.substr(0, count));
            handle_name_part(s.substr(count+1));
        }
    } catch (name_format_error & ex) {
        throw name_format_error(s);
    }
}

void
arg_option::handle_name_part(std::string s)
{
    if (is_valid_long_name(s)) {
        if (has_long_name())
            throw name_format_error(s);
        m_long_name = s;
    }
    else if (is_valid_short_name(s)) {
        if (has_short_name())
            throw name_format_error(s);
        m_short_name = s;
    }
    else {
        throw name_format_error(s);
    }
}



////////////////////////////////////////////////////////////////////////////////
//                            functions                                       //
////////////////////////////////////////////////////////////////////////////////


bool
prefix_is_long_name(const std::string & name)
{
    if (name.size() < 3)
        return false;
    
    return (name[0] == '-' && name[1] == '-' && isalpha(name[2]));
}

bool
prefix_is_short_name(const std::string & name)
{
    if (name.size() < 2)
        return false;
    
    return (name[0] == '-' && isalpha(name[1]));
}

bool
is_valid_arg_name(const std::string & name)
{
    
    if (prefix_is_long_name(name)) {
        return is_valid_long_name(name);
    }
    else if (prefix_is_short_name(name)) {
        return is_valid_short_name(name);
    }
    
    return false;
}

bool
is_valid_short_name(const std::string & name)
{
    if (name.size() != 2)
        return false;
    
    return (name[0] == '-' && isalpha(name[1]));
}

bool
is_valid_long_name(const std::string & name)
{
    if (name.size() < 3)
        return false;
    
    if (name[0] != '-' || name[1] != '-' || ! isalpha(name[2]))
        return false;
    
    if (name[name.size()-1] == '-')
        return false;
    
    for (unsigned i=3; i < name.size(); ++i) {
        char c = name[i];
        if (c == '-') {
            if (i + 1 == name.size() || ! isalpha(name[i+1]))
                return false;
        }
        else if (! isalpha(c)) {
            return false;
        }
    }
    
    return true;
}
 
} /* namespace argp */
} /* namespace elib */