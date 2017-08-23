NICE_PRINT = "\033[1;32m Building $(1)\033[0m\n"

XCODE_TOOLCHAIN = $(shell xcode-select --print-path)/Toolchains/XcodeDefault.xctoolchain
IOS_PLATFORM ?= iphoneos

# Pick latest SDK in the directory
#IOS_PLATFORM_DEVELOPER = $(shell xcrun -sdk ${IOS_PLATFORM} -show-sdk-platform-path)
IOS_SDK = $(shell xcrun -sdk ${IOS_PLATFORM} -show-sdk-path)

# Build separate architectures
all:
	@${MAKE} arch ARCH=armv7 IOS_PLATFORM=iphoneos >/dev/null
	@${MAKE} arch ARCH=arm64 IOS_PLATFORM=iphoneos >/dev/null
	@${MAKE} arch ARCH=i386 IOS_PLATFORM=iphonesimulator >/dev/null
	@${MAKE} arch ARCH=x86_64 IOS_PLATFORM=iphonesimulator >/dev/null

WORKDIR = ${CURDIR}/libs
PREFIX = ${WORKDIR}/build/${ARCH}
LIBDIR = ${PREFIX}/lib
INCLUDEDIR = ${PREFIX}/include

PROTOBUF_VER = 3.4.0
PROTOBUF_DIR = ${WORKDIR}/download/protobuf-${PROTOBUF_VER}

CXX = ${XCODE_TOOLCHAIN}/usr/bin/clang++
CC = ${XCODE_TOOLCHAIN}/usr/bin/clang
CFLAGS = -isysroot ${IOS_SDK} -I${IOS_SDK}/usr/include -arch ${ARCH} -miphoneos-version-min=6.0 -fembed-bitcode
CXXFLAGS = -stdlib=libc++ -isysroot ${IOS_SDK} -I${IOS_SDK}/usr/include -arch ${ARCH}  -miphoneos-version-min=6.0 -fembed-bitcode
LDFLAGS = -e _main -stdlib=libc++ -isysroot ${IOS_SDK} -L${LIBDIR} -L${IOS_SDK}/usr/lib -arch ${ARCH} -miphoneos-version-min=6.0
LIBTOOLFLAGS = -arch_only ${ARCH}

ifeq ($(ARCH),armv7)
CXXFLAGS += -D__ARM_EABI__=1
CFLAGS += -D__ARM_EABI__=1
endif

ifeq ($(ARCH),arm64)
CXXFLAGS += -D__ARM_EABI__=1
CFLAGS += -D__ARM_EABI__=1
endif

MACOSX_DEPLOYMENT_TARGET := 10.10
export MACOSX_DEPLOYMENT_TARGET

arch: status ${LIBDIR}/libprotobuf.a

status:
	@printf $(call NICE_PRINT,$(ARCH)) 1>&2;

# Install protobuf from brew
/usr/local/bin/protoc:
	brew install protobuf

# install protobuf from brew before building
${LIBDIR}/libprotobuf.a: /usr/local/bin/protoc ${PROTOBUF_DIR}
	@printf $(call NICE_PRINT,$@) 1>&2;
	cd ${PROTOBUF_DIR} && env CXX=${CXX} CC=${CC} CFLAGS="${CFLAGS}" CXXFLAGS="${CXXFLAGS}" LDFLAGS="${LDFLAGS}" \
	./configure --host=arm-apple-darwin --disable-shared --with-protoc=/usr/local/bin/protoc --prefix=${PREFIX} && \
	${MAKE} clean install

# Download protobuf
${PROTOBUF_DIR}:
	mkdir -p ${WORKDIR}/download
	wget https://github.com/google/protobuf/releases/download/v${PROTOBUF_VER}/protobuf-cpp-${PROTOBUF_VER}.tar.gz
	tar xzvf protobuf-cpp-${PROTOBUF_VER}.tar.gz
	rm protobuf-cpp-${PROTOBUF_VER}.tar.gz
	mv protobuf-${PROTOBUF_VER} ${PROTOBUF_DIR}
	cd ${PROTOBUF_DIR} && ./autogen.sh

clean:
	rm -rf libs