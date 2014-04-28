MSense : stress_test_timer.c SenseBarrier.h
	gcc -pthread -o MSense stress_test_timer.c -DUSE_SenseBarrier -DMORE_THREADS

Sense :  stress_test_timer.c SenseBarrier.h
	gcc -pthread -o Sense stress_test_timer.c -DUSE_SenseBarrier 

MTree : stress_test_timer.c TreeBarrier.h
	gcc -pthread -o MTree stress_test_timer.c -DUSE_TreeBarrier -DMORE_THREADS

Tree  : stress_test_timer.c TreeBarrier.h
	gcc -pthread -o Tree stress_test_timer.c -DUSE_TreeBarrier

MStatic : stress_test_timer.c StaticTreeBarrier.h
	gcc -pthread -o MStatic stress_test_timer.c -DUSE_StaticTreeBarrier -DMORE_THREADS

Static : stress_test_timer.c StaticTreeBarrier.h
	gcc -pthread -o Static stress_test_timer.c -DUSE_StaticTreeBarrier

.PHONY :
clean : 
	rm MSense Sense MTree Tree MStatic Static
