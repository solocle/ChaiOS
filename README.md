# ChaiOS
ChaiOS: Chai Operating System. Multiplatform, visual studio (but adaptable to other compilers).
Compiler support is limited to MSVC for now, but the design is such that all compiler specific stuff is done by macros (e.g. intrinsic funtions).
The Operating System currently supports x86 and x64. Architecture specific modules are in arch/ folders.

crtbase/ is the equivalent of libgcc. It consists of basic helper functions, written is assembly.
crtbase and AcpiSubsystem are the only static libraries, the rest are DLLs loaded at boot time.

Note: when leaving bug reports, please specify the hardware platform and boot method. Hardware issues can be notoriously problematic to debug, and not reproducable between different systems. Hence, it helps to know what you're working with.
