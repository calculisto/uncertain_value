DEPENDENCIES_HEADERS=

PROJECT=uncertain_value
LINK.o=${LINK.cc}
CXXFLAGS+=-std=c++2a -Wall -Wextra $(foreach dir, ${DEPENDENCIES_HEADERS}, -I../${dir})
LDFLAGS+=
LDLIBS+= -lfmt

