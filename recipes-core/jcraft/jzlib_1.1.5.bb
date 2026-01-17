SUMMARY = "zlib implementation in Java"
LICENSE = "BSD-3-Clause"
LIC_FILES_CHKSUM = "file://LICENSE.txt;md5=5726f2e9799112bcdb199b9c7233b454"

HOMEPAGE = "http://www.jcraft.com/jzlib"

SRC_URI = "git://github.com/ymnk/jzlib;protocol=https;branch=master"

inherit java-library

do_compile() {
  mkdir -p build

  javac -sourcepath . -d build `find com -name "*.java"`

  fastjar cf ${JARFILENAME} -C build .
}

SRCREV = "9d0fcb95caad5f205d849928087b2efa202be4d4"

BBCLASSEXTEND = "native"

