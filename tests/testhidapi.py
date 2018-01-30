#!/usr/bin/env python

import hid

#Get the list of all devices matching this vendor_id/product_id
vendor_id = 0x1a86
product_id = 0xe008
device_list = hid.enumerate(vendor_id, product_id)

for dev in device_list:
  for key, value in dev.items():
    print("{0}:{1}".format(key,value))
    if key == 'path':
      device = hid.device()
      if device.open_path(value):
      	print(device)
      else:
        print("Error!")



# some stuff
dev = hid.device()
dev.open(vendor_id,product_id)
