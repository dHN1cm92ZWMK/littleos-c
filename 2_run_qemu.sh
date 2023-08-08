#!/usr/bin/bash

qemu-system-i386 -D ./qemu.log --drive media=cdrom,file=os.iso,readonly=on
