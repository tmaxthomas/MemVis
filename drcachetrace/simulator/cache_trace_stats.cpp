#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "cache_trace_stats.h"

cache_trace_stats_t::cache_trace_stats_t(const std::string &miss_file, bool warmup_enabled) : 
        caching_device_stats_t(miss_file, warmup_enabled) {
    fd = fopen("/tmp/tmp.vzf", "a");
    char buf[8];
    strncpy(buf, "vzfh", 4);
    *(uint32_t *) (buf + 4) = 1;
    fwrite(buf, 1, 8, fd);
}

void cache_trace_stats_t::access(const memref_t &memref, bool hit) {

#ifdef DEBUG_MODE
    if(type_is_instr(memref.data.type)) {
        printf("Recording exec; size: %lu; addr: 0x%lx; map size: %lu; cache hit: %d\n",
                        memref.instr.size, memref.instr.addr, cache_map.size(), hit);
    } else if(memref.data.type == TRACE_TYPE_READ ||
              type_is_prefetch(memref.data.type)) { // Just pretend prefetches are reads
        printf("Recording read; size: %lu; addr: 0x%lx; map size: %lu; cache hit: %d\n",
                        memref.data.size, memref.data.addr, cache_map.size(), hit);
    } else if(memref.data.type == TRACE_TYPE_WRITE) {
        printf("Recording exec; size: %lu; addr: 0x%lx; map size: %lu; cache hit: %d\n",
                        memref.data.size, memref.data.addr, cache_map.size(), hit);
    }
#endif

    for(size_t i = 0; i < memref.data.size; i++) {    
        cts_t *entry = &(cache_map[memref.data.addr + i]);

        // First, record the access
        if(type_is_instr(memref.data.type)) {
            entry->execs++;
        } else if(memref.data.type == TRACE_TYPE_READ ||
                  type_is_prefetch(memref.data.type)) { // Just pretend prefetches are reads
            entry->reads++;
        } else if(memref.data.type == TRACE_TYPE_WRITE) {
            entry->writes++;
        } // No else block; anything else is getting thrown out
        
        //Then, record the cache access stats
        if(hit) {
            entry->hits++;
        } else {
            entry->misses++;
        }
    } 
}

void cache_trace_stats_t::child_access(const memref_t &memref, bool hit) {
    //Just recycle the access code. What could possibly go wrong?
    access(memref, hit);    
}

void cache_trace_stats_t::print_stats(std::string prefix) {
    char buf[32];
    memset(buf, 0, 32);
    strncpy(buf, prefix.c_str(), 24);
    *(uint64_t *) (buf + 16) = cache_map.size();
    fwrite(buf, 1, 24, fd);

    for(auto itr = cache_map.begin(); itr != cache_map.end(); itr++) {
        memset(buf, 0, 32);
        *(size_t *) buf = itr->first;
        *(uint32_t *) (buf + 8) = itr->second.reads;
        *(uint32_t *) (buf + 12) = itr->second.writes;
        *(uint64_t *) (buf + 16) = itr->second.execs;
        *(uint32_t *) (buf + 24) = itr->second.hits;
        *(uint32_t *) (buf + 28) = itr->second.misses;
        fwrite(buf, 1, 32, fd);
    }

    fclose(fd);
}
