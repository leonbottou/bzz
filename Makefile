SHELL=/bin/sh
CXXFLAGS=-O3
CXX=c++

PGMS=bzz
LIBOBJS=BSByteStream.o ByteStream.o GException.o ZPCodec.o
BZZOBJS=bzz.o

all: ${PGMS}

bzz: ${LIBOBJS} ${BZZOBJS}
	${CXX} -o $@ ${LIBOBJS} ${BZZOBJS}

clean: FORCE
	-rm 1>&2 -rf *.o ${PGMS} *.dSYM 

FORCE:

.PHONY: FORCE



BSByteStream.o: BSByteStream.cpp BSByteStream.h GException.h ByteStream.h ZPCodec.h
ByteStream.o: ByteStream.cpp ByteStream.h GException.h
GException.o: GException.cpp GException.h
ZPCodec.o: ZPCodec.cpp ZPCodec.h ByteStream.h GException.h
bzz.o: bzz.cpp GException.h ByteStream.h BSByteStream.h ZPCodec.h
