/*
    genieutils - A library for reading and writing data files of genie
               engine games.
    Copyright (C) 2011 - 2013  Armin Preiml
    Copyright (C) 2015 - 2021  Mikko "Tapsa" P

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once
#include "I_Headers.h"

#ifndef GENIE_IFILE_H
#define GENIE_IFILE_H

/*
#include "G_ISerializable.h"

#include <fstream>
*/

namespace genie
{

//------------------------------------------------------------------------------
/// Interface providing file loading and saving for ISerializable objects.
//
class IFile : public ISerializable
{
public:

  //----------------------------------------------------------------------------
  IFile();
  IFile(const IFile &) = delete;
  IFile &operator=(const IFile &) = delete;

  //----------------------------------------------------------------------------
  virtual ~IFile();
  void freelock(void);

  //----------------------------------------------------------------------------
  /// Sets name of file to work with.
  ///
  /// @param fileName.
  //
  void setFileName(const char *fileName);

  //----------------------------------------------------------------------------
  /// Get name of file worked on.
  ///
  /// @return fileName.
  //
  const char *getFileName(void) const;

  //----------------------------------------------------------------------------
  /// Loads the object from file. Can only be called if fileName is already set.
  ///
  /// @exception std::ios_base::failure thrown if file can't be read (file
  ///                                   doesn't exist, insufficient rights...)
  ///
  //
  void load(void);

  //----------------------------------------------------------------------------
  /// Loads the object from file.
  ///
  /// @param fileName file name
  /// @exception std::ios_base::failure thrown if file can't be read (file
  ///                                   doesn't exist, insufficient rights...)
  //
  virtual void load(const char *fileName);

  //----------------------------------------------------------------------------
  /// Saves data to file. Can only be called if fileName is set through set
  /// method or the file was loaded using the load method.
  ///
  /// @exception std::ios_base::failure thrown if file can't be written (
  ///                                   insufficient rights...)
  //
  void save(void);

  //----------------------------------------------------------------------------
  /// Saves data to a different file.
  ///
  /// @param fileName file name
  /// @exception std::ios_base::failure thrown if file can't be written (
  ///                                   insufficient rights...)
  //
  virtual void saveAs(const char *fileName);

protected:

  //----------------------------------------------------------------------------
  /// The unload method will be called before loading an object, but only
  /// if the object was already loaded once.
  //
  virtual void unload(void);

private:
  std::string fileName_;

  std::ifstream fileIn_;

  bool loaded_ = false;
};

class IMemory : public std::streambuf
{
public:
  IMemory(char *begin, char *end)
  {
    setg(begin, begin, end);
  }

  std::streampos seekoff(std::streamoff off, std::ios_base::seekdir dir,
    std::ios_base::openmode which = std::ios_base::in | std::ios_base::out) override
  {
    switch (dir)
    {
      case std::ios_base::beg:
        setg(eback(), eback() + off, egptr());
        break;
      case std::ios_base::end:
        setg(eback(), egptr() + off, egptr());
        break;
      case std::ios_base::cur:
        gbump(static_cast<int>(off));
        break;
    }
    return gptr() - eback();
  }

  std::streampos seekpos(std::streampos pos,
    std::ios_base::openmode which = std::ios_base::in | std::ios_base::out) override
  {
    return seekoff(pos - std::streampos(std::streamoff(0)), std::ios_base::beg, which);
  }
};

class IMemoryStream : public std::istream
{
public:
  IMemoryStream(char *begin, char *end) :
    std::istream(&buffer_), buffer_(begin, end)
  {
    rdbuf(&buffer_);
  }

private:
  IMemory buffer_;
};

}

#endif // GENIE_IFILE_H
