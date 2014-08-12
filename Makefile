OBJECTS = \
		  SdioCmd.o \
		  SdioCmd52.o \
		  SdioCmd53.o \
		  SdioCmd5.o \
		  SDIOParser.o \
		  tester.o


all: $(OBJECTS)
	g++ -g $(OBJECTS)

clean:
	rm *.o
	rm a.out

COMMON_HEADERS = SdioCmd.h SDIOParser.h
#.cpp.o: 
#	g++ -g -c $<
SdioCmd.o:   $(COMMON_HEADERS) SdioCmd5.h SdioCmd52.h SdioCmd53.h
SdioCmd5.o:  $(COMMON_HEADERS) SdioCmd5.h
SdioCmd52.o: $(COMMON_HEADERS) SdioCmd52.h
SdioCmd53.o: $(COMMON_HEADERS) SdioCmd52.h SdioCmd53.h
$(objects):  $(COMMON_HEADERS)

