//****************************************************************************
// BaseTypes.cpp -- $Id$
//
// Purpose
//   Provides implementations of header entities.
//
// Indentation
//   Four characters. No tabs!
//
// Modifications
//   2014-03-13 (MM) Added a numeric, case-insensitive string less functor.
//   2014-02-18 (MM) Added more to-string-vector methods.
//   2012-08-20 (MM) Added hex converters for three numeric types.
//   2012-07-29 (MM) Added string vector builders.
//   2012-07-03 (MM) Added a uuid getter.
//   2012-04-23 (MM) Added to-string methods.
//   2010-08-28 (MM) Got rid of EmptyString.
//   2010-08-10 (MM) Created.
//
// Copyright (c) 2010, 2012, 2014 Xiao Shengguang.  All rights reserved.
//****************************************************************************

#include "BaseTypes.h"
#include "Exception.h"
#include <cctype>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

namespace AOI
{
    namespace
    {
        int CStrCompNI(char const *string1, char const *string2);
    }

    bool StringEqualNoCase::operator()(String const &a, String const &b) const
    {
        return !StringLessNoCase()(a, b) && !StringLessNoCase()(b, a);
    }

    bool StringLessNoCase::operator()(String const &a, String const &b) const
    {
        // If this generates a compile-time error, "std::toupper" is
        // not the correct function to use.
        static_assert(sizeof(String::value_type) == 1, "Wide characters are not yet supported.");

        typedef String::const_iterator SCI;

        for (SCI ia = a.begin(), na = a.end(), ib = b.begin(), nb = b.end(); ia != na && ib != nb; ++ia, ++ib)
        {
            String::value_type ca = std::toupper(*ia);
            String::value_type cb = std::toupper(*ib);

            if (ca != cb)
                return ca < cb;
        }

        return a.size() < b.size();
    }

    bool StringLessNoCaseNumeric::operator()(String const &a, String const &b) const
    {
        static_assert(sizeof(String::value_type) == 1, "Wide characters are not yet supported.");

        return CStrCompNI(a.c_str(), b.c_str()) < 0;
    }

    String ToString(Int32 value)
    {
        try
        {
            return boost::lexical_cast<String>(value);
        }
        catch (boost::bad_lexical_cast const &)
        {
            throw Exception::InternalError(SL("Failed to convert an Int32 to a string."));
        }
    }

    String ToString(Int64 value)
    {
        try
        {
            return boost::lexical_cast<String>(value);
        }
        catch (boost::bad_lexical_cast const &)
        {
            throw Exception::InternalError(SL("Failed to convert an Int64 to a string."));
        }
    }

    String ToString(double value)
    {
        try
        {
            return boost::lexical_cast<String>(value);
        }
        catch (boost::bad_lexical_cast const &)
        {
            throw Exception::InternalError(SL("Failed to convert a double to a string."));
        }
    }

    String ToString(size_t value)
    {
        try
        {
            return boost::lexical_cast<String>(value);
        }
        catch (boost::bad_lexical_cast const &)
        {
            throw Exception::InternalError(SL("Failed to convert a size_t to a string."));
        }
    }

    StringVector ToStringVector(String const &s1)
    {
        return StringVector(1, s1);
    }

    StringVector ToStringVector(String const &s1, String const &s2)
    {
        StringVector v; v.reserve(2);
        v.push_back(s1);
        v.push_back(s2);
        return v;
    }

    StringVector ToStringVector(String const &s1, String const &s2, String const &s3)
    {
        StringVector v; v.reserve(3);
        v.push_back(s1);
        v.push_back(s2);
        v.push_back(s3);
        return v;
    }

    StringVector ToStringVector(String const &s1, String const &s2, String const &s3, String const &s4)
    {
        StringVector v; v.reserve(4);
        v.push_back(s1);
        v.push_back(s2);
        v.push_back(s3);
        v.push_back(s4);
        return v;
    }

    StringVector ToStringVector(String const &s1, String const &s2, String const &s3, String const &s4, String const &s5)
    {
        StringVector v; v.reserve(5);
        v.push_back(s1);
        v.push_back(s2);
        v.push_back(s3);
        v.push_back(s4);
        v.push_back(s5);
        return v;
    }

    StringVector ToStringVector(String const &s1, String const &s2, String const &s3, String const &s4, String const &s5, String const &s6)
    {
        StringVector v; v.reserve(6);
        v.push_back(s1);
        v.push_back(s2);
        v.push_back(s3);
        v.push_back(s4);
        v.push_back(s5);
        v.push_back(s6);
        return v;
    }

    StringVector ToStringVector(String const &s1, String const &s2, String const &s3, String const &s4, String const &s5, String const &s6, String const &s7)
    {
        StringVector v; v.reserve(7);
        v.push_back(s1);
        v.push_back(s2);
        v.push_back(s3);
        v.push_back(s4);
        v.push_back(s5);
        v.push_back(s6);
        v.push_back(s7);
        return v;
    }

    StringVector ToStringVector(String const &s1, String const &s2, String const &s3, String const &s4, String const &s5, String const &s6, String const &s7, String const &s8)
    {
        StringVector v; v.reserve(8);
        v.push_back(s1);
        v.push_back(s2);
        v.push_back(s3);
        v.push_back(s4);
        v.push_back(s5);
        v.push_back(s6);
        v.push_back(s7);
        v.push_back(s8);
        return v;
    }

    namespace
    {
        String::value_type const *BYTE_TO_HEX[] =
        {
            "00", "01", "02", "03", "04", "05", "06", "07", "08", "09", "0A", "0B", "0C", "0D", "0E", "0F",
            "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "1A", "1B", "1C", "1D", "1E", "1F",
            "20", "21", "22", "23", "24", "25", "26", "27", "28", "29", "2A", "2B", "2C", "2D", "2E", "2F",
            "30", "31", "32", "33", "34", "35", "36", "37", "38", "39", "3A", "3B", "3C", "3D", "3E", "3F",
            "40", "41", "42", "43", "44", "45", "46", "47", "48", "49", "4A", "4B", "4C", "4D", "4E", "4F",
            "50", "51", "52", "53", "54", "55", "56", "57", "58", "59", "5A", "5B", "5C", "5D", "5E", "5F",
            "60", "61", "62", "63", "64", "65", "66", "67", "68", "69", "6A", "6B", "6C", "6D", "6E", "6F",
            "70", "71", "72", "73", "74", "75", "76", "77", "78", "79", "7A", "7B", "7C", "7D", "7E", "7F",
            "80", "81", "82", "83", "84", "85", "86", "87", "88", "89", "8A", "8B", "8C", "8D", "8E", "8F",
            "90", "91", "92", "93", "94", "95", "96", "97", "98", "99", "9A", "9B", "9C", "9D", "9E", "9F",
            "A0", "A1", "A2", "A3", "A4", "A5", "A6", "A7", "A8", "A9", "AA", "AB", "AC", "AD", "AE", "AF",
            "B0", "B1", "B2", "B3", "B4", "B5", "B6", "B7", "B8", "B9", "BA", "BB", "BC", "BD", "BE", "BF",
            "C0", "C1", "C2", "C3", "C4", "C5", "C6", "C7", "C8", "C9", "CA", "CB", "CC", "CD", "CE", "CF",
            "D0", "D1", "D2", "D3", "D4", "D5", "D6", "D7", "D8", "D9", "DA", "DB", "DC", "DD", "DE", "DF",
            "E0", "E1", "E2", "E3", "E4", "E5", "E6", "E7", "E8", "E9", "EA", "EB", "EC", "ED", "EE", "EF",
            "F0", "F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "FA", "FB", "FC", "FD", "FE", "FF",
        };

        typedef __int32 unsigned UInt32;
        typedef __int64 unsigned UInt64;

        int    const BYTE_BITS = 8;
        Byte   const BYTE_MASK = 0xFF;
        String const ZERO(SL("0"));

        String UInt32ToHexAux(UInt32 v)
        {
            String s; s.reserve(sizeof(UInt32) * 2);
            s.append(BYTE_TO_HEX[v >> (BYTE_BITS * 3) & BYTE_MASK]);
            s.append(BYTE_TO_HEX[v >> (BYTE_BITS * 2) & BYTE_MASK]);
            s.append(BYTE_TO_HEX[v >> (BYTE_BITS * 1) & BYTE_MASK]);
            s.append(BYTE_TO_HEX[v >> (BYTE_BITS * 0) & BYTE_MASK]);
            return s;
        }

        String UInt64ToHexAux(UInt64 v)
        {
            String s; s.reserve(sizeof(UInt64) * 2);
            s.append(BYTE_TO_HEX[v >> (BYTE_BITS * 7) & BYTE_MASK]);
            s.append(BYTE_TO_HEX[v >> (BYTE_BITS * 6) & BYTE_MASK]);
            s.append(BYTE_TO_HEX[v >> (BYTE_BITS * 5) & BYTE_MASK]);
            s.append(BYTE_TO_HEX[v >> (BYTE_BITS * 4) & BYTE_MASK]);
            s.append(BYTE_TO_HEX[v >> (BYTE_BITS * 3) & BYTE_MASK]);
            s.append(BYTE_TO_HEX[v >> (BYTE_BITS * 2) & BYTE_MASK]);
            s.append(BYTE_TO_HEX[v >> (BYTE_BITS * 1) & BYTE_MASK]);
            s.append(BYTE_TO_HEX[v >> (BYTE_BITS * 0) & BYTE_MASK]);
            return s;
        }

        UInt64 HexToUInt64(String const &s)
        {
            UInt64 u = 0;
            for (String::const_iterator i = s.begin(), n = s.end(); i != n; ++i)
                switch (*i)
                {
                case '0':           (u <<= 4) |= 0x0; break;
                case '1':           (u <<= 4) |= 0x1; break;
                case '2':           (u <<= 4) |= 0x2; break;
                case '3':           (u <<= 4) |= 0x3; break;
                case '4':           (u <<= 4) |= 0x4; break;
                case '5':           (u <<= 4) |= 0x5; break;
                case '6':           (u <<= 4) |= 0x6; break;
                case '7':           (u <<= 4) |= 0x7; break;
                case '8':           (u <<= 4) |= 0x8; break;
                case '9':           (u <<= 4) |= 0x9; break;
                case 'A': case 'a': (u <<= 4) |= 0xA; break;
                case 'B': case 'b': (u <<= 4) |= 0xB; break;
                case 'C': case 'c': (u <<= 4) |= 0xC; break;
                case 'D': case 'd': (u <<= 4) |= 0xD; break;
                case 'E': case 'e': (u <<= 4) |= 0xE; break;
                case 'F': case 'f': (u <<= 4) |= 0xF; break;
                default:
                    {
                        String const fmt = SL("Invalid character in \"%s\" in hex-to-numeric conversion.");
                        throw Exception::InvalidArgument((boost::format(fmt) % s).str());
                    }
                }
            return u;
        }
    }

    String Int32ToHex(Int32 v)
    {
        typedef int CheckInt32a[sizeof(Int32) == 4];
        typedef int CheckInt32b[sizeof(Int32) == sizeof(UInt32)];
        if (v == 0) return ZERO;
        String s = UInt32ToHexAux(static_cast<UInt32>(v));
        return s.substr(s.find_first_not_of(SL('0')));
    }

    String Int64ToHex(Int64 v)
    {
        typedef int CheckInt64a[sizeof(Int64) == 8];
        typedef int CheckInt64b[sizeof(Int64) == sizeof(UInt64)];
        if (v == 0) return ZERO;
        String s = UInt64ToHexAux(static_cast<UInt64>(v));
        return s.substr(s.find_first_not_of(SL('0')));
    }

    String DoubleToHex(double v, bool zeroAs0)
    {
        typedef int CheckDouble1[sizeof(double) == 8];
        typedef int CheckDouble2[sizeof(double) == sizeof(UInt64)];
        if (v == 0 && zeroAs0) return ZERO;
        union { double d; UInt64 u; } x;
        x.d = v;
        return UInt64ToHexAux(static_cast<UInt64>(x.u));
    }

    Int32 HexToInt32(String const &s)
    {
        if (s.empty()) return 0;
        size_t const maxsize = sizeof(Int32) * 2;
        if (s.size() > maxsize)
        {
            String const fmt = SL("String length in hex-to-int32 conversion cannot exceed %d (\"%s\").");
            throw Exception::InvalidArgument((boost::format(fmt) % maxsize % s).str());
        }
        return static_cast<Int32>(HexToUInt64(s));
    }

    Int64 HexToInt64(String const &s)
    {
        if (s.empty()) return 0;
        size_t const maxsize = sizeof(Int64) * 2;
        if (s.size() > maxsize)
        {
            String const fmt = SL("String length in hex-to-int64 conversion cannot exceed %d (\"%s\").");
            throw Exception::InvalidArgument((boost::format(fmt) % maxsize % s).str());
        }
        return static_cast<Int64>(HexToUInt64(s));
    }

    double HexToDouble(String const &s)
    {
        if (s.empty() || s.size() == 1 && s[0] == SL('0')) return 0;
        size_t const reqsize = sizeof(double) * 2;
        if (s.size() > reqsize)
        {
            String const fmt = SL("String length in hex-to-double conversion must equal %d (\"%s\").");
            throw Exception::InvalidArgument((boost::format(fmt) % reqsize % s).str());
        }
        union { double d; UInt64 u; } x;
        x.u = HexToUInt64(s);
        return x.d;
    }

    String GetRandomUuid()
    {
        boost::uuids::uuid uuid = boost::uuids::random_generator()();
        return boost::lexical_cast<String>(uuid);
    }

    namespace
    {
        // Pulled from "Root3070/u/c_str.c", renamed, and tweaked.
        int CStrCompNI(char const *string1, char const *string2)
        {
            int const LESS = -1;
            int const SAME =  0;
            int const MORE =  1;

            int c1 = *(unsigned char *)string1;
            int c2 = *(unsigned char *)string2;

            while (c1 != '\0' && c2 != '\0')
            {
                if (std::isdigit(c1) && std::isdigit(c2))
                {
                    int z = 0;

                    for (; c1 == '0'; ++z, c1 = *(unsigned char *)++string1);
                    for (; c2 == '0'; --z, c2 = *(unsigned char *)++string2);

                    if (std::isdigit(c1) || std::isdigit(c2))
                    {
                        int         n;
                        char const *s1;
                        char const *s2;

                        if (!std::isdigit(c1)) return LESS;
                        if (!std::isdigit(c2)) return MORE;

                        for (s1 = string1; std::isdigit(c1 = *(unsigned char *)++string1););
                        for (s2 = string2; std::isdigit(c2 = *(unsigned char *)++string2););

                        if (string1 - s1 != string2 - s2)
                            return (string1 - s1 < string2 - s2) ? LESS : MORE;

                        if ((n = std::strncmp(s1, s2, string1 - s1)) != SAME)
                            return (n < SAME) ? LESS : MORE;
                    }

                    if (z != 0)
                        return (z < 0) ? LESS : MORE;

                    if (c1 == '\0' || c2 == '\0')
                        break;
                }

                if (std::isupper(c1)) c1 = std::tolower(c1);
                if (std::isupper(c2)) c2 = std::tolower(c2);

                if (c1 != c2)
                      return (c1 < c2) ? LESS : MORE;

                c1 = *(unsigned char *)++string1;
                c2 = *(unsigned char *)++string2;
            }

            if (c1 != c2)
                return (c1 < c2) ? LESS : MORE;

            return SAME;
        }
    }
}
