#ifndef NCNN_CPU_H
#define NCNN_CPU_H

#include <sched.h>
#include <android/log.h>

#define NCNN_LOGE(...) do { \
    fprintf(stderr, ##__VA_ARGS__); fprintf(stderr, "\n"); \
    __android_log_print(ANDROID_LOG_WARN, "CrashReportT", ##__VA_ARGS__); } while(0)

class CpuSet
{
public:
    CpuSet();
    void enable(int cpu);
    void disable(int cpu);
    void disable_all();
    bool is_enabled(int cpu) const;
    int num_enabled() const;

public:
    cpu_set_t cpu_set;
};

// bind all threads on little clusters if powersave enabled
// affacts HMP arch cpu like ARM big.LITTLE
// only implemented on android at the moment
// switching powersave is expensive and not thread-safe
// 0 = all cores enabled(default)
// 1 = only little clusters enabled
// 2 = only big clusters enabled
// return 0 if success for setter function
int get_cpu_powersave();
int set_cpu_powersave(int powersave);

// convenient wrapper
const CpuSet& get_cpu_thread_affinity_mask(int powersave);

// set explicit thread affinity
int set_cpu_thread_affinity(const CpuSet& thread_affinity_mask);

#endif // NCNN_CPU_H
