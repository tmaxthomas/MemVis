#ifndef __CACHE_NULL_STATS_H
#define __CACHE_NULL_STATS_H

#include <string>
#include "caching_device_stats.h"

class cache_null_stats_t : public caching_device_stats_t {
public:
    explicit cache_null_stats_t(const std::string &miss_file = "", bool warmup_enabled = false)
            : caching_device_stats_t(miss_file, warmup_enabled) {}

    virtual void access(const memref_t &memref, bool hits) override {}
    virtual void flush(const memref_t &memref) {}
    virtual void child_access(const memref_t &memref, bool hits) override {}
    virtual void reset() override {}
    virtual void print_stats(std::string prefix) override {}
}; 

#endif
