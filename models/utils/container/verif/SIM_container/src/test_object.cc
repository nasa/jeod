/*******************************************************************************
Purpose:
  (Test container model checkpoint and restart capabilities.)
*******************************************************************************/

#include <iostream>
#include <limits>
#include <string>

#include "../include/test_object.hh"
#include "utils/memory/include/jeod_alloc.hh"

//! Namespace jeod
namespace jeod
{

std::ostream & operator<<(std::ostream & out, const TestObject & obj)
{
    return (out << "ival=" << obj.ival << " dval=" << obj.dval);
}

bool operator<(const TestObject & x, const TestObject & y)
{
    return (x.ival < y.ival);
}

template<typename Type> std::string test_container_serialize(Type & val)
{
    switch(std::fpclassify(val))
    {
        case FP_NAN:
            return "NaN";

        case FP_INFINITE:
            if(std::signbit(val) == 0)
            {
                return "Inf";
            }
            else
            {
                return "-Inf";
            }

        default:
        {
            std::ostringstream obuf;
            obuf.precision(std::numeric_limits<double>::digits10 + 2);
            obuf << val;
            return obuf.str();
        }
    }
}

template<typename ElemType> struct PrintItem
{
    std::ostream & print(std::ostream & out, const ElemType & elem)
    {
        return (out << elem);
    }
};

template<> struct PrintItem<float>
{
    std::ostream & print(std::ostream & out, const float & elem)
    {
        return (out << test_container_serialize(elem));
    }
};

template<> struct PrintItem<double>
{
    std::ostream & print(std::ostream & out, const double & elem)
    {
        return (out << test_container_serialize(elem));
    }
};

template<typename ContainerType, typename ElemType> struct PrintContainer
{
    void print(const std::string & id, const ContainerType & container)
    {
        std::cout << "Container " << id << " contents:\n";
        for(auto it = container.begin(); it != container.end(); ++it)
        {
            const ElemType & elem = *it;
            (item_printer.print((std::cout << "Q("), elem)) << ")\n";
        }
        std::cout << "\n";
    }

    PrintItem<ElemType> item_printer;
};

template<typename ContainerType, typename ElemType>
inline void print_container(const std::string & id, const ContainerType & container)
{
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
    PrintContainer<ContainerType, ElemType> printer{};
    printer.print(id, container);
}

/**
Constructor
*/
TestContainer::TestContainer()
    : obj1(1),
      obj2(2),
      obj3(3)
{
    JEOD_REGISTER_CLASS(TestContainer);
    JEOD_REGISTER_CLASS(TestObject);
    JEOD_REGISTER_CHECKPOINTABLE(this, object_list);
    JEOD_REGISTER_CHECKPOINTABLE(this, object_vec);
    JEOD_REGISTER_CHECKPOINTABLE(this, object_set);

    JEOD_REGISTER_CHECKPOINTABLE(this, pointer_list);
    JEOD_REGISTER_CHECKPOINTABLE(this, pointer_vec);
    JEOD_REGISTER_CHECKPOINTABLE(this, pointer_set);

    JEOD_REGISTER_CHECKPOINTABLE(this, double_list);
    JEOD_REGISTER_CHECKPOINTABLE(this, double_vec);
    JEOD_REGISTER_CHECKPOINTABLE(this, double_set);

    JEOD_REGISTER_CHECKPOINTABLE(this, float_list);
    JEOD_REGISTER_CHECKPOINTABLE(this, float_vec);
    JEOD_REGISTER_CHECKPOINTABLE(this, float_set);

    JEOD_REGISTER_CHECKPOINTABLE(this, int_list);
    JEOD_REGISTER_CHECKPOINTABLE(this, int_vec);
    JEOD_REGISTER_CHECKPOINTABLE(this, int_set);

    JEOD_REGISTER_CHECKPOINTABLE(this, bool_list);
    JEOD_REGISTER_CHECKPOINTABLE(this, bool_vec);
    JEOD_REGISTER_CHECKPOINTABLE(this, bool_set);

    JEOD_REGISTER_CHECKPOINTABLE(this, string_list);
    JEOD_REGISTER_CHECKPOINTABLE(this, string_vec);
    JEOD_REGISTER_CHECKPOINTABLE(this, string_set);

    JEOD_REGISTER_CHECKPOINTABLE(this, simple);
}

/**
Destructor
*/
TestContainer::~TestContainer()
{
    JEOD_DEREGISTER_CHECKPOINTABLE(this, object_list);
    JEOD_DEREGISTER_CHECKPOINTABLE(this, object_vec);
    JEOD_DEREGISTER_CHECKPOINTABLE(this, object_set);

    JEOD_DEREGISTER_CHECKPOINTABLE(this, pointer_list);
    JEOD_DEREGISTER_CHECKPOINTABLE(this, pointer_vec);
    JEOD_DEREGISTER_CHECKPOINTABLE(this, pointer_set);

    JEOD_DEREGISTER_CHECKPOINTABLE(this, double_list);
    JEOD_DEREGISTER_CHECKPOINTABLE(this, double_vec);
    JEOD_DEREGISTER_CHECKPOINTABLE(this, double_set);

    JEOD_DEREGISTER_CHECKPOINTABLE(this, float_list);
    JEOD_DEREGISTER_CHECKPOINTABLE(this, float_vec);
    JEOD_DEREGISTER_CHECKPOINTABLE(this, float_set);

    JEOD_DEREGISTER_CHECKPOINTABLE(this, int_list);
    JEOD_DEREGISTER_CHECKPOINTABLE(this, int_vec);
    JEOD_DEREGISTER_CHECKPOINTABLE(this, int_set);

    JEOD_DEREGISTER_CHECKPOINTABLE(this, bool_list);
    JEOD_DEREGISTER_CHECKPOINTABLE(this, bool_vec);
    JEOD_DEREGISTER_CHECKPOINTABLE(this, bool_set);

    JEOD_DEREGISTER_CHECKPOINTABLE(this, string_list);
    JEOD_DEREGISTER_CHECKPOINTABLE(this, string_vec);
    JEOD_DEREGISTER_CHECKPOINTABLE(this, string_set);

    JEOD_DEREGISTER_CHECKPOINTABLE(this, simple);
}

/**
Shutdown.
*/
void TestContainer::shutdown()
{
    print_container<JeodObjectList<TestObject>::type, TestObject>("object_list", object_list);
    print_container<JeodObjectVector<TestObject>::type, TestObject>("object_vec", object_vec);
    print_container<JeodObjectSet<TestObject>::type, TestObject>("object_set", object_set);

    print_container<JeodPointerList<TestObject>::type, TestObject *>("pointer_list", pointer_list);
    print_container<JeodPointerVector<TestObject>::type, TestObject *>("pointer_vec", pointer_vec);
    print_container<JeodPointerSet<TestObject>::type, TestObject *>("pointer_set", pointer_set);

    print_container<JeodPrimitiveList<double>::type, double>("double_list", double_list);
    print_container<JeodPrimitiveVector<double>::type, double>("double_vec", double_vec);
    print_container<JeodPrimitiveSet<double>::type, double>("double_set", double_set);

    print_container<JeodPrimitiveList<float>::type, float>("float_list", float_list);
    print_container<JeodPrimitiveVector<float>::type, float>("float_vec", float_vec);
    print_container<JeodPrimitiveSet<float>::type, float>("float_set", float_set);

    print_container<JeodPrimitiveList<int>::type, int>("int_list", int_list);
    print_container<JeodPrimitiveVector<int>::type, int>("int_vec", int_vec);
    print_container<JeodPrimitiveSet<int>::type, int>("int_set", int_set);

    print_container<JeodPrimitiveList<bool>::type, bool>("bool_list", bool_list);
    print_container<JeodPrimitiveVector<bool>::type, bool>("bool_vec", bool_vec);
    print_container<JeodPrimitiveSet<bool>::type, bool>("bool_set", bool_set);

    print_container<JeodPrimitiveList<std::string>::type, std::string>("string_list", string_list);
    print_container<JeodPrimitiveVector<std::string>::type, std::string>("string_vec", string_vec);
    print_container<JeodPrimitiveSet<std::string>::type, std::string>("string_set", string_set);

    simple.print_contents();
}

/**
Set non-default values.
*/
void TestContainer::test1()
{
    object_list.push_back(1);
    object_list.push_back(42);
    object_vec.push_back(2);
    object_set.insert(3);

    pointer_list.push_back(&obj1);
    pointer_list.push_back(&obj2);

    pointer_vec.push_back(&obj1);
    pointer_vec.push_back(&obj2);
    pointer_vec.push_back(&obj3);

    pointer_set.insert(&obj3);
    pointer_set.insert(&obj1);
    pointer_set.insert(&obj2);

    double_list.push_back(3.14159);
    double_list.push_back(std::numeric_limits<double>::signaling_NaN());
    double_list.push_back(-std::numeric_limits<double>::infinity());
    double_list.push_back(std::numeric_limits<double>::infinity());

    double_set.insert(3);
    double_set.insert(1);
    double_set.insert(2);

    float_list.push_back(std::numeric_limits<float>::infinity());
    float_list.push_back(-std::numeric_limits<float>::infinity());
    float_list.push_back(std::numeric_limits<float>::signaling_NaN());

    float_vec.push_back(4);

    float_set.insert(6);

    int_list.push_back(0);
    int_list.push_back(42);

    int_vec.push_back(42 * 42);

    int_set.insert(42 * 42 * 42);
    int_set.insert(42 * 42);
    int_set.insert(42);

    bool_list.push_back(false);
    bool_list.push_back(true);

    bool_vec.push_back(true);
    bool_vec.push_back(false);
    bool_vec.push_back(true);
    bool_vec[0] = false;
    bool_vec[1] = true;
    bool_vec[2] = false;

    bool_set.insert(false);
    bool_set.insert(true);

    string_list.push_back("foo");
    string_list.push_back("bar");
    string_list.push_back("baz");

    string_vec.push_back("\\Start with slash");
    string_vec.push_back("Slash\\in middle");
    string_vec.push_back("Slash at end\\");
    string_vec.push_back("\nStart with newline");
    string_vec.push_back("Newline\nin middle");
    string_vec.push_back("Newline at end\n");
    string_vec.push_back("\"Start with quote");
    string_vec.push_back("Quote\"in middle");
    string_vec.push_back("Quote at end\"");

    string_set.insert("bbb");
    string_set.insert("ccc");
    string_set.insert("aaa");

    simple.assign(42);
}
} // namespace jeod
