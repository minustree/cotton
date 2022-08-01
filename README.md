# Cotton

### Pre-requisites for running this program:
- Linux OS
- Permission to access, read, and write to device folders such as /dev/fb0 (framebuffer) and /dev/tty (terminal)
- Resolution of at least 640x480
- Support for 16-bit color

### Compile the program using:
  gcc -o cotton cotton.c library.c

Make sure that graphics.h is in the same directory.

### To run the program:
  ./cotton

## CONTROLS
J - open/close left eye
L - open/close right eye
WASD - change direction to look at
Q - exit program

This program was tested using the QEMU virtual machine.
