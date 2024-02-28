SUMMARY = "OpenJDK for Java 11 built from source."
HOMEPAGE = "https://wiki.openjdk.org/display/JDKUpdates/JDK11u"
LICENSE = "GPL-2.0-with-classpath-exception"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/GPL-2.0-with-classpath-exception;md5=6133e6794362eff6641708cfcc075b80"

COMPATIBLE_HOST = "(x86_64|aarch64).*-linux"
OVERRIDES = "${TARGET_ARCH}"

DEPENDS = "autoconf-native bash-native binutils-native make-native openjdk-11-binary-native patchelf-native unzip-native zip-native"
export BOOT_JDK="${base_libdir}/jvm/java-11-openjdk-amd64"

JVM_RDEPENDS:aarch64 = " \
  alsa-lib (>= 0.9) \
  freetype (>= 2.13) \
  glibc (>= 2.17) \
  libx11 (>= 1.7) \
  libxext (>= 1.3) \
  libxi (>= 1.8) \
  libxrender (>= 0.9) \
  libxtst (>= 1.2) \
  zlib (>= 1.2) \
"

JVM_RDEPENDS:x86_64 = " \
  alsa-lib (>= 0.9) \
  freetype (>= 2.13) \
  glibc (>= 2.17) \
  libx11 (>= 1.7) \
  libxext (>= 1.3) \
  libxi (>= 1.8) \
  libxrender (>= 0.9) \
  libxtst (>= 1.2) \
  zlib (>= 1.2) \
"

RDEPENDS:${PN} = "${JVM_RDEPENDS}"

API_RELEASE_NAME = "jdk-${PV}"
API_OS = "linux"
API_ARCH:aarch64 = "aarch64"
API_ARCH:arm = "arm"
API_ARCH:x86_64 = "x64"
API_IMAGE_TYPE = "jdk"
API_JVM_IMPL = "hotspot"
API_HEAP_SIZE ?= "normal"
API_VENDOR = "eclipse"

# /v3/binary/version/{release_name}/{os}/{arch}/{image_type}/{jvm_impl}/{heap_size}/{vendor}
# https://github.com/adoptium/temurin11-binaries/releases/download/jdk-11.0.21%2B9/OpenJDK11U-jdk-sources_11.0.21_9.tar.gz
# https://openjdk-sources.osci.io/openjdk11/openjdk-11.0.22+7.tar.xz
SRC_URI = "https://openjdk-sources.osci.io/openjdk11/openjdk-11.0.22+7.tar.xz"
SRC_URI[sha256sum] = "e310b1a8343ffb857240ba97ea95f1319781d2d5614de99fc499f674bd268aa1"

S = "${WORKDIR}/jdk-${PV}"


do_configure() {
    CC=${CC} CXX=${CXX} CXXFILT=${RECIPE_SYSROOT_NATIVE} \
    bash configure \
        --with-boot-jdk=${BOOT_JDK} \
        --with-extra-cflags="${CFLAGS}" \
	--with-extra-cxxflags="${CXXFLAGS}" \
	--disable-javac-server
}

# Error: 'make -jN' is not supported, use 'make JOBS=N'
EXTRA_OEMAKE:remove = "${PARALLEL_MAKE}"
EXTRA_OEMAKE = "JOBS=${@oe.utils.parallel_make(d)}"

do_install() {
    cp -R ${B}/* ${D}/
}

inherit pkgconfig native
