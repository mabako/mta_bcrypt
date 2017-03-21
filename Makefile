CC = g++
PROG = bcrypt.so

# Compiler flags
# NOTE: add -g for debug, remove for release!
CPPFLAGS = -MD -Wall -I./ -std=c++11 -fPIC
LDFLAGS = -shared -Wl,-soname,$(PROG).1,-R./
LIBS = -lpthread -L./bcrypt -lbcrypt


#### Source and object files

SRC_LOCAL =		$(wildcard ./*.cpp ./extra/*.cpp)
OBJ_LOCAL =		$(patsubst %.cpp,%.o,$(SRC_LOCAL))
OBJS	= $(OBJ_LOCAL)
SRCS	= $(SRC_LOCAL)
DEPS	= $(patsubst %.o,%.d,$(OBJS))

#### Make rules

all : bcrypt

.PHONY: libbcrypt

bcrypt : libbcrypt $(OBJS)
	$(CC) $(CPPFLAGS) $(LDFLAGS) -o $(PROG) $(OBJS) $(LIBS)

libbcrypt :
	$(MAKE) -C bcrypt/

clean :
	$(MAKE) -C bcrypt/ clean
	rm -f *.o extra/*.o $(PROG)

-include $(DEPS)
