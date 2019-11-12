all: macos ios

include download.mk

macos: prebuild
	@${MAKE} -f macOS.mk

ios: prebuild
	@${MAKE} -f iOS.mk

download:
	mkdir -p download

prebuild: download

clean:
	rm -rf build

clean_all: clean
	rm -rf ${LIB_DIRS}
