FILESEXTRAPATHS:prepend := "${THISDIR}/files:"

SRC_URI:append = " \
    file://mctp_init.service \
    file://mctp_init.sh \
"

SYSTEMD_SERVICE:${PN}:append = " \
    mctp_init.service \
"

RDEPENDS:${PN}:append = "bash"

do_install:append () {
    install -d ${D}${libexecdir}/mctp
    install -m 0755 ${UNPACKDIR}/*.sh ${D}${libexecdir}/mctp

    install -d ${D}${systemd_system_unitdir}
    install -m 0644 ${UNPACKDIR}/*.service ${D}${systemd_system_unitdir}
}
