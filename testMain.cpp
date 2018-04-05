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
#include <iostream>

__inline__ uint64_t rdtsc(void) {
  uint32_t lo, hi;
  __asm__ __volatile__ (
  "        xorl %%eax,%%eax \n"
  "        cpuid"      // serialize
  ::: "%rax", "%rbx", "%rcx", "%rdx" );
  __asm__ __volatile__("rdtsc" : "=a"(lo), "=d"(hi));
  return ( uint64_t ) hi << 32 | lo;
}

int main(int argc, char *argv[]) {
  bool e;
  size_t index=0;
  size_t begin=0;
  size_t testCounter=0;
  size_t goodCounter=0;
  size_t errorCounter=0;
  size_t m=2;
  size_t start;
  size_t midCompile, midMatch;
  std::string str;
  std::vector<std::string> source;
  std::vector<std::string> ts;
  std::vector<std::vector<std::string> > testSource;
  std::vector<size_t> vs;
  std::vector<std::vector<size_t> > verfiySource;
  std::vector<std::string> errorString;
  std::vector<std::vector<size_t> > mesCompile;
  std::vector<std::vector<size_t> > mesMatch;
  
  //Good tests
  //Test 0
  index++;
  source.push_back("test");
  ts.push_back("test ");
  testSource.push_back(ts);
  vs.push_back(5);
  verfiySource.push_back(vs);
  
  //Test 1
  index++;
  source.push_back("test|hello");
  ts.clear();
  ts.push_back("test ");
  ts.push_back("hello ");
  testSource.push_back(ts);
  vs.clear();
  vs.push_back(5);
  vs.push_back(10);
  verfiySource.push_back(vs);
  
  //Test 2
  index++;
  source.push_back("Ah*");
  ts.clear();
  ts.push_back("Ah ");
  ts.push_back("Ahh ");
  ts.push_back("Ahhh ");
  ts.push_back("Ahhhh ");
  testSource.push_back(ts);
  vs.clear();
  vs.push_back(2);
  vs.push_back(2);
  vs.push_back(2);
  vs.push_back(2);
  verfiySource.push_back(vs);
  
  //Test 3
  index++;
  source.push_back("hou?se");
  ts.clear();
  ts.push_back("hose ");
  ts.push_back("house ");
  testSource.push_back(ts);
  vs.clear();
  vs.push_back(6);
  vs.push_back(6);
  verfiySource.push_back(vs);
  
  //Test 4
  index++;
  source.push_back("hou?s*e");
  ts.clear();
  ts.push_back("hose ");
  ts.push_back("house ");
  ts.push_back("hosse ");
  ts.push_back("hossse ");
  ts.push_back("hosssse ");
  ts.push_back("housse ");
  ts.push_back("houssse ");
  ts.push_back("housssse ");
  testSource.push_back(ts);
  vs.clear();
  vs.push_back(5);
  vs.push_back(5);
  vs.push_back(5);
  vs.push_back(5);
  vs.push_back(5);
  vs.push_back(5);
  vs.push_back(5);
  vs.push_back(5);
  verfiySource.push_back(vs);
  
  //Test 5
  index++;
  source.push_back("ta[a-f]le");
  ts.clear();
  ts.push_back("taale ");
  ts.push_back("table ");
  ts.push_back("tacle ");
  ts.push_back("tadle ");
  ts.push_back("taele ");
  ts.push_back("tafle ");
  testSource.push_back(ts);
  vs.clear();
  vs.push_back(6);
  vs.push_back(6);
  vs.push_back(6);
  vs.push_back(6);
  vs.push_back(6);
  vs.push_back(6);
  verfiySource.push_back(vs);
  
  //Test 6
  index++;
  source.push_back("ta[a-f]?le");
  ts.clear();
  ts.push_back("tale ");
  ts.push_back("taale ");
  ts.push_back("table ");
  ts.push_back("tacle ");
  ts.push_back("tadle ");
  ts.push_back("taele ");
  ts.push_back("tafle ");
  testSource.push_back(ts);
  vs.clear();
  vs.push_back(6);
  vs.push_back(6);
  vs.push_back(6);
  vs.push_back(6);
  vs.push_back(6);
  vs.push_back(6);
  vs.push_back(6);
  verfiySource.push_back(vs);
  
  //Test 7
  index++;
  source.push_back("ta[a-f]*le");
  ts.clear();
  ts.push_back("tale ");
  ts.push_back("taale ");
  ts.push_back("tabble ");
  ts.push_back("tacccle ");
  ts.push_back("taddddle ");
  ts.push_back("taeeeeele ");
  ts.push_back("taffffffle ");
  testSource.push_back(ts);
  vs.clear();
  vs.push_back(5);
  vs.push_back(5);
  vs.push_back(5);
  vs.push_back(5);
  vs.push_back(5);
  vs.push_back(5);
  vs.push_back(5);
  verfiySource.push_back(vs);
  
  //Test 8
  index++;
  source.push_back("d(oo)r");
  ts.clear();
  ts.push_back("door");
  testSource.push_back(ts);
  vs.clear();
  vs.push_back(5);
  verfiySource.push_back(vs);
  
  //Test 9
  index++;
  source.push_back("d(oo|uu)r");
  ts.clear();
  ts.push_back("door");
  ts.push_back("duur");
  testSource.push_back(ts);
  vs.clear();
  vs.push_back(7);
  vs.push_back(7);
  verfiySource.push_back(vs);
  
  //Test 10
  index++;
  source.push_back("d(od*o|ug*u)r");
  ts.clear();
  ts.push_back("door");
  ts.push_back("duur");
  ts.push_back("dodor");
  ts.push_back("doddor");
  ts.push_back("dodddor");
  ts.push_back("dugur");
  ts.push_back("duggur");
  ts.push_back("dugggur");
  testSource.push_back(ts);
  vs.clear();
  vs.push_back(7);
  vs.push_back(7);
  vs.push_back(7);
  vs.push_back(7);
  vs.push_back(7);
  vs.push_back(7);
  vs.push_back(7);
  vs.push_back(7);
  verfiySource.push_back(vs);
  
  //Test 11
  index++;
  source.push_back("d(ood*|uug*)r");
  ts.clear();
  ts.push_back("door");
  ts.push_back("duur");
  ts.push_back("doodr");
  ts.push_back("dooddr");
  ts.push_back("doodddr");
  ts.push_back("duugr");
  ts.push_back("duuggr");
  ts.push_back("duugggr");
  testSource.push_back(ts);
  vs.clear();
  vs.push_back(7);
  vs.push_back(7);
  vs.push_back(7);
  vs.push_back(7);
  vs.push_back(7);
  vs.push_back(7);
  vs.push_back(7);
  vs.push_back(7);
  verfiySource.push_back(vs);
  
  //Test 12
  index++;
  source.push_back("d(od?o|ug?u)r");
  ts.clear();
  ts.push_back("door");
  ts.push_back("duur");
  ts.push_back("dodor");
  ts.push_back("dugur");
  testSource.push_back(ts);
  vs.clear();
  vs.push_back(9);
  vs.push_back(9);
  vs.push_back(9);
  vs.push_back(9);
  verfiySource.push_back(vs);
  
  //Test 13
  index++;
  source.push_back("d(ood?|uug?)r");
  ts.clear();
  ts.push_back("door");
  ts.push_back("duur");
  ts.push_back("doodr");
  ts.push_back("duugr");
  testSource.push_back(ts);
  vs.clear();
  vs.push_back(9);
  vs.push_back(9);
  vs.push_back(9);
  vs.push_back(9);
  verfiySource.push_back(vs);
  
  //Test 14
  index++;
  source.push_back("d(oo|uu)*r");
  ts.clear();
  ts.push_back("doooor");
  ts.push_back("doooooor");
  ts.push_back("duuuur");
  ts.push_back("duuuuuur");
  ts.push_back("doouur");
  ts.push_back("duuoouur");
  testSource.push_back(ts);
  vs.clear();
  vs.push_back(7);
  vs.push_back(7);
  vs.push_back(7);
  vs.push_back(7);
  vs.push_back(7);
  vs.push_back(7);
  verfiySource.push_back(vs);
  
  //Test 15
  index++;
  source.push_back("d(ood*|uug*)*r");
  ts.clear();
  ts.push_back("doooor");
  ts.push_back("doooooor");
  ts.push_back("duuuur");
  ts.push_back("duuuuuur");
  ts.push_back("doouur");
  ts.push_back("duuoouur");
  ts.push_back("doodr");
  ts.push_back("dooddr");
  ts.push_back("doodddr");
  ts.push_back("duugr");
  ts.push_back("duuggr");
  ts.push_back("duugggr");
  ts.push_back("doooodooddooddduuuuguugguugggr");
  testSource.push_back(ts);
  vs.clear();
  vs.push_back(7);
  vs.push_back(7);
  vs.push_back(7);
  vs.push_back(7);
  vs.push_back(7);
  vs.push_back(7);
  vs.push_back(7);
  vs.push_back(7);
  vs.push_back(7);
  vs.push_back(7);
  vs.push_back(7);
  vs.push_back(7);
  vs.push_back(7);
  verfiySource.push_back(vs);
  
  //Test 16
  index++;
  source.push_back("d(ood?|uug?)*r");
  ts.clear();
  ts.push_back("doooor");
  ts.push_back("doooooor");
  ts.push_back("duuuur");
  ts.push_back("duuuuuur");
  ts.push_back("doodooduuguugoodoouur");
  testSource.push_back(ts);
  vs.clear();
  vs.push_back(9);
  vs.push_back(9);
  vs.push_back(9);
  vs.push_back(9);
  vs.push_back(9);
  verfiySource.push_back(vs);
  
  //Test 17
  index++;
  source.push_back("d(oo|uu)?r");
  ts.clear();
  ts.push_back("dr");
  ts.push_back("door");
  ts.push_back("duur");
  testSource.push_back(ts);
  vs.clear();
  vs.push_back(7);
  vs.push_back(7);
  vs.push_back(7);
  verfiySource.push_back(vs);
  
  //Test 18
  index++;
  source.push_back("d(ood*|uug*)?r");
  ts.clear();
  ts.push_back("dr");
  ts.push_back("door");
  ts.push_back("duur");
  ts.push_back("doodr");
  ts.push_back("dooddr");
  ts.push_back("doodddr");
  ts.push_back("duugr");
  ts.push_back("duuggr");
  ts.push_back("duugggr");
  testSource.push_back(ts);
  vs.clear();
  vs.push_back(7);
  vs.push_back(7);
  vs.push_back(7);
  vs.push_back(7);
  vs.push_back(7);
  vs.push_back(7);
  vs.push_back(7);
  vs.push_back(7);
  vs.push_back(7);
  verfiySource.push_back(vs);
  
  //Test 19
  index++;
  source.push_back("d(ood?|uug?)?r");
  ts.clear();
  ts.push_back("dr");
  ts.push_back("door");
  ts.push_back("duur");
  ts.push_back("doodr");
  ts.push_back("duugr");
  testSource.push_back(ts);
  vs.clear();
  vs.push_back(9);
  vs.push_back(9);
  vs.push_back(9);
  vs.push_back(9);
  vs.push_back(9);
  verfiySource.push_back(vs);
  
  //Test 20
  index++;
  source.push_back("d(od?o|ug?u)?r");
  ts.clear();
  ts.push_back("dr");
  ts.push_back("door");
  ts.push_back("duur");
  ts.push_back("dodor");
  ts.push_back("dugur");
  testSource.push_back(ts);
  vs.clear();
  vs.push_back(9);
  vs.push_back(9);
  vs.push_back(9);
  vs.push_back(9);
  vs.push_back(9);
  verfiySource.push_back(vs);
  
  //Test 21
  index++;
  source.push_back("d(o(fff|sss)?o|u(lll|yyy)?u)?r");
  ts.clear();
  ts.push_back("dr");
  ts.push_back("door");
  ts.push_back("duur");
  ts.push_back("dofffor");
  ts.push_back("dosssor");
  ts.push_back("dulllur");
  ts.push_back("duyyyur");
  testSource.push_back(ts);
  vs.clear();
  vs.push_back(19);
  vs.push_back(19);
  vs.push_back(19);
  vs.push_back(19);
  vs.push_back(19);
  vs.push_back(19);
  vs.push_back(19);
  verfiySource.push_back(vs);
  
  //Test 22
  index++;
  source.push_back("d(o(fff|sss)*o|u(lll|yyy)?u)?r");
  ts.clear();
  ts.push_back("dr");
  ts.push_back("door");
  ts.push_back("duur");
  ts.push_back("dofffor");
  ts.push_back("doffffffor");
  ts.push_back("dofffffffffor");
  ts.push_back("dosssor");
  ts.push_back("dossssssor");
  ts.push_back("dosssssssssor");
  ts.push_back("dosssor");
  ts.push_back("dulllur");
  ts.push_back("duyyyur");
  testSource.push_back(ts);
  vs.clear();
  vs.push_back(19);
  vs.push_back(19);
  vs.push_back(19);
  vs.push_back(19);
  vs.push_back(19);
  vs.push_back(19);
  vs.push_back(19);
  vs.push_back(19);
  vs.push_back(19);
  vs.push_back(19);
  vs.push_back(19);
  vs.push_back(19);
  verfiySource.push_back(vs);
  
  
  std::cout << "Good tests\n" << "==========\n\n\n"; 
  for(size_t i=begin; i<index; i++) {  
    try {
      e=false;
      CRegEx regEx;
      regEx(source[i]);
      std::cout << "Source \"" << source[i] << "\":\n";
      for(size_t j=0; j<testSource[i].size(); j++) {
        testCounter++;
        if(testSource[i].size()!=verfiySource[i].size()) {
          std::cout << "\t" << i <<  ":" << j << ". Test error.\n";
          errorCounter++;
          break;
        }
        else {
          if(regEx[testSource[i][j]]==verfiySource[i][j]) {
            std::cout << "\t" << i <<  ":" << j << ". Test string \"" << testSource[i][j] << "\" is ok.\n";
            goodCounter++;
          }
          else {
            std::cout << "\t" << i <<  ":" << j << ". Test string \"" << testSource[i][j] << "\" is not equal(regEx[\"" << testSource[i][j] << "\"]!=" <<  regEx[testSource[i][j]] << ") .\n";
            e=true;
            errorCounter++;
          }
        }
      }
      if(e) {
        std::cout << "\n\nState graphs:\n" << "=============\n";
        std::cout << regEx.print();
      }
    }
    catch(...) {
      std::cout << "\t" << i << ". Compile \"" << source[i] << "\" error.\n";
      errorCounter++;
    }
    std::cout << "\n";
  }
  
  begin=index;
  
  //Fail tests
  //Test 23
  index++;
  source.push_back("*");
  ts.clear();
  testSource.push_back(ts);
  vs.clear();
  verfiySource.push_back(vs);
  errorString.push_back("Next Symbol[*] unknow at 0.");
  
  //Test 24
  index++;
  source.push_back("?");
  ts.clear();
  testSource.push_back(ts);
  vs.clear();
  verfiySource.push_back(vs);
  errorString.push_back("Next Symbol[?] unknow at 0.");
  
  //Test 25
  index++;
  source.push_back("]");
  ts.clear();
  testSource.push_back(ts);
  vs.clear();
  verfiySource.push_back(vs);
  errorString.push_back("Next Symbol[]] unknow at 0.");
  
  //Test 26
  index++;
  source.push_back(")");
  ts.clear();
  testSource.push_back(ts);
  vs.clear();
  verfiySource.push_back(vs);
  errorString.push_back("Next Symbol[)] unknow at 0.");
  
  //Test 27
  index++;
  source.push_back("|");
  ts.clear();
  testSource.push_back(ts);
  vs.clear();
  verfiySource.push_back(vs);
  errorString.push_back("Next Symbol[|] unknow at 0.");
  
  //Test 28
  index++;
  source.push_back("-");
  ts.clear();
  testSource.push_back(ts);
  vs.clear();
  verfiySource.push_back(vs);
  errorString.push_back("Next Symbol[-] unknow at 0.");
  
  //Test 29
  index++;
  source.push_back("(|");
  ts.clear();
  testSource.push_back(ts);
  vs.clear();
  verfiySource.push_back(vs);
  errorString.push_back("Next Symbol[|] unknow at 1.\nSee previous error.");
  
  //Test 30
  index++;
  source.push_back("(?");
  ts.clear();
  testSource.push_back(ts);
  vs.clear();
  verfiySource.push_back(vs);
  errorString.push_back("Next Symbol[?] unknow at 1.\nSee previous error.");
  
  //Test 31
  index++;
  source.push_back("(*");
  ts.clear();
  testSource.push_back(ts);
  vs.clear();
  verfiySource.push_back(vs);
  errorString.push_back("Next Symbol[*] unknow at 1.\nSee previous error.");
  
  //Test 32
  index++;
  source.push_back("(-");
  ts.clear();
  testSource.push_back(ts);
  vs.clear();
  verfiySource.push_back(vs);
  errorString.push_back("Next Symbol[-] unknow at 1.\nSee previous error.");
  
  //Test 33
  index++;
  source.push_back("(]");
  ts.clear();
  testSource.push_back(ts);
  vs.clear();
  verfiySource.push_back(vs);
  errorString.push_back("Next Symbol[]] unknow at 1.\nSee previous error.");
  
  //Test 34
  index++;
  source.push_back("[a");
  ts.clear();
  testSource.push_back(ts);
  vs.clear();
  verfiySource.push_back(vs);
  errorString.push_back("Unexpected end.");
  
  //Test 35
  index++;
  source.push_back("[a-");
  ts.clear();
  testSource.push_back(ts);
  vs.clear();
  verfiySource.push_back(vs);
  errorString.push_back("Char leak by 4.");
  
  //Test 36
  index++;
  source.push_back("[a-z");
  ts.clear();
  testSource.push_back(ts);
  vs.clear();
  verfiySource.push_back(vs);
  errorString.push_back("Unexpected end.");
  
  //Test 37
  index++;
  source.push_back("[z-a");
  ts.clear();
  testSource.push_back(ts);
  vs.clear();
  verfiySource.push_back(vs);
  errorString.push_back("Left side is bigger than right side by 3.");
  
  //Test 38
  index++;
  source.push_back("af\\");
  ts.clear();
  testSource.push_back(ts);
  vs.clear();
  verfiySource.push_back(vs);
  errorString.push_back("Next Symbol[\\] unknow at 2.");
  
  //Test 39
  index++;
  source.push_back("af\\");
  ts.clear();
  testSource.push_back(ts);
  vs.clear();
  verfiySource.push_back(vs);
  errorString.push_back("Next Symbol[\\] unknow at 2.");
  
  //Test 40
  index++;
  source.push_back("ho(fff|sss)?se");
  ts.clear();
  testSource.push_back(ts);
  vs.clear();
  verfiySource.push_back(vs);
  errorString.push_back("Sign s exist.\nSee previous error.");
  
  //Test 41
  index++;
  source.push_back("house");
  ts.clear();
  ts.push_back("ho");
  testSource.push_back(ts);
  vs.clear();
  vs.push_back(6);
  verfiySource.push_back(vs);
  
  //Test 42
  index++;
  source.push_back("house|window");
  ts.clear();
  ts.push_back("hoyse");
  ts.push_back("wixdow");
  testSource.push_back(ts);
  vs.clear();
  vs.push_back(6);
  vs.push_back(12);
  verfiySource.push_back(vs);
  
  //Test 43
  index++;
  source.push_back("ho[a-f]se");
  ts.clear();
  ts.push_back("hohse");
  ts.push_back("ho*se");
  testSource.push_back(ts);
  vs.clear();
  vs.push_back(6);
  vs.push_back(6);
  verfiySource.push_back(vs);
  
  //Test 44
  index++;
  source.push_back("ho(fff|ggg)?se");
  ts.clear();
  ts.push_back("hofse");
  ts.push_back("hoggse");
  testSource.push_back(ts);
  vs.clear();
  vs.push_back(11);
  vs.push_back(11);
  verfiySource.push_back(vs);
  
  //Test 45
  index++;
  source.push_back("ho(fff|ggg)*se");
  ts.clear();
  ts.push_back("hofffffffse");
  ts.push_back("hoggggse");
  testSource.push_back(ts);
  vs.clear();
  vs.push_back(11);
  vs.push_back(11);
  verfiySource.push_back(vs);
  
  //Test 46
  index++;
  source.push_back("hell*o");
  ts.clear();
  ts.push_back("hellllko");
  testSource.push_back(ts);
  vs.clear();
  vs.push_back(5);
  verfiySource.push_back(vs);
  
  //Test 47
  index++;
  source.push_back("helll?o");
  ts.clear();
  ts.push_back("hellko");
  testSource.push_back(ts);
  vs.clear();
  vs.push_back(7);
  verfiySource.push_back(vs);
  
  //Test 48
  index++;
  source.push_back("ho[a-f]?se");
  ts.clear();
  ts.push_back("hohse");
  testSource.push_back(ts);
  vs.clear();
  vs.push_back(6);
  verfiySource.push_back(vs);
  
  //Test 49
  index++;
  source.push_back("ho[a-f]*se");
  ts.clear();
  ts.push_back("hoaaaaaagse");
  testSource.push_back(ts);
  vs.clear();
  vs.push_back(5);
  verfiySource.push_back(vs);
  
  std::cout << "Fail tests\n" << "==========\n\n\n"; 
  for(size_t i=begin; i<index; i++) {  
    try {
      e=false;
      std::cout << "Source \"" << source[i] << "\":\n";
      CRegEx regEx;
      regEx(source[i]);
      
      for(size_t j=0; j<testSource[i].size(); j++) {
        testCounter++;
        if(testSource[i].size()!=verfiySource[i].size()) {
          std::cout << "\t" << i <<  ":" << j << ". Test error.\n";
          errorCounter++;
          break;
        }
        else {
          if(regEx[testSource[i][j]]!=verfiySource[i][j]) {
            std::cout << "\t" << i <<  ":" << j << ". Test string \"" << testSource[i][j] << "\" is ok.\n";
            goodCounter++;
          }
          else {
            std::cout << "\t" << i <<  ":" << j << ". Test string \"" << testSource[i][j] << "\" is not equal(regEx[\"" << testSource[i][j] << "\"]!=" <<  regEx[testSource[i][j]] << ") .\n";
            e=true;
            errorCounter++;
          }
        }
      }
      if(e) {
        std::cout << "\n\nState graphs:\n" << "=============\n";
        std::cout << regEx.print();
      }
    }
    catch(std::exception &e) {
      std::string str=e.what();
      if(str==errorString[i-begin]) {
        std::cout << "\t" << i << ". Error string is ok.\n";
        goodCounter++;
      }
      else {
        std::cout << "\t" << i << ". Error string is not equal(\"" << str << "\"!=\"" <<  errorString[i-begin] << "\") .\n";
        errorCounter++;
      }
      
    }
    std::cout << "\n";
  }
  
  begin=index;
  
  //Test 50
  index++;
  source.push_back(" *empty");
  ts.clear();
  ts.push_back("empty");
  ts.push_back(" empty");
  ts.push_back("  empty");
  ts.push_back("   empty");
  testSource.push_back(ts);
  vs.clear();
  vs.push_back(6);
  vs.push_back(6);
  vs.push_back(6);
  vs.push_back(6);
  verfiySource.push_back(vs);
  
  //Test 51
  index++;
  source.push_back(" ?empty");
  ts.clear();
  ts.push_back("empty");
  ts.push_back(" empty");
  testSource.push_back(ts);
  vs.clear();
  vs.push_back(7);
  vs.push_back(7);
  verfiySource.push_back(vs);
  
  //Test 52
  index++;
  source.push_back("empty *");
  ts.clear();
  ts.push_back("empty ");
  ts.push_back("empty  ");
  ts.push_back("empty   ");
  ts.push_back("empty    ");
  testSource.push_back(ts);
  vs.clear();
  vs.push_back(6);
  vs.push_back(6);
  vs.push_back(6);
  vs.push_back(6);
  verfiySource.push_back(vs);
  
  //Test 53
  index++;
  source.push_back("empty ?");
  ts.clear();
  ts.push_back("empty");
  ts.push_back("empty ");
  testSource.push_back(ts);
  vs.clear();
  vs.push_back(6);
  vs.push_back(7);
  verfiySource.push_back(vs);
  
  //Test 54
  index++;
  source.push_back("(house)(car)");
  ts.clear();
  ts.push_back("housecar");
  testSource.push_back(ts);
  vs.clear();
  vs.push_back(9);
  verfiySource.push_back(vs);
  
  
  std::cout << "Good tests\n" << "==========\n\n\n"; 
  for(size_t i=begin; i<index; i++) {  
    try {
      e=false;
      CRegEx regEx;
      regEx(source[i]);
      std::cout << "Source \"" << source[i] << "\":\n";
      for(size_t j=0; j<testSource[i].size(); j++) {
        testCounter++;
        if(testSource[i].size()!=verfiySource[i].size()) {
          std::cout << "\t" << i <<  ":" << j << ". Test error.\n";
          errorCounter++;
          break;
        }
        else {
          if(regEx[testSource[i][j]]==verfiySource[i][j]) {
            std::cout << "\t" << i <<  ":" << j << ". Test string \"" << testSource[i][j] << "\" is ok.\n";
            goodCounter++;
          }
          else {
            std::cout << "\t" << i <<  ":" << j << ". Test string \"" << testSource[i][j] << "\" is not equal(regEx[\"" << testSource[i][j] << "\"]!=" <<  regEx[testSource[i][j]] << ") .\n";
            e=true;
            errorCounter++;
          }
        }
      }
      if(e) {
        std::cout << "\n\nState graphs:\n" << "=============\n";
        std::cout << regEx.print();
      }
    }
    catch(...) {
      std::cout << "\t" << i << ". Compile \"" << source[i] << "\" error.\n";
      errorCounter++;
    }
    std::cout << "\n";
  }

  std::cout << "Runtime tests\n" << "=============\n\n\n";
  std::cout << "art, n, count\n";
  for(size_t i=0; i<24; i++) {
    str="";
    for(size_t j=0; j<m; j++) {
      try {
        str+="s";
      }
      catch(std::exception &e) {
        std::cout << "\tError: " << e.what() << ".\n";
        testCounter++;
        errorCounter++;
      }
    }
    
    midCompile=0;
    midMatch=0;
    for(size_t j=0; j<10; j++) {
      start=rdtsc();
      CRegEx regEx;
      try {
        regEx(str);
      }
      catch(std::exception &e) {
        std::cout << "\tError: " << e.what() << ".\n";
        testCounter++;
        errorCounter++;
      }
      midCompile+=rdtsc()-start;
      
      start=rdtsc();
      regEx[str];
      midMatch+=rdtsc()-start;
    }
    vs.clear();
    vs.push_back(i);
    vs.push_back(midCompile);
    mesCompile.push_back(vs);
    
    vs.clear();
    vs.push_back(i);
    vs.push_back(midMatch);
    mesMatch.push_back(vs);
    
    std::cout << "compiler, " <<  m << ", " << midCompile/10 << "\n";
    std::cout << "match,    " <<  m << ", " << midMatch/10   << "\n";
    
    m*=2;
  }
  
  std::cout << "Compile\nn, count\n";
  for(size_t i=0; i<mesCompile.size(); i++) {
    std::cout <<  mesCompile[i][0] << ", " << mesCompile[i][1]/10 << "\n";
  }
  
  std::cout << "Match\nn, count\n";
  for(size_t i=0; i<mesMatch.size(); i++) {
    std::cout <<  mesMatch[i][0] << ", " << mesMatch[i][1]/10 << "\n";
  }

  std::cout << "\n\nResult: " << testCounter << " Tests\t" <<  goodCounter << " Good\t" <<  errorCounter << " Failure\n";

  return 0;
}

