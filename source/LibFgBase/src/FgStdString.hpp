//
// Coypright (c) 2020 Singular Inversions Inc. (facegen.com)
// Use, modification and distribution is subject to the MIT License,
// see accompanying file LICENSE.txt or facegen.com/base_library_license.txt
//
// std::string related functions
//

#ifndef FGSTDSTRING_HPP
#define FGSTDSTRING_HPP

#include "FgStdLibs.hpp"
#include "FgTypes.hpp"
#include "FgStdVector.hpp"
#include "FgOpt.hpp"

namespace Fg {

typedef std::string             String;
typedef Svec<String>            Strings;
typedef Svec<Strings>           Stringss;
typedef Svec<std::u32string>    String32s;

// More general than std::to_string since it uses operator<< which can be defined for
// user-defined types as well. Also, to_string can cause ambiguous call errors.
template<class T>
String
toStr(const T & val)
{
    std::ostringstream   msg;
    msg << val;
    return msg.str();
}

template<>
inline String
toStr(String const & str)
{return str; }

template<class T>
Strings
toStrings(Svec<T> const & v)
{
    Strings         ret;
    ret.reserve(v.size());
    for (T const & e : v)
        ret.push_back(toStr(e));
    return ret;
}

// Default uses standard stream input "lexical conversions".
// Only valid strings for the given type are accepted, extra characters including whitespace are errors
// Define fully specialized versions where this behaviour is not desired:
template<class T>
Opt<T>
fromStr(String const & str)
{
    T                   val;
    std::istringstream  iss(str);
    iss >> val;
    if (iss.fail() || (!iss.eof()))
        return Opt<T>();
    return Opt<T>(val);
}
// Only valid integer representations within the range of int32 will return a valid value, whitespace invalid:
template<> Opt<int> fromStr<int>(String const &);
template<> Opt<uint> fromStr<uint>(String const &);

// Throws if the String is not formatted correctly:
template<class T>
T
fromStrThrow(String const & str)
{
    Opt<T>    oval = fromStr<T>(str);
    if (!oval.valid())
        throw FgException("Unable to convert String to type",String(typeid(T).name())+":"+str);
    return oval.val();
}

// Ensures a minimum number of digits are printed:
template<class T>
String
toStrDigits(T val,uint numDigits)
{
    std::ostringstream   oss;
    oss << std::setw(numDigits) << std::setfill('0') << val;
    return oss.str();
}

// Sets the desired total number of digits (precision):
template<class T>
String
toStrPrecision(T val,uint numDigits)
{
    std::ostringstream   oss;
    oss.precision(numDigits);
    oss << val;
    return oss.str();
}

// Set the number of digits beyond fixed point:
String
toStrFixed(double val,uint fractionalDigits=0);

// Multiply by 100 and put a percent sign after:
String
toStrPercent(double val,uint fractionalDigits=0);

String
toLower(String const & s);

String
toUpper(String const & s);

// Returned list of strings does NOT include separators but DOES include empty
// strings where there are consecutive separators:
Svec<String>
splitAtSeparators(String const & str,char sep);

String
replaceAll(String const & str,char orig,char repl);

// Pad a String to desired len (does not truncate of longer):
String
padToLen(String const & str,size_t len,char ch=' ');

// Inspired by Python join():
String
cat(const Svec<String> & strings,String const & separator);

inline
String
cat(String const & s0,String const & s1)
{
    String      ret(s0);
    ret.append(s1);
    return ret;
}

inline
String
cat(String const & s0,String const & s1,String const & s2)
{
    String      ret(s0);
    ret.append(s1);
    ret.append(s2);
    return ret;
}

template<typename T>
bool
contains(const std::basic_string<T> & str,T ch)
{return (str.find(ch) != std::basic_string<T>::npos); }

template<typename T>
bool
contains(const std::basic_string<T> & str,const std::basic_string<T> & pattern)
{return (str.find(pattern) != std::basic_string<T>::npos); }

template<typename T>
bool
contains(const std::basic_string<T> & str,const T * pattern_c_str)
{return contains(str,std::basic_string<T>(pattern_c_str)); }

template<typename T>
std::basic_string<T>
cutHead(const std::basic_string<T> & str,size_t size)
{
    FGASSERT(size <= str.size());
    return std::basic_string<T>(str.begin(),str.begin()+size);
}

template<class T>
std::basic_string<T>
cutRest(const std::basic_string<T> & str,size_t start=1)
{
    FGASSERT(start <= str.size());      // Can be size zero
    return std::basic_string<T>(str.begin()+start,str.end());
}

template<class T>
std::basic_string<T>
cutSubstr(const std::basic_string<T> & str,size_t start,size_t size)
{
    FGASSERT(start+size <= str.size());
    return  std::basic_string<T>(str.begin()+start,str.begin()+start+size);
}

// Returns at least size 1, with 1 additional for each split element:
template<class T>
Svec<std::basic_string<T> >
splitAtChar(const std::basic_string<T> & str,T ch)
{
    Svec<std::basic_string<T> >  ret;
    std::basic_string<T>                ss;
    for(T c : str) {
        if (c == ch) {
            ret.push_back(ss);
            ss.clear();
        }
        else
            ss.push_back(c);
    }
    ret.push_back(ss);
    return ret;
}

template<class T>
bool
beginsWith(const std::basic_string<T> & base,const std::basic_string<T> & pattern)
{
    return (base.rfind(pattern,0) != std::basic_string<T>::npos);
}

template<class T>
bool
beginsWith(const std::basic_string<T> & base,const T * pattern_c_str)
{
    return (base.rfind(pattern_c_str,0) != std::basic_string<T>::npos);
}

template<class T>
bool
endsWith(const std::basic_string<T> & str,const std::basic_string<T> & pattern)
{
    if (pattern.size() > str.size())
        return false;
    return (str.find(pattern,str.size()-pattern.size()) != std::basic_string<T>::npos);
}

template<class T>
bool
endsWith(const std::basic_string<T> & str,const T * pattern_c_str)
{
    return endsWith(str,std::basic_string<T>(pattern_c_str));
}

}

#endif
