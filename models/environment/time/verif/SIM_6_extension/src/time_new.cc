/******************************************************************************
PURPOSE:
  (Define member functions for class Time_New.)

REFERENCE:
  (None)

ASSUMPTIONS AND LIMITATIONS:
  (None)

LIBRARY DEPENDENCY:
  (time_new.cc)


******************************************************************************/

/* System Includes */
#include <cstdio>
#include <cstring>

/* JEOD Includes */
#include "utils/named_item/include/named_item.hh"

/* Model Includes */
#include "../include/time_new.hh"

//! Namespace jeod
namespace jeod
{

/******************************************************************************
 Function: Time_New::Time_New
 Purpose: (Construct a Time_New)
 Class:   (N/A)
******************************************************************************/
TimeNew::TimeNew()
{
    name = "New";
}

/****************************************************************************/
} // namespace jeod
