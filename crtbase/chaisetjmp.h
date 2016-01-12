#ifndef CHAIOS_CRTBASE_SETJMP_H
#define CHAIOS_CRTBASE_SETJMP_H

//Suppresses inbuilt version
#define _INC_SETJMP

#define _JBLEN 16
typedef size_t __JBTYPE;
typedef __JBTYPE jmp_buf[_JBLEN];


struct jmp_buf_struct {
	size_t bx;
	size_t si;
	size_t di;
	size_t bp;
	size_t sp;
	size_t ip;
};

#undef setjmp

#ifdef __cplusplus
extern "C" {
#endif

	int __cdecl setjmp(jmp_buf buffer);
	void __cdecl longjmp(jmp_buf buffer, int val);

#ifdef __cplusplus
}
#endif

#endif
