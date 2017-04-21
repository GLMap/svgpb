# SVGView
Set of tools to render simple SVG files on iOS using Core Graphics

## Prerequisites

To download and build protobuf just run `make`.

## Build

`svg2pb` is used in Build Rule of SVGViewTest target to create protobuf version of SVG images and it should be build first.

1. Launch **Archive** action on svg2pb target to build and install `svg2pb` into `/usr/local/bin`. You may need to update *Developer Team* to build it.
2. Run `SVGViewDemo` application
