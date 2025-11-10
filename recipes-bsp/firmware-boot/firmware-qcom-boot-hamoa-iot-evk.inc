DESCRIPTION = "QCOM NHLOS Firmware for Qualcomm Hamoa-IOT platform"
LICENSE = "Proprietary"
LIC_FILES_CHKSUM = "file://${UNPACKDIR}/LICENSE.${BPN};md5=6e1bae7ef13289c332a27b917fb49764"

FW_ARTIFACTORY = "softwarecenter.qualcomm.com/download/software/chip/ubuntu_qualcomm_iot-spf-1-0/ubuntu-qualcomm-iot-spf-1-0_test_device_public/"
FW_BUILD_ID = "r1.0_${PV}/hamoa_iot-ubun-1-0"
BOOTBINARIES = "HAMOA_bootbinaries/spinor"

SRC_URI = " \
    https://${FW_ARTIFACTORY}/${FW_BUILD_ID}/HAMOA_bootbinaries.zip;downloadfilename=HAMOA_bootbinaries_${PV}.zip;name=bootbinaries \
    https://${FW_ARTIFACTORY}/${FW_BUILD_ID}/LICENSE.txt;downloadfilename=LICENSE.${BPN};name=license \
    https://artifacts.codelinaro.org/artifactory/codelinaro-le/Qualcomm_Linux/X1E80100/cdt/IQ-X.1.0-EVK-CDT.tar.gz;downloadfilename=IQ-X.1.0-EVK-CDT_${PV}.tar.gz;name=hamoa-iot-evk \
    "
SRC_URI[hamoa-iot-evk.sha256sum] = "c8b8891438bb933d59bfae76452699640e28411125798ebc5bc112f4ab003742"
SRC_URI[license.sha256sum] = "3ad8f1fd82f2918c858cec2d0887b7df6f71a06416beecfdb3efe7d62874d863"

QCOM_BOOT_IMG_SUBDIR = "hamoa-iot-evk"

include firmware-qcom-boot-common.inc
