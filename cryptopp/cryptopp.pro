# The Crypto++ Library (as a compilation) is currently licensed under the Boost
# Software License 1.0 (http://www.boost.org/users/license.html).
#
# Boost Software License - Version 1.0 - August 17th, 2003
#
# Permission is hereby granted, free of charge, to any person or organization
# obtaining a copy of the software and accompanying documentation covered by
# this license (the "Software") to use, reproduce, display, distribute,
# execute, and transmit the Software, and to prepare derivative works of the
# Software, and to permit third-parties to whom the Software is furnished to
# do so, all subject to the following:
#
# The copyright notices in the Software and this entire statement, including
# the above license grant, this restriction and the following disclaimer,
# must be included in all copies of the Software, in whole or in part, and
# all derivative works of the Software, unless such copies or derivative
# works are solely in the form of machine-executable object code generated by
# a source language processor.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
# SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
# FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
# ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
# DEALINGS IN THE SOFTWARE.

include(../QtSESAM.pri)

QT -= core gui

TARGET = cryptopp
TEMPLATE = lib
CONFIG += staticlib warn_off c++11

linux:QMAKE_CXXFLAGS += -std=c++11
linux:CONFIG(release):QMAKE_CXXFLAGS += -O3

VER_MAJ = 5
VER_MIN = 6
VER_PAT = 2

win32-g++ {
    QMAKE_CXXFLAGS_RELEASE += -O3
}

win32-msvc* {
    QMAKE_CXXFLAGS += /wd4100
    QMAKE_CXXFLAGS_DEBUG += /sdl
    QMAKE_CXXFLAGS_RELEASE += /GA /GL /Ox
    QMAKE_LFLAGS += /LTCG
    DEFINES += _SCL_SECURE_NO_WARNINGS
    DEFINES -= UNICODE
}

SOURCES += \
    3way.cpp \
    adler32.cpp \
    algebra.cpp \
    algparam.cpp \
    arc4.cpp \
    asn.cpp \
    authenc.cpp \
    base32.cpp \
    base64.cpp \
    basecode.cpp \
    bench.cpp \
    bench2.cpp \
    bfinit.cpp \
    blowfish.cpp \
    blumshub.cpp \
    camellia.cpp \
    cast.cpp \
    casts.cpp \
    cbcmac.cpp \
    ccm.cpp \
    channels.cpp \
    cmac.cpp \
    cpu.cpp \
    crc.cpp \
    cryptlib.cpp \
    cryptlib_bds.cpp \
    datatest.cpp \
    default.cpp \
    des.cpp \
    dessp.cpp \
    dh.cpp \
    dh2.cpp \
    dll.cpp \
    dlltest.cpp \
    dsa.cpp \
    eax.cpp \
    ec2n.cpp \
    eccrypto.cpp \
    ecp.cpp \
    elgamal.cpp \
    emsa2.cpp \
    eprecomp.cpp \
    esign.cpp \
    files.cpp \
    filters.cpp \
    fips140.cpp \
    fipsalgt.cpp \
    gcm.cpp \
    gf2_32.cpp \
    gf2n.cpp \
    gf256.cpp \
    gfpcrypt.cpp \
    gost.cpp \
    gzip.cpp \
    hex.cpp \
    hmac.cpp \
    hrtimer.cpp \
    ida.cpp \
    idea.cpp \
    integer.cpp \
    iterhash.cpp \
    luc.cpp \
    mars.cpp \
    marss.cpp \
    md2.cpp \
    md4.cpp \
    md5.cpp \
    misc.cpp \
    modes.cpp \
    mqueue.cpp \
    mqv.cpp \
    nbtheory.cpp \
    network.cpp \
    oaep.cpp \
    osrng.cpp \
    panama.cpp \
    pch.cpp \
    pkcspad.cpp \
    polynomi.cpp \
    pssr.cpp \
    pubkey.cpp \
    queue.cpp \
    rabin.cpp \
    randpool.cpp \
    rc2.cpp \
    rc5.cpp \
    rc6.cpp \
    rdtables.cpp \
    regtest.cpp \
    rijndael.cpp \
    ripemd.cpp \
    rng.cpp \
    rsa.cpp \
    rw.cpp \
    safer.cpp \
    salsa.cpp \
    seal.cpp \
    seed.cpp \
    serpent.cpp \
    sha.cpp \
    sha3.cpp \
    shacal2.cpp \
    shark.cpp \
    sharkbox.cpp \
    skipjack.cpp \
    socketft.cpp \
    sosemanuk.cpp \
    square.cpp \
    squaretb.cpp \
    strciphr.cpp \
    tea.cpp \
    tftables.cpp \
    tiger.cpp \
    tigertab.cpp \
    trdlocal.cpp \
    ttmac.cpp \
    twofish.cpp \
    validat0.cpp \
    validat1.cpp \
    validat2.cpp \
    validat3.cpp \
    vmac.cpp \
    wait.cpp \
    wake.cpp \
    whrlpool.cpp \
    winpipes.cpp \
    xtr.cpp \
    xtrcrypt.cpp \
    zdeflate.cpp \
    zinflate.cpp \
    zlib.cpp \
    simple.cpp

HEADERS += \
    3way.h \
    adler32.h \
    aes.h \
    algebra.h \
    algparam.h \
    arc4.h \
    argnames.h \
    asn.h \
    authenc.h \
    base32.h \
    base64.h \
    basecode.h \
    bench.h \
    blowfish.h \
    blumshub.h \
    camellia.h \
    cast.h \
    cbcmac.h \
    ccm.h \
    channels.h \
    cmac.h \
    config.h \
    cpu.h \
    crc.h \
    cryptlib.h \
    default.h \
    des.h \
    dh.h \
    dh2.h \
    dll.h \
    dmac.h \
    dsa.h \
    eax.h \
    ec2n.h \
    eccrypto.h \
    ecp.h \
    elgamal.h \
    emsa2.h \
    eprecomp.h \
    esign.h \
    factory.h \
    files.h \
    filters.h \
    fips140.h \
    fltrimpl.h \
    gcm.h \
    gf2_32.h \
    gf2n.h \
    gf256.h \
    gfpcrypt.h \
    gost.h \
    gzip.h \
    hex.h \
    hkdf.h \
    hmac.h \
    hrtimer.h \
    ida.h \
    idea.h \
    integer.h \
    iterhash.h \
    lubyrack.h \
    luc.h \
    mars.h \
    md2.h \
    md4.h \
    md5.h \
    mdc.h \
    misc.h \
    modarith.h \
    modes.h \
    modexppc.h \
    mqueue.h \
    mqv.h \
    nbtheory.h \
    network.h \
    nr.h \
    oaep.h \
    oids.h \
    osrng.h \
    panama.h \
    pch.h \
    pkcspad.h \
    polynomi.h \
    pssr.h \
    pubkey.h \
    pwdbased.h \
    queue.h \
    rabin.h \
    randpool.h \
    rc2.h \
    rc5.h \
    rc6.h \
    resource.h \
    rijndael.h \
    ripemd.h \
    rng.h \
    rsa.h \
    rw.h \
    safer.h \
    salsa.h \
    seal.h \
    secblock.h \
    seckey.h \
    seed.h \
    serpent.h \
    serpentp.h \
    sha.h \
    sha3.h \
    shacal2.h \
    shark.h \
    skipjack.h \
    smartptr.h \
    socketft.h \
    sosemanuk.h \
    square.h \
    stdcpp.h \
    strciphr.h \
    tea.h \
    tiger.h \
    trap.h \
    trdlocal.h \
    trunhash.h \
    ttmac.h \
    twofish.h \
    validate.h \
    vmac.h \
    wait.h \
    wake.h \
    whrlpool.h \
    winpipes.h \
    words.h \
    xtr.h \
    xtrcrypt.h \
    zdeflate.h \
    zinflate.h \
    zlib.h \
    simple.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}