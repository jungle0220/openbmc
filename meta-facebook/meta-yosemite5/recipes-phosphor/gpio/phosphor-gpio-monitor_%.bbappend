FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"

inherit obmc-phosphor-systemd systemd

SRC_URI += "file://yosemite5-phosphor-multi-gpio-monitor.json \
            file://gpio_sel_logger \
            file://gpio_sel_logger@.service \
            file://reset_btn \
            file://reset_btn@.service \
            file://gpio_bypass \
            file://gpio_bypass@.service \
            file://multi-gpios-sys-init \
            file://multi-gpios-sys-init.service \
            file://assert-power-good-drop \
            file://assert-power-good-drop.service \
            file://deassert-power-good-drop \
            file://deassert-power-good-drop.service \
            "

RDEPENDS:${PN}:append = " bash"

FILES:${PN} += "${systemd_system_unitdir}/*"

SYSTEMD_SERVICE:${PN} += " \
    gpio_sel_logger@.service \
    reset_btn@.service \
    multi-gpios-sys-init.service \
    assert-power-good-drop.service \
    deassert-power-good-drop.service \
    "

SYSTEMD_AUTO_ENABLE = "enable"

do_install:append:() {
    install -d ${D}${datadir}/phosphor-gpio-monitor
    install -m 0644 ${UNPACKDIR}/yosemite5-phosphor-multi-gpio-monitor.json \
                    ${D}${datadir}/phosphor-gpio-monitor/phosphor-multi-gpio-monitor.json
    install -m 0644 ${UNPACKDIR}/gpio_sel_logger@.service ${D}${systemd_system_unitdir}/
    install -m 0644 ${UNPACKDIR}/reset_btn@.service ${D}${systemd_system_unitdir}/
    install -m 0644 ${UNPACKDIR}/gpio_bypass@.service ${D}${systemd_system_unitdir}/
    install -m 0644 ${UNPACKDIR}/multi-gpios-sys-init.service ${D}${systemd_system_unitdir}/
    install -m 0755 ${UNPACKDIR}/assert-power-good-drop.service ${D}${systemd_system_unitdir}/
    install -m 0755 ${UNPACKDIR}/deassert-power-good-drop.service ${D}${systemd_system_unitdir}/
    install -d ${D}${libexecdir}/${PN}
    install -m 0755 ${UNPACKDIR}/gpio_sel_logger ${D}${libexecdir}/${PN}/
    install -m 0755 ${UNPACKDIR}/reset_btn ${D}${libexecdir}/${PN}/
    install -m 0755 ${UNPACKDIR}/gpio_bypass ${D}${libexecdir}/${PN}/
    install -m 0755 ${UNPACKDIR}/multi-gpios-sys-init ${D}${libexecdir}/${PN}/
    install -m 0755 ${UNPACKDIR}/assert-power-good-drop ${D}${libexecdir}/${PN}/
    install -m 0755 ${UNPACKDIR}/deassert-power-good-drop ${D}${libexecdir}/${PN}/
}

SYSTEMD_OVERRIDE:${PN}-monitor += "phosphor-multi-gpio-monitor.conf:phosphor-multi-gpio-monitor.service.d/phosphor-multi-gpio-monitor.conf"

