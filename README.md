ProcessProtect
*********************************************************************
This application can help you hide or protect any process.

This application includes windows kernel programming. Only tested on Windows 7 64bit.
Disable the windows driver signature enforcement before testing!
This application may cause BSOD on your machine. Test at your own risk!!!!!

Function
*********************************************************************
After hiding a process, it will disappear from the task manager.
After protecting a process, it cannot be terminated by task manager.

Code Details
*********************************************************************
The app consists of two part: the ring3 gui and the ring0 driver.

Ring3 is writen with C++ and MFC
Ring0 is writen with C (In the DRIVER folder)

Hiding and protection is implemented with Direct Kernel Object Manipulation; that is to change the EPROCESS structure.
Usually, getting the list of process is to call ZwQuerySystemInformation enumerate through a double linked list of EPROCESS structure. If we disconnect one of the structure off the linked list, then ZwQuerySystemInformation function cannot get the information of that process. The offset 0x188 of EPROCESS stucture is the list entry.
To protect the process, we can set the flag at offset 0x440 to zero, so that CreateProcess will fail on this process.
Before modifying those kernel structure, we need to raises the hardware priority to DISPATCH_LEVEL, thereby masking off interrupts that may cause BSOD.


Reference: MSDN, pediy
Driver templete from Tesla.Angela
