#include <jni.h>
#include <string>
#include <errno.h>
#include <sys/syscall.h>
#include <android/log.h>
#include <unistd.h>
#include <sched.h>

#include "cpu.h"

long getCores() {
	return sysconf(_SC_NPROCESSORS_CONF);
}

void setCurrentThreadAffinity(long mask) {
	long syscallres;
	pid_t tid = gettid();
	syscallres = syscall(__NR_sched_setaffinity, tid, sizeof(mask), &mask);
	if (syscallres == -1) {
        __android_log_print(ANDROID_LOG_WARN, "CrashReportT", "warning: could not setaffinity to cpu: mask = %ld, err=%d", mask, errno);
	} else {
        __android_log_print(ANDROID_LOG_WARN, "CrashReportT", "mask = %ld, tid = %d has setted cpu affinity success", mask, tid);
    }
}

extern "C" JNIEXPORT void JNICALL
Java_com_huchao_myaffinity_MainActivity_attachToCpu(JNIEnv *env, jclass, jint cpu) {
    long cores = getCores();
    __android_log_print(ANDROID_LOG_WARN, "CrashReportT", "get cpu number = %ld", cores);
    if (cpu >= cores) {
        __android_log_print(ANDROID_LOG_WARN, "CrashReportT", "your set cpu is beyond the cores, exit...");
        return;
    }

    cpu_set_t mask;
    CPU_ZERO(&mask);
    CPU_SET(cpu, &mask);
    setCurrentThreadAffinity((long)(&mask));
}

extern "C" JNIEXPORT void JNICALL
Java_com_huchao_myaffinity_MainActivity_getCurrentThreadAffinity(JNIEnv *env, jclass) {
	cpu_set_t get;
	pid_t tid = gettid();
	CPU_ZERO(&get);
    long syscallres = syscall(__NR_sched_getaffinity, tid, sizeof(get), &get);
	if (syscallres == -1){
		__android_log_print(ANDROID_LOG_WARN, "CrashReportT", "warning: could not get thread affinity");
	} else {
		long cores = getCores();
		int i;
		for (i = 0; i < cores; i++) {
			if (CPU_ISSET(i, &get)) {
				__android_log_print(ANDROID_LOG_WARN, "CrashReportT", "this thread %d(tid) is running processor : %d", tid, i);
				return;
			}
		}
	}
}

void setCurrentThreadAffinity2(unsigned long mask) {
    pid_t tid = gettid();
    long syscallres = syscall(__NR_sched_setaffinity, tid, sizeof(mask), &mask);
    if (syscallres == -1) {
        __android_log_print(ANDROID_LOG_WARN, "CrashReportT", "warning: could not setaffinity to cpu: mask = %lu, err=%d", mask, errno);
    } else {
        __android_log_print(ANDROID_LOG_WARN, "CrashReportT", "mask = %ld, tid = %d has setted cpu affinity success", mask, tid);
    }
}

extern "C" JNIEXPORT void JNICALL
Java_com_huchao_myaffinity_MainActivity_attachToCpu2(JNIEnv *env, jclass, jint cpu) {
    long cores = getCores();
    __android_log_print(ANDROID_LOG_WARN, "CrashReportT", "get cpu number = %ld", cores);
    if (cpu >= cores) {
        __android_log_print(ANDROID_LOG_WARN, "CrashReportT", "your set cpu is beyond the cores, exit...");
        return;
    }

    unsigned long mask = 1lu << (unsigned int)cpu;
    setCurrentThreadAffinity2(mask);
}

extern "C" JNIEXPORT void JNICALL
Java_com_huchao_myaffinity_MainActivity_getCurrentThreadAffinity2(JNIEnv *env, jclass) {
    unsigned long mask = 0;
    pid_t tid = gettid();
    long syscallres = syscall(__NR_sched_getaffinity, tid, sizeof(mask), &mask);
    if (syscallres == -1) {
    	__android_log_print(ANDROID_LOG_WARN, "CrashReportT", "warning: could not get thread affinity");
    } else {
        for (unsigned long i = 0; i < sizeof(unsigned long) * 8; i++) {
            if ((mask & (1ul << i)) != 0) {
                __android_log_print(ANDROID_LOG_WARN, "CrashReportT", "mask: %lu, this thread %d(tid) is running processor: %lu", mask, tid, i);
            }
        }
    }
}

extern "C" JNIEXPORT void JNICALL
Java_com_huchao_myaffinity_MainActivity_getCurrentThreadAffinityNcnn(JNIEnv *env, jclass) {
    const CpuSet& cpuset = get_cpu_thread_affinity_mask(2);
    int na = 1;
}

extern "C" JNIEXPORT void JNICALL
Java_com_huchao_myaffinity_MainActivity_attachToCpuNcnn(JNIEnv *env, jclass) {
    set_cpu_powersave(2);
    int na = 1;
}

extern "C" JNIEXPORT jlong JNICALL
Java_com_huchao_myaffinity_MainActivity_getCurrentThreadAffinityDebug(JNIEnv *env, jclass) {
    long mask = 0;
    pid_t tid = gettid();
    long syscallres = syscall(__NR_sched_getaffinity, tid, sizeof(mask), &mask);
    if (syscallres == -1) {
        __android_log_print(ANDROID_LOG_WARN, "CrashReportT", "warning: could not get thread affinity, err=%d", errno);
    }
    return mask;
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_huchao_myaffinity_MainActivity_setCurrentThreadAffinityDebug(JNIEnv *env, jclass, jlong mask) {
    pid_t tid = gettid();
    long syscallres = syscall(__NR_sched_setaffinity, tid, sizeof(mask), &mask);
    if (syscallres == -1) {
        __android_log_print(ANDROID_LOG_WARN, "CrashReportT", "warning: could not setaffinity to cpu: mask = %d, err=%d", mask, errno);
        return false;
    }

    __android_log_print(ANDROID_LOG_WARN, "CrashReportT", "mask = %d, tid = %d has setted cpu affinity success", mask, tid);
    return true;
}
