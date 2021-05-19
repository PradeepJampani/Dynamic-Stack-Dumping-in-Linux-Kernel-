
PRADEEP CHOWDARY JAMPANI (1215335693)

Steps to run the program:

GENERAL SETUP:

1) unzip the package to any working directory in your machine.

2) Install and setup the board according to the instructions provided in the following link:

https://www.dropbox.com/sh/7t0tf61zkxmtxxw/AAB4HiFN5YmckFe6-l2J6kkTa/Galileo%20Gen%202%20SDK?dl=0&preview=Setting-Up_Galileo_2018.pdf&subfolder_nav_tracking=1



COMPILATION:

1) apply the patch using patch -p1 <patchname . The patch gets attached and folder Dumpstack_test is created which has source code for the dumpstack 

2) follow the steps to compile and built the kernel :-

* Use    the pre-built    toolchain and    the    Linux kernel source linux3-19-r0.tar.bz2from Dropbox/CSE438-530-Gen2_2017/Boot_image&SDK.  	Also, the“.config” file  in  the  source  code canbe usedfor kernel building.

* Include the cross-compilation tools in your PATH:export PATH=path_to_sdk/sysroots/x86_64-pokysdk-linux/usr/bin/i586-poky-linux:$PATH

*Cross-compile the kernelARCH=x86LOCALVERSION= CROSS_COMPILE=i586-poky-linux-make -j4  

*Build and extract the kernel modules from the build to a target directory (e.g ../galileo-install) ARCH=x86LOCALVERSION=  INSTALL_MOD_PATH=../galileo-installCROSS_COMPILE=i586-poky-linux-make modules_install

*Extract thekernel image (bzImage) from the build to a target directory (e.g ../galileo-install)cp arch/x86/boot/bzImage ../galileo-install/

*Install the new kernel and modules from the target directory (e.g ../galileo-install) to your micro SD card

*Replace the bzImage found in the first partition (ESP) of your micro SD card with the one from your target directory (backup the bzImage on the micro SD card e.g. rename it to bzImage.old)

*Reboot into your new kernel


EXECUTION:

1) The main process divides into two sub child processes .

2) The child process1 creates a syscall and then calls two threads and each thread calls 3 times insdump . The syscall is called after calling the process and all probes inserted sucessfully will display the insdump . There is a hashtable dictinory that is implemented to keep a track of all the insdumps inserted . When ever the process is killed or exited , all the ids with respect to the process are removed using rmdump.

3) The child process2 does similar operation to child process1 but child process2 calls only single thread in the process and all the ids are removed when the child process2 is exited . AT this stage there should be no probes to hit if all rmdumps were exexcuted successfully by child process 1 and 2, to check this the syscall is called again after sleeping for a while, and the syscall doesnot print any dumpstacks here. 




TRACE OF THE PROGRAM at the end:- 

[   31.374007] Removed the 9 probe
[   31.377199] Removed the 10 probe
[   31.382688] Removed the 8 probe
[   31.385878] Removed the 6 probe
[   31.389053] Removed the 4 probe
[   31.395509] Removed the 3 probe
[   31.398706] Removed the 0 probe
[   31.404989] watchdog watchdog0: watchdog did not stop!
in child2
 
calling kernel symbol after unregistering all probes
[   36.016991] watchdog watchdog0: watchdog did not stop!

1. AT this stage all the probes are removed and the kernel symbol is called 







 



