require classpath.inc

include classpath-features-check.inc

LIC_FILES_CHKSUM = "file://LICENSE;md5=92acc79f1f429143f4624d07b253702a"

SRC_URI += " \
            file://sun-security-getproperty.patch;striplevel=0 \
            file://ecj_java_dir.patch \
            file://autotools.patch \
            file://miscompilation.patch \
            file://toolwrapper-exithook.patch \
            file://use_libdir.patch \
            file://freetype2.patch \
            file://aarch64.patch \
           "

SRC_URI[sha256sum] = "f929297f8ae9b613a1a167e231566861893260651d913ad9b6c11933895fecc8"
