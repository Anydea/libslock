MSense : stress_test_timer.c SenseBarrier.h
	gcc -pthreads -o MSense stress_test_timer.c -DUSE_SenseBarrier -DMORE_THREADS -lrt

Sense :  stress_test_timer.c SenseBarrier.h
	gcc -pthreads -o Sense stress_test_timer.c -DUSE_SenseBarrier 

MTree : stress_test_timer.c TreeBarrier.h
	gcc -pthreads -o MTree stress_test_timer.c -DUSE_TreeBarrier -DMORE_THREADS -lm -lrt

Tree  : stress_test_timer.c TreeBarrier.h
	gcc -pthreads -o Tree stress_test_timer.c -DUSE_TreeBarrier -lm

MStatic : stress_test_timer.c StaticTreeBarrier.h
	gcc -pthreads -o MStatic stress_test_timer.c -DUSE_StaticTreeBarrier -DMORE_THREADS -lm -lrt

Static : stress_test_timer.c StaticTreeBarrier.h
	gcc -pthreads -o Static stress_test_timer.c -DUSE_StaticTreeBarrier -lm

LMSense : stress_latency.c SenseBarrier.h
	gcc -pthreads  -o MSense stress_latency.c -DUSE_SenseBarrier -DMORE_THREADS -lrt

LSense :  stress_latency.c SenseBarrier.h
	gcc -pthreads -o Sense stress_latency.c -DUSE_SenseBarrier 

LMTree : stress_latency.c TreeBarrier.h
	gcc -pthreads -o MTree stress_latency.c -DUSE_TreeBarrier -DMORE_THREADS -lm -lrt

LTree  : stress_latency.c TreeBarrier.h
	gcc -pthreads -o Tree stress_latency.c -DUSE_TreeBarrier -lm

LMStatic : stress_latency.c StaticTreeBarrier.h
	gcc -pthreads -o MStatic stress_latency.c -DUSE_StaticTreeBarrier -DMORE_THREADS -lm -lrt

LStatic : stress_latency.c StaticTreeBarrier.h
	gcc -pthreads -o Static stress_latency.c -DUSE_StaticTreeBarrier -lm


.PHONY :
clean : 
	rm MSense Sense MTree Tree MStatic Static
