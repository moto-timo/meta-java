DESCRIPTION = "Common way for components to be created, initialized, configured, started. (API-only)"
AUTHOR = "Apache Software Foundation"
LICENSE = "Apache-2.0"
LIC_FILES_CHKSUM = "file://LICENSE.txt;md5=1dece7821bf3fd70fe1309eaa37d52a2"
PR = "r2"

SRC_URI = " \
   https://archive.apache.org/dist/excalibur/excalibur-framework/source/${BP}-src.tar.gz;name=archive \
   https://archive.apache.org/dist/avalon/logkit/source/logkit-1.2.2-src.tar.gz;name=logkit \
   "

inherit java-library

do_compile() {
  mkdir -p build

  #	Allow reaching method definitions from logkit (stupid cyclic dependency).
  srcpath=src/java:${UNPACKDIR}/logkit-1.2.2-dev/src/java

  javac -encoding ISO-8859-1 -sourcepath $srcpath -d build `find src/java -name "*.java"`

  # Remove classes that belong to logkit ...
  rm -rf ${S}/build/org/apache/log

  fastjar cf ${JARFILENAME} -C build .
}


SRC_URI[archive.sha256sum] = "a4d56a053609ddfc77f6a42c3f15a11708d5e0eb29ffc60a40b87e4cc7331d47"
SRC_URI[logkit.sha256sum] = "2c81edc87571fbd05797da7f65515e089c62cbb735bdbd10f93e29bd3aa3ddb8"

BBCLASSEXTEND = "native"
