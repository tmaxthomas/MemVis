#ifndef __CACHE_TRACE_STATS_H
#define __CACHE_TRACE_STATS_H

#include "caching_device_stats.h"

#include <map>
#include <stdint.h>
#include <stdio.h>

class cache_trace_stats_t : public caching_device_stats_t {
public:
    explicit cache_trace_stats_t(const std::string &miss_file, bool warmup_enabled);

    virtual void access(const memref_t &memref, bool hit) override;
    
    virtual void child_access(const memref_t &memref, bool hit) override;

    virtual void print_stats(std::string prefix) override;

    virtual bool operator!() override { return !success; }
protected:
    struct cts_t { uint32_t hits; uint32_t misses; };
    FILE *fd;

    std::map<size_t, cts_t> cache_map;
};

#endif
