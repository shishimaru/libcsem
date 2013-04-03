# $Id$

TARGET = extract.cgi
OBJS = extract.o utils.o cJSON.o
CC = gcc
CFLAGS = -O3 -Wall -pedantic -Wformat=2 -Wstrict-aliasing=2
CPPFLAGS = -I./
CPPFLAGS += -I../../../../
LDFLAGS = 
LDFLAGS += -L../../../../
LIBS = 
LIBS +=  -lcsem -lxml2 -lcurl -lldap -lrt -lssl -lcrypto

$(TARGET): $(OBJS) ../../../../libcsem.a
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LDFLAGS) $(LIBS)

.c.o:
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $<

clean:
	rm -f $(TARGET) $(OBJS)


