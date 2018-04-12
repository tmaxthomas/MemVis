#include <stdlib.h>
#include <string.h>
#include "cache_trace_stats.h"

cache_trace_stats_t::cache_trace_stats_t(const std::string &miss_file, bool warmup_enabled) : 
        caching_device_stats_t(miss_file, warmup_enabled) {
    fd = fopen("out.vzf", "a");        
}

void cache_trace_stats_t::access(const memref_t &memref, bool hit) {
    if(hit) {
        cache_map[memref.data.addr].hits++;
    } else {
        cache_map[memref.data.addr].misses++;
    }
}

void cache_trace_stats_t::child_access(const memref_t &memref, bool hit) {
    if(hit) {
        cache_map[memref.data.addr].hits++;
    }
    // Don't need to handle the miss case, apparently
}

// "print" the stats. Don't, actually. This is meant to be piped
// to a file.
//
// Actually, never run this by itself. It's a bad idea.
void cache_trace_stats_t::print_stats(std::string prefix) {
    char buf[16];
    memset(buf, 0, 16);
    strncpy(buf, prefix.c_str(), 16);
    fwrite(buf, 1, 16, fd);

    for(auto itr = cache_map.begin(); itr != cache_map.end(); itr++) {
        memset(buf, 0, 16);
        *(size_t *) buf = itr->first;
        *(uint32_t *) (buf + 8) = itr->second.hits;
        *(uint32_t *) (buf + 12) = itr->second.misses;
        fwrite(buf, 1, 16, fd);
    }

    fclose(fd);
}
