SUMMARY = "Lexical analyzer generator for Java"
AUTHOR = "Elliot Berk, A. Appel, C. Scott Ananian"

JLEX_MAIN_FILENAME = "jlex-${PV}-Main.java"
LICENSE = "JLEX"
LIC_FILES_CHKSUM = "file://${UNPACKDIR}/${JLEX_MAIN_FILENAME};beginline=148;endline=166;md5=9bf4a6a951053991db64f9d7330d648a"

PACKAGE_ARCH = "${TUNE_PKGARCH}"

RDEPENDS:${PN} = "java2-runtime"
RDEPENDS:${PN}:class-native = ""

inherit java-library

SRC_URI = "https://www.cs.princeton.edu/~appel/modern/java/JLex/Archive/${PV}/Main.java;downloadfilename=${JLEX_MAIN_FILENAME}"

S = "${WORKDIR}/${BPN}"

do_configure() {
cat > ${B}/jlex << EOF
#!/bin/sh

cp=${datadir_java}/${BP}.jar

exec ${bindir}/java -cp $cp JLex.Main "$@"
EOF
}

do_compile() {
	mkdir -p build
	cp ${JLEX_MAIN_FILENAME} Main.java

	javac -d build Main.java

	fastjar cf ${BP}.jar -C build .
}

do_install:append() {
	install -d ${D}${bindir}
	install -m 0755 ${B}/jlex ${D}${bindir}/
}

PACKAGES = "${PN}"

FILES:${PN} += "${datadir_java}"

SRC_URI[sha256sum] = "aeebaece3b3a53972bb0ba0f810540386c267070ee9dca6ffa43c6ff74a54bd7"

BBCLASSEXTEND = "native"
