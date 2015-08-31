//
// Copyright (c) 2015 Singular Inversions Inc. (facegen.com)
// Use, modification and distribution is subject to the MIT License,
// see accompanying file LICENSE.txt or facegen.com/base_library_license.txt
//
// Authors:     Andrew Beatty
// Created:     Jan 27, 2012
//
// Terminology:
//
// Path - any string identifying a directory or file, with relative of absolute path
// Base - the filename without directory or extension
// Dir - the directory (relative or absolute) without the filename. Must end with delimiter.
// Ext - the filename extension (or null string of none) without the period.
// Name - the Base and, if Ext is non null, then a dot then Ext.
//
// Notes:
//
// boost::filesystem::path uses a single string as its internal format, which is not
// very useful.
//

#ifndef FGPATH_HPP
#define FGPATH_HPP

#include "FgStdString.hpp"
#include "FgString.hpp"

#ifdef _WIN32
    #define FG_FS_DIR_SEP "\\"
#else
    #define FG_FS_DIR_SEP "/"
#endif

// Native directory separator converter:
inline std::string
fgNs(const std::string & path)
{
#ifdef _WIN32
    return fgReplace(path,'/','\\');
#else
    return fgReplace(path,'\\','/');
#endif
}

inline FgString
fgNfs(const FgString & path)
{
#ifdef _WIN32
    return path.replace('/','\\');
#else
    return path.replace('\\','/');
#endif
}

std::string
fgDirSep();                         // Directory separator ('/' on Unix, '\' on Windows)

// Doesn't handle double-delim paths (eg. //server/share) or non-root dirs on different
// drive letters (eg. C:reldir):
struct  FgPath
{
    // Empty string for relative path. If non-empty, must end in '/':
    FgString                drive;  // Only non-empty on Windows (eg C:)
    bool                    root;   // Path starts at root ? (otherwise relative)
    std::vector<FgString>   dirs;   // No delimiters in in dir names. Can begin with '..' entries.
    FgString                base;   // Base filename
    FgString                ext;    // Filename extension (no '.')

    FgPath() : root(false) {}

    // Leaving this implicit allows FgString to be used as an arg for functions taking FgPath.
    // Anything in 'path' not suffixed by a directory delimiter is assumed to be a file.
    FgPath(const FgString & path);

    FgPath(
        const FgString & d,bool r,const std::vector<FgString> & ds,
        const FgString & b,const FgString & e)
        : drive(d), root(r), dirs(ds), base(b), ext(e)
    {}

    // All delimiters in native form. All directory names end with delimiter:
    FgString
    str() const;

    // Only the first N directories with no filename:
    FgString
    str(size_t n) const;

    FgString
    dirOnly() const
    {return str(dirs.size()); }

    FgString
    nameOnly() const;

    FgString
    dirBase() const
    {return dirOnly() + base; }

    FgPath
    operator+(const FgPath &  rhs) const;

    void
    popDirs(uint);
};

FgString
fgPathToBase(const FgString & path);

FgString
fgPathToDirBase(const FgString & path);

FgString
fgPathToExt(const FgString & path);
std::string
fgPathToExt(const std::string & path);

FgString
fgPathToName(const FgString & path);

// Returns false if there is an existing extension not equal to the specified one:
bool
fgCheckSetExtension(
    std::string &       path,
    const std::string & ext);

#endif