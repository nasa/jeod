#include "utils/container/include/object_list.hh"
#include "utils/container/include/pointer_list.hh"
#include "utils/container/include/primitive_list.hh"
#include <iostream>
#include <list>

using namespace jeod;

struct Foo
{
    Foo() = default;

    Foo(int val)
        : bar(val)
    {
    }

    int bar{};
};

inline bool operator<(const Foo & x, const Foo & y)
{
    return x.bar < y.bar;
}

void print_foo_list(const std::list<Foo *> list)
{
    std::cout << "Start\n";
    for(auto foo : list)
    {
        std::cout << " " << foo->bar << "\n";
    }
    std::cout << "End\n\n";
}

template<typename Type1, typename Type2>
void compare_foo_list(const std::string & name1, const Type1 & arg1, const std::string & name2, const Type2 & arg2)
{
    const char trueFalseStrings[2][6] = {"true", "false"};
    int index;
    std::cout << "Comparisons of " << name1 << " to " << name2 << ":\n";

    index = 1;
    if(arg1 == arg2)
    {
        index = 0;
    }
    std::cout << name1 << " == " << name2 << " -> " << trueFalseStrings[index] << "\n";

    index = 1;
    if(arg1 < arg2)
    {
        index = 0;
    }
    std::cout << name1 << " <  " << name2 << " -> " << trueFalseStrings[index] << "\n";

    index = 1;
    if((arg1 > arg2))
    {
        index = 0;
    }
    std::cout << name1 << " >  " << name2 << " -> " << trueFalseStrings[index] << "\n";

    index = 1;
    if(arg1 != arg2)
    {
        index = 0;
    }
    std::cout << name1 << " != " << name2 << " -> " << trueFalseStrings[index] << "\n";

    index = 1;
    if(arg1 >= arg2)
    {
        index = 0;
    }
    std::cout << name1 << " >= " << name2 << " -> " << trueFalseStrings[index] << "\n";

    index = 1;
    if(arg1 <= arg2)
    {
        index = 0;
    }
    std::cout << name1 << " <= " << name2 << " -> " << trueFalseStrings[index] << "\n";
}

bool foo_leq(Foo * const foo1, Foo * const foo2)
{
    return foo1->bar <= foo2->bar;
}

bool foo_eq(Foo * const foo1, Foo * const foo2)
{
    return foo1->bar == foo2->bar;
}

int main()
{
    Foo foo1(1);
    Foo foo2(2);
    Foo foo3(3);
    Foo foo4(4);
    Foo foo1alt(1);

    std::list<Foo *> stl_foo_pointer_list;
    JeodPointerList<Foo>::type jeod_foo_pointer_list;

    stl_foo_pointer_list.push_back(&foo2);
    stl_foo_pointer_list.push_back(&foo3);
    stl_foo_pointer_list.push_front(&foo1);

    std::cout << "Initial contents of stl_foo_pointer_list:\n";
    print_foo_list(stl_foo_pointer_list);

    std::cout << "Initial contents of jeod_foo_pointer_list:\n";
    print_foo_list(jeod_foo_pointer_list);

    std::cout << "Initial contents of another_foo_pointer_list (STL copy):\n";
    JeodPointerList<Foo>::type another_foo_pointer_list(stl_foo_pointer_list);
    print_foo_list(another_foo_pointer_list);

    std::cout << "Clearing contents of another_foo_pointer_list.\n";
    another_foo_pointer_list.clear();
    std::cout << "Is empty = " << another_foo_pointer_list.empty() << "\n";
    std::cout << "Cleared contents of another_foo_pointer_list:\n";
    print_foo_list(another_foo_pointer_list);

    compare_foo_list("jeod_foo_pointer_list", jeod_foo_pointer_list, "stl_foo_pointer_list", stl_foo_pointer_list);
    compare_foo_list("jeod_foo_pointer_list",
                     jeod_foo_pointer_list,
                     "another_foo_pointer_list",
                     another_foo_pointer_list);

    std::cout << "\nAdding 1, 2, and 4 to another_foo_pointer_list:\n";
    another_foo_pointer_list.push_back(&foo1alt);
    another_foo_pointer_list.push_back(&foo2);
    another_foo_pointer_list.push_back(&foo4);

    std::cout << "\nDuplicating front, back of another_foo_pointer_list:\n";
    another_foo_pointer_list.push_front(&foo1alt);
    another_foo_pointer_list.push_back(&foo4);
    std::cout << "After duplicating front, back:\n";
    print_foo_list(another_foo_pointer_list);

    std::cout << "\nPopping front, back of another_foo_pointer_list:\n";
    another_foo_pointer_list.pop_back();
    another_foo_pointer_list.pop_front();
    std::cout << "After deleting (duplicate) front, back:\n";
    print_foo_list(another_foo_pointer_list);

    std::cout << "Updated contents of another_foo_pointer_list:\n";
    print_foo_list(another_foo_pointer_list);
    compare_foo_list("jeod_foo_pointer_list",
                     jeod_foo_pointer_list,
                     "another_foo_pointer_list",
                     another_foo_pointer_list);
    compare_foo_list("stl_foo_pointer_list",
                     stl_foo_pointer_list,
                     "another_foo_pointer_list",
                     another_foo_pointer_list);

    std::cout << "\nCopying stl_foo_pointer_list to jeod_foo_pointer_list:\n";
    jeod_foo_pointer_list = stl_foo_pointer_list;
    std::cout << "Updated contents of jeod_foo_pointer_list:\n";
    print_foo_list(jeod_foo_pointer_list);
    compare_foo_list("jeod_foo_pointer_list", jeod_foo_pointer_list, "stl_foo_pointer_list", stl_foo_pointer_list);
    compare_foo_list("jeod_foo_pointer_list",
                     jeod_foo_pointer_list,
                     "another_foo_pointer_list",
                     another_foo_pointer_list);

    // Need to test splice, reverse, remove, remove_if, merege, merge w/ compare

    std::cout << "\nCopying another_foo_pointer_list "
                 "to jeod_foo_pointer_list:\n";
    jeod_foo_pointer_list = another_foo_pointer_list;
    std::cout << "Updated contents of jeod_foo_pointer_list:\n";
    print_foo_list(jeod_foo_pointer_list);
    compare_foo_list("jeod_foo_pointer_list", jeod_foo_pointer_list, "stl_foo_pointer_list", stl_foo_pointer_list);
    compare_foo_list("jeod_foo_pointer_list",
                     jeod_foo_pointer_list,
                     "another_foo_pointer_list",
                     another_foo_pointer_list);

    std::cout << "\nSwapping contents of jeod_foo_pointer_list "
                 "and stl_foo_pointer_list:\n";
    jeod_foo_pointer_list.swap_contents(stl_foo_pointer_list);
    std::cout << "Updated contents of jeod_foo_pointer_list:\n";
    print_foo_list(jeod_foo_pointer_list);
    std::cout << "Updated contents of stl_foo_pointer_list:\n";
    print_foo_list(stl_foo_pointer_list);
    compare_foo_list("jeod_foo_pointer_list", jeod_foo_pointer_list, "stl_foo_pointer_list", stl_foo_pointer_list);
    compare_foo_list("jeod_foo_pointer_list",
                     jeod_foo_pointer_list,
                     "another_foo_pointer_list",
                     another_foo_pointer_list);
    compare_foo_list("stl_foo_pointer_list",
                     stl_foo_pointer_list,
                     "another_foo_pointer_list",
                     another_foo_pointer_list);

    std::cout << "\nClearing contents of another_foo_pointer_list:\n";
    another_foo_pointer_list.clear();
    std::cout << "Updated contents of another_foo_pointer_list:\n";
    print_foo_list(another_foo_pointer_list);
    compare_foo_list("jeod_foo_pointer_list", jeod_foo_pointer_list, "stl_foo_pointer_list", stl_foo_pointer_list);
    compare_foo_list("jeod_foo_pointer_list",
                     jeod_foo_pointer_list,
                     "another_foo_pointer_list",
                     another_foo_pointer_list);
    compare_foo_list("stl_foo_pointer_list",
                     stl_foo_pointer_list,
                     "another_foo_pointer_list",
                     another_foo_pointer_list);

    std::cout << "\nSwapping contents of jeod_foo_pointer_list "
                 "and another_foo_pointer_list:\n";
    jeod_foo_pointer_list.swap_contents(another_foo_pointer_list);
    std::cout << "Updated contents of jeod_foo_pointer_list:\n";
    print_foo_list(jeod_foo_pointer_list);
    std::cout << "Updated contents of another_foo_pointer_list:\n";
    print_foo_list(another_foo_pointer_list);
    compare_foo_list("jeod_foo_pointer_list", jeod_foo_pointer_list, "stl_foo_pointer_list", stl_foo_pointer_list);
    compare_foo_list("jeod_foo_pointer_list",
                     jeod_foo_pointer_list,
                     "another_foo_pointer_list",
                     another_foo_pointer_list);
    compare_foo_list("stl_foo_pointer_list",
                     stl_foo_pointer_list,
                     "another_foo_pointer_list",
                     another_foo_pointer_list);

    std::cout << "\nMerging contents of stl_foo_pointer_list "
                 "into jeod_foo_pointer_list:\n";
    jeod_foo_pointer_list.merge(stl_foo_pointer_list);
    std::cout << "Updated contents of jeod_foo_pointer_list:\n";
    print_foo_list(jeod_foo_pointer_list);
    std::cout << "Updated contents of stl_foo_pointer_list:\n";
    print_foo_list(stl_foo_pointer_list);

    std::cout << "\nMerging contents of another_foo_pointer_list "
                 "into jeod_foo_pointer_list:\n";
    jeod_foo_pointer_list.merge(another_foo_pointer_list);
    std::cout << "Updated contents of jeod_foo_pointer_list:\n";
    print_foo_list(jeod_foo_pointer_list);
    std::cout << "Updated contents of another_foo_pointer_list:\n";
    print_foo_list(another_foo_pointer_list);

    std::cout << "\nCopying jeod_foo_pointer_list to stl_foo_pointer_list:\n";
    stl_foo_pointer_list = jeod_foo_pointer_list;
    std::cout << "Updated contents of stl_foo_pointer_list:\n";
    print_foo_list(stl_foo_pointer_list);

    std::cout << "\nSorting contents of jeod_foo_pointer_list "
                 "(default compare)\n";
    jeod_foo_pointer_list.sort();
    std::cout << "Updated contents of jeod_foo_pointer_list:\n";
    print_foo_list(jeod_foo_pointer_list);

    std::cout << "\nRemoving duplicates from jeod_foo_pointer_list "
                 "(default predicate):\n";
    jeod_foo_pointer_list.unique();
    std::cout << "Updated contents of jeod_foo_pointer_list:\n";
    print_foo_list(jeod_foo_pointer_list);

    std::cout << "\nCopying stl_foo_pointer_list to jeod_foo_pointer_list:\n";
    jeod_foo_pointer_list = stl_foo_pointer_list;
    std::cout << "Updated contents of jeod_foo_pointer_list:\n";
    print_foo_list(jeod_foo_pointer_list);

    std::cout << "\nSorting contents of jeod_foo_pointer_list "
                 "(explicit compare)\n";
    jeod_foo_pointer_list.sort(foo_leq);
    std::cout << "Updated contents of jeod_foo_pointer_list:\n";
    print_foo_list(jeod_foo_pointer_list);

    std::cout << "\nRemoving duplicates from jeod_foo_pointer_list "
                 "(explicit predicate):\n";
    jeod_foo_pointer_list.unique(foo_eq);
    std::cout << "Updated contents of jeod_foo_pointer_list:\n";
    print_foo_list(jeod_foo_pointer_list);

    std::cout << "\nTest passed.\n";
    return 0;
}
