#ifndef JEOD_MEMORY_DEBUG
#define JEOD_MEMORY_DEBUG 2
#endif

#include <cstdio>
#include <cstdlib>
#include <new>

#include "utils/memory/include/jeod_alloc.hh"
#include "utils/memory/include/memory_messages.hh"

#include "test_harness/include/cmdline_parser.hh"
#include "test_harness/include/test_sim_interface.hh"

using namespace jeod;

int message_level = MessageHandler::Notice;
unsigned int memory_level;

/*
 * To verify that the JEOD memory manager allocates and frees memory
 * as expected, we need to override new and delete.
 * But first we need to keep track of allocated memory.
 */
class MemoryPool
{
public:
    static bool is_active()
    {
        return (the_memory_pool != nullptr);
    }

    MemoryPool()
    {
        if(the_memory_pool != nullptr)
        {
            MessageHandler::fail(__FILE__,
                                 __LINE__,
                                 "utils/memory/verif/unit_tests/basic/error",
                                 "Multiple memory pools!");
        }
        the_memory_pool = this;
        active = true;
    }

    ~MemoryPool()
    {
        if(the_memory_pool == this)
        {
            the_memory_pool = nullptr;
        }
    }

    void * alloc(size_t size)
    {
        void * buffer = malloc(size);

        if(active)
        {
            scramble(buffer, size);

            if(message_level >= MessageHandler::Debug)
            {
                fprintf(stderr,
                        "\nDebug utils/memory/verif/unit_tests/basic/debug "
                        "at %s line %d:\n"
                        "Reserving %zd bytes at %p\n",
                        __FILE__,
                        __LINE__,
                        size,
                        buffer);
            }

            active = false;
            alloc_table.insert(AllocTable::value_type(buffer, size));
            active = true;
        }
        return buffer;
    }

    bool is_allocated(const void * ptr)
    {
        bool allocated;
        auto * test_ptr = (void *)((char *)ptr - 16);

        active = false;
        {
            auto iter = alloc_table.find(test_ptr);
            allocated = (iter != alloc_table.end());
        }
        active = true;
        return allocated;
    }

    void release(void * ptr)
    {
        if(active)
        {
            active = false;
            {
                auto iter = alloc_table.find(ptr);
                if(iter != alloc_table.end())
                {
                    if(message_level >= MessageHandler::Debug)
                    {
                        fprintf(stderr,
                                "\nDebug utils/memory/verif/unit_tests/basic/debug "
                                "at %s line %d:\n"
                                "Freeing buffer at %p, size=%zd\n",
                                __FILE__,
                                __LINE__,
                                ptr,
                                iter->second);
                    }
                    free(ptr);

                    alloc_table.erase(iter);
                }
                else
                {
                    MessageHandler::fail(__FILE__,
                                         __LINE__,
                                         "utils/memory/verif/unit_tests/basic/error",
                                         "Attempt to free unallocated buffer at %p",
                                         ptr);
                }
            }
            active = true;
        }

        else
        {
            free(ptr);
        }

        return;
    }

private:
    static MemoryPool * the_memory_pool;

    void scramble(void * buffer, size_t size)
    {
        auto * buff = reinterpret_cast<char *>(buffer);

        for(size_t ii = 0; ii < size; ++ii)
        {
            buff[ii] = 0xdf;
        }
    }

    using AllocTable = std::map<const void *, size_t>;
    bool active{};
    AllocTable alloc_table;
} memory_pool;

void * operator new(std::size_t size)
{
    if(MemoryPool::is_active())
    {
        return memory_pool.alloc(size);
    }
    else
    {
        return malloc(size);
    }
}

void * operator new[](std::size_t size)
{
    if(MemoryPool::is_active())
    {
        return memory_pool.alloc(size);
    }
    else
    {
        return malloc(size);
    }
}

void operator delete(void * ptr) throw()
{
    if(MemoryPool::is_active())
    {
        memory_pool.release(ptr);
    }
    else
    {
        free(ptr);
    }
}

void operator delete[](void * ptr) throw()
{
    if(MemoryPool::is_active())
    {
        memory_pool.release(ptr);
    }
    else
    {
        free(ptr);
    }
}

/*
 * Define a set of classes, some of which have diamond inheritance.
 */

class Base
{
public:
    Base()
    {
        trace("Constructing", what_are_you_really(), (void *)this);
    }

    virtual ~Base()
    {
        trace("Destructing", what_are_you_really(), (void *)this);
        check_ok();
    }

    virtual char * address()
    {
        return (char *)this;
    }

    virtual const char * what_are_you_really()
    {
        return "Base";
    }

    virtual bool are_you_ok()
    {
        return yes_im_ok();
    }

    virtual int * elem_addr()
    {
        return &base;
    }

    bool yes_im_ok()
    {
        return (base == 1);
    }

    const char * what_are_you()
    {
        return "Base";
    }

    void check_ok()
    {
        if(!checking)
        {
            if(!are_you_ok())
            {
                MessageHandler::warn(__FILE__,
                                     __LINE__,
                                     MemoryMessages::corrupted_memory,
                                     "%s @ 0x%p is corrupted",
                                     what_are_you_really(),
                                     address());
            }
            checking = true;
        }
    }

    void assert_ok()
    {
        if(!are_you_ok())
        {
            MessageHandler::fail(__FILE__,
                                 __LINE__,
                                 MemoryMessages::corrupted_memory,
                                 "%s @ 0x%p is corrupted",
                                 what_are_you_really(),
                                 address());
        }
    }

    void trace(const char * what, const char * who, const void * where)
    {
        print_debug(3, stderr, " %s %-4s @ %p\n", what, who, where);
    }

protected:
    bool checking{};
    int base{1};
};

class Foo : public virtual Base
{
public:
    Foo()
    {
        trace("Constructing", what_are_you_really(), (void *)this);
    }

    ~Foo() override
    {
        trace("Destructing", what_are_you_really(), (void *)this);
        check_ok();
    }

    char * address() override
    {
        return (char *)this;
    }

    const char * what_are_you_really() override
    {
        return "Foo";
    }

    bool are_you_ok() override
    {
        return yes_im_ok() && Base::are_you_ok();
    }

    int * elem_addr() override
    {
        return &foo;
    }

    bool yes_im_ok()
    {
        return (foo == 2);
    }

    const char * what_are_you()
    {
        return "Foo";
    }

protected:
    int foo{2};
};

class Bar : public Foo
{
public:
    Bar()
    {
        trace("Constructing", what_are_you_really(), (void *)this);
    }

    ~Bar() override
    {
        trace("Destructing", what_are_you_really(), (void *)this);
        check_ok();
    }

    char * address() override
    {
        return (char *)this;
    }

    const char * what_are_you_really() override
    {
        return "Bar";
    }

    bool are_you_ok() override
    {
        return yes_im_ok() && Foo::are_you_ok();
    }

    int * elem_addr() override
    {
        return &bar;
    }

    bool yes_im_ok()
    {
        return (bar == 3);
    }

    const char * what_are_you()
    {
        return "Bar";
    }

protected:
    int bar{3};
};

class Baz : public virtual Base
{
public:
    Baz()
    {
        trace("Constructing", what_are_you_really(), (void *)this);
    }

    ~Baz() override
    {
        trace("Destructing", what_are_you_really(), (void *)this);
        check_ok();
    }

    char * address() override
    {
        return (char *)this;
    }

    const char * what_are_you_really() override
    {
        return "Baz";
    }

    bool are_you_ok() override
    {
        return yes_im_ok() && Base::are_you_ok();
    }

    int * elem_addr() override
    {
        return &baz;
    }

    bool yes_im_ok()
    {
        return (baz == 4);
    }

    const char * what_are_you()
    {
        return "Baz";
    }

protected:
    int baz{4};
};

class Qux : public Bar,
            public Baz
{
public:
    Qux()
    {
        trace("Constructing", what_are_you_really(), (void *)this);
    }

    ~Qux() override
    {
        trace("Destructing", what_are_you_really(), (void *)this);
        check_ok();
    }

    char * address() override
    {
        return (char *)this;
    }

    const char * what_are_you_really() override
    {
        return "Qux";
    }

    bool are_you_ok() override
    {
        return yes_im_ok() && Bar::are_you_ok() && Baz::are_you_ok();
    }

    int * elem_addr() override
    {
        return &qux;
    }

    bool yes_im_ok()
    {
        return (qux == 5);
    }

    const char * what_are_you()
    {
        return "Qux";
    }

protected:
    int qux{5};
};

class Quxx : public Baz,
             public Bar
{
public:
    Quxx()
    {
        trace("Constructing", what_are_you_really(), (void *)this);
    }

    ~Quxx() override
    {
        trace("Destructing", what_are_you_really(), (void *)this);
        check_ok();
    }

    char * address() override
    {
        return (char *)this;
    }

    const char * what_are_you_really() override
    {
        return "Quxx";
    }

    bool are_you_ok() override
    {
        return yes_im_ok() && Baz::are_you_ok() && Bar::are_you_ok();
    }

    int * elem_addr() override
    {
        return &quxx;
    }

    bool yes_im_ok()
    {
        return (quxx == 6);
    }

    const char * what_are_you()
    {
        return "Quxx";
    }

protected:
    int quxx{6};
};

bool overwrite_outside = false;
bool verbose = false;
MemoryPool * MemoryPool::the_memory_pool = nullptr;
TestSimInterface sim_interface;

int main(int argc, char ** argv)
{
    int * i42 = nullptr;
    double * darr = nullptr;
    char * carr = nullptr;
    Base ** barr = nullptr;

    // Parse the command line.
    // Option -outside => overwrite the memory manager markers.
    // Option -inside  => overwrite data inside the allocated classes.
    CmdlineParser cmdline_parser;
    cmdline_parser.add("outside", &overwrite_outside);
    cmdline_parser.add("verbose", &verbose);
    cmdline_parser.parse(argc, argv);

    // Determine the message handler and memory manager report thresholds.
    if(verbose)
    {
        message_level = MessageHandler::Debug;
        memory_level = 3;
    }
    else
    {
        message_level = MessageHandler::Notice;
        memory_level = 2;
    }

    // Initialize JEOD.
    MessageHandler::set_suppression_level(message_level);
    JeodMemoryManager::set_debug_level(memory_level);
    if(overwrite_outside)
    {
        JeodMemoryManager::set_guard_enabled(true);
        sim_interface.add_allowed_code("utils/memory/corrupted_memory");
    }

    sim_interface.add_allowed_code("utils/memory/verif/unit_tests/basic/debug");
    sim_interface.add_allowed_code("utils/memory/debug");
    sim_interface.add_verboten_code("utils/memory");

    // Allocate memory.
    // 1. Primitive object.
    MessageHandler::inform(__FILE__,
                           __LINE__,
                           "utils/memory/verif/unit_tests/basic/debug",
                           "i42 = JEOD_ALLOC_PRIM_OBJECT (int, 42);");
    i42 = JEOD_ALLOC_PRIM_OBJECT(int, 42);
    if(memory_pool.is_allocated(i42) && (*i42 == 42))
    {
        MessageHandler::inform(__FILE__,
                               __LINE__,
                               "utils/memory/verif/unit_tests/basic/debug",
                               "i42 properly initialized and allocated");
    }
    else
    {
        MessageHandler::fail(__FILE__, __LINE__, "utils/memory/verif/unit_tests/basic/error", "i42 allocation failed");
    }

    // 2. Primitive array.
    MessageHandler::inform(__FILE__,
                           __LINE__,
                           "utils/memory/verif/unit_tests/basic/debug",
                           "darr = JEOD_ALLOC_PRIM_ARRAY (3, double);");
    darr = JEOD_ALLOC_PRIM_ARRAY(3, double);
    if(memory_pool.is_allocated(darr) && (std::fpclassify(darr[0]) == FP_ZERO) &&
       (std::fpclassify(darr[1]) == FP_ZERO) && (std::fpclassify(darr[2]) == FP_ZERO))
    {
        MessageHandler::inform(__FILE__,
                               __LINE__,
                               "utils/memory/verif/unit_tests/basic/debug",
                               "darr properly initialized and allocated");
    }
    else
    {
        MessageHandler::fail(__FILE__, __LINE__, "utils/memory/verif/unit_tests/basic/error", "darr allocation failed");
    }

    // 3. Primitive pointer array.
    MessageHandler::inform(__FILE__,
                           __LINE__,
                           "utils/memory/verif/unit_tests/basic/debug",
                           "carr = JEOD_ALLOC_PRIM_ARRAY (1, char);");
    carr = JEOD_ALLOC_PRIM_ARRAY(1, char);
    if(memory_pool.is_allocated(carr) && (*carr == 0))
    {
        MessageHandler::inform(__FILE__,
                               __LINE__,
                               "utils/memory/verif/unit_tests/basic/debug",
                               "carr properly initialized and allocated");
    }
    else
    {
        MessageHandler::fail(__FILE__, __LINE__, "utils/memory/verif/unit_tests/basic/error", "carr allocation failed");
    }

    // 4. Structured pointer array.
    MessageHandler::inform(__FILE__,
                           __LINE__,
                           "utils/memory/verif/unit_tests/basic/debug",
                           "barr = JEOD_ALLOC_CLASS_POINTER_ARRAY (2, Base);");
    JEOD_REGISTER_CLASS(Base);
    barr = JEOD_ALLOC_CLASS_POINTER_ARRAY(2, Base);
    if(memory_pool.is_allocated(barr) && (barr[0] == nullptr) && (barr[1] == nullptr))
    {
        MessageHandler::inform(__FILE__,
                               __LINE__,
                               "utils/memory/verif/unit_tests/basic/debug",
                               "barr properly initialized and allocated");
    }
    else
    {
        MessageHandler::fail(__FILE__, __LINE__, "utils/memory/verif/unit_tests/basic/error", "barr allocation failed");
    }

    // 5. Structured array.
    MessageHandler::inform(__FILE__,
                           __LINE__,
                           "utils/memory/verif/unit_tests/basic/debug",
                           "qarr = JEOD_ALLOC_CLASS_ARRAY (1, Qux);");
    Qux * qarr = JEOD_ALLOC_CLASS_ARRAY(1, Qux);
    if(memory_pool.is_allocated(qarr) && (qarr->are_you_ok()))
    {
        MessageHandler::inform(__FILE__,
                               __LINE__,
                               "utils/memory/verif/unit_tests/basic/debug",
                               "qarr properly initialized and allocated");
    }
    else
    {
        MessageHandler::fail(__FILE__, __LINE__, "utils/memory/verif/unit_tests/basic/error", "qarr allocation failed");
    }

    // 6. Structured object.
    MessageHandler::inform(__FILE__,
                           __LINE__,
                           "utils/memory/verif/unit_tests/basic/debug",
                           "qobj = JEOD_ALLOC_CLASS_OBJECT (Quxx, ());");
    Quxx * qobj = JEOD_ALLOC_CLASS_OBJECT(Quxx, ());
    if(memory_pool.is_allocated(qobj) && (qobj->are_you_ok()))
    {
        MessageHandler::inform(__FILE__,
                               __LINE__,
                               "utils/memory/verif/unit_tests/basic/debug",
                               "qobj properly initialized and allocated");
    }
    else
    {
        MessageHandler::fail(__FILE__, __LINE__, "utils/memory/verif/unit_tests/basic/error", "qobj allocation failed");
    }

    MessageHandler::inform(__FILE__,
                           __LINE__,
                           "utils/memory/verif/unit_tests/basic/debug",
                           "bobj  = JEOD_ALLOC_CLASS_OBJECT (Bar, ());");
    Bar * bobj = JEOD_ALLOC_CLASS_OBJECT(Bar, ());
    if(memory_pool.is_allocated(bobj) && (bobj->are_you_ok()))
    {
        MessageHandler::inform(__FILE__,
                               __LINE__,
                               "utils/memory/verif/unit_tests/basic/debug",
                               "bobj properly initialized and allocated");
    }
    else
    {
        MessageHandler::debug(__FILE__,
                              __LINE__,
                              "utils/memory/verif/unit_tests/basic/error",
                              "bobj allocation failed");
    }

    // Recast qobj and qarr to a base class pointers.
    barr[0] = qobj;
    barr[1] = qarr;

    // Free all memory.
    MessageHandler::inform(__FILE__,
                           __LINE__,
                           "utils/memory/verif/unit_tests/basic/debug",
                           "JEOD_DELETE_OBJECT (bobj = %p);",
                           bobj);
    JEOD_DELETE_OBJECT(bobj);

    MessageHandler::inform(__FILE__,
                           __LINE__,
                           "utils/memory/verif/unit_tests/basic/debug",
                           "JEOD_DELETE_OBJECT (barr[0] = %p);",
                           barr[0]);
    JEOD_DELETE_OBJECT(barr[0]);

    MessageHandler::inform(__FILE__,
                           __LINE__,
                           "utils/memory/verif/unit_tests/basic/debug",
                           "JEOD_DELETE_ARRAY (barr[1] = %p);",
                           barr[1]);
    JEOD_DELETE_ARRAY(barr[1]);

    MessageHandler::inform(__FILE__,
                           __LINE__,
                           "utils/memory/verif/unit_tests/basic/debug",
                           "JEOD_DELETE_ARRAY (barr = %p)",
                           barr);
    JEOD_DELETE_ARRAY(barr);

    MessageHandler::inform(__FILE__,
                           __LINE__,
                           "utils/memory/verif/unit_tests/basic/debug",
                           "JEOD_DELETE_ARRAY (carr = %p)",
                           carr);
    JEOD_DELETE_ARRAY(carr);

    MessageHandler::inform(__FILE__,
                           __LINE__,
                           "utils/memory/verif/unit_tests/basic/debug",
                           "JEOD_DELETE_ARRAY (darr = %p)",
                           darr);
    JEOD_DELETE_ARRAY(darr);

    MessageHandler::inform(__FILE__,
                           __LINE__,
                           "utils/memory/verif/unit_tests/basic/debug",
                           "JEOD_DELETE_OBJECT (i42 = %p)",
                           i42);
    JEOD_DELETE_OBJECT(i42);

    // Determine whether this test passed.
    bool all_freed = JeodMemoryManager::is_table_empty();
    bool passed;

    // Shutdown the memory manager.
    JeodMemoryManager::set_debug_level(JEOD_MEMORY_DEBUG);
    sim_interface.shutdown();

    passed = all_freed;

    if(overwrite_outside)
    {
        passed = passed && (sim_interface.get_allowed_count() == 0);
    }

    // Generate summary message.
    std::string temp1 = "";
    if(argc > 1)
    {
        temp1 = argv[1];
    }

    const char passFailStrings[2][7] = {"passed", "failed"};
    int index = 1;
    if(passed)
    {
        index = 0;
    }

    print_debug(0, stdout, "Test %s %s\n", temp1.c_str(), passFailStrings[index]);

    if(passed)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}
