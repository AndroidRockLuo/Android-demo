#include <jni.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

typedef int (*PFNcall)(void);
#define PROT PROT_EXEC|PROT_WRITE|PROT_READ
#define FLAGS MAP_ANONYMOUS| MAP_FIXED |MAP_SHARED


#define PAGE_START(addr) (~(getpagesize() - 1) & (addr))

/*__asm __volatile (
1 "stmfd sp!,{r4-r8,lr}\n"
2 "mov r6,#0\n"  ����ͳ��ѭ��������debug�õ�
3 "mov r7,#0\n"  Ϊr7����ֵ

4 "mov r8,pc\n"  4��7��������ø���$address����ָ��ĵ�ַ pc--> add r7,#1
5 "mov r4,#0\n"  Ϊr4����ֵ
6 "add r7,#1\n"  ��������$address�ġ���ָ�
7 "ldr r5,[r8]\n"   r5 �����ŵľ���add r7,#1


8 "code:\n"
9 "add r4,#1\n"  �����$address���Ƕ�r4��1

10 "mov r8,pc\n"  10,11,12�е����þ��ǰѵ�6�е�ָ��д����9��
11 "sub r8,#12\n"   r8--> add r4,#1
12 "str r5,[r8]\n"    ��add r7,#1д�뵽add r4,#1��


13 "add r6,#1\n"   r6��������


14 "cmp r4,#10\n"  ����ѭ������
15 "bge out\n"


16 "cmp r7,#10\n"   ����ѭ������
17 "bge out\n"
18 "b code\n"      10���ڵ�ѭ������ȥ

19 "out:\n"
20 "mov r0,r4\n"    ��r4��ֵ��Ϊ����ֵ
21 "ldmfd sp!,{r4-r8,pc}\n"
);
 * */


char code[]=
"\xF0\x41\x2D\xE9\x00\x60\xA0\xE3\x00\x70\xA0\xE3\x0F\x80\xA0\xE1"
"\x00\x40\xA0\xE3\x01\x70\x87\xE2\x00\x50\x98\xE5\x01\x40\x84\xE2"
"\x0F\x80\xA0\xE1\x0C\x80\x48\xE2\x00\x50\x88\xE5\x01\x60\x86\xE2"
"\x0A\x00\x54\xE3\x02\x00\x00\xAA\x0A\x00\x57\xE3\x00\x00\x00\xAA"
"\xF5\xFF\xFF\xEA\x04\x00\xA0\xE1\xF0\x81\xBD\xE8";


extern "C" JNIEXPORT jint Java_com_example_enulatorcache_MainActivity_start(JNIEnv *env, jobject thiz){
	int a;
	PFNcall call = 0;
	//��mmap����bug���������ֻ��ִ��һ�Σ��ڶ��α���
	//void *exec = mmap((void*)0x10000000,(size_t)4096 ,PROT ,FLAGS,-1,(off_t)0);
	void *exec = malloc(0x1000);
	memcpy(exec ,code,sizeof(code)+1);
	void *page_start_addr = (void *)PAGE_START((uint32_t)exec);
	mprotect(page_start_addr, getpagesize(), PROT);

	call=(PFNcall)exec;
	call();

	__asm __volatile (
	"mov %0,r0\n"
	:"=r"(a)
	:
	:
	);

	free(exec);
	//munmap((void*)0x10000000,(size_t)4096);
	if(a==0xA){ //���
		return 0;
	}else{
		return 1;
	}

	return a;
}
