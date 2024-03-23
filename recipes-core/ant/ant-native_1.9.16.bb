SUMMARY = "Another Neat Tool - build system for Java"
AUTHOR = "Apache Software Foundation"
HOMEPAGE = "http://ant.apache.org"
LICENSE = "Apache-2.0 & SAX-PD-2.0"
LIC_FILES_CHKSUM = "file://LICENSE;md5=c6050248a5a97f67ac931b5254c058cb"

SRC_URI = "\
    https://dlcdn.apache.org/ant/source/apache-ant-${PV}-src.tar.bz2 \
    file://ant \
"

SRC_URI[sha512sum] = "996e4939864ec2e05f24c76f10618b5866d26d2fd6d50fe9a7fe675576c34e72e0189fd6caab9bfe0f3c7f3b00c3582691d460ecb9086b9c57954a9dbb1522cb"

S = "${WORKDIR}/apache-ant-${PV}"

inherit java-library native

DEPENDS = " \
    antlr-native \
    bcel-native \
    bsf-native \
    commons-logging-native \
    commons-net-native \
    gnujaf-native \
    gnumail-native \
    jdepend-native \
    jsch-native \
    junit-native \
    log4j1.2-native \
    oro-native \
    regexp-native \
    xalan-j-native \
    xerces-j-native \
    xml-commons-resolver1.1-native \
"

do_deletecruft() {
	# Removes things that need proprietary Jar files or are otherwise problematic
	rm -rf ${S}/src/main/org/apache/tools/ant/taskdefs/optional/image
	rm -rf ${S}/src/main/org/apache/tools/ant/types/optional/image
	rm -rf ${S}/src/main/org/apache/tools/ant/taskdefs/optional/ejb
	rm -rf ${S}/src/main/org/apache/tools/ant/taskdefs/optional/scm
	rm -rf ${S}/src/main/org/apache/tools/ant/taskdefs/optional/starteam
	rm -rf ${S}/src/main/org/apache/tools/ant/taskdefs/optional/NetRexxC.java
}

addtask deletecruft before do_patch after do_deletebinaries

do_compile() {
  mkdir -p build

  oe_makeclasspath cp -s jsch bsf xalan2 xercesImpl resolver gnumail gnujaf bcel regexp log4j1.2 antlr oro junit jdepend commons-net commons-logging
  cp=build:$cp

  find src/main -name "*.java" > java_files

  javac -sourcepath src/main -cp $cp -d build @java_files

  mkdir -p build/org/apache/tools/ant/types/conditions

  cp -r src/resources/org build/
  (cd src/main && find . \( -name "*.properties" -or -name "*.xml" -or -name "*.mf" \) -exec cp {} ../../build/{} \;)

  echo "VERSION=${PV}" > build/org/apache/tools/ant/version.txt
  echo "DATE=`date -R`" >> build/org/apache/tools/ant/version.txt

  fastjar cf ${JARFILENAME} -C build .

  oe_makeclasspath cp -s ecj-bootstrap jsch bsf xalan2 xercesImpl resolver gnumail gnujaf bcel regexp log4j1.2 antlr oro junit jdepend commons-net commons-logging
  cp=${STAGING_DATADIR_JAVA_NATIVE}/ant.jar:${STAGING_DATADIR}/classpath/tools.zip:$cp
  sed -i -e"s|@JAR_FILE@|$cp|" ${WORKDIR}/ant
}

do_install:append() {
	install -d ${D}${bindir}
	install -m 0755 ${WORKDIR}/ant ${D}${bindir}
}

