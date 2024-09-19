#ifndef JEOD_MEMORY_DEBUG
#define JEOD_MEMORY_DEBUG 3
#endif

#include "utils/memory/include/jeod_alloc.hh"
#include "utils/memory/include/memory_messages.hh"
#include <unistd.h>

#include "test_harness/include/cmdline_parser.hh"
#include "test_harness/include/test_sim_interface.hh"

using namespace jeod;

const unsigned int zero = 0;
bool overwrite_outside = false;
bool leave_unfreed_memory = false;
TestSimInterface sim_interface;

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

#if JEOD_MEMORY_DEBUG > 2
#define OLD_DEBUG 3
#elif JEOD_MEMORY_DEBUG > 1
#define OLD_DEBUG 2
#elif JEOD_MEMORY_DEBUG > 0
#define OLD_DEBUG 1
#else
#define OLD_DEBUG 0
#endif
#undef JEOD_MEMORY_DEBUG
#if OLD_DEBUG > 2
#define JEOD_MEMORY_DEBUG 3
#else
#define JEOD_MEMORY_DEBUG 2
#endif

#define WITH_PRINTF_LOCKED(stmt)                                                                                       \
    do                                                                                                                 \
    {                                                                                                                  \
        TestSimInterface::lock_printf();                                                                               \
        stmt;                                                                                                          \
        TestSimInterface::unlock_printf();                                                                             \
    } while(0)

class Allocator
{
public:
    virtual ~Allocator() = default;
    virtual void * run(unsigned int nrepeat) = 0;
};

template<typename DerivedType, typename BaseType> class TypeAllocator : public Allocator
{
public:
    void * run(unsigned int nrepeat) override
    {
        unsigned int nkeep = 60 / nrepeat;
        unsigned int istart;
        int twait = 10000 * nkeep;
        BaseType ** temp_objs;
        BaseType ** keep_objs;
        DerivedType * obj;

        JEOD_REGISTER_CLASS(BaseType);

        WITH_PRINTF_LOCKED(temp_objs = JEOD_ALLOC_CLASS_POINTER_ARRAY(nkeep, BaseType));

        WITH_PRINTF_LOCKED(keep_objs = JEOD_ALLOC_CLASS_POINTER_ARRAY(nrepeat + 1, BaseType));

        WITH_PRINTF_LOCKED(istart = 0; keep_objs[0] = obj = JEOD_ALLOC_CLASS_OBJECT(DerivedType, ());
                           print_debug(2,
                                       stderr,
                                       "Saved %s @ 0x%p as a %s (really a %s) @ 0x%p\n",
                                       obj->what_are_you(),
                                       obj,
                                       keep_objs[0]->what_are_you(),
                                       keep_objs[0]->what_are_you_really(),
                                       keep_objs[0]));

        if(overwrite_outside && leave_unfreed_memory)
        {
            WITH_PRINTF_LOCKED(istart = 1; keep_objs[1] = obj = JEOD_ALLOC_CLASS_OBJECT(DerivedType, ()););
        }

        if(overwrite_outside)
        {
            // Message here!
            (reinterpret_cast<int *>(obj))[-1] = 0;
        }

        for(unsigned int ii = istart; ii < nrepeat; ++ii)
        {
            WITH_PRINTF_LOCKED(keep_objs[ii + 1] = obj = JEOD_ALLOC_CLASS_OBJECT(DerivedType, ()));

            usleep(twait);
            for(unsigned int jj = 0; jj < nkeep; ++jj)
            {
                WITH_PRINTF_LOCKED(temp_objs[jj] = obj = JEOD_ALLOC_CLASS_OBJECT(DerivedType, ()));
                usleep(1);
            }

            usleep(twait);
            for(unsigned int jj = 0; jj < nkeep; ++jj)
            {
                WITH_PRINTF_LOCKED(temp_objs[jj]->assert_ok(); JEOD_DELETE_OBJECT(temp_objs[jj]));
                usleep(1);
            }
            usleep(twait);
        }

        istart = 0;
        if(leave_unfreed_memory)
        {
            istart = 1;
        }
        for(unsigned int ii = istart; ii < nrepeat + 1; ++ii)
        {
            WITH_PRINTF_LOCKED(JEOD_DELETE_OBJECT(keep_objs[ii]));
        }

        WITH_PRINTF_LOCKED(JEOD_DELETE_ARRAY(temp_objs));

        WITH_PRINTF_LOCKED(JEOD_DELETE_ARRAY(keep_objs));

        return reinterpret_cast<void *>(const_cast<unsigned int *>(&zero));
    }
};

#undef JEOD_MEMORY_DEBUG
#define JEOD_MEMORY_DEBUG OLD_DEBUG

void * run_thread(void *);

/**
 * A TestThread uses an Allocator to allocate and delete lots of memory.
 */
class TestThread
{
    // Member functions.
public:
    // Non-default constructor.
    TestThread(Allocator * alloc, unsigned int nrep)
        : allocator(alloc),
          nrepeat(nrep)
    {
    }

    TestThread() = default;
    ~TestThread() = default;

    void start()
    {
        pthread_create(&thread, nullptr, run_thread, reinterpret_cast<void *>(this));
    }

    void * run()
    {
        return allocator->run(nrepeat);
    }

    void join()
    {
        pthread_join(thread, nullptr);
    }

    // Member data
protected:
    Allocator * allocator{};
    unsigned int nrepeat{};

private:
    pthread_t thread{};
};

template<typename DerivedType, typename BaseType> class TypeTestThread : public TestThread
{
private:
    using AllocatorType = TypeAllocator<DerivedType, BaseType>;

public:
    TypeTestThread() = default;

    // Non-default constructor.
    TypeTestThread(unsigned int nrep)
        : TestThread((JEOD_ALLOC_CLASS_OBJECT(AllocatorType, ())), nrep)
    {
    }

    // Destructor.
    ~TypeTestThread()
    {
        JEOD_DELETE_OBJECT(allocator);
    }
};

void * run_thread(void * arg)
{
    auto * thread = reinterpret_cast<TestThread *>(arg);
    return thread->run();
}

int main(int argc, char ** argv)
{
    TestThread * runs[12];
    unsigned int nthreads;

    int message_level;
    unsigned int memory_level;

    CmdlineParser cmdline_parser;

    // Parse the command line.
    // Option -outside => overwrite the memory manager markers.
    // Option -inside  => overwrite data inside the allocated classes.
    cmdline_parser.add("outside", &overwrite_outside);
    cmdline_parser.add("leak", &leave_unfreed_memory);
    cmdline_parser.parse(argc, argv);

    // Determine the message handler and memory manager report thresholds.
#if JEOD_MEMORY_DEBUG == 3
    message_level = MessageHandler::Debug;
    memory_level = 3;
#else
    message_level = MessageHandler::Notice;
    memory_level = 2
#endif

    // Initialize JEOD
    MessageHandler::set_suppression_level(message_level);
    JeodMemoryManager::set_debug_level(memory_level);
    if(message_level == MessageHandler::Debug)
    {
        sim_interface.enable_lock();
    }
    if(overwrite_outside)
    {
        JeodMemoryManager::set_guard_enabled(true);
    }
    if(overwrite_outside || leave_unfreed_memory)
    {
        sim_interface.add_allowed_code("utils/memory/corrupted_memory");
    }

    sim_interface.add_allowed_code("utils/memory/debug");
    sim_interface.add_verboten_code("utils/memory");

    // Create the runs.
    using TestThreadType00 = TypeTestThread<Base, Base>;
    using TestThreadType01 = TypeTestThread<Foo, Base>;
    using TestThreadType02 = TypeTestThread<Bar, Base>;
    using TestThreadType03 = TypeTestThread<Baz, Base>;
    using TestThreadType04 = TypeTestThread<Qux, Base>;
    using TestThreadType05 = TypeTestThread<Quxx, Base>;
    using TestThreadType06 = TypeTestThread<Foo, Foo>;
    using TestThreadType07 = TypeTestThread<Bar, Foo>;
    using TestThreadType08 = TypeTestThread<Qux, Foo>;
    using TestThreadType09 = TypeTestThread<Quxx, Foo>;
    using TestThreadType10 = TypeTestThread<Qux, Baz>;
    using TestThreadType11 = TypeTestThread<Quxx, Baz>;

    nthreads = 0;
    runs[nthreads++] = JEOD_ALLOC_CLASS_OBJECT(TestThreadType00, (1));
    runs[nthreads++] = JEOD_ALLOC_CLASS_OBJECT(TestThreadType01, (2));
    runs[nthreads++] = JEOD_ALLOC_CLASS_OBJECT(TestThreadType02, (3));
    runs[nthreads++] = JEOD_ALLOC_CLASS_OBJECT(TestThreadType03, (4));
    runs[nthreads++] = JEOD_ALLOC_CLASS_OBJECT(TestThreadType04, (5));
    runs[nthreads++] = JEOD_ALLOC_CLASS_OBJECT(TestThreadType05, (6));
    runs[nthreads++] = JEOD_ALLOC_CLASS_OBJECT(TestThreadType06, (10));
    runs[nthreads++] = JEOD_ALLOC_CLASS_OBJECT(TestThreadType07, (12));
    runs[nthreads++] = JEOD_ALLOC_CLASS_OBJECT(TestThreadType08, (12));
    runs[nthreads++] = JEOD_ALLOC_CLASS_OBJECT(TestThreadType09, (15));
    runs[nthreads++] = JEOD_ALLOC_CLASS_OBJECT(TestThreadType10, (30));
    runs[nthreads++] = JEOD_ALLOC_CLASS_OBJECT(TestThreadType11, (60));

    // Start the threads.
    print_debug(1, stderr, "Starting threads\n");
    for(unsigned int ii = 0; ii < nthreads; ++ii)
    {
        runs[ii]->start();
    }

    // Wait on the threads to complete.
    for(unsigned int ii = 0; ii < nthreads; ++ii)
    {
        runs[ii]->join();
        JEOD_DELETE_OBJECT(runs[ii]);
    }
    print_debug(1, stderr, "\nThreads finished\n");

    // Determine whether this test passed.
    bool all_freed = JeodMemoryManager::is_table_empty();
    bool passed;

    // Shutdown the memory manager.
    JeodMemoryManager::set_debug_level(JEOD_MEMORY_DEBUG);
    sim_interface.shutdown();

    unsigned int allowed_count = sim_interface.get_allowed_count();

    if(!(overwrite_outside || leave_unfreed_memory))
    {
        passed = all_freed && (allowed_count == 0);
    }
    else if(!leave_unfreed_memory)
    {
        passed = all_freed && (allowed_count == nthreads);
    }
    else if(!overwrite_outside)
    {
        passed = (!all_freed) && (allowed_count == nthreads + 1);
    }
    else
    {
        passed = (!all_freed) && (allowed_count == 2 * nthreads + 1);
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
