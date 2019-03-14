
obj-m   := procyeo14.o
KDIR    :=      /lib/modules/$(shell uname -r)/build
PWD     :=      $(shell pwd)
default:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules
clean:
	rm      -rf *.ko
	rm      -rf *.mod.*
	rm      -rf .*.cmd
	rm      -rf *.o
	rm      -rf .tmp_versions
	rm	-rf Module.*
	rm	-rf modules.*
	rm	-rf .cache.mk
