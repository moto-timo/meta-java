require openjdk-8-release-common.inc

COMPATIBLE_HOST = "^$"
COMPATIBLE_HOST:armv7a = "arm"
COMPATIBLE_HOST:armv7ve = "arm"

CHANGESET_ID="ga-aarch32-20201026"

CORBA_CHANGESET:arm = "jdk8u${PV}-${CHANGESET_ID}"
SRC_URI[corba.sha256sum] = "44edc982427ae8c645cd414b0a6bb05e0da7e12b3fe99f4013af33f8a531364f"

HOTSPOT_CHANGESET:arm = "jdk8u${PV}-${CHANGESET_ID}"
SRC_URI[hotspot.sha256sum] = "2fd94fc5d83d3d4488f35ce23686c3f3b522b430bb198dbed21c9d13bbe79798"

JAXP_CHANGESET:arm = "jdk8u${PV}-${CHANGESET_ID}"
SRC_URI[jaxp.sha256sum] = "1ba68f7af9e9425e340122c1d548068cac0e2b4f6936ef2ad6e32fdfc6943295"

JAXWS_CHANGESET:arm = "jdk8u${PV}-${CHANGESET_ID}"
SRC_URI[jaxws.sha256sum] = "cdb87a31cb33d1aa6cef2ba2dfd554dc6517f81b0e409368ad3c4e575ed3f41c"

JDK_CHANGESET:arm = "jdk8u${PV}-${CHANGESET_ID}"
SRC_URI[jdk.sha256sum] = "e147fe91c5f291516c88de681db92bf5a2a0ded829dd4c10a7d5f7dc2baf42e9"

LANGTOOLS_CHANGESET:arm = "jdk8u${PV}-${CHANGESET_ID}"
SRC_URI[langtools.sha256sum] = "cd7e6caac72e439df1582520b00b07f5f502cdb65e86b9ed0907eede0284cdd8"

NASHORN_CHANGESET:arm = "jdk8u${PV}-${CHANGESET_ID}"
SRC_URI[nashorn.sha256sum] = "c7aae27155c2fd7643b726eca697d06227ba9127a9c3ced7e1ab88669c08ffb9"

OPENJDK_CHANGESET:arm = "jdk8u${PV}-${CHANGESET_ID}"
SRC_URI[openjdk.sha256sum] = "532ce894685b330057d7932cf147efcb418c4723030e39f2de53a20cf8c50ba8"

PACKAGECONFIG:append:armv7a = " client"
PACKAGECONFIG:append:armv7ve = " client"

do_install:append() {
    if [ -d ${D}${JDK_HOME} ] ; then
        sed -e 's,^\(-server ERROR$\),# \1,' \
            -e 's,^-client IGNORE,-client ERROR,' \
            -e 's,ALIASED_TO -server,ALIASED_TO -client,' \
            -i ${D}${JDK_HOME}/jre/lib/${JDK_ARCH}/jvm.cfg
    fi
    if [ -d ${D}${JRE_HOME} ] ; then
        sed -e 's,^\(-server ERROR$\),# \1,' \
            -e 's,^-client IGNORE,-client ERROR,' \
            -e 's,ALIASED_TO -server,ALIASED_TO -client,' \
            -i ${D}${JRE_HOME}/lib/${JDK_ARCH}/jvm.cfg
    fi
}

JDK_ARCH:armv7a = "aarch32"
JDK_ARCH:armv7ve = "aarch32"
