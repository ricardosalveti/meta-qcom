# LK is built with the prebuilt AOSP arm-eabi-4.8 toolchain published by
# Qualcomm, which only exists as x86-64 host binaries: the upstream path is
# literally prebuilts/gcc/linux-x86/arm/arm-eabi-4.8, and no aarch64 host
# build of it has ever been published. On any other build host the compiler
# cannot be executed at all, and do_compile fails in a fairly cryptic way,
# with the shell trying to interpret the ELF binary as a script.
#
# Skip the affected recipes on such hosts. This covers the LK recipes
# themselves as well as the boot firmware recipes that depend on them, so
# that a build never deploys a boot binary set silently missing the
# bootloader. Include this from any recipe that builds LK or that depends on
# what LK deploys.

python () {
    build_arch = d.getVar('BUILD_ARCH')
    if build_arch != 'x86_64':
        raise bb.parse.SkipRecipe("LK requires an x86-64 build host, as its "
                                  "arm-eabi-4.8 toolchain is only published "
                                  "as x86-64 binaries, and this host is %s"
                                  % build_arch)
}
