require icedtea7-native.inc

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
