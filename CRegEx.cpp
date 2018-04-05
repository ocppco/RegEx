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

#include "./include/CRegEx.h"

//Global
std::string charToString(const char* format, ...) {
  char buffer[256];
  std::string str;
  
  va_list arglist;

  va_start(arglist, format);
  vsnprintf(buffer, sizeof(buffer), format, arglist);
  va_end(arglist);

  str=buffer;
  
  return str;
}

// Privat
size_t CRegEx::getSym() {
  size_t currentState=1;
  size_t oldState;

  indexOld=index;
  symOld=sym;

  if(index>=source.size()) {
    sym=-1;
    return sym;
  }

  do { 
    oldState=currentState;
    currentState=stateToken[currentState][source[index]&0xFF];

    if(currentState==2) {
      letter=source[index]&0xFF;
    }

    index++;
    if(index>source.size()) {
      sym=oldState;
      return sym;
    }
  }while(currentState!=3);
  
  index--;
  sym=oldState;

  return sym;
}

std::string CRegEx::findString(size_t indOld) {
  size_t indexSave=index;
  size_t indexOldSave=indexOld;
  size_t symSave=sym;
  std::string res;
  
  index=indOld;
  
  getSym();
  while(sym==2 || sym==12) {
    res+=letter;
    getSym();
  }
  
  index=indexSave;
  indexOld=indexOldSave;
  sym=symSave;
  
  return res;
}

void CRegEx::REGToDEA() {
  bool sub;
  bool selectionState[256];
  char c;
  size_t beginState=currentState;
  std::set<size_t>::iterator it;
  std::set<size_t> markState;
  
  markState.insert(beginState);
  
  do {
    sub=true;
    stateDefault.clear();
    switch(getSym()) {
      //End
      case -1:
        for(it=markState.begin(); it!=markState.end(); ++it) {  
          if(q[*it].getEndState() || q[*it].getMark()) {
            q[*it].setMark(false);
            q[*it].setEndState(false);
            if(endState==0) {
              q.push_back(stateDefault);
              for(size_t j=0; j<256; j++) {
                if(q[*it].getNextState(j)==0) {
                  try {
                    q[*it].setNextState(j, q.size()-1);
                  }
                  catch(std::exception &e) {
                    throw std::logic_error(charToString("%s\nSee previous error.", e.what()));
                  }
                }
              }
              endState=q.size()-1;
              markState.erase(it);
            }
            else {
              for(size_t j=0; j<256; j++) {
                if(q[*it].getNextState(j)==0) {
                  try {
                    q[*it].setNextState(j, endState);
                  }
                  catch(std::exception &e) {
                    throw std::logic_error(charToString("%s\nSee previous error.", e.what()));
                  }
                }
              }
            }
          }
        }
        stackState.clear();
        return;

      //letter
      case 2:
        c=letter;
        //look a head
        switch(getSym()) {
          //finish
          case -1:
            sub=false;
          //letter
          case 2:
          //(
          case 7:
          //)
          case 8:
          //|
          case 9:
          //[
          case 10:
            if(q[currentState].getNextState(c)==0)
            { 
              q.push_back(stateDefault);
              for(it=markState.begin(); it!=markState.end(); ++it) {
                if(q[*it].getMark()) {
                  q[*it].setMark(false);
                  try {
                    q[*it].setNextState(c, q.size()-1);
                  }
                  catch(std::exception &e) {
                    throw std::logic_error(charToString("%s\nSee previous error.", e.what()));
                  }
                  markState.erase(it);
                }
              }
              
              currentState=q.size()-1;              
            }
            else {
              q[currentState].setMark(false);
              currentState=q[currentState].getNextState(c);
            }
            if(sub) {
              index=indexOld;
            }
            break;

          //*
          case 5:
            for(it=markState.begin(); it!=markState.end(); ++it) {
              if(q[*it].getMark()) {
                if(q[*it].getNextState(c)!=0) {
                  if(q[*it].getNextState(c)==*it) {
                    continue;
                  }
                  else {
                    throw std::logic_error(charToString("Next State[%c] is engaged at %llu.", source[indexOld], indexOld));
                  }  
                }
                else {
                  try {
                    q[*it].setNextState(c, *it);
                  }
                  catch(std::exception &e) {
                    throw std::logic_error(charToString("%s\nSee previous error.", e.what()));
                  }
                }
              }
            }
            break;

          //?
          case 6:
            q.push_back(stateDefault);
            for(it=markState.begin(); it!=markState.end(); ++it) {
              if(q[*it].getMark()) {
                if(q[*it].getNextState(c)!=0) {
                  throw std::logic_error(charToString("Next State[%c] is engaged at %llu.", source[indexOld], indexOld));
                }
                try {
                  q[*it].setNextState(c, q.size()-1);
                }
                catch(std::exception &e) {
                  throw std::logic_error(charToString("%s\nSee previous error.", e.what()));
                }
              }
            }
            currentState=q.size()-1;
            break;

          //Error
          default:
            throw std::invalid_argument(charToString("Next Symbol[%c] unknow at %llu.", source[indexOld], indexOld));
        }
        change=true;
        q[currentState].setMark(true);
        markState.insert(currentState);
        markState.insert(beginState);
        break;

      //(
      case 7:
        try {
          REGToDEA();
        }
        catch(std::exception &e) {
          throw std::logic_error(charToString("%s\nSee previous error.", e.what()));
        }
        for(it=stackState.begin(); it!=stackState.end(); ++it) {
          markState.insert(*it);
        }
        break;

      //)
      case 8:
        if(!change) {
          throw std::invalid_argument(charToString("Next Symbol[%c] unknow at %llu.", (char)source[indexOld], indexOld));
        }
        switch(getSym()) {
          //finish
          case -1:
            sub=false;
          //letter
          case 2:
          //(
          case 7:
          //)
          case 8:
          //|
          case 9:
          //[
          case 10:
            if(sub) {
              index=indexOld;
            }
            for(it=markState.begin(); it!=markState.end(); ++it) {
              if(q[*it].getEndState()) {
                q[*it].setMark(true);
                q[*it].setEndState(false);
              }
            }
            break;

          //*
          case 5:
            q[beginState].setMark(true);
            q[beginState].setEndState(false);
            for(it=markState.begin(); it!=markState.end(); ++it) {
              if((q[*it].getEndState() || q[*it].getMark()) && *it!=beginState) {
                q[*it].setMark(true);
                q[*it].setEndState(false);
                for(size_t j=0; j<256; j++) {
                  if(q[beginState].getNextState(j)!=0) {
                    try {
                      q[*it].setNextState(j, q[beginState].getNextState(j));
                    }
                    catch(std::exception &e) {
                      throw std::logic_error(charToString("%s\nSee previous error.", e.what()));
                    }
                  }
                }
              }
            }
            break;

          //?
          case 6:
            q[beginState].setMark(true);
            q[beginState].setEndState(false);
            for(it=markState.begin(); it!=markState.end(); ++it) {
              if(q[*it].getEndState() || q[*it].getMark()) {
                q[*it].setMark(true);
                q[*it].setEndState(false);
              }
            }
            break;

          default:
            throw std::invalid_argument(charToString("Unknow char %c by %llu.", source[indexOld], indexOld));
        }
        stackState.clear();
        stackState=markState;
        return;

      //|
      case 9:
        if(!change) {
          throw std::invalid_argument(charToString("Next Symbol[%c] unknow at %llu.", (char)source[indexOld], indexOld));
        }
        for(it=markState.begin(); it!=markState.end(); ++it) {
          if(q[*it].getMark()) {
            q[*it].setMark(false);
            q[*it].setEndState(true);
          }
        }
        q[beginState].setMark(true);
        currentState=beginState;
        markState.insert(currentState);
        break;

      //[
      case 10:
        for(size_t i=0; i<256; i++) {
          selectionState[i]=false;
        }
        getSym();
        do {
          if(sym==12) {
            throw std::logic_error(charToString("Char leak by %llu.", indexOld));
          }
          c=letter;
          switch(getSym()) {
            case -1:
              throw std::invalid_argument(charToString("Unexpected end."));
              break;
              
            //letter
            case 2:
              selectionState[(int)c]=true;
              selectionState[(int)letter]=true;
              getSym();
              break;
            
            //*
            case 5:
              selectionState[(int)c]=true;
              selectionState['*']=true;
              getSym();
              break;
              
            //?  
            case 6:
              selectionState[(int)c]=true;
              selectionState['?']=true;
              getSym();
              break;
              
            //(
            case 7:
              selectionState[(int)c]=true;
              selectionState['(']=true;
              getSym();
              break;
              
            //)
            case 8:
              selectionState[(int)c]=true;
              selectionState[')']=true;
              getSym();
              break;
              
            //|
            case 9:
              selectionState[(int)c]=true;
              selectionState['|']=true;
              getSym();
              break;
              
            //[
            case 10:
              selectionState[(int)c]=true;
              selectionState['[']=true;
              getSym();
              break;
              
            //]
            case 11:
              selectionState[(int)c]=true;
              break;
              
            //-
            case 12:
              if(getSym()!=2) {
                throw std::logic_error(charToString("Char leak by %llu.", indexOld));
              }
              if(c>=letter) {
                throw std::range_error(charToString("Left side is bigger than right side by %llu.", indexOld));
              }
              for(int i=c; i<=letter; i++) {
                selectionState[i]=true;
              }
              getSym();
              break;  
              
            default:
              throw std::invalid_argument(charToString("Unknow char %c by %llu.", source[indexOld], indexOld));
          }
        }while(sym!=11);
        
        //look a head
        switch(getSym()) {
          //finish
          case -1:
            sub=false;
          //letter
          case 2:
          //(
          case 7:
          //)
          case 8:
          //|
          case 9:
          //[
          case 10:
            for(int i=0; i<256; i++) {
              if(q[currentState].getNextState(i)!=0 && selectionState[i]) {
                throw std::logic_error(charToString("Next state [%llu::%c] is occupied by %llu.", currentState, i, indexOld));
              }
            }
            q.push_back(stateDefault);
            for(it=markState.begin(); it!=markState.end(); ++it) {
              if(q[*it].getMark()) {
                for(int j=0; j<256; j++) {
                  if(q[*it].getNextState(j)!=0 && selectionState[j]) {
                    throw std::logic_error(charToString("Next state [%llu::%c] is occupied by %llu.", *it, j, indexOld));
                  }
                }
                q[*it].setMark(false);
                for(int j=0; j<256; j++) {
                  if(selectionState[j]) {
                    try {
                      q[*it].setNextState((char)j, q.size()-1);
                    }
                    catch(std::exception &e) {
                      throw std::logic_error(charToString("%s\nSee previous error.", e.what()));
                    }
                  }
                }
              }
            }
            
            currentState=q.size()-1;              
            
            if(sub) {
              index=indexOld;
            }
            break;

          //*
          case 5:
            for(it=markState.begin(); it!=markState.end(); ++it) {
              if(q[*it].getMark()) {
                for(int j=0; j<256; j++) {
                  if(q[*it].getNextState(j)!=0 && selectionState[j]) {
                    if(q[*it].getNextState(j)==*it) {
                      continue;
                    }
                    else {
                      throw std::logic_error(charToString("Next State[%c] is engaged at %llu.", source[indexOld], indexOld));
                    }  
                  }
                  else {
                    if(selectionState[j]) {
                      try {
                        q[*it].setNextState(j, *it);
                      }
                      catch(std::exception &e) {
                        throw std::logic_error(charToString("%s\nSee previous error.", e.what()));
                      }
                    }
                  }
                }
              }
            }
            break;

          //?
          case 6:
            q.push_back(stateDefault);
            for(it=markState.begin(); it!=markState.end(); ++it) {
              if(q[*it].getMark()) {
                for(int j=0; j<256; j++) {
                  if(q[*it].getNextState(j)!=0 && selectionState[j]) {
                    throw std::logic_error(charToString("Next State[%c] is engaged at %llu.", source[indexOld], indexOld));
                  }
                  if(selectionState[j]) {
                    try {
                      q[*it].setNextState(j, q.size()-1);
                    }
                    catch(std::exception &e) {
                      throw std::logic_error(charToString("%s\nSee previous error.", e.what()));
                    }
                  }
                }
              }
            }
            currentState=q.size()-1;
            break;

          //Error
          default:
            throw std::invalid_argument(charToString("Next Symbol[%c] unknow at %llu.", source[indexOld], indexOld));
        }
        change=true;
        q[currentState].setMark(true);
        markState.insert(currentState);
        markState.insert(beginState);
        break;

      //Error
      default:
        throw std::invalid_argument(charToString("Next Symbol[%c] unknow at %llu.", (char)source[indexOld], indexOld));
    }
  }while(true);
}

// Konstruktoren
CRegEx::CRegEx() {
  size_t nextStateSize=9;
  size_t allSign[STATETOKENSIZE]={0, 2, 3, 0, 2, 3, 3, 3, 3, 3, 3, 3, 3};
  size_t nextState[nextStateSize][3]={{1, '\\', 4}, {1, '*', 5}, {1, '?', 6} , {1, '(', 7},
                          {1, ')', 8}, {1, '|', 9}, {1, '[', 10}, {1, ']', 11}, {1, '-', 12}};

  for(size_t i=0; i<STATETOKENSIZE; i++) {
    for(size_t j=0; j<256; j++) {
      stateToken[i][j]=allSign[i];
    } 
  }

  for(size_t i=0; i<nextStateSize; i++) {
    stateToken[nextState[i][0]][nextState[i][1]]=nextState[i][2];
  }

  endState=0;
  q.push_back(stateDefault);
  q.push_back(stateDefault);
}

CRegEx::~CRegEx() {
}

// Public Funktionen
void CRegEx::operator()(const std::string &source) {
  change=false;
  index=0;
  currentState=1;
  this->source=source;
  for(size_t i=1; i<q.size(); i++) {
    q[i].setEndState(false);
    q[i].setMark(false);
  }
  q[1].setMark(true);

  REGToDEA();
}

size_t CRegEx::operator[](const std::string& source) {
  size_t currentStateOld;

  index=0;
  currentState=1;  

  do {
    currentStateOld=currentState;
    if(index<source.size()) { 
      currentState=q[currentState].getNextState(source[index++]&0xFF);
    }
    else {
      currentState=endState;
    }
  }while(currentState!=0 && currentState!=endState);

  return (currentState==endState) ? currentStateOld : 0;
}

size_t CRegEx::match(const std::string source, size_t begin, size_t &end) {
  size_t currentStateOld;

  index=begin;
  currentState=1;  

  if(begin>=source.size()) {
    throw std::range_error(charToString("Out of source."));
  }

  do {
    currentStateOld=currentState;
    if(index<source.size()) { 
      currentState=q[currentState].getNextState(source[index++]&0xFF);
    }
    else {
      currentState=endState;
    }
  }while(currentState!=0 && currentState!=endState);

  end=index-1;
 
  return (currentState==endState) ? currentStateOld : 0;
}

#ifdef DEBUG
std::string CRegEx::print() {
  std::string debugInfo;

  debugInfo+="Endstate: ";
  debugInfo+=std::to_string(endState);
  debugInfo+="\n\n";
  debugInfo+="Size: ";
  debugInfo+=std::to_string(q.size());
  debugInfo+="\n\n";
  for(size_t j=0; j<q.size(); j++) {
    debugInfo+="State " + std::to_string(j) + ":\n";
    for(int i=1; i<256; i++) {
      if(q[j].getNextState(i)!=0) {
        debugInfo+="\tNext state [";
        if(i>=33 && i<=126) {
          debugInfo+=i;
        }
        else {
          debugInfo+=std::to_string(i);
        }
        debugInfo+="]->"+std::to_string(q[j].getNextState(i))+"\n";
      }
    }
  }

  return debugInfo;
}
#endif
