# Open Source Nintendo Image Shrinker

Gamecube/Wii discs seem to work on a block size of 0x40000(262,144) bytes

## Here is some useful information about various nintendo iso images

* GC ISO
  * 0x57058000(1,459,978,240) bytes
  * 5569 0x40000(262,144) byte blocks
  * 1 0x18000(98,304) byte block
  * 5,570 blocks total
* Wii Single Layer ISO
  * 0x118240000(4,699,979,776) bytes
  * 17,929 0x40000(262,144) byte blocks
* Wii Dual Layer ISO
  * 0x1FB4E0000(8,511,160,320) bytes
  * 32,467 0x40000(262,144) byte blocks
  * 1 0x20000(131072) byte block
  * 32,468 blocks total

## So, here is my proposed format to describe a shrunken gamecube/wii image
If we use a single block of size 0x40000 bytes as a table to describe our shrunken image and if the largest image (a dual layer wii image) has 32,468 blocks then each block can have 8 bytes available to describe it within our table.

### Each block in our full image will be described by an 8 byte section in our table

### The first 8 byte section will just be a magic number to identify a shrunken image
* 00-07 'O','S','N','I','S',0x00,0x??,0x??
* where the first 0x?? is a version number
* I'm starting at 0 for development and when there is a viable working algorithm I'll up it to 1
* the second 0x?? is image type where 0x01 = GC, 0x10 = WII, and 0x11 is a Dual Layer WII 

## Each additional section will describe a block of data
* Data block
  * 00-03 block number where it can be found in the shrunken image
  * 04-07 CRC32 of the data block
* Generated junk block - a block of junk generated by the fancy algorithm.
  * 00-03 0xFF,0xFF,0xFF,0xFF
  * 04-07 CRC32 of the junk block
  * The algorithm creates a unique deterministic block of junk given the discId, block number, and disc number.  Since it is completely reproducable and atomic this information can be recreated without knowing any other data from the image.
* Repeat junk block - a block of a single uniform repeated byte
  * 00-07 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x?? - the repeated char
* No Data
  * 00-07 0x00
  * Once we see an entry of all 0's we are at the end of our image and can ignore all future blocks, which should also be zero.

This should provide a robust definition of an image that can be used to restore an exact duplicate of the original image as long as the junk generating algorithm is known.  Also, this should be an efficient image for being able to randomly access any given byte of a shrunken image as if it was the original image just by doing a lookup in the table and then either seeking to the location within the shrunken image, or by generating the junk data as necessary.

### TODO
1. Figure out why WII images don't shrink as much as they should
2. Update Nintendont and Dolphin to be able to read these images natively
