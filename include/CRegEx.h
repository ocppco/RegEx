/*  
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) 2018  Bernd Juelich

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef INCLUDE_CREGEX_H_
#define INCLUDE_CREGEX_H_

#include <vector>
#include <stack>
#include <string>
#include <set>
#include "CState.h"

#define STATETOKENSIZE 13

class CRegEx {
  bool change;
  bool end;
  char letter;
  size_t index;
  size_t indexOld;
  size_t currentState;
  size_t endState;
  size_t sym, symOld;
  size_t stateToken[STATETOKENSIZE][256];

  CState stateDefault;
  std::vector<CState> q;
  std::vector<size_t> F;
  std::string source;
  std::set<size_t> stackState;

  size_t getSym();
  std::string findString(size_t indOld);
  void REGToDEA();
 public:
  CRegEx();
  ~CRegEx();

  void operator()(const std::string &source);
  size_t operator[](const std::string& source);
  size_t match(const std::string source, size_t begin, size_t &end);
  
  #ifdef DEBUG
  std::string print();
  #endif
};

#endif  // INCLUDE_CREGEX_H_
