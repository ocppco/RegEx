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

#include "./include/CState.h"

// Privat

// Konstruktoren
CState::CState() {
  clear();
}

CState::~CState() {
}

// Public Funktionen
void CState::clear() {
  endState=false;
  mark=false;
}

void CState::setEndState(bool endState) {
  this->endState=endState;
}

bool CState::getEndState() {
  return endState;
}

void CState::setMark(bool mark) {
  this->mark=mark;
}

bool CState::getMark() {
  return mark;
}

void CState::setNextState(int charSign, size_t nextStatePointer) {
  std::map<int, size_t>::iterator it;
  
  it=nextState.find(charSign);
  if(it!=nextState.end()) {
    char err[]="Sign   exist.";
    err[5]=(char)charSign;
    throw std::logic_error(err);
  }
  
  nextState.insert(std::pair<int, size_t>(charSign, nextStatePointer));
}

size_t CState::getNextState(int charSign) {
  size_t r=0;
  std::map<int, size_t>::iterator it;
  
  it=nextState.find(charSign);
  if(it!=nextState.end()) {
    r=it->second;
  }
  
  return r;
}

