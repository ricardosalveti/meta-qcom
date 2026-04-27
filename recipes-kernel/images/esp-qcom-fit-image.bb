DESCRIPTION = "EFI System Partition Image with U-Boot FIT kernel for Qualcomm boards"

PACKAGE_INSTALL = " \
    u-boot-qcom \
    u-boot-scr-fit \
"

# Inherit image-related classes but NOT UKI-related ones
inherit image features_check

require esp-qcom-image-base.inc

# Remove EFI UKI-specific kernel device tree configuration
KERNEL_DEVICETREE = ""

# Setup boot files for FIT image-based boot
setup_fit_boot_files() {
    mkdir -p ${IMAGE_ROOTFS}/boot
    
    # Copy FIT image from deploy directory
    if [ -f ${DEPLOY_DIR_IMAGE}/fitImage ]; then
        cp ${DEPLOY_DIR_IMAGE}/fitImage ${IMAGE_ROOTFS}/boot/
    else
        bberror "fitImage not found in DEPLOY_DIR_IMAGE"
    fi
    
    # Copy U-Boot boot script from deploy directory
    if [ -f ${DEPLOY_DIR_IMAGE}/boot.scr ]; then
        cp ${DEPLOY_DIR_IMAGE}/boot.scr ${IMAGE_ROOTFS}/boot/
    else
        bberror "boot.scr not found in DEPLOY_DIR_IMAGE"
    fi
    
    # Move U-Boot EFI content from /boot to root if present
    if [ -d ${IMAGE_ROOTFS}/boot/EFI ]; then
        mv ${IMAGE_ROOTFS}/boot/EFI/* ${IMAGE_ROOTFS}/EFI 2>/dev/null || true
    fi
    
    # Remove everything except boot and EFI directories
    find ${IMAGE_ROOTFS} -mindepth 1 ! -path "${IMAGE_ROOTFS}/boot*" ! -path "${IMAGE_ROOTFS}/EFI*" -exec rm -rf {} + 2>/dev/null || true
}

IMAGE_PREPROCESS_COMMAND:append = " setup_fit_boot_files"

# Dependencies on FIT image and boot script
do_image[depends] += "kernel-fit-qcom-image:do_deploy u-boot-scr-fit:do_deploy"
