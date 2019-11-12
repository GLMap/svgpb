NICE_PRINT = "\033[1;32m Building $(1)\033[0m\n"


export PREFIX = ${CURDIR}/build/macOS/${ARCH}
LIBDIR = ${PREFIX}/lib

export CFLAGS = -O3 -fvisibility=hidden
export CPPFLAGS = ${CROSSFLAGS}
export CXXFLAGS = ${CFLAGS} -std=c++14 -stdlib=libc++ -fno-aligned-allocation
export LDFLAGS = ${CROSSFLAGS} -stdlib=libc++ -L${LIBDIR}
export MACOSX_DEPLOYMENT_TARGET := 10.14

LIBTOOLFLAGS = -arch_only ${ARCH}

# Build separate architectures
all:
	mkdir -p download
	@${MAKE} -f macOS.mk macos_arch ARCH=x86_64 >/dev/null

macos_arch: status | ${LIBDIR}/libprotobuf-lite.a

status:
	@printf $(call NICE_PRINT,$(ARCH)) 1>&2;

include download.mk

${LIBDIR}/libprotobuf-lite.a: download/protobuf
	@printf $(call NICE_PRINT,$@) 1>&2;
	cd ${PROTOBUF_DIR} && \
	env LDFLAGS="${LDFLAGS}" ./configure --prefix=${PREFIX} --disable-shared && \
	${MAKE} clean && \
	${MAKE} -j && \
	${MAKE} install