#ifndef BASIC_TABLE_H
#define BASIC_TABLE_H

#include <cstddef>
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <type_traits>
#include <algorithm>

#ifdef _MSC_VER
#define constexpr const
#endif

template<typename charT>
class basic_Table
{
public:

    /// 针对流的一些操作
    using char_type       = charT;
    using string_t        = std::basic_string<charT>;
    using ostream_t       = std::basic_ostream<charT>;
    using istream_t       = std::basic_istream<charT>;
    using ostringstream_t = std::basic_ostringstream<charT>;
    using ios_t           = std::basic_ios<charT>;
    using iostate         = std::ios_base::iostate;
    using fmtflags        = std::ios_base::fmtflags;
    using streamsize      = std::streamsize;
    using streamoff       = std::streamoff;

    bool good() const { return buffer.good(); }
    bool eof () const { return buffer.eof(); }
    bool fail() const { return buffer.fail(); }
    bool bad () const { return buffer.bad(); }

    bool operator !() const;
    explicit operator bool() const;
    iostate rdstate() const;
    void setstate(iostate state);
    void clear(iostate state = std::ios_base::goodbit);
    fmtflags flags() const;
    fmtflags flags(fmtflags fmtfl);
    fmtflags setf(fmtflags fmtfl);
    fmtflags setf(fmtflags fmtfl, fmtflags mask);
    void unsetf(fmtflags mask);
    streamsize precision() const;
    streamsize precision(streamsize prec);
    char narrow(char_type wc, char dfault) const;
    char_type widen(char c) const;


public:

    /// 定义表格是否存在上边框、下边框、水平分隔线，表头
    static constexpr unsigned long upper_border = 1UL << 0;
    static constexpr unsigned long lower_border = 1UL << 1;
    static constexpr unsigned long horizon_line = 1UL << 2;
    static constexpr unsigned long header       = 1UL << 3;

    /// 定义表格边框的属性
    struct Property;

    basic_Table(Property pty);
    basic_Table &operator << (ostream_t &(*pf)(ostream_t &));
    basic_Table &operator << (ios_t &(*pf)(ios_t &));
    basic_Table &operator << (std::ios_base &(*pf)(std::ios_base &));
    template<typename T>
    basic_Table &operator << (T t);

    ostream_t &output(ostream_t &) const;
    istream_t &input(istream_t &);

    void set_iecm(const std::map<char_type, char_type> &m) { iecm = m; }
    void set_oecm(const std::map<char_type, char_type> &m) { oecm = m; }
    void set_property(const Property &pty) { property = pty; }


private:

    string_t output_trans(const string_t &) const;
    string_t input_trans(const string_t &) const;

    string_t generate_upper_border(const std::vector<std::size_t> &colwidth) const
        { return do_generate_border(colwidth, property.upper_left,
                                    property.upper_mid, property.upper_right, property.hsep); }
    string_t generate_lower_border(const std::vector<std::size_t> &colwidth) const
        { return do_generate_border(colwidth, property.lower_left,
                                    property.lower_mid, property.lower_right, property.hsep); }
    string_t generate_horizon_line(const std::vector<std::size_t> &colwidth) const
        { return do_generate_border(colwidth, property.mid_left,
                                    property.mid_mid, property.mid_right, property.hsep); }
    string_t do_generate_border(const std::vector<std::size_t> &colwidth,
                                const string_t&left, const string_t &mid,
                                const string_t &right, char_type sep) const;

    struct Elem;
    ostringstream_t buffer;

    mutable std::map<char_type, char_type> oecm; /// output escaped-char map
    mutable std::map<char_type, char_type> iecm; /// input escaped-char map

    Property property;
    std::vector<std::vector<Elem>> data;
    std::size_t curline;
    std::size_t table_width;
};


template<typename charT>
struct basic_Table<charT>::Property
{
    string_t upper_left;
    string_t upper_mid;
    string_t upper_right;

    string_t mid_left;
    string_t mid_mid;
    string_t mid_right;

    string_t lower_left;
    string_t lower_mid;
    string_t lower_right;

    string_t vsep_left;
    string_t vsep_mid;
    string_t vsep_right;
    char_type hsep;

    unsigned long borders;
};


template<typename charT>
struct basic_Table<charT>::Elem
{
    string_t cont;
    bool is_left_align;
};



template<typename charT>
basic_Table<charT>::basic_Table(Property pty):
        property(pty), curline(0), table_width(0)
{
    buffer << std::left;
}


template<typename charT>
basic_Table<charT> &
basic_Table<charT>::operator << (ostream_t &(*pf)(ostream_t &))
{
    decltype(pf) pEndl = std::endl;
    if(pf == pEndl)
    {
        ++curline;
        if(!data.empty() && table_width < data.back().size())
            table_width = data.back().size();
        return *this;
    }

    decltype(pf) pEnds = std::ends;
    if(pf != pEnds)
        buffer << pf;

    return *this;
}


template<typename charT>
basic_Table<charT> &
basic_Table<charT>::operator << (ios_t &(*pf)(ios_t &))
{
    buffer << pf;
    return *this;
}


template<typename charT>
basic_Table<charT> &
basic_Table<charT>::operator << (std::ios_base &(*pf)(std::ios_base &))
{
    buffer << pf;
    return *this;
}


template<typename charT>
template<typename T>
basic_Table<charT> &
basic_Table<charT>::operator << (T t)
{
    buffer.str(string_t());
    buffer << std::setfill(widen(' ')) << std::setw(0) << t;

    if(!std::is_class<typename
        std::remove_cv<typename std::remove_reference<T>::type>::type>::value
       || std::is_convertible<T, string_t>::value)
    {
        data.resize(curline + 1);
        data.back().push_back({buffer.str(), (flags() & std::ios_base::left) != 0});
    }

    return *this;
}


template<typename charT>
typename
basic_Table<charT>::string_t
basic_Table<charT>::output_trans(const string_t &src) const
{
    string_t ret;
    char_type backslash = widen('\\'), quote = widen('\"');

    /// 反斜线和引号只能使用预定义的转义字符
    oecm[backslash] = backslash;
    oecm[quote] = quote;

    for(auto ch: src)
    {
        auto iter = oecm.find(ch);
        if(iter != oecm.cend())
        {
            ret.push_back(backslash);
            ret.push_back(iter->second);
        }
        else
            ret.push_back(ch);
    }

    bool has_quote = false;
    for(const auto &vsep:
        {property.vsep_left, property.vsep_mid, property.vsep_right})
        has_quote = has_quote || (src.find(vsep) != string_t::npos && !vsep.empty());
    if(has_quote)
        ret = quote + ret + quote;

    return ret;
}


template<typename charT>
typename
basic_Table<charT>::string_t
basic_Table<charT>::input_trans(const string_t &src) const
{

    return src;
}



template<typename charT>
typename
basic_Table<charT>::string_t
basic_Table<charT>::do_generate_border(const std::vector<std::size_t> &colwidth,
                                       const string_t &left, const string_t &mid,
                                       const string_t &right, char_type sep) const
{
    string_t ret = left;
    for(decltype(colwidth.size()) col = 0; col != colwidth.size(); ++col)
    {
        ret += string_t(colwidth[col], sep);
        if(col != colwidth.size() - 1)
            ret += mid;
    }
    return ret + right;
}



template<typename charT>
typename
basic_Table<charT>::ostream_t &
basic_Table<charT>::output(ostream_t &os) const
{
    if(data.empty()) return os;

    std::vector<std::vector<Elem>> trans; /// height: data.size(), width: table_width;
    std::vector<std::size_t> colwidth(table_width); /// length of each column

    for(decltype(data.size()) row = 0; row != data.size(); ++row)
    {
        trans.resize(row + 1);
        for(decltype(data[row].size()) col = 0; col != data[row].size(); ++col)
        {
            auto cont = output_trans(data[row][col].cont);
            colwidth[col] = std::max(colwidth[col], cont.size());
            trans.back().push_back({cont, data[row][col].is_left_align});
        }
        trans.back().resize(table_width);
    }

    if(property.borders & upper_border)
        os << generate_upper_border(colwidth) << std::endl;
    for(decltype(trans.size()) row = 0; row != trans.size(); ++row)
    {
        os << property.vsep_left;
        for(decltype(trans[row].size()) col = 0 ; col != trans[row].size(); ++col)
        {
            os << (trans[row][col].is_left_align ? std::left : std::right)
                << std::setw(colwidth[col]) << trans[row][col].cont;

            if(col != trans[row].size() - 1)
                os << property.vsep_mid;
        }
        os << property.vsep_right << std::endl;

        if((row == 0 && (property.borders & header))
           || (property.borders & horizon_line))
               if(row != trans.size() - 1)
            os << generate_horizon_line(colwidth) << std::endl;
    }
    if(property.borders & lower_border)
        os << generate_lower_border(colwidth) << std::endl;

    return os;
}



template<typename charT>
typename
basic_Table<charT>::istream_t &
basic_Table<charT>::input(istream_t &is)
{

    return is;
}



template<typename charT>
inline
std::basic_ostream<charT> &
operator <<(std::basic_ostream<charT> &os, const basic_Table<charT> &table)
{
    return table.output(os);
}

template<typename charT>
inline
std::basic_istream<charT> &
operator >>(std::basic_istream<charT> &is, basic_Table<charT> &table)
{
    return table.input(is);
}


template<typename charT>
inline
bool basic_Table<charT>::operator !() const { return !buffer; }


template<typename charT>
inline
basic_Table<charT>::operator bool() const
 { return buffer ? true : false; }


template<typename charT>
inline typename
basic_Table<charT>::iostate
basic_Table<charT>::rdstate() const { return buffer.rdstate(); }


template<typename charT>
inline
void basic_Table<charT>::setstate(iostate state)
 { buffer.setstate(state); }


template<typename charT>
inline
void basic_Table<charT>::clear(iostate state)
 { buffer.clear(state); }


template<typename charT>
inline
char basic_Table<charT>::narrow(char_type wc, char dfault) const
 { return buffer.narrow(wc, dfault); }


template<typename charT>
inline typename
basic_Table<charT>::char_type
basic_Table<charT>::widen(char c) const { return buffer.widen(c); }


template<typename charT>
inline typename
basic_Table<charT>::fmtflags
basic_Table<charT>::flags() const { return buffer.flags(); }


template<typename charT>
inline typename
basic_Table<charT>::fmtflags
basic_Table<charT>::flags(fmtflags fmtfl) { return buffer.flags(fmtfl); }


template<typename charT>
inline typename
basic_Table<charT>::fmtflags
basic_Table<charT>::setf(fmtflags fmtfl) { return buffer.setf(fmtfl); }


template<typename charT>
inline typename
basic_Table<charT>::fmtflags
basic_Table<charT>::setf(fmtflags fmtfl, fmtflags mask)
 { return buffer.setf(fmtfl, mask); }


template<typename charT>
inline
void basic_Table<charT>::unsetf(fmtflags mask)
 { buffer.unsetf(mask); }


template<typename charT>
inline typename
basic_Table<charT>::streamsize
basic_Table<charT>::precision() const { return buffer.precision(); }


template<typename charT>
inline typename
basic_Table<charT>::streamsize
basic_Table<charT>::precision(streamsize prec)
 { return buffer.precision(prec); }


#endif // BASIC_TABLE_H
