  section BOOTLDR
  incbin "obj/bootnibbles"

  section DEVICE
  incbin "ide_device/lide.device"
