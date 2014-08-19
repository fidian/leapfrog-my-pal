LeapPad My Pal Software
=======================

It was time to get rid of "My Pal Scout" and "My Pal Violet".  I tried to find ways to remove my daughter's preferences from the device but that did not work.  (They're listed below in case they work for you.)  Normally I can search for instructions on how to wipe or factory reset the device.  This time the search was uneventful.

Luckily, Robert Copeland posted [an article](http://bobcopeland.com/blog/2011/07/my-pal-scsi/) about how to read the contents of the flash.  I copied the program and made it work through an extremely simplistic command-line program.  Then I modified it enough to make one that performs the wiping.

You should be able to pull down the audio files that are saved.  Apparently they are 4 bit and highly compressed and could be in a different byte order.  The child's name (for the spelling during a song) is also visible in the dumped memory.  Perhaps these tools will help you put your own music and songs into the plush toy.


Clone and Compile
-----------------

In theory this will work with other systems.  I've only tried it on Ubuntu Linux and didn't code in much in the way of error handling.

Fist you need to get a copy of this repository.  You can use GitHub to clone it with git, download a live archive of the [master branch](https://github.com/fidian/leappad-my-pal/archive/master.zip) or individually download the files to your computer.

Next, compile.  `make` should build out two binaries.


Finding My Pal
--------------

Clone this repository and run `make` to build the tools.  Plug in Scout or Violet and use `dmesg` to see what device yours shows up as.  Here's some relevant information from my system.  The important part is finding the device.  Mine was `/dev/sg2`.

```
[2623464.997717] usb 2-1.2: new full-speed USB device number 105 using ehci-pci
[2623465.093078] usb 2-1.2: New USB device found, idVendor=0f63, idProduct=0d00
[2623465.093090] usb 2-1.2: New USB device strings: Mfr=1, Product=2, SerialNumber=0
[2623465.093096] usb 2-1.2: Product: My Pal Scout    
[2623465.093101] usb 2-1.2: Manufacturer: Leapfrog
[2623465.093668] usb-storage 2-1.2:1.0: USB Mass Storage device detected
[2623465.093836] scsi40 : usb-storage 2-1.2:1.0
[2623466.095154] scsi 40:0:0:0: Direct-Access     Leapfrog My Pal Scout     1.00 PQ: 0 ANSI: 0
[2623466.098988] sd 40:0:0:0: Attached scsi generic sg2 type 0
[2623466.101368] sd 40:0:0:0: [sdb] Test WP failed, assume Write Enabled
[2623466.103422] sd 40:0:0:0: [sdb] Asking for cache data failed
[2623466.103431] sd 40:0:0:0: [sdb] Assuming drive cache: write through
[2623466.114365] sd 40:0:0:0: [sdb] Test WP failed, assume Write Enabled
[2623466.118105] sd 40:0:0:0: [sdb] Asking for cache data failed
[2623466.118112] sd 40:0:0:0: [sdb] Assuming drive cache: write through
[2623466.118117] sd 40:0:0:0: [sdb] Attached SCSI disk
```

In case you are curious, this was Scout.  Violet's IDs are like this.

```
[2625677.991370] usb 2-1.2: new full-speed USB device number 108 using ehci-pci
[2625678.086874] usb 2-1.2: New USB device found, idVendor=0f63, idProduct=0018
[2625678.086886] usb 2-1.2: New USB device strings: Mfr=1, Product=2, SerialNumber=0
[2625678.086893] usb 2-1.2: Product: My Pal Scout    
[2625678.086898] usb 2-1.2: Manufacturer: Leapfrog
```


Reading My Pal
--------------

This is the simple part.  Just use `read_flash` and pass it the device name.  You'll need read access to the device, so I used `sudo` to accomplish it.  Status information is written to stderr and the binary data is written to stdout.

```sh
sudo ./read_flash /dev/sg2 > flash_memory.bin
```


Wiping My Pal
-------------

After reading the memory, I noticed that it used 0xFF instead of 0x00 for chunks of memory that I believed to be empty.  The wiping program writes 0xFF to simulate that nothing is written.

```sh
sudo ./wipe_flash /dev/sg2
```

This is MUCH slower.  I think each call is actually timing out and so it will take minutes.  I'd happily accept pull requests to improve this software.


Other Methods Of Wiping My Pal
------------------------------

I've been told that you can take the battery cover off and use a paperclip or a pin.  Hold it in the tiny hole for 10 seconds and you will hear a bark.  That will reset your animal.  Mine did not have any pins.

When you use the [LeapFrog Connect] software you are supposed to press Control + Shift + R.  When you do that a window opens and tells you to that the next time you plug in a device it shall get reset.  No matter how I tried I could not get the animal to forget my daughter's name.  I tried multiple wipes, plugging it in while the window was up, restarting the software, etc.  Customer support did not help.

Another person got this advice from customer support:  "In your LeapFrog Connect profile, click on the profile name under the device, and then 'settings' and 'reset.'"  Unfortunately that does not exist as an option for myself nor the other person seeking assistance.


[LeapFrog Connect]: http://www.leapfrog.com/en-us/support/connect