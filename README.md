# ChaiOS
ChaiOS: Chai Operating System. Multiplatform, visual studio (but adaptable to other compilers).
Compiler support is limited to MSVC for now, but the design is such that all compiler specific stuff is done by macros (e.g. intrinsic funtions).
The Operating System currently supports x86 and x64. Architecture specific modules are in arch/ folders.

crtbase/ is the equivalent of libgcc. It consists of
