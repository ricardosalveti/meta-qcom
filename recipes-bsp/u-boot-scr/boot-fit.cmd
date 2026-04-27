# U-Boot boot script for FIT image based kernel boot
# This script loads the FIT image containing kernel, initramfs, and DTBs,
# then executes bootm to boot the system.

# Set address for loading FIT image (adjust based on available memory)
setenv fit_addr_r 0x85000000

# Load FIT image from boot partition
echo "Loading FIT image..."
load mmc 0:1 ${fit_addr_r} /boot/fitImage
if test $? -ne 0; then
    echo "Error: Failed to load FIT image"
    exit 1
fi

# Boot FIT image using default configuration
# This will load kernel, initramfs, and select the appropriate DTB
echo "Booting FIT image..."
bootm ${fit_addr_r}
