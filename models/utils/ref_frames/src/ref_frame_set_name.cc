/**
 * @addtogroup Models
 * @{
 * @addtogroup Utils
 * @{
 * @addtogroup RefFrames
 * @{
 *
 * @file models/utils/ref_frames/src/ref_frame_set_name.cc
 * Define the RefFrame::set_name methods.
 */

/*******************************************************************************
  Purpose:
    ()

  LIBRARY DEPENDENCY:
    ((ref_frame_set_name.cc)
     (utils/sim_interface/src/memory_interface.cc)
     (utils/named_item/src/named_item.cc))

   

*******************************************************************************/


// Model includes
#include "../include/ref_frame.hh"

// JEOD includes
#include "utils/named_item/include/named_item.hh"


//! Namespace jeod
namespace jeod {

/**
 * Create a copy of the provided name.
 * \param[in] name_item1 First part of the name
 */
void
RefFrame::set_name (
   const char * name_item1)
{
   name = NamedItem::construct_name_string (name_item1);
}


/**
 * Construct a name as a dot-conjoined string.
 * \param[in] name_item1 First part of the name
 * \param[in] name_item2 Second part of the name
 */
void
RefFrame::set_name (
   const char * name_item1,
   const char * name_item2)
{
   name = NamedItem::construct_name_string (name_item1,
                                            name_item2);
}


/**
 * Construct a name as a dot-conjoined string.
 * \param[in] name_item1 First part of the name
 * \param[in] name_item2 Second part of the name
 * \param[in] name_item3 Third part of the name
 */
void
RefFrame::set_name (
   const char * name_item1,
   const char * name_item2,
   const char * name_item3)
{
   // FIXME: should use variadic function of construct_name_string()
   set_name (name_item1, name_item2);

   name = NamedItem::construct_name_string (name.c_str(),
                                            name_item3);
}


/**
 * Construct a name as a dot-conjoined string.
 * \param[in] name_item1 First part of the name
 * \param[in] name_item2 Second part of the name
 * \param[in] name_item3 Third part of the name
 * \param[in] name_item4 Fourth part of the name
 */
void
RefFrame::set_name (
   const char * name_item1,
   const char * name_item2,
   const char * name_item3,
   const char * name_item4)
{
   // FIXME: should use variadic function of construct_name_string()
   set_name (name_item1, name_item2, name_item3);

   name = NamedItem::construct_name_string (name.c_str(),
                                            name_item4);
}


/**
 * Construct a name as a dot-conjoined string.
 * \param[in] name_item1 First part of the name
 * \param[in] name_item2 Second part of the name
 * \param[in] name_item3 Third part of the name
 * \param[in] name_item4 Fourth part of the name
 * \param[in] name_item5 Fifth part of the name
 */
void
RefFrame::set_name (
   const char * name_item1,
   const char * name_item2,
   const char * name_item3,
   const char * name_item4,
   const char * name_item5)
{
   // FIXME: should use variadic function of construct_name_string()
   set_name (name_item1, name_item2, name_item3, name_item4);

   name = NamedItem::construct_name_string (name.c_str(),
                                            name_item5);
}


/**
 * Construct a name as a dot-conjoined string.
 * \param[in] name_item1 First part of the name
 * \param[in] name_item2 Second part of the name
 * \param[in] name_item3 Third part of the name
 * \param[in] name_item4 Fourth part of the name
 * \param[in] name_item5 Fifth part of the name
 * \param[in] name_item6 Sixth part of the name
 */
void
RefFrame::set_name (
   const char * name_item1,
   const char * name_item2,
   const char * name_item3,
   const char * name_item4,
   const char * name_item5,
   const char * name_item6)
{
   // FIXME: should use variadic function of construct_name_string()
   set_name (name_item1, name_item2, name_item3, name_item4, name_item5);

   name = NamedItem::construct_name_string (name.c_str(),
                                            name_item6);
}


/**
 * Construct a name as a dot-conjoined string.
 * \param[in] name_item1 First part of the name
 * \param[in] name_item2 Second part of the name
 * \param[in] name_item3 Third part of the name
 * \param[in] name_item4 Fourth part of the name
 * \param[in] name_item5 Fifth part of the name
 * \param[in] name_item6 Sixth part of the name
 * \param[in] name_item7 Seventh part of the name
 */
void
RefFrame::set_name (
   const char * name_item1,
   const char * name_item2,
   const char * name_item3,
   const char * name_item4,
   const char * name_item5,
   const char * name_item6,
   const char * name_item7)
{
   // FIXME: should use variadic function of construct_name_string()
   set_name (name_item1, name_item2, name_item3, name_item4, name_item5, name_item6);
   name = NamedItem::construct_name_string (name.c_str(),
                                            name_item7);
}


} // End JEOD namespace

/**
 * @}
 * @}
 * @}
 */
