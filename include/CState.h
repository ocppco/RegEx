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

#ifndef INCLUDE_CSTATE_H_
#define INCLUDE_CSTATE_H_

#include <stdarg.h>
#include <stddef.h>
#include <string>
#include <map>
#include <iostream>
#include <stdexcept>
#include <string>

class CState {
  bool endState;
  bool mark;
  std::map<int, size_t> nextState;
 public:
  CState();
  ~CState();

  void clear();
  
  void setEndState(bool endState);
  bool getEndState();

  void setMark(bool mark);
  bool getMark();

  void setNextState(int charSign, size_t nextStatePointer);
  size_t getNextState(int charSign);
};

#endif  // INCLUDE_CSTATE_H_
