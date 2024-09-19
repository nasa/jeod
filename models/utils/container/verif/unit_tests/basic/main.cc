#include <algorithm>
#include <functional>
#include <iostream>
#include <list>
#include <set>
#include <string>
#include <vector>

#include "utils/container/include/object_list.hh"
#include "utils/container/include/object_set.hh"
#include "utils/container/include/object_vector.hh"
#include "utils/container/include/pointer_list.hh"
#include "utils/container/include/pointer_set.hh"
#include "utils/container/include/pointer_vector.hh"
#include "utils/container/include/primitive_list.hh"
#include "utils/container/include/primitive_set.hh"
#include "utils/container/include/primitive_vector.hh"

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

inline bool operator<=(const Foo & x, const Foo & y)
{
    return x.bar <= y.bar;
}

inline bool operator>(const Foo & x, const Foo & y)
{
    return x.bar > y.bar;
}

inline bool operator>=(const Foo & x, const Foo & y)
{
    return x.bar >= y.bar;
}

inline bool operator==(const Foo & x, const Foo & y)
{
    return x.bar == y.bar;
}

inline bool operator!=(const Foo & x, const Foo & y)
{
    return x.bar != y.bar;
}

inline bool operator<(const Foo & x, int y)
{
    return x.bar < y;
}

inline bool operator<=(const Foo & x, int y)
{
    return x.bar <= y;
}

inline bool operator>(const Foo & x, int y)
{
    return x.bar > y;
}

inline bool operator>=(const Foo & x, int y)
{
    return x.bar >= y;
}

inline bool operator==(const Foo & x, int y)
{
    return x.bar == y;
}

inline bool operator!=(const Foo & x, int y)
{
    return x.bar != y;
}

template<typename Type> struct WithinOne
{
    bool operator()(const Type & first, const Type & second)
    {
        Type del = first.bar - second.bar;
        return (del >= -1) && (del <= 1);
    }
};

template<typename Type, int val> struct WithinOneOf
{
    bool operator()(const Type & first)
    {
        Type del = first.bar - val;
        return (del >= -1) && (del <= 1);
    }
};

inline std::ostream & operator<<(std::ostream & out, const Foo & obj)
{
    return (out << obj.bar);
}

template<typename ElemType> void print_expected(const ElemType & item1)
{
    std::cout << "Expect: contents = " << item1 << "\n";
}

template<typename ElemType> void print_expected(const ElemType & item1, const ElemType & item2)
{
    std::cout << "Expect: contents = " << item1 << " " << item2 << "\n";
}

template<typename ElemType> void print_expected(const ElemType & item1, const ElemType & item2, const ElemType & item3)
{
    std::cout << "Expect: contents = " << item1 << " " << item2 << " " << item3 << "\n";
}

template<typename JeodContainerType, typename STLContainerType, typename ElemType> struct TestContainer
{
    void forward_copy(const JeodContainerType & source, JeodContainerType & dest)
    {
        dest.clear();
        for(auto it = source.begin(); it != source.end(); ++it)
        {
            dest.insert(dest.end(), *it);
        }
    }

    void reverse_copy(const JeodContainerType & source, JeodContainerType & dest)
    {
        dest.clear();
        for(auto it = source.rbegin(); it != source.rend(); ++it)
        {
            dest.insert(dest.end(), *it);
        }
    }

    template<typename Type> void summarize_contents(Type & container, const std::string & id)
    {
        std::cout << "Expect: max_size()=big number\n";
        std::cout << "Summarizing " << id << "\n";
        std::cout << "  max_size()=" << container.max_size() << "\n";
        std::cout << "\n";
    }

    template<typename Type>
    void examine_contents(const Type & container, const std::string & id, bool reverse = false) const
    {
        std::cout << "Examining " << id << "\n";
        std::cout << "  empty()=" << container.empty() << "\n";
        std::cout << "  size()=" << container.size() << "\n";
        std::cout << "  forward contents:";
        for(auto it = container.begin(); it != container.end(); ++it)
        {
            std::cout << " " << *it;
        }
        if(reverse)
        {
            std::cout << "\n";
            std::cout << "  reverse contents:";
            for(auto it = container.rbegin(); it != container.rend(); ++it)
            {
                std::cout << " " << *it;
            }
        }
        std::cout << "\n\n";
    }

    template<typename Type> void examine_contents(Type & container, const std::string & id, bool reverse = false) const
    {
        std::cout << "Examining " << id << "\n";
        std::cout << "  empty()=" << container.empty() << "\n";
        std::cout << "  size()=" << container.size() << "\n";
        std::cout << "  forward contents:";
        for(auto it = container.begin(); it != container.end(); ++it)
        {
            std::cout << " " << *it;
        }
        if(reverse)
        {
            std::cout << "\n";
            std::cout << "  reverse contents:";
            for(auto it = container.rbegin(); it != container.rend(); ++it)
            {
                std::cout << " " << *it;
            }
        }
        std::cout << "\n\n";
    }

    void test_special(const std::string & id,
                      JeodContainerType & jeod_container1,
                      const JeodContainerType & jeod_container2)
    {
        (void)id;
        (void)jeod_container1;
        (void)jeod_container2;
    }

    void perform_test(const std::string & id, const ElemType & item1, const ElemType & item2, const ElemType & item3)
    {
        typename JeodContainerType::iterator beg_iter, end_iter;
        STLContainerType stl_container1;
        stl_container1.insert(stl_container1.end(), item1);
        stl_container1.insert(stl_container1.end(), item2);
        stl_container1.insert(stl_container1.end(), item3);

        JeodContainerType jeod_container1;
        summarize_contents(jeod_container1, id + " jeod_container1");
        std::cout << "Expect: Empty container\n";
        examine_contents(jeod_container1, id + " jeod_container1");

        print_expected(item1, item2, item3);
        JeodContainerType jeod_container2(stl_container1);
        examine_contents(jeod_container2, id + " jeod_container2");

        print_expected(item1, item2, item3);
        JeodContainerType jeod_container3(jeod_container2);
        examine_contents(jeod_container3, id + " jeod_container3", true);

        std::cout << "Erase remove first, last elements of jeod_container3\n";
        jeod_container3.erase(jeod_container3.begin());
        beg_iter = jeod_container3.begin();
        end_iter = jeod_container3.end();
        jeod_container3.erase(++beg_iter, end_iter);
        print_expected(item2);
        examine_contents(jeod_container3, id + " jeod_container3");

        std::cout << "Insert first, last elements of jeod_container3\n";
        jeod_container3.insert(jeod_container3.begin(), item1);
        beg_iter = jeod_container2.begin();
        ++beg_iter;
        ++beg_iter;
        jeod_container3.insert(jeod_container3.end(), *beg_iter);
        print_expected(item1, item2, item3);
        examine_contents(jeod_container3, id + " jeod_container3");

        std::cout << "Swapping jeod_container2 and jeod_container1\n";
        jeod_container2.swap_contents(jeod_container1);
        print_expected(item1, item2, item3);
        examine_contents(jeod_container1, id + " jeod_container1");
        std::cout << "Expect: Empty container\n";
        examine_contents(jeod_container2, id + " jeod_container2");

        std::cout << "Swapping jeod_container2 and stl_container1\n";
        jeod_container2.swap_contents(stl_container1);
        print_expected(item1, item2, item3);
        examine_contents(jeod_container2, id + " jeod_container2");
        std::cout << "Expect: Empty container\n";
        examine_contents(stl_container1, "stl_container1");

        std::cout << "Swapping back\n";
        stl_container1.swap(jeod_container2);
        std::cout << "Expect: Empty container\n";
        examine_contents(jeod_container2, id + " jeod_container2");
        print_expected(item1, item2, item3);
        examine_contents(stl_container1, "stl_container1");

        std::cout << "Swapping back again\n";
        static_cast<STLContainerType &>(jeod_container2).swap(stl_container1);
        print_expected(item1, item2, item3);
        examine_contents(jeod_container2, id + " jeod_container2");
        std::cout << "Expect: Empty container\n";
        examine_contents(stl_container1, "stl_container1");

        std::cout << "Clearing jeod_container1\n";
        jeod_container1.clear();
        std::cout << "Expect: Empty container\n";
        examine_contents(jeod_container1, id + " jeod_container1");

        std::cout << "Assignment jeod_container1=stl_container1\n";
        jeod_container1 = stl_container1;
        std::cout << "Expect: Empty container\n";
        examine_contents(jeod_container1, id + " jeod_container1");

        std::cout << "Assignment jeod_container1=jeod_container2\n";
        jeod_container1 = jeod_container2;
        print_expected(item1, item2, item3);
        examine_contents(jeod_container1, id + " jeod_container1");

        std::cout << "Assignment jeod_container1=stl_container1\n";
        jeod_container1 = stl_container1;
        std::cout << "Expect: Empty container\n";
        examine_contents(jeod_container1, id + " jeod_container1");

        std::cout << "Forward copy jeod_container2 to jeod_container1\n";
        forward_copy(jeod_container2, jeod_container1);
        print_expected(item1, item2, item3);
        examine_contents(jeod_container1, id + " jeod_container1");

        std::cout << "Reverse copy jeod_container2 to jeod_container1\n";
        reverse_copy(jeod_container2, jeod_container1);
        print_expected(item3, item2, item1);
        examine_contents(jeod_container1, id + " jeod_container1");

        this->test_special(id, jeod_container1, jeod_container2);
    }
};

template<typename JeodContainerType, typename STLContainerType, typename ElemType>
void test_sequence_container(const TestContainer<JeodContainerType, STLContainerType, ElemType> & examiner,
                             const std::string & id,
                             JeodContainerType & jeod_container1,
                             const JeodContainerType & jeod_container2,
                             JeodContainerType & jeod_container4)
{
    ElemType four(4);
    typename JeodContainerType::const_iterator beg_citer, end_citer;
    typename JeodContainerType::iterator beg_iter, end_iter;

    std::cout << "Testing front, back\n";
    std::cout << "Expect 3, 1, 1, 3\n";
    std::cout << "jeod_container1.front()=" << jeod_container1.front() << "\n";
    std::cout << "jeod_container2.front()=" << jeod_container2.front() << "\n";
    std::cout << "jeod_container1.back()=" << jeod_container1.back() << "\n";
    std::cout << "jeod_container2.back()=" << jeod_container2.back() << "\n";
    std::cout << "\n";

    std::cout << "Assigning elements of jeod_container2 to jeod_container4\n";
    beg_citer = jeod_container2.begin();
    end_citer = jeod_container2.end();
    jeod_container4.assign(++beg_citer, end_citer);
    std::cout << "Expect: contents = 1 2 3\n";
    examiner.examine_contents(jeod_container2, id + " jeod_container2", true);
    std::cout << "Expect: contents = 2 3\n";
    examiner.examine_contents(jeod_container4, id + " jeod_container4");

    std::cout << "Assigning four fours to jeod_container4\n";
    jeod_container4.assign(4, four);
    std::cout << "Expect: contents = 4 4 4 4\n";
    examiner.examine_contents(jeod_container4, id + " jeod_container4");

    std::cout << "Making that 1 2 3 4\n";
    beg_iter = jeod_container4.begin();
    *beg_iter = 1;
    ++beg_iter;
    *beg_iter = 2;
    ++beg_iter;
    *beg_iter = 3;
    std::cout << "Expect: contents = 1 2 3 4\n";
    examiner.examine_contents(jeod_container4, id + " jeod_container4");

    std::cout << "Erasing last element\n";
    ++beg_iter;
    jeod_container4.erase(beg_iter);
    std::cout << "Expect: contents = 1 2 3\n";
    examiner.examine_contents(jeod_container4, id + " jeod_container4");

    std::cout << "Erasing first element\n";
    beg_iter = jeod_container4.begin();
    end_iter = beg_iter;
    jeod_container4.erase(beg_iter, ++end_iter);
    std::cout << "Expect: contents = 2 3\n";
    examiner.examine_contents(jeod_container4, id + " jeod_container4");

    std::cout << "Inserting elements from jeod_container1 in jeod_container4\n";
    beg_iter = jeod_container1.begin();
    end_iter = jeod_container1.end();
    jeod_container4.insert(jeod_container4.begin(), beg_iter, end_iter);
    std::cout << "Expect: contents = 3 2 1 2 3\n";
    examiner.examine_contents(jeod_container4, id + " jeod_container4");

    std::cout << "Adding 4 to the end of jeod_container4 via resize\n";
    jeod_container4.resize(6, 4);
    std::cout << "Expect: contents = 3 2 1 2 3 4\n";
    examiner.examine_contents(jeod_container4, id + " jeod_container4");

    std::cout << "Adding 5 to the end of jeod_container4 via push_back\n";
    jeod_container4.push_back(5);
    std::cout << "Expect: contents = 3 2 1 2 3 4 5\n";
    examiner.examine_contents(jeod_container4, id + " jeod_container4");

    std::cout << "Inserting four fours at the end of jeod_container4\n";
    jeod_container4.insert(jeod_container4.end(), 4, four);
    std::cout << "Expect: contents = 3 2 1 2 3 4 5 4 4 4 4\n";
    examiner.examine_contents(jeod_container4, id + " jeod_container4");

    std::cout << "Removing last elements via pop_back, resize\n";
    jeod_container4.pop_back();
    jeod_container4.resize(6);
    std::cout << "Expect: contents = 3 2 1 2 3 4\n";
    examiner.examine_contents(jeod_container4, id + " jeod_container4");
}

template<typename JeodContainerType, typename STLContainerType, typename ElemType>
void test_associative_container(const TestContainer<JeodContainerType, STLContainerType, ElemType> & examiner,
                                const std::string & id,
                                JeodContainerType & jeod_container1,
                                const JeodContainerType & jeod_container2,
                                JeodContainerType & jeod_container4)
{
    typename JeodContainerType::key_compare key_comp = jeod_container1.key_comp();
    typename JeodContainerType::value_compare value_comp = jeod_container1.value_comp();
    typename JeodContainerType::const_iterator citer;
    typename JeodContainerType::iterator iter;
    std::pair<typename JeodContainerType::iterator, typename JeodContainerType::iterator> range;
    std::pair<typename JeodContainerType::const_iterator, typename JeodContainerType::const_iterator> crange;
    int count;

    examiner.examine_contents(jeod_container1, id + " jeod_container1");
    std::cout << "jeod_container1.count(2)=" << jeod_container1.count(2) << "\n";
    count = 0;
    iter = jeod_container1.begin();
    while((iter != jeod_container1.end()) && (key_comp(*iter, 2)))
    {
        ++count;
        ++iter;
    }
    std::cout << "Number of elements smaller than 2 via key_comp=" << count << "\n";
    count = 0;
    iter = jeod_container1.begin();
    while((iter != jeod_container1.end()) && (value_comp(*iter, 2)))
    {
        ++count;
        ++iter;
    }
    std::cout << "Number of elements smaller than 2 via value_comp=" << count << "\n";

    std::cout << "Looking for 2 in jeod_container1\n";
    iter = jeod_container1.find(2);
    std::cout << "find(2): Found=" << (iter != jeod_container1.end()) << "\n";
    iter = jeod_container1.lower_bound(2);
    std::cout << "lower_bound(2): Found=" << (iter != jeod_container1.end()) << "\n";
    iter = jeod_container1.upper_bound(2);
    std::cout << "upper_bound(2): Found=" << (iter != jeod_container1.end()) << "\n";
    std::cout << "Looking for 42 in jeod_container1\n";
    range = jeod_container1.equal_range(2);
    std::cout << "equal_range(2): Found=" << (range.first != jeod_container1.end()) << "\n";
    iter = jeod_container1.find(42);
    std::cout << "find(42): Found=" << (iter != jeod_container1.end()) << "\n";
    std::cout << "\n";

    std::cout << "Looking for 2 in jeod_container2\n";
    iter = jeod_container2.find(2);
    std::cout << "find(2): Found=" << (iter != jeod_container2.end()) << "\n";
    iter = jeod_container2.lower_bound(2);
    std::cout << "lower_bound(2): Found=" << (iter != jeod_container2.end()) << "\n";
    iter = jeod_container2.upper_bound(2);
    std::cout << "upper_bound(2): Found=" << (iter != jeod_container2.end()) << "\n";
    crange = jeod_container2.equal_range(2);
    std::cout << "equal_range(2): Found=" << (crange.first != jeod_container2.end()) << "\n";
    std::cout << "Looking for 42 in jeod_container2\n";
    iter = jeod_container2.find(42);
    std::cout << "find(42): Found=" << (iter != jeod_container2.end()) << "\n";
    std::cout << "\n";

    examiner.examine_contents(jeod_container4, id + " jeod_container4");
    std::cout << "Erasing 7, 5,6, 4 from jeod_container4\n";
    iter = jeod_container4.find(7);
    jeod_container4.erase(iter);
    iter = jeod_container4.end();
    --iter;
    --iter;
    jeod_container4.erase(iter, jeod_container4.end());
    jeod_container4.erase(4);
    examiner.examine_contents(jeod_container4, id + " jeod_container4");
}

template<typename JeodContainerType, typename STLContainerType, typename ElemType>
void test_list(const TestContainer<JeodContainerType, STLContainerType, ElemType> & examiner,
               const std::string & id,
               JeodContainerType & jeod_container1,
               const JeodContainerType & jeod_container2,
               JeodContainerType & jeod_container4)
{
    typename JeodContainerType::const_iterator beg_citer, end_citer;
    typename JeodContainerType::iterator beg_iter, end_iter;

    test_sequence_container(examiner, id, jeod_container1, jeod_container2, jeod_container4);

    JeodContainerType jeod_container1_save(jeod_container1);
    JeodContainerType jeod_container4_save(jeod_container4);

    std::cout << "Sorting jeod_container1, jeod_container4\n";
    jeod_container1.sort();
    jeod_container4.sort();
    std::cout << "Expect: contents = 1 2 3\n";
    examiner.examine_contents(jeod_container1, id + " jeod_container1");
    std::cout << "Expect: contents = 1 2 2 3 3 4\n";
    examiner.examine_contents(jeod_container4, id + " jeod_container4");

    std::cout << "Merging jeod_container4 into jeod_container1\n";
    jeod_container1.merge(jeod_container4);
    std::cout << "Expect: contents = 1 1 2 2 2 3 3 3 4\n";
    examiner.examine_contents(jeod_container1, id + " jeod_container1");
    std::cout << "Expect: Empty container\n";
    examiner.examine_contents(jeod_container4, id + " jeod_container4");

    std::cout << "Removing non-unique members from jeod_container1\n";
    jeod_container1.unique();
    std::cout << "Expect: contents = 1 2 3 4\n";
    examiner.examine_contents(jeod_container1, id + " jeod_container1");

    std::cout << "Restore jeod_container1, 4 and sort in descending order\n";
    jeod_container1 = jeod_container1_save;
    jeod_container1.sort(std::greater<Foo>());
    std::cout << "Expect: contents = 3 2 1\n";
    examiner.examine_contents(jeod_container1, id + " jeod_container1");
    jeod_container4 = jeod_container4_save;
    jeod_container4.sort(std::greater<Foo>());
    std::cout << "Expect: contents = 4 3 3 2 2 1\n";
    examiner.examine_contents(jeod_container4, id + " jeod_container4");

    std::cout << "Merging jeod_container4 into jeod_container1 (>)\n";
    jeod_container1.merge(jeod_container4, std::greater<Foo>());
    std::cout << "Expect: contents = 4 3 3 3 2 2 2 1 1\n";
    examiner.examine_contents(jeod_container1, id + " jeod_container1");
    std::cout << "Expect: Empty container\n";
    examiner.examine_contents(jeod_container4, id + " jeod_container4");

    std::cout << "Removing nearby members from jeod_container1\n";
    jeod_container1.unique(WithinOne<Foo>());
    std::cout << "Expect: contents = 4 2\n";
    examiner.examine_contents(jeod_container1, id + " jeod_container1");

    std::cout << "Restore jeod_container1, jeod_container4\n";
    jeod_container1 = jeod_container1_save;
    jeod_container4 = jeod_container4_save;
    std::cout << "Expect: contents = 3 2 1\n";
    examiner.examine_contents(jeod_container1, id + " jeod_container1");
    std::cout << "Expect: contents = 3 2 1 2 3 4\n";
    examiner.examine_contents(jeod_container4, id + " jeod_container4");

    std::cout << "Splice jeod_container4 into jeod_container1 after begin\n";
    beg_iter = jeod_container1.begin();
    ++beg_iter;
    jeod_container1.splice(beg_iter, jeod_container4);
    std::cout << "Expect: contents = 3 3 2 1 2 3 4 2 1\n";
    examiner.examine_contents(jeod_container1, id + " jeod_container1");
    std::cout << "Expect: Empty container\n";
    examiner.examine_contents(jeod_container4, id + " jeod_container4");

    std::cout << "Restore jeod_container1, jeod_container4 as above\n";
    jeod_container1 = jeod_container1_save;
    jeod_container4 = jeod_container4_save;
    std::cout << "Splice all but first elem of jeod_container4 into "
              << "jeod_container1 before begin\n";
    beg_iter = jeod_container4.begin();
    end_iter = jeod_container4.end();
    ++beg_iter;
    jeod_container1.splice(jeod_container1.begin(), jeod_container4, beg_iter, end_iter);
    std::cout << "Expect: contents = 2 1 2 3 4 3 2 1\n";
    examiner.examine_contents(jeod_container1, id + " jeod_container1");
    std::cout << "Expect: contents = 3\n";
    examiner.examine_contents(jeod_container4, id + " jeod_container4");

    std::cout << "Restore jeod_container1, jeod_container4 as above\n";
    jeod_container1 = jeod_container1_save;
    jeod_container4 = jeod_container4_save;
    std::cout << "Splice 2nd elem in jeod_container4 into "
              << "jeod_container1 at end\n";
    beg_iter = jeod_container4.begin();
    ++beg_iter;
    jeod_container1.splice(jeod_container1.end(), jeod_container4, beg_iter);
    std::cout << "Expect: contents = 3 2 1 2\n";
    examiner.examine_contents(jeod_container1, id + " jeod_container1");
    std::cout << "Expect: contents = 3 1 2 3 4\n";
    examiner.examine_contents(jeod_container4, id + " jeod_container4");

    std::cout << "Replacing init elem of jeod_container4 with zero\n";
    jeod_container4.pop_front();
    jeod_container4.push_front(0);
    std::cout << "Expect: contents = 0 1 2 3 4\n";
    examiner.examine_contents(jeod_container4, id + " jeod_container4");

    std::cout << "Removing items near 2 from jeod_container4\n";
    jeod_container4.remove_if(WithinOneOf<Foo, 2>());
    std::cout << "Expect: contents = 0 4\n";
    examiner.examine_contents(jeod_container4, id + " jeod_container4");

    std::cout << "Reversing jeod_container4\n";
    jeod_container4.reverse();
    std::cout << "Expect: contents = 4 0\n";
    examiner.examine_contents(jeod_container4, id + " jeod_container4");

    std::cout << "Removing threes from jeod_container1\n";
    std::cout << "Expect: contents = 3 2 1 2\n";
    examiner.examine_contents(jeod_container1, id + " jeod_container1");
    jeod_container1.remove(3);
    std::cout << "Expect: contents = 2 1 2\n";
    examiner.examine_contents(jeod_container1, id + " jeod_container1");
}

template<typename JeodContainerType, typename STLContainerType, typename ElemType>
void test_vector(const TestContainer<JeodContainerType, STLContainerType, ElemType> & examiner,
                 const std::string & id,
                 JeodContainerType & jeod_container1,
                 const JeodContainerType & jeod_container2,
                 JeodContainerType & jeod_container4)
{
    typename JeodContainerType::const_iterator beg_citer, end_citer;
    typename JeodContainerType::iterator beg_iter, end_iter;

    test_sequence_container(examiner, id, jeod_container1, jeod_container2, jeod_container4);

    JeodContainerType jeod_container1_save(jeod_container1);
    JeodContainerType jeod_container4_save(jeod_container4);

    std::cout << "jeod_container1.capacity()=" << jeod_container1.capacity() << "\n";
    std::cout << "Reserving twice that amount:\n";
    jeod_container1.reserve(jeod_container1.capacity() * 2);
    std::cout << "jeod_container1.capacity()=" << jeod_container1.capacity() << "\n";
    examiner.examine_contents(jeod_container1, id + " jeod_container1");

    std::cout << "jeod_container1[2]=" << jeod_container1[2] << "\n";
    std::cout << "Setting to 0\n";
    jeod_container1[2] = 0;
    std::cout << "jeod_container1[2]=" << jeod_container1[2] << "\n";
    std::cout << "jeod_container1.at(1)=" << jeod_container1.at(1) << "\n";
    std::cout << "Setting to 0\n";
    jeod_container1.at(1) = 0;
    std::cout << "jeod_container1.at(1)=" << jeod_container1.at(1) << "\n";
    std::cout << "\n";

    examiner.examine_contents(jeod_container2, id + " jeod_container2");
    std::cout << "jeod_container2[0]=" << jeod_container2[0] << "\n";
    std::cout << "jeod_container2.at(1)=" << jeod_container2.at(1) << "\n";

    std::cout << "\n";
}

template<typename JeodContainerType, typename STLContainerType, typename ElemType>
void test_set(const TestContainer<JeodContainerType, STLContainerType, ElemType> & examiner,
              const std::string & id,
              JeodContainerType & jeod_container1,
              const JeodContainerType & jeod_container2,
              JeodContainerType & jeod_container4)
{
    typename JeodContainerType::const_iterator beg_citer, end_citer;
    typename JeodContainerType::iterator beg_iter, end_iter;
    int four_to_six[] = {4, 5, 6};

    std::cout << "Adding 4,5,6, 7 to jeod_container4 via insert:\n";
    jeod_container4.insert(four_to_six, four_to_six + 3);
    jeod_container4.insert(7);
    examiner.examine_contents(jeod_container4, id + " jeod_container4");

    test_associative_container(examiner, id, jeod_container1, jeod_container2, jeod_container4);
}

template<>
void TestContainer<JeodPrimitiveVector<double>::type, std::vector<double>, double>::test_special(
    const std::string & id,
    JeodPrimitiveVector<double>::type & jeod_container1,
    const JeodPrimitiveVector<double>::type & jeod_container2)
{
    JeodPrimitiveVector<double>::type jeod_container4;
    test_vector(*this, id, jeod_container1, jeod_container2, jeod_container4);
}

template<>
void TestContainer<JeodPrimitiveList<double>::type, std::list<double>, double>::test_special(
    const std::string & id,
    JeodPrimitiveList<double>::type & jeod_container1,
    const JeodPrimitiveList<double>::type & jeod_container2)
{
    JeodPrimitiveList<double>::type jeod_container4;
    test_list(*this, id, jeod_container1, jeod_container2, jeod_container4);
}

template<>
void TestContainer<JeodObjectList<Foo>::type, std::list<Foo>, Foo>::test_special(
    const std::string & id,
    JeodObjectList<Foo>::type & jeod_container1,
    const JeodObjectList<Foo>::type & jeod_container2)
{
    JeodObjectList<Foo>::type jeod_container4;
    test_list(*this, id, jeod_container1, jeod_container2, jeod_container4);
}

template<>
void TestContainer<JeodObjectSet<Foo>::type, std::set<Foo>, Foo>::test_special(
    const std::string & id,
    JeodObjectSet<Foo>::type & jeod_container1,
    const JeodObjectSet<Foo>::type & jeod_container2)
{
    JeodObjectSet<Foo>::type jeod_container4;
    test_set(*this, id, jeod_container1, jeod_container2, jeod_container4);
}

template<typename JeodContainerType, typename STLContainerType, typename ElemType>
void test_container(const std::string & id, const ElemType & item1, const ElemType & item2, const ElemType & item3)
{
    TestContainer<JeodContainerType, STLContainerType, ElemType> test_driver;
    test_driver.perform_test(id, item1, item2, item3);
}

int main()
{
    Foo foo1(1);
    Foo foo2(2);
    Foo foo3(3);

    test_container<JeodPrimitiveList<double>::type, std::list<double>, double>("JeodPrimitiveList<double>::type",
                                                                               1,
                                                                               2,
                                                                               3);

    test_container<JeodPrimitiveList<std::string>::type, std::list<std::string>, std::string>(
        "JeodPrimitiveList<std::string>::type", "a", "b", "c");

    test_container<JeodObjectList<Foo>::type, std::list<Foo>, Foo>("JeodObjectList<Foo>::type", foo1, foo2, foo3);

    test_container<JeodPointerList<Foo>::type, std::list<Foo *>, Foo *>("JeodPointerList<Foo*>::type",
                                                                        &foo1,
                                                                        &foo2,
                                                                        &foo3);

    test_container<JeodPrimitiveVector<bool>::type, std::vector<bool>, bool>("JeodPrimitiveVector<bool>::type",
                                                                             false,
                                                                             false,
                                                                             true);

    test_container<JeodPrimitiveVector<double>::type, std::vector<double>, double>("JeodPrimitiveVector<double>::type",
                                                                                   1,
                                                                                   2,
                                                                                   3);

    test_container<JeodPrimitiveVector<std::string>::type, std::vector<std::string>, std::string>(
        "JeodPrimitiveVector<std::string>::type", "a", "b", "c");

    test_container<JeodObjectVector<Foo>::type, std::vector<Foo>, Foo>("JeodObjectVector<Foo>::type", foo1, foo2, foo3);

    test_container<JeodPointerVector<Foo>::type, std::vector<Foo *>, Foo *>("JeodPointerVector<Foo*>::type",
                                                                            &foo1,
                                                                            &foo2,
                                                                            &foo3);

    test_container<JeodPrimitiveSet<double>::type, std::set<double>, double>("JeodPrimitiveSet<double>::type", 1, 2, 3);

    test_container<JeodPrimitiveSet<std::string>::type, std::set<std::string>, std::string>(
        "JeodPrimitiveSet<std::string>::type", "a", "b", "c");

    test_container<JeodObjectSet<Foo>::type, std::set<Foo>, Foo>("JeodObjectSet<Foo>::type", foo1, foo2, foo3);

    test_container<JeodPointerSet<Foo>::type, std::set<Foo *>, Foo *>("JeodPointerSet<Foo*>::type",
                                                                      &foo1,
                                                                      &foo2,
                                                                      &foo3);
}
