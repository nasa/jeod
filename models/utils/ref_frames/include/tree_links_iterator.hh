//=============================================================================
// Notices:
//
// Copyright © 2025 United States Government as represented by the Administrator
// of the National Aeronautics and Space Administration.  All Rights Reserved.
//
//
// Disclaimers:
//
// No Warranty: THE SUBJECT SOFTWARE IS PROVIDED "AS IS" WITHOUT ANY WARRANTY OF
// ANY KIND, EITHER EXPRESSED, IMPLIED, OR STATUTORY, INCLUDING, BUT NOT LIMITED
// TO, ANY WARRANTY THAT THE SUBJECT SOFTWARE WILL CONFORM TO SPECIFICATIONS, ANY
// IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, OR
// FREEDOM FROM INFRINGEMENT, ANY WARRANTY THAT THE SUBJECT SOFTWARE WILL BE ERROR
// FREE, OR ANY WARRANTY THAT DOCUMENTATION, IF PROVIDED, WILL CONFORM TO THE
// SUBJECT SOFTWARE. THIS AGREEMENT DOES NOT, IN ANY MANNER, CONSTITUTE AN
// ENDORSEMENT BY GOVERNMENT AGENCY OR ANY PRIOR RECIPIENT OF ANY RESULTS,
// RESULTING DESIGNS, HARDWARE, SOFTWARE PRODUCTS OR ANY OTHER APPLICATIONS
// RESULTING FROM USE OF THE SUBJECT SOFTWARE.  FURTHER, GOVERNMENT AGENCY
// DISCLAIMS ALL WARRANTIES AND LIABILITIES REGARDING THIRD-PARTY SOFTWARE,
// IF PRESENT IN THE ORIGINAL SOFTWARE, AND DISTRIBUTES IT "AS IS."
//
// Waiver and Indemnity:  RECIPIENT AGREES TO WAIVE ANY AND ALL CLAIMS AGAINST THE
// UNITED STATES GOVERNMENT, ITS CONTRACTORS AND SUBCONTRACTORS, AS WELL AS ANY
// PRIOR RECIPIENT.  IF RECIPIENT'S USE OF THE SUBJECT SOFTWARE RESULTS IN ANY
// LIABILITIES, DEMANDS, DAMAGES, EXPENSES OR LOSSES ARISING FROM SUCH USE,
// INCLUDING ANY DAMAGES FROM PRODUCTS BASED ON, OR RESULTING FROM, RECIPIENT'S
// USE OF THE SUBJECT SOFTWARE, RECIPIENT SHALL INDEMNIFY AND HOLD HARMLESS THE
// UNITED STATES GOVERNMENT, ITS CONTRACTORS AND SUBCONTRACTORS, AS WELL AS ANY
// PRIOR RECIPIENT, TO THE EXTENT PERMITTED BY LAW.  RECIPIENT'S SOLE REMEDY FOR
// ANY SUCH MATTER SHALL BE THE IMMEDIATE, UNILATERAL TERMINATION OF THIS
// AGREEMENT.
//
//=============================================================================
//
//
/**
 * @addtogroup Models
 * @{
 * @addtogroup Utils
 * @{
 * @addtogroup RefFrames
 * @{
 *
 * @file models/utils/ref_frames/include/tree_links_iterator.hh
 * Define the template TreeLinksRange and related templates, which are used to
 * iterate over trees.
 */

/*
Purpose: ()
*/

#ifndef JEOD_TREE_LINKS_ITERATOR_HH
#define JEOD_TREE_LINKS_ITERATOR_HH

// Model includes
#include "class_declarations.hh"

// JEOD includes
#include "utils/sim_interface/include/jeod_class.hh"

// System includes
#include <vector>

//! Namespace jeod
namespace jeod
{

/**
 * @file
 * The JEOD 4.0 version of the tree links iterators is motivated by the c++11
 * range-based for, which requires a range expression from which a begin
 * iterator and an end sentinel can be formed. One way the compiler can form the
 * begin iterator and end sentinel is to have the range expression be an object
 * that implements begin() and end() member functions. The loops that use
 * the JEOD 4.0 tree links iterators are of the form
 *
 *     for (auto element : TreeLinksSomeRange<LinksType>(arglist)) {
 *        body;
 *     }
 */

/**
 * Class template that defines member types ForwardIterator and ReverseIterator
 * for walking over a std::vector of pointers to Links objects.
 * This primary template definition is for a non-const Links type.
 * @tparam Links  Link object type.
 */
template<class Links> struct JeodLinksIterators
{
    using ForwardIterator = typename std::vector<Links *>::iterator;
    using ReverseIterator = typename std::vector<Links *>::reverse_iterator;
};

/**
 * Partial specialization of JeodLinksIterators for const Links types.
 * Like the primary definition, this specialization defines member types
 * ForwardIterator and ReverseIterator, but this are now const iterators.
 * @tparam Links  Link object type.
 */
template<class Links> struct JeodLinksIterators<const Links>
{
    using ForwardIterator = typename std::vector<Links *>::const_iterator;
    using ReverseIterator = typename std::vector<Links *>::const_reverse_iterator;
};

/**
 * Base class template for all tree links range types.
 * @tparam Iterator  The type of iterator stored as the begin_ and end_ data
 * members and returned by the begin and end member functions.
 */
template<class Iterator> class TreeLinksRange
{
public:
    /**
     * Constructor.
     * @tparam T  The type of argument begin_in.
     * @tparam U  The type of argument end_in.
     * @param begin_in  Value used to construct the begin_ data member.
     * @param end_in  Value used to construct the end_ data member.
     */
    template<typename T, typename U>
    TreeLinksRange(T begin_in, U end_in)
        : begin_(begin_in),
          end_(end_in)
    {
    }

    /**
     * Mutable accessor to the begin_ data member.
     */
    Iterator & begin()
    {
        return begin_;
    }

    /**
     * Mutable accessor to the end_ data member.
     */
    Iterator & end()
    {
        return end_;
    }

private:
    /**
     * Object returned (by reference) by the begin member function.
     */
    Iterator begin_; //!< trick_units(--)

    /**
     * Object returned (by reference) by the end member function.
     */
    Iterator end_; //!< trick_units(--)
};

/**
 * A TreeLinksAscendRange walks up a Links object's path_to_node_ data
 * member, starting at the start node and ending just before the end node.
 */
template<class Links>
class TreeLinksAscendRange : public TreeLinksRange<typename JeodLinksIterators<Links>::ReverseIterator>
{
public:
    using ReverseIterator = typename JeodLinksIterators<Links>::ReverseIterator;

    /**
     * Non-default constructor.
     * Create a TreeLinksAscendRange that walks over the entire path_to_node_
     * from the bottom to the top.
     */
    explicit TreeLinksAscendRange(Links & links)
        : TreeLinksRange<ReverseIterator>(ReverseIterator(links.path_to_node_.end()),
                                          ReverseIterator(links.path_to_node_.begin()))
    {
    }

    /**
     * Non-default constructor.
     * Create a TreeLinksAscendRange given the start and end indices in the
     * input Links object's path_to_node_ vector.
     * Behavior is undefined if start_index > path_to_node_.size() or if
     * end_index >= start_index.
     * @param links  Object whose path_to_node_ vector is to be traversed,
     *   in reverse.
     * @param start_index  Index of the element in the path_to_node_ vector
     *   that immediately follows the initial element to be visited in a
     *   range-based for loop. For example, using path_to_node_.size()
     *   starts at the final element of the vector.
     * @param end_index  Index of the element in the path_to_node_ vector
     *   that is the last element to be visited in a range-based for loop.
     *   For example, using zero stops the iteration at the initial element
     *   in the vector.
     */
    TreeLinksAscendRange(Links & links, unsigned int start_index, unsigned int end_index = 0)
        : TreeLinksRange<ReverseIterator>(ReverseIterator(links.path_to_node_.begin() + start_index),
                                          ReverseIterator(links.path_to_node_.begin() + end_index))
    {
    }
};

/**
 * A TreeLinksDescentRange walks down a Links object's path_to_node_ data
 * member, starting at the start node and ending just before the end node.
 */
template<class Links>
class TreeLinksDescentRange : public TreeLinksRange<typename JeodLinksIterators<Links>::ForwardIterator>
{
public:
    using ForwardIterator = typename JeodLinksIterators<Links>::ForwardIterator;

    /**
     * Constructor.
     * Create a TreeLinksDescentRange the marches from the start_index node of
     * the links object's path_to_node_ vector to the last node.
     * Behavior is undefined if start_index > path_to_node_.size().
     * @param links  Object whose path_to_node_ vector is to be traversed,
     *   in reverse.
     * @param start_index  Index of the first node the path_to_node_ vector to
     *   be visited.
     */
    explicit TreeLinksDescentRange(Links & links, unsigned int start_index = 0)
        : TreeLinksRange<ForwardIterator>(ForwardIterator(links.path_to_node_.begin() + start_index),
                                          ForwardIterator(links.path_to_node_.end()))
    {
    }
};

/**
 * A TreeLinksChildrenRange walks over a Links object's children_.
 */
template<class Links>
class TreeLinksChildrenRange : public TreeLinksRange<typename JeodLinksIterators<Links>::ForwardIterator>
{
public:
    using ForwardIterator = typename JeodLinksIterators<Links>::ForwardIterator;

    /**
     * Default constructor.
     * Creates a range that will visit all children.
     */
    explicit TreeLinksChildrenRange(Links & links)
        : TreeLinksRange<ForwardIterator>(ForwardIterator(links.children_.begin()),
                                          ForwardIterator(links.children_.end()))
    {
    }
};

} // namespace jeod

#endif

/**
 * @}
 * @}
 * @}
 */
