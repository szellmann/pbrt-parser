// ======================================================================== //
// Copyright 2015-2019 Ingo Wald                                            //
//                                                                          //
// Licensed under the Apache License, Version 2.0 (the "License");          //
// you may not use this file except in compliance with the License.         //
// You may obtain a copy of the License at                                  //
//                                                                          //
//     http://www.apache.org/licenses/LICENSE-2.0                           //
//                                                                          //
// Unless required by applicable law or agreed to in writing, software      //
// distributed under the License is distributed on an "AS IS" BASIS,        //
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. //
// See the License for the specific language governing permissions and      //
// limitations under the License.                                           //
// ======================================================================== //

#pragma once

#include "pbrtParser/math.h"

// boost
#if PBRT_PARSER_HAVE_BOOST
#include <boost/utility/string_ref.hpp>
#include <boost/iostreams/device/mapped_file.hpp>
#endif
// stl
#include <stddef.h>
#include <stdio.h>
#include <stdexcept>

/*! namespace for all things pbrt parser, both syntactical *and* semantical parser */
namespace pbrt {

  namespace syntactic {

    struct PBRT_PARSER_INTERFACE StreamPos {
      //! absolute position in stream
      std::size_t fpos = 0;

      //! current line
      int line = 1;

      //! current column
      int col = 0;
    };

    struct PBRT_PARSER_INTERFACE CFile {
      CFile(const std::string &fn): name(fn) {
        file = fopen(fn.c_str(),"r");
        if (!file)
          throw std::runtime_error("could not open file '"+fn+"'");
      }
      void close() { fclose(file); file = NULL; }
      virtual ~CFile() { 
        if (file) fclose(file);
      }
      /*! get name of the file */
      std::string getFileName() const { return name; }
      /*! check if file is open */
      bool good() const {
        return file != NULL;
      }
      /*! check if eof was reached */
      bool eof() const {
        return feof(file);
      }
      /*! get current pos in file */
      std::size_t tellg() const {
        return ftell(file);
      }
      /*! get next character in file */
      int getc() {
        // A little trick so we don't have to call ungetc on peek()
        if (peeked >= 0) {
          int c = peeked;
          peeked = -1;
          return c;
        }

        int c = fgetc(file);
        pos.fpos++;
        if (c == '\n') {
          pos.line++;
          pos.col = 0;
        } else {
          pos.col++;
        }

        return c;
      }
      /*! unget last character in file */
      void ungetc(int c) {
        if (peeked >= 0) 
          throw std::runtime_error("can't push back more than one char ...");
        peeked = c;
      }
    private:
      StreamPos pos;
      std::string name;
      FILE *file;
      int peeked = -1;
    };

#if PBRT_PARSER_HAVE_BOOST
    /*! file name and handle, to be used by tokenizer and loc */
    struct PBRT_PARSER_INTERFACE MappedFile {
      MappedFile(const std::string &fn): name(fn), file(fn) {
        if (!file.is_open())
          throw std::runtime_error("could not open file '"+fn+"'");
        str = boost::string_ref(file.data(), file.size());
      }
      void close() { file.close(); }
      /*! get name of the file */
      std::string getFileName() const { return name; }
      /*! check if file is open */
      bool good() const {
        return file.is_open();
      }
      /*! check if eof was reached */
      bool eof() const {
        return pos.fpos >= file.size();
      }
      /*! get current pos in file */
      std::size_t tellg() const {
        return pos.fpos;
      }
      /*! get next character in file */
      int getc() {
        if (pos.fpos + 1 >= str.length())
          return EOF;

        int c = str[pos.fpos++];
        if (c == '\n') {
          pos.line++;
          pos.col = 0;
        } else {
          pos.col++;
        }

        return c;
      }
      /*! unget last character in file */
      void ungetc(int c) {
        if (pos.fpos == 0) 
          throw std::runtime_error("can't unget char ...");
        pos.fpos--;
      }

    private:
      StreamPos pos;
      std::string name;
      boost::iostreams::mapped_file_source file;
      boost::string_ref str;
    };
#endif

    template <class File>
    class PBRT_PARSER_INTERFACE Stream {
    public:
        Stream(File& file): file_(file) {
          buffer_.reserve(100);
        }

        template <class Func>
        bool skip_if(Func skip_func) {
          int c = file_.getc();

          while (skip_func(c)) {
            c = file_.getc();
            if (c < 0)
              return false;
          }

          return true;
        }
        bool skip_to(char to) {
          return skip_if([to](int c) { return c != to; });
        }
    private:
        File& file_;
        std::vector<char> buffer_;
    };
  } // ::pbrt::syntactic
} // ::pbrt
