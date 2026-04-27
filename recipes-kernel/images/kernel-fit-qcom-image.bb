DESCRIPTION = "U-Boot FIT image containing kernel, initramfs, and device trees for Qualcomm boards"
LICENSE = "MIT"

DEPENDS = "virtual/kernel u-boot-tools-native dtc-native"

INHIBIT_DEFAULT_DEPS = "1"

B = "${WORKDIR}/build"

KERNEL_IMAGEDEST = "/boot"
KERNEL_DEPLOYSUBDIR = ""

# This recipe builds the actual FIT image from kernel-fit-image recipe components
# It requires kernel-fit-image recipe to be built first to generate necessary artifacts
do_compile() {
    mkdir -p ${B}
    # The FIT image is built by the kernel-fit-image recipe
    # We just need to ensure it's copied to our work directory
    if [ ! -f "${DEPLOY_DIR_IMAGE}/fitImage" ]; then
        bberror "fitImage not found in DEPLOY_DIR_IMAGE. Ensure kernel-fit-image recipe was built."
    fi
}

do_install() {
    install -d ${D}/boot
    install -m 0644 ${DEPLOY_DIR_IMAGE}/fitImage ${D}/boot/
}

do_deploy() {
    install -d ${DEPLOY_DIR_IMAGE}
    # fitImage is already deployed by kernel-fit-image recipe
    # This task just ensures proper dependencies are tracked
}

# Mark this recipe as depending on kernel-fit-image to ensure proper build order
# kernel-fit-image is built from the kernel recipe when KERNEL_CLASSES includes kernel-fit-extra-artifacts
python() {
    # Ensure kernel-fit-image is built before this recipe
    d.appendVarFlag('do_compile', 'depends', ' virtual/kernel:do_deploy')
}

addtask deploy before do_build

PACKAGE_ARCH = "${MACHINE_ARCH}"
EXCLUDE_FROM_WORLD = "1"
