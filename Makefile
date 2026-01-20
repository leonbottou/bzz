SHELL=/bin/sh
DEFS=
CXXFLAGS=-O3 ${DEFS}
CXX=c++
BLOCKSIZE=1M

PGMS=bzz
LIBOBJS=BSByteStream.o ByteStream.o GException.o ZPCodec.o
BZZOBJS=bzz.o

all: ${PGMS}

bzz: ${LIBOBJS} ${BZZOBJS}
	${CXX} -o $@ ${LIBOBJS} ${BZZOBJS}

clean: FORCE
	-rm 1>&2 -rf *.o ${PGMS} *.dSYM

test: bzz FORCE
	@echo "=== Roundtrip tests with compression performance (blocksize=${BLOCKSIZE}) ==="
	@echo ""
	@echo "File                      Size  Compressed    BPC   Status"
	@echo "------------------------------------------------------------"
	@failed=0; \
	for corpus in calgary canterbury large; do \
	  if [ -d "$$corpus" ]; then \
	    for f in $$corpus/*; do \
	      name=`basename "$$f"`; \
	      size=`wc -c < "$$f" | tr -d ' '`; \
	      if ./bzz -e${BLOCKSIZE} "$$f" /tmp/bzz_test.bzz 2>/dev/null && \
	         ./bzz -d /tmp/bzz_test.bzz /tmp/bzz_test.out 2>/dev/null && \
	         diff -q "$$f" /tmp/bzz_test.out > /dev/null 2>&1; then \
	        comp=`wc -c < /tmp/bzz_test.bzz | tr -d ' '`; \
	        bpc=`echo "scale=3; ($$comp * 8.0) / $$size" | bc`; \
	        printf "%-20s %9d %10d  %6s   OK\n" "$$name" "$$size" "$$comp" "$$bpc"; \
	      else \
	        printf "%-20s %9d          -       -   FAIL\n" "$$name" "$$size"; \
	        failed=1; \
	      fi; \
	    done; \
	  fi; \
	done; \
	rm -f /tmp/bzz_test.bzz /tmp/bzz_test.out; \
	echo "------------------------------------------------------------"; \
	if [ $$failed -eq 0 ]; then echo "All roundtrip tests passed."; else exit 1; fi

FORCE:

.PHONY: FORCE



BSByteStream.o: BSByteStream.cpp BSByteStream.h GException.h ByteStream.h ZPCodec.h
ByteStream.o: ByteStream.cpp ByteStream.h GException.h
GException.o: GException.cpp GException.h
ZPCodec.o: ZPCodec.cpp ZPCodec.h ByteStream.h GException.h
bzz.o: bzz.cpp GException.h ByteStream.h BSByteStream.h ZPCodec.h
