MSense : stress_test_timer.c SenseBarrier.h
	gcc -pthread -o MSense -g stress_test_timer.c -DUSE_SenseBarrier -DMORE_THREADS

Sense :  stress_test_timer.c SenseBarrier.h
	gcc -pthread -o Sense -g stress_test_timer.c -DUSE_SenseBarrier 

MTree : stress_test_timer.c TreeBarrier.h
	gcc -pthread -o MTree -g stress_test_timer.c -DUSE_TreeBarrier -DMORE_THREADS -lm

Tree  : stress_test_timer.c TreeBarrier.h
	gcc -pthread -o Tree -g stress_test_timer.c -DUSE_TreeBarrier -lm

MStatic : stress_test_timer.c StaticTreeBarrier.h
	gcc -pthread -o MStatic -g stress_test_timer.c -DUSE_StaticTreeBarrier -DMORE_THREADS -lm

Static : stress_test_timer.c StaticTreeBarrier.h
	gcc -pthread -o Static -g stress_test_timer.c -DUSE_StaticTreeBarrier -lm

LMSense : stress_latency.c SenseBarrier.h
	gcc -pthread -o LMSense -g stress_latency.c -DUSE_SenseBarrier -DMORE_THREADS 

LSense :  stress_latency.c SenseBarrier.h
	gcc -pthread -o LSense -g stress_latency.c -DUSE_SenseBarrier 

LMTree : stress_latency.c TreeBarrier.h
	gcc -pthread -o LMTree -g stress_latency.c -DUSE_TreeBarrier -DMORE_THREADS -lm

LTree  : stress_latency.c TreeBarrier.h
	gcc -pthread -o LTree -g stress_latency.c -DUSE_TreeBarrier -lm

LMStatic : stress_latency.c StaticTreeBarrier.h
	gcc -pthread -o LMStatic -g stress_latency.c -DUSE_StaticTreeBarrier -DMORE_THREADS -lm

LStatic : stress_latency.c StaticTreeBarrier.h
	gcc -pthread -o LStatic -g stress_latency.c -DUSE_StaticTreeBarrier -lm


.PHONY :
clean : 
	rm MSense Sense MTree Tree MStatic Static LMSense LSense LMTree LTree LMStatic LStatic
