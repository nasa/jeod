/**
 * @addtogroup Models
 * @{
 * @addtogroup Utils
 * @{
 * @addtogroup ModelTemplate
 * @{
 *
 * @file models/utils/model_template/src/foo.cc
 * Implement class Foo.
 */

/*******************************************************************************

Purpose:
  ()


*******************************************************************************/

// System includes
#include <iostream>

// JEOD includes
#include "utils/message/include/message_handler.hh"

// Model includes
#include "../include/foo.hh"
#include "../include/template_messages.hh"

//! Namespace jeod
namespace jeod
{

/**
 * Foo default constructor.
 */
Foo::Foo()
{
    std::cout << "Constructed Foo " << this << std::endl;
}

/**
 * Foo destructor.
 */
Foo::~Foo()
{
    std::cout << "Destructed Foo " << this << std::endl;
}

/**
 * Setter for Foo::num.
 */
void Foo::set_num(int val)
{
    if(num < -42)
    {
        MessageHandler::error(__FILE__,
                              __LINE__,
                              TemplateMessages::big_negnum,
                              "Number %d is invalid (less than -42).",
                              val);
    }
    else if(num == 0)
    {
        MessageHandler::error(__FILE__, __LINE__, TemplateMessages::zero, "Number is invalid (zero is not allowed).");
    }
    else if(num > 42)
    {
        MessageHandler::error(__FILE__,
                              __LINE__,
                              TemplateMessages::big_negnum,
                              "Number %d is invalid (greater than 42).",
                              val);
    }
    else
    {
        num = val;
    }
}

/**
 * Getter for Foo::num.
 */
int Foo::get_num()
{
    return num;
}

} // namespace jeod

/**
 * @}
 * @}
 * @}
 */
