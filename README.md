libvitamtp
======================

## What is this?

libVitaMTP is a library based off of libMTP that does low level USB 
communications with the Vita. It can read and receive MTP commands 
that the Vita sends, which are a proprietary set of commands that is 
based on the MTP open standard.

## What is working?

As of the writing of this, the sending of Vita/PSX/PSP/PSM applications to 
and from the device works. Sending/receiving of PSP saves also work. Backups 
and restoring backups also work. However, edge cases have not been tested, 
and if you run into errors, please report it to the GitHub issues page.

## What needs work?

The major things that are not done yet are 1) media transfers (videos, music, 
and photos), 2) edge cases like "what happens if a file is deleted on the 
computer while it is being transfered?" and 3) testing libvitamtp on multiple 
configurations, hosts, and computers. For the last two, the help of beta 
testers is requested.

## How do I use/test libvitamtp?

Issues Page: https://github.com/codestation/vitamtp/issues

First try to compile it using the directions in INSTALL, if the process fails 
post the output into the GitHub issues page.

First, note that libVitaMTP is designed with developers in mind. Users should 
stick to [QCMA](https://github.com/codestation/qcma). Full documentation on the interface will be provided in the 
future, but for now, read the interface in vitamtp.h and the function headers 
in vitamtp.c to figure out how functions work and what you can do. OpenCMA has 
been designed with simplicity in mind so there is no harm in reading the code 
and comments for opencma.c and related files. Your implementation should be 
very similar if you wish to replicate CMA functionality.

## Credits?

[Yifan Lu](http://yifan.lu/) has been responsible for this abomination of code

dridri also takes some blame for his help in figuring out many structures and codes
