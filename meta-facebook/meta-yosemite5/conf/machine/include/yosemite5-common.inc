MACHINEOVERRIDES =. "yosemite5-common:"

require conf/distro/include/phosphor-static-norootfs.inc
require conf/distro/include/pldm.inc
require conf/machine/include/facebook-compute-singlehost.inc
require conf/machine/include/obmc-bsp-common.inc
require conf/machine/include/tpm2.inc
require conf/machine/include/facebook-ncsi-nic.inc

FLASH_SIZE = "131072"

VIRTUAL-RUNTIME_obmc-host-state-manager = "phosphor-state-manager-host"
VIRTUAL-RUNTIME_obmc-chassis-state-manager = "phosphor-state-manager-chassis"

