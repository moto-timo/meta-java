SUMMARY = "Streaming pull XML parser for java"
LICENSE = "BSD-4-Clause"
LIC_FILES_CHKSUM = "file://LICENSE.txt;md5=d40c660a53e36a103a35c170d38d0ef6"
HOMEPAGE = "http://www.extreme.indiana.edu/xgws/xsoap/xpp/"

SRC_URI = "http://www.extreme.indiana.edu/xgws/xsoap/xpp/download/PullParser2/PullParser2.1.10.tgz"

S = "${WORKDIR}/PullParser${PV}"

inherit java-library

DEPENDS = "fastjar-native virtual/javac-native"

do_compile() {
    if [ -d build-oe ]; then
        rm -rf build-oe
    fi
    mkdir -p build-oe

    sourcepath="src/java/drivers/jaxp11:src/java/drivers/sax2:src/java/impl/factory:src/java/impl/format:src/java/impl/node:src/java/impl/pullparser:src/java/impl/tag:src/java/intf"

    findpath="$(echo "$sourcepath" | tr : " ")"

    javac -sourcepath $sourcepath -d build-oe `find $findpath -name "*.java"`

    mkdir -p build-oe/META-INF/services
    cp src/java/drivers/jaxp11/META-INF/services/javax.xml.parsers.SAXParserFactory build-oe/META-INF/services
    cp src/java/impl/factory/META-INF/services/org.gjt.xpp.XmlPullParserFactory build-oe/META-INF/services

    fastjar -C build-oe -c -f ${JARFILENAME} .
}

SRC_URI[sha256sum] = "ad82569b809e29c19b8223feaa12923f97bb4bbc942ff985857f9d853db489cf"
