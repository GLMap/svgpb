PROTOBUF_VER = 3.10.1

PROTOBUF_DIR = download/protobuf-${PROTOBUF_VER}

LIB_DIRS = \
	${PROTOBUF_DIR}

${PROTOBUF_DIR}:
	rm -f download/protobuf

	wget https://github.com/google/protobuf/releases/download/v${PROTOBUF_VER}/protobuf-cpp-${PROTOBUF_VER}.tar.gz
	tar xzvf protobuf-cpp-${PROTOBUF_VER}.tar.gz
	rm protobuf-cpp-${PROTOBUF_VER}.tar.gz
	mv protobuf-${PROTOBUF_VER} ${PROTOBUF_DIR}
#	cd ${PROTOBUF_DIR} && ./autogen.sh

download/protobuf: | ${PROTOBUF_DIR}	
	echo ${PROTOBUF_VER} > download/protobuf
