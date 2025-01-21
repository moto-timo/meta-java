require jakarta-commons.inc
LIC_FILES_CHKSUM = "file://LICENSE.txt;md5=2a944942e1496af1886903d274dedb13"

PR = "${INC_PR}.1"

SUMMARY = "Java argument parsing helper classes"

SRC_URI = "http://archive.apache.org/dist/commons/cli/source/${BP}-src.tar.gz"

SRC_URI[sha256sum] = "929eb140c136673e7f5029cd206c81b3c1f5e66bba0dfdcd021b9dd5596356d2"

BBCLASSEXTEND = "native"
