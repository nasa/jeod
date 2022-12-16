#ifndef JEOD_MEMORY_DEBUG
#define JEOD_MEMORY_DEBUG 3
#endif

#include <unistd.h>
#include "utils/memory/include/jeod_alloc.hh"
#include "utils/memory/include/memory_messages.hh"

#include "test_harness/include/test_sim_interface.hh"
#include "test_harness/include/cmdline_parser.hh"

using namespace jeod;

const unsigned int zero = 0;
bool overwrite_outside = false;
bool leave_unfreed_memory = false;
TestSimInterface sim_interface;




/*
 * Define a set of classes, some of which have diamond inheritance.
 */

class Base {

 public:
   Base ()
   :
      checking(false),
      base(1)
   {
      trace ("Constructing", what_are_you_really(), (void*)this);
   }

   virtual ~Base ()
   {
      trace ("Destructing", what_are_you_really(), (void*)this);
      check_ok();
   }

   virtual char * address () {return (char*)this;}
   virtual const char * what_are_you_really () {return "Base";}
   virtual bool are_you_ok () {return yes_im_ok();}
   virtual int * elem_addr () {return &base;}

   bool yes_im_ok () {return (base == 1);}
   const char * what_are_you () {return "Base";}

   void check_ok () {
      if (! checking) {
         if (! are_you_ok()) {
            MessageHandler::warn (
               __FILE__, __LINE__, MemoryMessages::corrupted_memory,
               "%s @ 0x%p is corrupted",
               what_are_you_really(), address());
         }
         checking = true;
      }
   }

   void assert_ok () {
      if (! are_you_ok()) {
         MessageHandler::fail (
            __FILE__, __LINE__, MemoryMessages::corrupted_memory,
            "%s @ 0x%p is corrupted",
            what_are_you_really(), address());
      }
   }

   void trace (const char * what, const char * who, const void * where) {
      print_debug (3, stderr, " %s %-4s @ %p\n", what, who, where);
   }

 protected:
   bool checking;
   int base;
};

class Foo : public virtual Base {
 public:
   Foo ()
   :
      foo(2)
   {
      trace ("Constructing", what_are_you_really(), (void*)this);
   }

   virtual ~Foo ()
   {
      trace ("Destructing", what_are_you_really(), (void*)this);
      check_ok();
   }

   virtual char * address () {return (char*)this;}
   virtual const char * what_are_you_really () {return "Foo";}
   virtual bool are_you_ok () {return yes_im_ok() && Base::are_you_ok();}
   virtual int * elem_addr () {return &foo;}

   bool yes_im_ok () {return (foo == 2);}
   const char * what_are_you () {return "Foo";}

 protected:
   int foo;
};

class Bar : public Foo {
 public:
   Bar ()
   :
      bar(3)
   {
      trace ("Constructing", what_are_you_really(), (void*)this);
   }

   virtual ~Bar ()
   {
      trace ("Destructing", what_are_you_really(), (void*)this);
      check_ok();
   }

   virtual char * address () {return (char*)this;}
   virtual const char * what_are_you_really () {return "Bar";}
   virtual bool are_you_ok () {return yes_im_ok() && Foo::are_you_ok();}
   virtual int * elem_addr () {return &bar;}

   bool yes_im_ok () {return (bar == 3);}
   const char * what_are_you () {return "Bar";}

 protected:
   int bar;
};

class Baz : public virtual Base {
 public:
   Baz ()
   :
       baz(4)
   {
      trace ("Constructing", what_are_you_really(), (void*)this);
   }

   virtual ~Baz ()
   {
      trace ("Destructing", what_are_you_really(), (void*)this);
      check_ok();
   }

   virtual char * address () {return (char*)this;}
   virtual const char * what_are_you_really () {return "Baz";}
   virtual bool are_you_ok () {return yes_im_ok() && Base::are_you_ok();}
   virtual int * elem_addr () {return &baz;}

   bool yes_im_ok () {return (baz == 4);}
   const char * what_are_you () {return "Baz";}

 protected:
   int baz;
};

class Qux : public Bar, public Baz {
 public:
   Qux ()
   :
       qux(5)
   {
      trace ("Constructing", what_are_you_really(), (void*)this);
   }

   virtual ~Qux ()
   {
      trace ("Destructing", what_are_you_really(), (void*)this);
      check_ok();
   }

   virtual char * address () {return (char*)this;}
   virtual const char * what_are_you_really () {return "Qux";}
   virtual bool are_you_ok () {return yes_im_ok() &&
                                      Bar::are_you_ok() &&
                                      Baz::are_you_ok();}
   virtual int * elem_addr () {return &qux;}

   bool yes_im_ok () {return (qux == 5);}
   const char * what_are_you () {return "Qux";}

 protected:
   int qux;
};

class Quxx : public Baz, public Bar {
 public:
   Quxx ()
   :
       quxx(6)
   {
      trace ("Constructing", what_are_you_really(), (void*)this);
   }

   virtual ~Quxx ()
   {
      trace ("Destructing", what_are_you_really(), (void*)this);
      check_ok();
   }

   virtual char * address () {return (char*)this;}
   virtual const char * what_are_you_really () {return "Quxx";}
   virtual bool are_you_ok () {return yes_im_ok() &&
                                      Baz::are_you_ok() &&
                                      Bar::are_you_ok();}
   virtual int * elem_addr () {return &quxx;}

   bool yes_im_ok () {return (quxx == 6);}
   const char * what_are_you () {return "Quxx";}

 protected:
   int quxx;
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

#define WITH_PRINTF_LOCKED(stmt) \
   do { \
      TestSimInterface::lock_printf(); \
      stmt; \
      TestSimInterface::unlock_printf(); \
   } while (0)


class Allocator {
 public:
   virtual ~Allocator () {}
   virtual void * run (unsigned int nrepeat) = 0;
};

template <typename DerivedType, typename BaseType>
class TypeAllocator : public Allocator {
 public:
   void * run (unsigned int nrepeat)
   {
      unsigned int nkeep = 60 / nrepeat;
      unsigned int istart;
      int twait = 10000*nkeep;
      BaseType ** temp_objs;
      BaseType ** keep_objs;
      DerivedType * obj;

      JEOD_REGISTER_CLASS (BaseType);

      WITH_PRINTF_LOCKED (
         temp_objs = JEOD_ALLOC_CLASS_POINTER_ARRAY(nkeep, BaseType));

      WITH_PRINTF_LOCKED (
         keep_objs = JEOD_ALLOC_CLASS_POINTER_ARRAY(nrepeat+1, BaseType));

      WITH_PRINTF_LOCKED (
         istart = 0;
         keep_objs[0] = obj = JEOD_ALLOC_CLASS_OBJECT (DerivedType, ());
         print_debug (
            2, stderr, "Saved %s @ 0x%p as a %s (really a %s) @ 0x%p\n",
            obj->what_are_you(),
            obj,
            keep_objs[0]->what_are_you(),
            keep_objs[0]->what_are_you_really(),
            keep_objs[0]));

      if (overwrite_outside && leave_unfreed_memory) {
         WITH_PRINTF_LOCKED (
            istart = 1;
            keep_objs[1] = obj = JEOD_ALLOC_CLASS_OBJECT (DerivedType, ());
         );
      }

      if (overwrite_outside) {
         // Message here!
         (reinterpret_cast<int *>(obj))[-1] = 0;
      }

      for (unsigned int ii = istart; ii < nrepeat; ++ii) {
         WITH_PRINTF_LOCKED (
            keep_objs[ii+1] = obj = JEOD_ALLOC_CLASS_OBJECT (DerivedType, ()));

         usleep (twait);
         for (unsigned int jj = 0; jj < nkeep; ++jj) {
            WITH_PRINTF_LOCKED (
               temp_objs[jj] = obj = JEOD_ALLOC_CLASS_OBJECT (DerivedType, ()));
            usleep (1);
         }

         usleep (twait);
         for (unsigned int jj = 0; jj < nkeep; ++jj) {
            WITH_PRINTF_LOCKED (
               temp_objs[jj]->assert_ok();
               JEOD_DELETE_OBJECT (temp_objs[jj]));
            usleep (1);
         }
         usleep (twait);
      }

      istart = 0;
      if (leave_unfreed_memory) {
         istart = 1;
      }
      for (unsigned int ii = istart; ii < nrepeat+1; ++ii) {
         WITH_PRINTF_LOCKED (
            JEOD_DELETE_OBJECT (keep_objs[ii]));
      }

      WITH_PRINTF_LOCKED (
         JEOD_DELETE_ARRAY (temp_objs));

      WITH_PRINTF_LOCKED (
         JEOD_DELETE_ARRAY (keep_objs));

      return reinterpret_cast<void *> (const_cast <unsigned int *>(&zero));
   }
};
#undef JEOD_MEMORY_DEBUG
#define JEOD_MEMORY_DEBUG OLD_DEBUG


void * run_thread (void *);

/**
 * A TestThread uses an Allocator to allocate and delete lots of memory.
 */
class TestThread {

 // Member functions.
 public:

   // Default constructor, needed by memory manager
   TestThread (
      void)
   :
      allocator(NULL),
      nrepeat(0)
   { }

   // Non-default constructor.
   TestThread (
      Allocator * alloc,
      unsigned int nrep)
   :
      allocator(alloc),
      nrepeat(nrep)
   { }

   // Destructor.
   ~TestThread (
      void)
   { }

   void start (
      void)
   {
      pthread_create (&thread, NULL,
                      run_thread, reinterpret_cast<void*>(this));
   }

   void * run (
      void)
   {
      return allocator->run (nrepeat);
   }

   void join (
      void)
   {
      pthread_join (thread, NULL);
   }

 // Member data
 protected:
   Allocator * allocator;
   unsigned int nrepeat;
 private:
   pthread_t thread;
};

template <typename DerivedType, typename BaseType>
class TypeTestThread : public TestThread {

 private:
   typedef TypeAllocator<DerivedType,BaseType> AllocatorType;

 public:

   // Default constructor, needed by memory manager
   TypeTestThread (
      void)
   :
      TestThread()
   { }

   // Non-default constructor.
   TypeTestThread (
      unsigned int nrep)
   :
      TestThread((JEOD_ALLOC_CLASS_OBJECT(AllocatorType, ())),
                 nrep)
   { }

   // Destructor.
   ~TypeTestThread (
      void)
   {
      JEOD_DELETE_OBJECT (allocator);
   }
};


void *
run_thread (
   void * arg)
{
   TestThread * thread = reinterpret_cast<TestThread *> (arg);
   return thread->run ();
}


int main (
   int argc,
   char ** argv)
{

   TestThread * runs[12];
   unsigned int nthreads;

   int message_level;
   unsigned int memory_level;

   CmdlineParser cmdline_parser;

   // Parse the command line.
   // Option -outside => overwrite the memory manager markers.
   // Option -inside  => overwrite data inside the allocated classes.
   cmdline_parser.add ("outside", &overwrite_outside);
   cmdline_parser.add ("leak",    &leave_unfreed_memory);
   cmdline_parser.parse (argc, argv);


   // Determine the message handler and memory manager report thresholds.
#if JEOD_MEMORY_DEBUG > 1
   message_level = MessageHandler::Debug;
   memory_level  = JEOD_MEMORY_DEBUG;
#elif JEOD_MEMORY_DEBUG > 0
   message_level = MessageHandler::Notice;
   memory_level  = JEOD_MEMORY_DEBUG;
#else
   message_level = MessageHandler::Warning;
   memory_level  = 1;
#endif

   // FIXME
   message_level = MessageHandler::Notice;
   memory_level  = 3;

   // Initialize JEOD
   MessageHandler::set_suppression_level (message_level);
   JeodMemoryManager::set_debug_level (memory_level);
   if (message_level == MessageHandler::Debug) {
      sim_interface.enable_lock ();
   }
   if (overwrite_outside) {
      JeodMemoryManager::set_guard_enabled (true);
   }
   if (overwrite_outside || leave_unfreed_memory) {
      sim_interface.add_allowed_code ("utils/memory/corrupted_memory");
   }

   sim_interface.add_allowed_code ("utils/memory/debug");
   sim_interface.add_verboten_code ("utils/memory");

   // Create the runs.
   typedef TypeTestThread<Base,Base> TestThreadType00;
   typedef TypeTestThread<Foo,Base>  TestThreadType01;
   typedef TypeTestThread<Bar,Base>  TestThreadType02;
   typedef TypeTestThread<Baz,Base>  TestThreadType03;
   typedef TypeTestThread<Qux,Base>  TestThreadType04;
   typedef TypeTestThread<Quxx,Base> TestThreadType05;
   typedef TypeTestThread<Foo,Foo>   TestThreadType06;
   typedef TypeTestThread<Bar,Foo>   TestThreadType07;
   typedef TypeTestThread<Qux,Foo>   TestThreadType08;
   typedef TypeTestThread<Quxx,Foo>  TestThreadType09;
   typedef TypeTestThread<Qux,Baz>   TestThreadType10;
   typedef TypeTestThread<Quxx,Baz>  TestThreadType11;

   nthreads = 0;
   runs[nthreads++] = JEOD_ALLOC_CLASS_OBJECT (TestThreadType00,  (1));
   runs[nthreads++] = JEOD_ALLOC_CLASS_OBJECT (TestThreadType01,  (2));
   runs[nthreads++] = JEOD_ALLOC_CLASS_OBJECT (TestThreadType02,  (3));
   runs[nthreads++] = JEOD_ALLOC_CLASS_OBJECT (TestThreadType03,  (4));
   runs[nthreads++] = JEOD_ALLOC_CLASS_OBJECT (TestThreadType04,  (5));
   runs[nthreads++] = JEOD_ALLOC_CLASS_OBJECT (TestThreadType05,  (6));
   runs[nthreads++] = JEOD_ALLOC_CLASS_OBJECT (TestThreadType06, (10));
   runs[nthreads++] = JEOD_ALLOC_CLASS_OBJECT (TestThreadType07, (12));
   runs[nthreads++] = JEOD_ALLOC_CLASS_OBJECT (TestThreadType08, (12));
   runs[nthreads++] = JEOD_ALLOC_CLASS_OBJECT (TestThreadType09, (15));
   runs[nthreads++] = JEOD_ALLOC_CLASS_OBJECT (TestThreadType10, (30));
   runs[nthreads++] = JEOD_ALLOC_CLASS_OBJECT (TestThreadType11, (60));

   // Start the threads.
   print_debug (1, stderr, "Starting threads\n");
   for (unsigned int ii = 0; ii < nthreads; ++ii) {
      runs[ii]->start ();
   }

   // Wait on the threads to complete.
   for (unsigned int ii = 0; ii < nthreads; ++ii) {
      runs[ii]->join ();
      JEOD_DELETE_OBJECT (runs[ii]);
   }
   print_debug (1, stderr, "\nThreads finished\n");

   // Determine whether this test passed.
   bool all_freed = JeodMemoryManager::is_table_empty();
   bool passed;

   // Shutdown the memory manager.
   JeodMemoryManager::set_debug_level (JEOD_MEMORY_DEBUG);
   sim_interface.shutdown ();

   unsigned int allowed_count = sim_interface.get_allowed_count();

   if (!(overwrite_outside || leave_unfreed_memory)) {
      passed = all_freed && (allowed_count == 0);
   }
   else if (! leave_unfreed_memory) {
      passed = all_freed && (allowed_count == nthreads);
   }
   else if (! overwrite_outside) {
      passed = (! all_freed) && (allowed_count == nthreads+1);
   }
   else {
      passed = (! all_freed) && (allowed_count == 2*nthreads+1);
   }

   // Generate summary message.
   print_debug (0, stdout, "Test %s %s\n",
                (argc > 1 ? argv[1] : ""),
                (passed ? "passed" : "failed"));


   if (passed) {
      return 0;
   }
   else {
      return 1;
   }
}
