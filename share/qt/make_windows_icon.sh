#!/bin/bash
# create multiresolution windows icon
ICON_DST=../../src/qt/res/icons/Pseudocoin.ico

convert ../../src/qt/res/icons/Pseudocoin-16.png ../../src/qt/res/icons/Pseudocoin-32.png ../../src/qt/res/icons/Pseudocoin-48.png ${ICON_DST}
