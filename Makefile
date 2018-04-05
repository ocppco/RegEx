CC              = g++
CCLINK          = g++
CCOPTIONS       = -Wall -std=c++11 
LDOPTIONS       = -O2
                  
TESTOBJS        = CState.cpp \
                  CRegEx.cpp \
                  testMain.cpp

TESTPROGRAM     = testpro

$(TESTPROGRAM): $(TESTOBJS)
	$(CCLINK) $(CCOPTIONS) $(LIBS) $(TESTOBJS) $(LDOPTIONS) -D DEBUG -o $@

clean:  
	rm -f $(TESTPROGRAM)

