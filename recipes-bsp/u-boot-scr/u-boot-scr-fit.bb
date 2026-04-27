DESCRIPTION = "U-Boot boot script for FIT image based kernel boot"
AUTHOR = "Qualcomm"
DEPENDS = "u-boot-tools-native"
LICENSE = "MIT"

B = "${WORKDIR}/build"

inherit deploy

do_compile() {
    mkdir -p ${B}
    mkimage -A ${UBOOT_ARCH} -T script -C none -n "FIT Boot Script" \
        -d ${WORKDIR}/boot-fit.cmd ${B}/boot.scr
}

do_install() {
    install -d ${D}/boot
    install -m 0644 ${B}/boot.scr ${D}/boot/
}

do_deploy() {
    install -d ${DEPLOY_DIR_IMAGE}
    install -m 0644 ${B}/boot.scr ${DEPLOY_DIR_IMAGE}/
}

addtask deploy before do_build after do_compile
