#ifndef __KITE_DATA_CACHE_H__
#define __KITE_DATA_CACHE_H__

#include "data_memory.h"
#include "inst.h"

// Cache block
class block_t {
public:
    block_t() : valid(false), dirty(false), tag(0), data(0) {}
    block_t(uint64_t m_tag, int64_t *m_data, bool m_valid = false, bool m_dirty = false) :
        valid(m_valid), dirty(m_dirty), tag(m_tag), data(m_data) {}
    block_t(const block_t &b) : valid(b.valid), dirty(b.dirty), tag(b.tag), data(b.data) {}
    ~block_t() {}
    
    // Comparison operators
    bool operator==(const block_t &b) { return tag == b.tag; }
    bool operator!=(const block_t &b) { return tag != b.tag; }
    
    bool valid, dirty;                          // Valid, dirty flags
    uint64_t tag;                               // Block tag
    uint64_t last_access;                       // Last access cycle
    int64_t *data;                              // Block data 
};

// Cache
class data_cache_t {
public:
    data_cache_t(uint64_t *m_ticks, uint64_t m_cache_size,
                 uint64_t m_block_size = 8, uint64_t m_ways = 1);
    ~data_cache_t();

    void connect(data_memory_t *m_memory);      // Connect to the lower-level memory.
    bool is_free() const;                       // Is cache free?
    void read(inst_t *m_inst);                  // Read data from cache.
    void write(inst_t *m_inst);                 // Write data in cache.
    void handle_response(int64_t *m_data);      // Handle a memory response.
    bool run();                                 // Run data cache, and return true when busy.
    void print_stats();                         // Print cache stats.

private:
    data_memory_t *memory;                      // Pointer to the lower-level memory
    uint64_t *ticks;                            // Pointer to processor clock ticks
    block_t** blocks;                           // Cache blocks

    uint64_t cache_size;                        // Cache size in bytes
    uint64_t block_size;                        // Block size in bytes
    uint64_t num_sets;                          // Number of sets
    uint64_t num_ways;                          // N-way set-associative cache
    uint64_t block_offset;                      // Cache block offset
    uint64_t set_offset;                        // Cache set offset
    uint64_t block_mask;                        // Block mask for doubleword addressing
    uint64_t set_mask;                          // Set mask for indexing

    uint64_t num_accesses;                      // Number of accesses
    uint64_t num_misses;                        // Number of misses
    uint64_t num_loads;                         // Number of loads
    uint64_t num_stores;                        // Number of stores
    uint64_t num_writebacks;                    // Number of writebacks

    inst_t *missed_inst;                        // Missed memory instruction
};

#endif 

