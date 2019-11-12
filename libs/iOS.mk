NICE_PRINT = "\033[1;32m Building $(1)\033[0m\n"

XCODE_TOOLCHAIN = $(shell xcode-select --print-path)/Toolchains/XcodeDefault.xctoolchain
IOS_PLATFORM ?= iphoneos

IOS_SDK = $(shell xcrun -sdk ${IOS_PLATFORM} -show-sdk-path)

export PATH := ${CURDIR}/build/macOS/x86_64/bin:${PATH}

export PREFIX = ${CURDIR}/build/iOS/${ARCH}
LIBDIR = ${PREFIX}/lib

export CXX = ${XCODE_TOOLCHAIN}/usr/bin/clang++
export CC = ${XCODE_TOOLCHAIN}/usr/bin/clang
CROSSFLAGS = -arch ${ARCH} -isysroot ${IOS_SDK} -miphoneos-version-min=8.0
export CFLAGS = ${CROSSFLAGS} -O3 -fembed-bitcode -fvisibility=hidden
export CPPFLAGS = ${CROSSFLAGS} -I${IOS_SDK}/usr/include -I${PREFIX}/include
export CXXFLAGS = ${CFLAGS} -std=c++14 -stdlib=libc++ -fno-aligned-allocation
export LDFLAGS = ${CROSSFLAGS} -e _main -stdlib=libc++ -L${LIBDIR} -L${IOS_SDK}/usr/lib
HOST = arm-apple-darwin
LIBTOOLFLAGS = -arch_only ${ARCH}

# Build separate architectures
all:
	mkdir -p download
	@${MAKE} -f iOS.mk ios_arch ARCH=x86_64 IOS_PLATFORM=iphonesimulator >/dev/null
	@${MAKE} -f iOS.mk ios_arch ARCH=armv7 IOS_PLATFORM=iphoneos >/dev/null
	@${MAKE} -f iOS.mk ios_arch ARCH=arm64 IOS_PLATFORM=iphoneos >/dev/null
	@${MAKE} -f iOS.mk ios_arch ARCH=i386 IOS_PLATFORM=iphonesimulator >/dev/null

ios_arch: status | ${LIBDIR}/libprotobuf-lite.a

status:
	@printf $(call NICE_PRINT,$(ARCH)) 1>&2;

include download.mk

# it fixes lazy linking in protobuf. Otherwise you'll see "-bind_at_load and -bitcode_bundle (Xcode setting ENABLE_BITCODE=YES) cannot be used together"
export MACOSX_DEPLOYMENT_TARGET=10.10

${LIBDIR}/libprotobuf-lite.a: download/protobuf
	@printf $(call NICE_PRINT,$@) 1>&2;
	cd ${PROTOBUF_DIR} && \
	env LDFLAGS="${LDFLAGS}" ./configure --host=${HOST} --prefix=${PREFIX} --disable-shared --with-protoc="${CURDIR}/build/macOS/x86_64/bin/protoc" && \
	${MAKE} clean && \
	${MAKE} -j && \
	${MAKE} install