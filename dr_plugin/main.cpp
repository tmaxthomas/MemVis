/* ******************************************************************************
 * Copyright (c) 2011-2016 Google, Inc.  All rights reserved.
 * Copyright (c) 2010 Massachusetts Institute of Technology  All rights reserved.
 * ******************************************************************************/

/* Code Manipulation API Sample:
 * memtrace_x86.c
 *
 * Collects the instruction address, data address, and size of every
 * memory reference and dumps the results to a file.
 * This is an x86-specific implementation of a memory tracing client.
 * For a simpler (and slower) arch-independent version, please see memtrace_simple.c.
 *
 * Illustrates how to create generated code in a local code cache and
 * perform a lean procedure call to that generated code.
 *
 * (1) Fills a buffer and dumps the buffer when it is full.
 * (2) Inlines the buffer filling code to avoid a full context switch.
 * (3) Uses a lean procedure call for clean calls to reduce code cache size.
 *
 * Illustrates the use of drutil_expand_rep_string() to expand string
 * loops to obtain every memory reference and of
 * drutil_opnd_mem_size_in_bytes() to obtain the size of OP_enter
 * memory references.
 *
 * The OUTPUT_TEXT define controls the format of the trace: text or binary.
 * Creating a text trace file makes the tool an order of magnitude (!) slower
 * than creating a binary file; thus, the default is binary.
 */

#include <stdio.h>
#include <string.h> /* for memset */
#include <stddef.h> /* for offsetof */
#include <stdint.h>
#include <fcntl.h> /* Yay, OS dependence! */
#include <sys/types.h>
#include <sys/stat.h>
#include <map>
#include <dr/dr_api.h>
#include <dr/drmgr.h>
#include <dr/drreg.h>
#include <dr/drutil.h>

/* Each mem_ref_t includes the type of reference (read or write),
 * the address referenced, and the size of the reference.
 */
typedef struct _mem_ref_t {
    bool  write;
    void *addr;
    size_t size;
    app_pc pc;
} mem_ref_t;

/* Max number of mem_ref a buffer can have */
#define MAX_NUM_MEM_REFS 8192
/* The size of memory buffer for holding mem_refs. When it fills up,
 * we dump data from the buffer to the file.
 */
#define MEM_BUF_SIZE (sizeof(mem_ref_t) * MAX_NUM_MEM_REFS)

/* thread private log file and counter */
typedef struct {
    char   *buf_ptr;
    char   *buf_base;
    /* buf_end holds the negative value of real address of buffer end. */
    ptr_int_t buf_end;
    void   *cache;
    file_t  log;
    FILE *logf;
    uint64  num_refs;
} per_thread_t;

static size_t page_size;
static client_id_t client_id;
static app_pc code_cache;
static void  *mutex;    /* for multithread support */
static uint64 num_refs; /* keep a global memory reference count */
static int tls_index;
static int fd;
static int block_size;

static void event_exit(void);
static void event_thread_init(void *drcontext);
static void event_thread_exit(void *drcontext);
static dr_emit_flags_t event_bb_app2app(void *drcontext, void *tag, instrlist_t *bb,
                                        bool for_trace, bool translating);

static dr_emit_flags_t event_bb_insert(void *drcontext, void *tag, instrlist_t *bb,
                                       instr_t *instr, bool for_trace, bool translating,
                                       void *user_data);

static void clean_call(void);
static void memtrace(void *drcontext);
static void code_cache_init(void);
static void code_cache_exit(void);
static void instrument_mem(void        *drcontext,
                           instrlist_t *ilist,
                           instr_t     *where,
                           int          pos,
                           bool         write);

static std::map<uintptr_t, uint64_t> read_map, write_map, exec_map;

DR_EXPORT void
dr_client_main(client_id_t id, int argc, const char *argv[]) {
    //Before anything else, make sure we have the args we need
    DR_ASSERT(argc == 3);
    fd = open(argv[1], O_CREAT, S_IWUSR);
    DR_ASSERT(fd >= 0);
    block_size = atoi(argv[2]);

    
    /* We need 2 reg slots beyond drreg's eflags slots => 3 slots */
    drreg_options_t ops = {sizeof(ops), 3, false};
    /* Specify priority relative to other instrumentation operations: */
    drmgr_priority_t priority = {
        sizeof(priority), /* size of struct */
        "memtrace",       /* name of our operation */
        NULL,             /* optional name of operation we should precede */
        NULL,             /* optional name of operation we should follow */
        0};               /* numeric priority */
    dr_set_client_name("DynamoRIO Sample Client 'memtrace'",
                       "http://dynamorio.org/issues");
    page_size = dr_page_size();
    drmgr_init();
    drutil_init();
    client_id = id;
    mutex = dr_mutex_create();
    dr_register_exit_event(event_exit);
    if (!drmgr_register_thread_init_event(event_thread_init) ||
        !drmgr_register_thread_exit_event(event_thread_exit) ||
        !drmgr_register_bb_app2app_event(event_bb_app2app, &priority) ||
        !drmgr_register_bb_instrumentation_event(NULL, event_bb_insert, &priority) ||
        drreg_init(&ops) != DRREG_SUCCESS) {
        /* something is wrong: can't continue */
        DR_ASSERT(false);
        return;
    }
    tls_index = drmgr_register_tls_field();
    DR_ASSERT(tls_index != -1);

    code_cache_init();
    /* make it easy to tell, by looking at log file, which client executed */
    dr_log(NULL, LOG_ALL, 1, "Client 'memtrace' initializing\n");
    if (dr_is_notify_on()) {
        dr_fprintf(STDERR, "Client memtrace is running\n");
    }
}


static void
event_exit()
{
    code_cache_exit();

    if (!drmgr_unregister_tls_field(tls_index) ||
        !drmgr_unregister_thread_init_event(event_thread_init) ||
        !drmgr_unregister_thread_exit_event(event_thread_exit) ||
        !drmgr_unregister_bb_insertion_event(event_bb_insert) ||
        drreg_exit() != DRREG_SUCCESS)
        DR_ASSERT(false);

    dr_mutex_destroy(mutex);
    drutil_exit();
    drmgr_exit();

    for(auto itr = write_map.begin(); itr != write_map.end(); itr++) {
        
    }
}

static void
event_thread_init(void *drcontext)
{
    per_thread_t *data;

    /* allocate thread private data */
    data = dr_thread_alloc(drcontext, sizeof(per_thread_t));
    drmgr_set_tls_field(drcontext, tls_index, data);
    data->buf_base = dr_thread_alloc(drcontext, MEM_BUF_SIZE);
    data->buf_ptr  = data->buf_base;
    /* set buf_end to be negative of address of buffer end for the lea later */
    data->buf_end  = -(ptr_int_t)(data->buf_base + MEM_BUF_SIZE);
    data->num_refs = 0;

    /* We're going to dump our data to a per-thread file.
     * On Windows we need an absolute path so we place it in
     * the same directory as our library. We could also pass
     * in a path as a client argument.
     */
    data->log = log_file_open(client_id, drcontext, NULL /* using client lib path */,
                              "memtrace",
                              DR_FILE_ALLOW_LARGE);
    data->logf = log_stream_from_file(data->log);
    fprintf(data->logf,
            "Format: <instr address>,<(r)ead/(w)rite>,<data size>,<data address>\n");
}


static void
event_thread_exit(void *drcontext)
{
    per_thread_t *data;

    memtrace(drcontext);
    data = drmgr_get_tls_field(drcontext, tls_index);
    dr_mutex_lock(mutex);
    num_refs += data->num_refs;
    dr_mutex_unlock(mutex);
    log_stream_close(data->logf); /* closes fd too */
    dr_thread_free(drcontext, data->buf_base, MEM_BUF_SIZE);
    dr_thread_free(drcontext, data, sizeof(per_thread_t));
}


/* we transform string loops into regular loops so we can more easily
 * monitor every memory reference they make
 */
static dr_emit_flags_t
event_bb_app2app(void *drcontext, void *tag, instrlist_t *bb,
                 bool for_trace, bool translating)
{
    if (!drutil_expand_rep_string(drcontext, bb)) {
        DR_ASSERT(false);
        /* in release build, carry on: we'll just miss per-iter refs */
    }
    return DR_EMIT_DEFAULT;
}

/* event_bb_insert calls instrument_mem to instrument every
 * application memory reference.
 */
static dr_emit_flags_t
event_bb_insert(void *drcontext, void *tag, instrlist_t *bb,
                instr_t *instr, bool for_trace, bool translating,
                void *user_data)
{
    int i;
    if (instr_get_app_pc(instr) == NULL)
        return DR_EMIT_DEFAULT;
    if (instr_reads_memory(instr)) {
        for (i = 0; i < instr_num_srcs(instr); i++) {
            if (opnd_is_memory_reference(instr_get_src(instr, i))) {
                instrument_mem(drcontext, bb, instr, i, false);
            }
        }
    }
    if (instr_writes_memory(instr)) {
        for (i = 0; i < instr_num_dsts(instr); i++) {
            if (opnd_is_memory_reference(instr_get_dst(instr, i))) {
                instrument_mem(drcontext, bb, instr, i, true);
            }
        }
    }
    return DR_EMIT_DEFAULT;
}

static void
memtrace(void *drcontext)
{
    per_thread_t *data;
    int num_refs;
    mem_ref_t *mem_ref;

    data      = drmgr_get_tls_field(drcontext, tls_index);
    mem_ref   = (mem_ref_t *)data->buf_base;
    num_refs  = (int)((mem_ref_t *)data->buf_ptr - mem_ref);

    //Very clearly not written by the DynamoRIO team
    for (int i = 0; i < num_refs; i++) {
        uintptr_t mem_block_addr = ((uintptr_t) mem_ref->addr) & ~0xfff,
                  exec_block_addr = ((uintptr_t) mem_ref->pc) & ~0xfff;
        if(mem_ref->write) {
            write_map[mem_block_addr] += size;
        } else {
            read_map[base_block_addr] += size;
        }
        exec_map[exec_block_addr]++;
        
        ++mem_ref;
    }

    memset(data->buf_base, 0, MEM_BUF_SIZE);
    data->num_refs += num_refs;
    data->buf_ptr   = data->buf_base;
}

/* clean_call dumps the memory reference info to the log file */
static void
clean_call(void)
{
    void *drcontext = dr_get_current_drcontext();
    memtrace(drcontext);
}

static void
code_cache_init(void)
{
    void         *drcontext;
    instrlist_t  *ilist;
    instr_t      *where;
    byte         *end;

    drcontext  = dr_get_current_drcontext();
    code_cache = dr_nonheap_alloc(page_size,
                                  DR_MEMPROT_READ  |
                                  DR_MEMPROT_WRITE |
                                  DR_MEMPROT_EXEC);
    ilist = instrlist_create(drcontext);
    /* The lean procecure simply performs a clean call, and then jump back */
    /* jump back to the DR's code cache */
    where = INSTR_CREATE_jmp_ind(drcontext, opnd_create_reg(DR_REG_XCX));
    instrlist_meta_append(ilist, where);
    /* clean call */
    dr_insert_clean_call(drcontext, ilist, where, (void *)clean_call, false, 0);
    /* Encodes the instructions into memory and then cleans up. */
    end = instrlist_encode(drcontext, ilist, code_cache, false);
    DR_ASSERT((size_t)(end - code_cache) < page_size);
    instrlist_clear_and_destroy(drcontext, ilist);
    /* set the memory as just +rx now */
    dr_memory_protect(code_cache, page_size, DR_MEMPROT_READ | DR_MEMPROT_EXEC);
}


static void
code_cache_exit(void)
{
    dr_nonheap_free(code_cache, page_size);
}


/*
 * instrument_mem is called whenever a memory reference is identified.
 * It inserts code before the memory reference to to fill the memory buffer
 * and jump to our own code cache to call the clean_call when the buffer is full.
 */
static void
instrument_mem(void *drcontext, instrlist_t *ilist, instr_t *where,
               int pos, bool write)
{
    instr_t *instr, *call, *restore;
    opnd_t   ref, opnd1, opnd2;
    reg_id_t reg1, reg2;
    drvector_t allowed;
    per_thread_t *data;
    app_pc pc;

    data = drmgr_get_tls_field(drcontext, tls_index);

    /* Steal two scratch registers.
     * reg2 must be ECX or RCX for jecxz.
     */
    drreg_init_and_fill_vector(&allowed, false);
    drreg_set_vector_entry(&allowed, DR_REG_XCX, true);
    if (drreg_reserve_register(drcontext, ilist, where, &allowed, &reg2) !=
        DRREG_SUCCESS ||
        drreg_reserve_register(drcontext, ilist, where, NULL, &reg1) != DRREG_SUCCESS) {
        DR_ASSERT(false); /* cannot recover */
        drvector_delete(&allowed);
        return;
    }
    drvector_delete(&allowed);

    if (write)
       ref = instr_get_dst(where, pos);
    else
       ref = instr_get_src(where, pos);

    /* use drutil to get mem address */
    drutil_insert_get_mem_addr(drcontext, ilist, where, ref, reg1, reg2);

    /* The following assembly performs the following instructions
     * buf_ptr->write = write;
     * buf_ptr->addr  = addr;
     * buf_ptr->size  = size;
     * buf_ptr->pc    = pc;
     * buf_ptr++;
     * if (buf_ptr >= buf_end_ptr)
     *    clean_call();
     */
    drmgr_insert_read_tls_field(drcontext, tls_index, ilist, where, reg2);
    /* Load data->buf_ptr into reg2 */
    opnd1 = opnd_create_reg(reg2);
    opnd2 = OPND_CREATE_MEMPTR(reg2, offsetof(per_thread_t, buf_ptr));
    instr = INSTR_CREATE_mov_ld(drcontext, opnd1, opnd2);
    instrlist_meta_preinsert(ilist, where, instr);

    /* Move write/read to write field */
    opnd1 = OPND_CREATE_MEM32(reg2, offsetof(mem_ref_t, write));
    opnd2 = OPND_CREATE_INT32(write);
    instr = INSTR_CREATE_mov_imm(drcontext, opnd1, opnd2);
    instrlist_meta_preinsert(ilist, where, instr);

    /* Store address in memory ref */
    opnd1 = OPND_CREATE_MEMPTR(reg2, offsetof(mem_ref_t, addr));
    opnd2 = opnd_create_reg(reg1);
    instr = INSTR_CREATE_mov_st(drcontext, opnd1, opnd2);
    instrlist_meta_preinsert(ilist, where, instr);

    /* Store size in memory ref */
    opnd1 = OPND_CREATE_MEMPTR(reg2, offsetof(mem_ref_t, size));
    /* drutil_opnd_mem_size_in_bytes handles OP_enter */
    opnd2 = OPND_CREATE_INT32(drutil_opnd_mem_size_in_bytes(ref, where));
    instr = INSTR_CREATE_mov_st(drcontext, opnd1, opnd2);
    instrlist_meta_preinsert(ilist, where, instr);

    /* Store pc in memory ref */
    pc = instr_get_app_pc(where);
    /* For 64-bit, we can't use a 64-bit immediate so we split pc into two halves.
     * We could alternatively load it into reg1 and then store reg1.
     * We use a convenience routine that does the two-step store for us.
     */
    opnd1 = OPND_CREATE_MEMPTR(reg2, offsetof(mem_ref_t, pc));
    instrlist_insert_mov_immed_ptrsz(drcontext, (ptr_int_t) pc, opnd1,
                                     ilist, where, NULL, NULL);

    /* Increment reg value by pointer size using lea instr */
    opnd1 = opnd_create_reg(reg2);
    opnd2 = opnd_create_base_disp(reg2, DR_REG_NULL, 0,
                                  sizeof(mem_ref_t),
                                  OPSZ_lea);
    instr = INSTR_CREATE_lea(drcontext, opnd1, opnd2);
    instrlist_meta_preinsert(ilist, where, instr);

    /* Update the data->buf_ptr */
    drmgr_insert_read_tls_field(drcontext, tls_index, ilist, where, reg1);
    opnd1 = OPND_CREATE_MEMPTR(reg1, offsetof(per_thread_t, buf_ptr));
    opnd2 = opnd_create_reg(reg2);
    instr = INSTR_CREATE_mov_st(drcontext, opnd1, opnd2);
    instrlist_meta_preinsert(ilist, where, instr);

    /* we use lea + jecxz trick for better performance
     * lea and jecxz won't disturb the eflags, so we won't insert
     * code to save and restore application's eflags.
     */
    /* lea [reg2 - buf_end] => reg2 */
    opnd1 = opnd_create_reg(reg1);
    opnd2 = OPND_CREATE_MEMPTR(reg1, offsetof(per_thread_t, buf_end));
    instr = INSTR_CREATE_mov_ld(drcontext, opnd1, opnd2);
    instrlist_meta_preinsert(ilist, where, instr);
    opnd1 = opnd_create_reg(reg2);
    opnd2 = opnd_create_base_disp(reg1, reg2, 1, 0, OPSZ_lea);
    instr = INSTR_CREATE_lea(drcontext, opnd1, opnd2);
    instrlist_meta_preinsert(ilist, where, instr);

    /* jecxz call */
    call  = INSTR_CREATE_label(drcontext);
    opnd1 = opnd_create_instr(call);
    instr = INSTR_CREATE_jecxz(drcontext, opnd1);
    instrlist_meta_preinsert(ilist, where, instr);

    /* jump restore to skip clean call */
    restore = INSTR_CREATE_label(drcontext);
    opnd1 = opnd_create_instr(restore);
    instr = INSTR_CREATE_jmp(drcontext, opnd1);
    instrlist_meta_preinsert(ilist, where, instr);

    /* clean call */
    /* We jump to lean procedure which performs full context switch and
     * clean call invocation. This is to reduce the code cache size.
     */
    instrlist_meta_preinsert(ilist, where, call);
    /* mov restore DR_REG_XCX */
    opnd1 = opnd_create_reg(reg2);
    /* this is the return address for jumping back from lean procedure */
    opnd2 = opnd_create_instr(restore);
    /* We could use instrlist_insert_mov_instr_addr(), but with a register
     * destination we know we can use a 64-bit immediate.
     */
    instr = INSTR_CREATE_mov_imm(drcontext, opnd1, opnd2);
    instrlist_meta_preinsert(ilist, where, instr);
    /* jmp code_cache */
    opnd1 = opnd_create_pc(code_cache);
    instr = INSTR_CREATE_jmp(drcontext, opnd1);
    instrlist_meta_preinsert(ilist, where, instr);

    /* Restore scratch registers */
    instrlist_meta_preinsert(ilist, where, restore);
    if (drreg_unreserve_register(drcontext, ilist, where, reg1) != DRREG_SUCCESS ||
        drreg_unreserve_register(drcontext, ilist, where, reg2) != DRREG_SUCCESS)
        DR_ASSERT(false);
}
