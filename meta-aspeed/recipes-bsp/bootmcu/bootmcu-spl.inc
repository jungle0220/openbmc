SUMMARY = "BootMCU firmware and execute SPL"
DESCRIPTION = "BootMCU is designated to load the first, verified image for the main processor"
PACKAGE_ARCH = "${MACHINE_ARCH}"

PROVIDES += "virtual/bootmcu"
DEPENDS = "kern-tools-native swig-native ${PYTHON_PN}-setuptools-native fmc-imgtool-native fmc-images"

do_install[noexec] = "1"

inherit deploy cml1 python3native

EXTRA_OEMAKE = 'CROSS_COMPILE=${TARGET_PREFIX} CC="${TARGET_PREFIX}gcc ${TOOLCHAIN_OPTIONS}" V=1'
EXTRA_OEMAKE += 'HOSTCC="${BUILD_CC} ${BUILD_CFLAGS} ${BUILD_LDFLAGS}"'
EXTRA_OEMAKE += 'STAGING_INCDIR=${STAGING_INCDIR_NATIVE} STAGING_LIBDIR=${STAGING_LIBDIR_NATIVE}'

PACKAGECONFIG ??= "openssl"
# u-boot will compile its own tools during the build, with specific
# configurations (aka when CONFIG_FIT_SIGNATURE is enabled) openssl is needed as
# a host build dependency.
PACKAGECONFIG[openssl] = ",,openssl-native"

# Allow setting an additional version string that will be picked up by the
# u-boot build system and appended to the u-boot version.  If the .scmversion
# file already exists it will not be overwritten.
UBOOT_LOCALVERSION ?= ""


# returns all the elements from the src uri that are .cfg files
def find_cfgs(d):
    sources=src_patches(d, True)
    sources_list=[]
    for s in sources:
        if s.endswith('.cfg'):
            sources_list.append(s)

    return sources_list


do_configure () {
    oe_runmake -C ${S} O=${B} ${BOOTMCU_MACHINE}
    merge_config.sh -m .config ${@" ".join(find_cfgs(d))}
    cml1_do_configure
}

do_compile() {
    unset LDFLAGS
    unset CFLAGS
    unset CPPFLAGS

    if [ ! -e ${B}/.scmversion -a ! -e ${S}/.scmversion ]
    then
        echo ${UBOOT_LOCALVERSION} > ${B}/.scmversion
        echo ${UBOOT_LOCALVERSION} > ${S}/.scmversion
    fi

    oe_runmake -C ${S} O=${B} all
}

do_deploy() {
    sub_ver=$(cat ${B}/.subversion)
    install -d ${DEPLOYDIR}

    # spl
    install -m 644 ${B}/spl/u-boot-spl.dtb ${DEPLOYDIR}
    install -m 644 ${B}/spl/u-boot-spl-nodtb.bin ${DEPLOYDIR}
    install -m 644 ${B}/spl/u-boot-spl.bin ${DEPLOYDIR}

    # spl fmc AST2700 A1
    install -m 644 ${BOOTMCU_FW_BINARY} ${DEPLOYDIR}
    install -m 644 ${BOOTMCU_FW_BINARY} ${DEPLOYDIR}/${BOOTMCU_FW_BINARY}-${sub_ver}
}

addtask deploy before do_build after do_compile

# Use fmc-imgtool to create fmc image since A1
# export CRYPTOGRAPHY_OPENSSL_NO_LEGACY variable to fix the following errors.
# OpenSSL 3.0 legacy provider failed to load
# https://github.com/pyca/cryptography/issues/10598
do_create_fmc_image() {
    export CRYPTOGRAPHY_OPENSSL_NO_LEGACY=1

    fmc-imgtool \
        --verbose \
        --version 2 \
        --input ${B}/spl/u-boot-spl.bin \
        --output ${B}/${BOOTMCU_FW_BINARY} \
        --prebuilt-dir ${STAGING_DATADIR}/fmc-images/prebuilt/
}

addtask create_fmc_image before do_deploy after do_compile

ALLOW_EMPTY:${PN} = "1"
