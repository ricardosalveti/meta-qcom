DESCRIPTION = "QCOM NHLOS Firmware for Qualcomm Hamoa-IOT platform"
LICENSE = "Proprietary"
LIC_FILES_CHKSUM = "file://Qualcomm-Technologies-Inc.-Proprietary;md5=58d50a3d36f27f1a1e6089308a49b403"

FW_ARTIFACTORY = "softwarecenter.qualcomm.com/download/software/chip/ubuntu_qualcomm_iot-spf-1-0/ubuntu-qualcomm-iot-spf-1-0_test_device_public/"
FW_BUILD_ID = "r1.0_${PV}/hamoa_iot-ubun-1-0"
BOOTBINARIES = "HAMOA_bootbinaries"

SRC_URI = " \
    https://${FW_ARTIFACTORY}/${FW_BUILD_ID}/${BOOTBINARIES}.zip;downloadfilename=${BOOTBINARIES}.zip;name=bootbinaries \
    https://artifacts.codelinaro.org/artifactory/codelinaro-le/Qualcomm_Linux/X1E80100/cdt/IQ-X.1.0-EVK-CDT.tar.gz;downloadfilename=IQ-X.1.0-EVK-CDT_${PV}.tar.gz;name=hamoa-iot-evk \
    "
SRC_URI[hamoa-iot-evk.sha256sum] = "c8b8891438bb933d59bfae76452699640e28411125798ebc5bc112f4ab003742"

QCOM_BOOT_IMG_SUBDIR = "hamoa-iot-evk"

include firmware-qcom-boot-common.inc

do_deploy:append() {
    if [ -d "${UNPACKDIR}/${BOOTBINARIES}/spinor" ]; then
        SAIL_FILES="gpt_backup0.bin gpt_main0.bin prog_firehose_ddr.elf patch0.xml rawprogram0.xml sailhyp.elf sailsw1.elf"

        install -d ${DEPLOYDIR}/${QCOM_BOOT_IMG_SUBDIR}/sail_nor
        for sail_file in ${SAIL_FILES}; do
            install -m 0644 ${UNPACKDIR}/${BOOTBINARIES}/sail_nor/${sail_file} ${DEPLOYDIR}/${QCOM_BOOT_IMG_SUBDIR}/sail_nor
        done
    fi
}