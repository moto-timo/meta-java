#require icedtea7-native.inc

SUMMARY = "Harness to build the source code from OpenJDK using Free Software build tools"
HOMEPAGE = "http://icedtea.classpath.org"
LICENSE  = "GPL-2.0-with-classpath-exception"
INC_PR = "r1"

DEPENDS = "virtual/javac-native virtual/java-native classpath-native \
           rhino-native ant-native libxslt-native attr-native \
           giflib-native jpeg-native libpng-native \
           glib-2.0-native freetype-native zlib-native \
           fontconfig-native zip-native \
	   unzip-native make-native util-linux-native \
	   xorgproto-native libx11-native \
          "

PACKAGECONFIG ??= "${@bb.utils.contains('DISTRO_FEATURES', 'x11', 'x11', '', d)}"
PACKAGECONFIG[x11] = ",--disable-headful,libx11-native xorgproto-native libxt-native libxext-native libxrender-native"

OEMAKE_BUILD_HEADLESS_ONLY = "${@bb.utils.contains('PACKAGECONFIG', 'x11', '', 'BUILD_HEADLESS_ONLY=1', d)}"
CFLAGS:append = "${@bb.utils.contains('PACKAGECONFIG', 'x11', '', ' -DHEADLESS=true', d)}"

# Disable dead store elimination and set C++ standard to C++98.
# There are dead stores in the JVM that would be pretty hard to
# remove, so disable the optimisation in the compiler.
CFLAGS:append = " -std=gnu++98 -fno-tree-dse -fno-tree-vectorize"
CXXFLAGS:append = " -std=gnu++98 -fno-tree-dse"

# WORKAROUND: ignore errors from new compilers
CFLAGS:append = " -Wno-error=stringop-overflow -Wno-error=return-type"

# oe.utils.get_host_compiler_version returns "gcc, 9.2" and we want -> "9"
HOST_COMPILER_MAJOR="${@oe.utils.get_host_compiler_version(d)[1].split(".", 2)[0]}"
CFLAGS:append = " ${@oe.utils.less_or_equal('HOST_COMPILER_MAJOR', '8', '', ' -Wno-error=format-overflow', d)}"

# Explicitly pass -fcommon since GCC 10 defaults to -fno-common for C sources
CFLAGS:append = " -fcommon"

inherit java autotools pkgconfig
inherit openjdk-build-helper
inherit python3native
inherit native

JAVA_HOME[unexport] = "1"

ICEDTEA_PATCHES = ""
OPENJDK_PATCHES = ""

SRC_URI = " \
  ${ICEDTEA_URI} \
  ${OPENJDK_URI} \
  ${HOTSPOT_URI} \
  ${CORBA_URI} \
  ${JAXP_URI} \
  ${JAXWS_URI} \
  ${JDK_URI} \
  ${LANGTOOLS_URI} \
  ${ICEDTEA_PATCHES} \
  ${OPENJDK_PATCHES} \
  file://allow-headless-build.patch;striplevel=0 \
  file://jaxws_fix_NullPointerException.patch;apply=no \
  file://timezoneszip.patch \
  file://sigsegv.patch;apply=no \
  file://vframeArray_Fix_sigsegv.patch;apply=no \
  "

S = "${WORKDIR}/${ICEDTEA}"
B = "${S}/build"

# openjdk looks in /usr/include and /usr/lib for freetype by default.
export ALT_FREETYPE_HEADERS_PATH = "${STAGING_INCDIR}/freetype2"
export ALT_FREETYPE_LIB_PATH = "${STAGING_LIBDIR}"
export ALT_CUPS_HEADERS_PATH = "${STAGING_INCDIR}"

# libfreetype.so.6 is copied from STAGING_LIBDIR
# which is already stripped.
INSANE_SKIP:${PN} = "already-stripped"

EXTRA_OECONF = "\
    --with-parallel-jobs=${@openjdk_build_helper_get_parallel_make(d)} \
    --disable-tests \
    --disable-hotspot-tests \
    --disable-langtools-tests \
    --disable-jdk-tests \
    --disable-pulse-java \
    --disable-docs \
    --disable-nss \
    --disable-system-lcms \
    --disable-system-gtk \
    --with-jdk-home=${WORKDIR}/fake-jdk \
    --with-javac=${STAGING_BINDIR_NATIVE}/javac \
    --with-rhino=${STAGING_DATADIR_NATIVE}/java/rhino.jar \
    --with-openjdk-src-zip=${DL_DIR}/${OPENJDK_FILE_DOWNLOAD} \
    --with-hotspot-src-zip=${DL_DIR}/${HOTSPOT_FILE_DOWNLOAD} \
    --with-corba-src-zip=${DL_DIR}/${CORBA_FILE_DOWNLOAD} \
    --with-jaxp-src-zip=${DL_DIR}/${JAXP_FILE_DOWNLOAD} \
    --with-jaxws-src-zip=${DL_DIR}/${JAXWS_FILE_DOWNLOAD} \
    --with-jdk-src-zip=${DL_DIR}/${JDK_FILE_DOWNLOAD} \
    --with-langtools-src-zip=${DL_DIR}/${LANGTOOLS_FILE_DOWNLOAD} \
    ac_cv_path_UNZIP_BIN="${STAGING_BINDIR_NATIVE}/unzip" \
"

JDK_DIR = "icedtea7-native"

JDK_INSTALL_DIR = "${D}/${libdir_jvm}/${JDK_DIR}"

do_configure:prepend() {
	arch="${@openjdk_build_helper_get_icedtea_arch(d)}"
	libarch="${@openjdk_build_helper_get_icedtea_arch(d)}"

	# Automatically copy everything that starts with "icedtea" and ends with
	# ".patch" into the patches directory.
	find ${WORKDIR} -maxdepth 1 -name "icedtea*.patch" -exec cp {} ${S}/patches \;

	# Prepare JDK-like directory with Classpath' files which we can treat as a
	# SYSTEM_GCJ_DIR afterwards.
	mkdir -p ${WORKDIR}/fake-jdk/jre/lib/
	ln -sf ${STAGING_DATADIR_NATIVE}/classpath/glibj.zip ${WORKDIR}/fake-jdk/jre/lib/rt.jar

        mkdir -p ${WORKDIR}/fake-jdk/bin
        ln -sf ${STAGING_BINDIR_NATIVE}/java  ${WORKDIR}/fake-jdk/bin/java
	#ln -sf ${STAGING_BINDIR_NATIVE}/javac ${WORKDIR}/fake-jdk/bin/javac
	ln -sf ${STAGING_BINDIR_NATIVE}/gjavah ${WORKDIR}/fake-jdk/bin/javah
	ln -sf ${STAGING_BINDIR_NATIVE}/gjar  ${WORKDIR}/fake-jdk/bin/jar
	ln -sf ${STAGING_BINDIR_NATIVE}/grmic ${WORKDIR}/fake-jdk/bin/rmic
        ln -sf ${STAGING_BINDIR_NATIVE}/gnative2ascii ${WORKDIR}/fake-jdk/bin/native2ascii

	mkdir -p ${WORKDIR}/fake-jdk/include
	for i in ${STAGING_INCDIR_NATIVE}/classpath/* ;do
		test -r $i | continue;
		bn=`basename $i`
		ln -sf $i ${WORKDIR}/fake-jdk/include/$bn;
	done
}

POST_CONFIGURE_CLEAN_X11 = "${@bb.utils.contains('PACKAGECONFIG', 'x11', '', 'rm openjdk*/jdk/src/solaris/classes/sun/awt/X11/*.java', d)}"

do_configure:append() {
	oe_runmake patch-fsg

	for F in openjdk/jdk/make/common/shared/Compiler-gcc.gmk ;
	do
		sed -i \
			-e"s|\$(COMPILER_PATH)gcc\$(GCC_SUFFIX)|${CC}|" \
			-e"s|\$(COMPILER_PATH)gcc\$(GCC_SUFFIX) -E|${CPP}|" \
			-e"s|\$(COMPILER_PATH)g++\$(GCC_SUFFIX)|${CXX}|" \
			$F
	done

	for F in openjdk/hotspot/make/linux/makefiles/gcc.make openjdk/hotspot/agent/src/os/linux/Makefile ;
	do
		sed -i \
			-e"s|HOST_GCC = gcc$|HOST_GCC = ${CC}|" \
			-e"s|HOST_CPP = g++$|HOST_CPP = ${CXX}|" \
			-e"s|gcc\$(GCC_SUFFIX)|${CC}|" \
			-e"s|g++\$(GCC_SUFFIX)|${CXX}|" \
			$F
	done
	${POST_CONFIGURE_CLEAN_X11}
	oe_runmake patch-boot

	patch -p1 < ${WORKDIR}/jaxws_fix_NullPointerException.patch
	patch -p1 < ${WORKDIR}/sigsegv.patch
	patch -p1 < ${WORKDIR}/vframeArray_Fix_sigsegv.patch
}

EXTRA_OEMAKE = ' \
                OE_CFLAGS="${CFLAGS}" \
                OE_CPPFLAGS="${CPPFLAGS}" \
                OE_CXXFLAGS="${CXXFLAGS}" \
                OE_LDFLAGS="${LDFLAGS}" \
                ZIPEXE="${STAGING_BINDIR_NATIVE}/zip" \
		${OEMAKE_BUILD_HEADLESS_ONLY} \
		DISABLE_HOTSPOT_OS_VERSION_CHECK=ok \
                UNZIP_BIN="${STAGING_BINDIR_NATIVE}/unzip" \
               '

do_compile() {
	# Use JAVA_TOOL_OPTIONS from environment. On some machines this is needed
	# in order to start java when the amount of RAM is limited for the user.
	export JAVA_TOOL_OPTIONS="${JAVA_TOOL_OPTIONS}"

	arch="${@openjdk_build_helper_get_icedtea_arch(d)}"
	libarch="${@openjdk_build_helper_get_icedtea_arch(d)}"

        install -d openjdk-boot/control/build/linux-$arch/hotspot/import/jre/lib
        if [ $libarch != $arch ]; then
                # Something is strange with the directoy naming. OpenJDK uses both variants.
                ln -sf $libarch openjdk-boot/control/build/linux-$arch/hotspot/import/jre/lib/$arch
        fi

        common_dirs="openjdk.build-boot/tmp/sun openjdk.build/tmp/sun"
	for CM in ${common_dirs}
	do
          for DM in javax.sound/jsoundalsa sun.awt.X11/xawt sun.awt/jawt;
          do
                bbnote "creating ${CM}/${DM}/obj64/.files_compiled"
                install -d ${CM}/${DM}/obj64
                touch ${CM}/${DM}/obj64/.files_compiled

                bbnote "creating ${CM}/${DM}/obj/.files_compiled"
                install -d ${CM}/${DM}/obj
                touch ${CM}/${DM}/obj/.files_compiled
          done
        done

        # Now cross-compile bootstrap JDK
        oe_runmake icedtea
}

do_install() {
	JDK_ARCH="${@openjdk_build_helper_get_icedtea_arch(d)}"

	install -d ${JDK_INSTALL_DIR}

	SDKDIR=openjdk.build/j2sdk-image
	for F in ${SDKDIR}/bin ${SDKDIR}/include ${SDKDIR}/jre ${SDKDIR}/lib
	do
		cp -RL $F ${JDK_INSTALL_DIR}
	done

	#cp openjdk-boot/jdk/src/solaris/hpi/export/*.h ${JDK_INSTALL_DIR}/include/hpi
	#cp openjdk-boot/jdk/src/share/hpi/export/*.h ${JDK_INSTALL_DIR}/include/hpi

	install -d ${JDK_INSTALL_DIR}/include/javavm
	cp openjdk-boot/jdk/src/share/javavm/export/*.h ${JDK_INSTALL_DIR}/include/javavm
	cp openjdk-boot/jdk/src/solaris/javavm/export/*.h ${JDK_INSTALL_DIR}/include/javavm

	# Fix libjvm.so dependency of libfontmanager.so
	ln -sf server/libjvm.so ${JDK_INSTALL_DIR}/jre/lib/${JDK_ARCH}/libjvm.so
	ln -sf server/libjvm.so ${JDK_INSTALL_DIR}/jre/lib/${JDK_ARCH}/libjvm.so.0

	# Fix missing write permissions on the files.
	chmod ug+w -R ${JDK_INSTALL_DIR}
}

# end icedtea7-native.inc

LIC_FILES_CHKSUM = "file://COPYING;md5=59530bdf33659b29e73d4adb9f9f6552"

#FILESPATH =. "${FILE_DIRNAME}/openjdk-7-03b147:"

# Force arm mode for now
ARM_INSTRUCTION_SET:armv4t = "ARM"

ICEDTEA_URI = "https://icedtea.wildebeest.org/download/source/${ICEDTEA}.tar.gz;name=iced"

ICEDTEA_DROPS_URL = "http://icedtea.wildebeest.org/download/drops/icedtea7/${PV}"

OPENJDK_URI = "${ICEDTEA_DROPS_URL}/openjdk.tar.bz2;name=openjdk;unpack=false"
SRC_URI[openjdk.sha256sum] = "e745fe51872e3c8833a1f6560ab9d6090eec0242b1164e69fb708b1dbe801838"

HOTSPOT_URI = "${ICEDTEA_DROPS_URL}/hotspot.tar.bz2;name=hotspot;unpack=false"
SRC_URI[hotspot.sha256sum] = "897469ccfb3e4b826499a65d7477490a2fbef8f668a25b07e141cd5eddef9521"

CORBA_URI = "${ICEDTEA_DROPS_URL}/corba.tar.bz2;name=corba;unpack=false"
SRC_URI[corba.sha256sum] = "926e4e3368c3b912bfa08c27605be2038f0b6de9d91a1c254bb020b458c5dd15"

JAXP_URI = "${ICEDTEA_DROPS_URL}/jaxp.tar.bz2;name=jaxp;unpack=false"
SRC_URI[jaxp.sha256sum] = "d8470dbeb43fd681ac2d8479b11877bbc41faec794a73b5f0327c8a264c8f223"

JAXWS_URI = "${ICEDTEA_DROPS_URL}/jaxws.tar.bz2;name=jaxws;unpack=false"
SRC_URI[jaxws.sha256sum] = "31d90ce0830a4eac4c4d379efc557062cce4464d0598779781daf286052a9116"

JDK_URI = "${ICEDTEA_DROPS_URL}/jdk.tar.bz2;name=jdk;unpack=false"
SRC_URI[jdk.sha256sum] = "7c3c402f94e3938cc0eb6ff3822c4f73f57a21b45c562a40b35d8f0ec7395f65"

LANGTOOLS_URI = "${ICEDTEA_DROPS_URL}/langtools.tar.bz2;name=langtools;unpack=false"
SRC_URI[langtools.sha256sum] = "2ab500fd270b0cb9a277b6c236189c10923bcc7125b570ba6a77852932eeb0cd"

CACAO_VERSION = "c182f119eaad"
CACAO_FILE = "cacao-${CACAO_VERSION}.tar.gz"
CACAO_URI = "http://icedtea.wildebeest.org/download/drops/cacao/${CACAO_FILE};name=cacao;unpack=false"
SRC_URI[cacao.sha256sum] = "eade43f6c5da8b3b5a64e6b206b624eaca88539f80101d7164d6aa426eb590b1"

JAMVM_VERSION = "2.0.0"
JAMVM_FILE = "jamvm-${JAMVM_VERSION}.tar.gz"
JAMVM_URI = "https://icedtea.classpath.org/download/drops/jamvm/${JAMVM_FILE};name=jamvm;unpack=false"
SRC_URI[jamvm.sha256sum] = "76428e96df0ae9dd964c7a7c74c1e9a837e2f312c39e9a357fa8178f7eff80da"

# readdir_r was deprecated in glibc-2.24. Ignore the error for now
# NOTE: When updating the recipe, please check if this is still needed
CFLAGS:append = " -Wno-error=deprecated-declarations"
