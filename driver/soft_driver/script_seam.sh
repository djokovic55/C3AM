#! usr/bin/bash

echo "Reinitializing seam driver"

sudo rmmod seam_driver.ko
make clean
make
sudo insmod seam_driver.ko
sudo chmod a+rw /dev/xlnx,seam
sudo chmod a+rw /dev/xlnx,dma_seam


echo "Reinitializing seam driver, done"