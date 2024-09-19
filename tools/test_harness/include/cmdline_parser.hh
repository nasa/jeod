//=============================================================================
// Notices:
//
// Copyright © 2023 United States Government as represented by the Administrator
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
 * @addtogroup Tools
 * @{
 * @addtogroup TestHarness
 * @{
 *
 * @file tools/test_harness/include/cmdline_parser.hh
 * Define the class CmdlineParser.
 */

/*******************************************************************************

Purpose:
  ()

Library dependencies:
  ((../src/cmdline_parser.cc))



*******************************************************************************/

#ifndef JEOD_CMDLINE_PARSER_H
#define JEOD_CMDLINE_PARSER_H

// System includes
#include <cstdio>
#include <map>
#include <string>

// JEOD includes
#include "utils/sim_interface/include/config.hh"

//! Namespace jeod
namespace jeod
{

/**
 * A CmdlineParser parses command line options.
 *
 * Using a command line parser in a unit test program is as easy as 1-2-3:
 *  1. Declar variables.
 *  2. Specify the options to be parsed, including where to put the data.
 *  3. Parse the command line.
 *
 * A simple example that recognizes -foo and -bar as command line switches:
 * @code
 *    // Step 1: Declare variables.
 *    TestSimInterface sim_interface;
 *    CmdlineParser cmdline_parser;
 *    bool foo, bar;
 *
 *    // Step 2: Specify the options to be parsed and where to put the data.
 *    cmdline_parser.add ("foo", foo);
 *    cmdline_parser.add ("bar", bar);
 *
 *    // Step 3: Parse the command line.
 *    cmdline_parser.parse ();
 * @endcode
 *
 * Suppose the program is compiled and linked to generate an executable named
 * 'my_program'. If, for example, a user runs the program as ./my_program -bar,
 * the values of the variable foo will be false and bar will true in this case.
 *
 * The model also supports options that take arguments. To illustrate, the
 * following makes the 'bar' option an integer with a default value of 42;
 * the 'foo' option remains a simple switch.
 * @code
 *    // Step 1: Declare variables.
 *    TestSimInterface sim_interface;
 *    CmdlineParser cmdline_parser;
 *    bool foo = false;
 *    int bar = 42;
 *
 *    // Step 2: Specify the options to be parsed and where to put the data.
 *    cmdline_parser.add_switch ("foo", foo);
 *    cmdline_parser.add_int ("bar", bar);
 *
 *    // Step 2: Parse the command line.
 *    cmdline_parser.parse ();
 * @endcode
 *
 * This modified version is almost as simple as the simple example. The variable
 * bar is now an int instead of a bool and the calls to add() are now calls to
 * to add_<type>(). The add() method would have worked here for foo, but it is
 * better to use the full name in this case for consistency.
 *
 * The model supports the following option types:
 *  - Switch. Switches are the simplest kind of command line option. The switch
 *    value is false/true if the option is absent from/present in the argument
 *    list.
 *  - Toggle. A toggle is similar to a switch but adds the ability to turn the
 *    switch off. A toggle named 'foo' means that -foo sets the value for 'foo'
 *    while -nofoo clears the value.
 *  - Counter. A counter increments an integer count for each occurrence on the
 *    command line. For example, with a counter named 'debug', the value of
 *    the counter will be two if the command line contains two separate -debug
 *    options.
 *  - String. A string option represents a command line option that takes a
 *    string as an argument. As with the add_int() example above, a default
 *    value must be specified with string options.
 *  - Integer. An integer option represents a command line option that takes a
 *    integer value as an argument.
 *  - Double. A double option represents a command line option that takes a
 *    double value as an argument. These too have a default value.
 */
class CmdlineParser
{
    // Types
public:
    // Member functions
public:
    CmdlineParser() = default;
    ~CmdlineParser();
    explicit CmdlineParser(const CmdlineParser &) = delete;
    CmdlineParser & operator=(const CmdlineParser &) = delete;

    // Parse the command line. Parsed option names and values are eaten.
    // The options to be parsed must be added (add_XXX) prior to calling parse.
    int parse(int & argc, char ** argv);

    // Simple interface.
    // Call add() for each switch to be regognized, then call parse().

    // Add a switch name.
    void add(const char *, bool *);

    // Type-specific interface.
    // These classify the add methods by the option type.

    // Add a switch name.
    void add_switch(const char *, bool *);

    // Add a toggle name.
    // Toggles are like switches but can be turned off with -no<name>.
    // The second argument is the default value for the toggle.
    void add_toggle(const char *, bool, bool *);

    // Add a counter name.
    // Counters start at zero and bump one for each -<name> encountered.
    void add_counter(const char *, int *);

    // Add a name that takes a string argument.
    // The second argument is the default value for the option.
    void add_string(const char *, const char *, const char **);

    // Add a name that takes an integer argument.
    // The second argument is the default value for the option.
    void add_int(const char *, int, int *);

    // Add a name that takes an double argument.
    // The second argument is the default value for the option.
    void add_double(const char *, double, double *);

private:
    // Check whether it is OK to call the named method.
    void check_timing(const char *, int, const char *, const std::string &);

    // Check whether the option name has already been registered.
    bool check_registration(const char *, int, const char *, const std::string &);

    // Check timing and type prerequisites.
    bool check_prereqs(const char *, int, const char *, const std::string &);

    // Check timing and type prerequisites.
    bool check_toggle_prereqs(const char *, int, const char *, const std::string &, const std::string &);

    // Types
private:
    /**
     * The BaseOptionValue is the base class for all option descriptor classes.
     * All instantiable classes that derive from BaseOptionValue will have
     *  - Definitions of the three pure virtual methods declared below.
     *     -- takes_argument() indicates whether the option takes an argument,
     *     -- set_default() sets the default value for the option, and
     *     -- parse_value() sets the option value based on the argument.
     *  - A reference to a target variable.
     *    CmdlineParser::parse() calls the set_default() method for each
     *    registered option to set the option's target to the default value.
     *    When CmdlineParser::parse() recognizes an option on the command line,
     *    it calls the option's parse_value() method to set the option's target.
     */
    class BaseOptionValue
    {
        // Member functions
    public:
        /**
         * Non-default constructor.
         * \param[in] opt_name The option name.
         *                     This is the string, sans the leading hyphen,
         *                     that will be recognized on the command line.
         */
        BaseOptionValue(const std::string & opt_name)
            : name(opt_name)
        {
        }

        /**
         * Destructor.
         */
        virtual ~BaseOptionValue() {}

        /**
         * Does an option of the derived type take an argument?
         * \return true if an option of the derived type requires an argument,
         *         false if it does not take an argument.
         */
        virtual bool takes_argument() = 0;

        /**
         * Called at the onset of CmdlineParser::parser(); the method must
         * set the option's target to the default value for the option.
         */
        virtual void set_default() = 0;

        /**
         * Called when the option is present on the command line; the method
         * must set the option's target to the value corresponding to the value
         * specified in the provided argument.
         * \param[in] arg Pointer to the string containing the option value.
         *                This will be a null pointer for options that don't
         *                take an argument.
         */
        virtual void parse_value(const char * arg) = 0;

        // Member data
    public:
        /**
         * The option name, usually a duplicate of the key, but in the
         * case of a Negated key, the key plus a leading "no"
         */
        std::string name;
    };

    /**
     * A SwitchOptionValue represents a command line switch.
     * Switches do not take arguments. A switch named "foo" means that when
     * "-foo" is one of the command line arguments the value of the target for
     * that switch be set to true. The default value for a switch is false.
     */
    class SwitchOptionValue : public BaseOptionValue
    {
        // Member functions
    public:
        /**
         * Non-default constructor.
         * \param[in]      opt_name    Option name
         * \param[in,out]  opt_target  The target variable
         */
        SwitchOptionValue(const std::string & opt_name, bool & opt_target)
            : BaseOptionValue(opt_name),
              target(opt_target)
        {
        }

        /**
         * Does an option of the derived type take an argument?
         * \return false. Switches do not take arguments.
         */
        virtual bool takes_argument()
        {
            return false;
        }

        /**
         * Set the target to the default value for the option.
         * The default for a switch is always false.
         */
        virtual void set_default()
        {
            target = false;
        }

        /**
         * Set the target value when the option is present on the command line.
         * In the case of a switch the target is set to true.
         * \param[in] arg  Ignored (switches don't take arguments)
         */
        virtual void parse_value(const char * arg JEOD_UNUSED)
        {
            target = true;
        }

        // Member data
    protected:
        /**
         * Where the parsed value will be stored.
         */
        bool & target; //!< trick_units(--)
    };

    /**
     * A ToggleOptionValue is a refinement of a command line switch.
     * A toggle behaves identically to a switch with two exceptions:
     *  - There is always a NegatedOptionValue paired with a toggle.
     *  - Toggles have a default value.
     */
    class ToggleOptionValue : public SwitchOptionValue
    {
        // Member functions
    public:
        /**
         * Non-default constructor.
         * \param[in]      opt_name    Option name
         * \param[in]      opt_default The default value for the option
         * \param[in,out]  opt_target  The target variable
         */
        ToggleOptionValue(const std::string & opt_name, bool opt_default, bool & opt_target)
            : SwitchOptionValue(opt_name, opt_target),
              default_value(opt_default)
        {
        }

        /**
         * Set the target to the default value for the option.
         */
        virtual void set_default()
        {
            target = default_value;
        }

        // Member data
    protected:
        /**
         * The default for this option.
         */
        bool default_value; //!< trick_units(--)
    };

    /**
     * A NegatedptionValue is a refinement of a command line switch.
     * A negated toggle behaves identically to a switch with three exceptions:
     *  - There is always a ToggleOptionValue paired with a negated toggle.
     *  - The set_default() method does nothing; the toggle sets the default.
     *  - The parse_value() method turns the target off.
     */
    class NegatedOptionValue : public SwitchOptionValue
    {
        // Member functions
    public:
        /**
         * Non-default constructor.
         * \param[in]      opt_name    Option name
         * \param[in,out]  opt_target  The target variable
         */
        NegatedOptionValue(const std::string & opt_name, bool & opt_target)
            : SwitchOptionValue(opt_name, opt_target)
        {
        }

        /**
         * Set the target to the default value for the option.
         * This method does nothing; the ToggleOptionValue paired with a
         * NegatedOptionValue sets the default.
         */
        virtual void set_default() {}

        /**
         * Set the target value when the option is present on the command line.
         * In the case of a negated toggle the target is set to false.
         * \param[in] arg  Ignored (switches don't take take arguments)
         */
        virtual void parse_value(const char * arg JEOD_UNUSED)
        {
            target = false;
        }
    };

    /**
     * A BaseIntOptionValue is a BaseOptionValue whose target is an int.
     */
    class BaseIntOptionValue : public BaseOptionValue
    {
        // Member functions
    public:
        /**
         * Non-default constructor.
         * \param[in]      opt_name    Option name
         * \param[in,out]  opt_target  The target variable
         */
        BaseIntOptionValue(const std::string & opt_name, int & opt_target)
            : BaseOptionValue(opt_name),
              target(opt_target)
        {
        }

        // Member data
    protected:
        /**
         * Where the parsed value will be stored.
         */
        int & target; //!< trick_units(--)
    };

    /**
     * A CounterOptionValue represents a counter: Each occurrence of the
     * option on the command line increments the target value.
     * The default for a counter is zero.
     */
    class CounterOptionValue : public BaseIntOptionValue
    {
        // Member functions
    public:
        /**
         * Non-default constructor.
         * \param[in]      opt_name    Option name
         * \param[in,out]  opt_target  The target variable
         */
        CounterOptionValue(const std::string & opt_name, int & opt_target)
            : BaseIntOptionValue(opt_name, opt_target)
        {
        }

        /**
         * Does an option of the derived type take an argument?
         * \return false. Counters do not take arguments.
         */
        virtual bool takes_argument()
        {
            return false;
        }

        /**
         * Set the target to the default value for the option.
         * The default for a counter is always zero.
         */
        virtual void set_default()
        {
            target = 0;
        }

        /**
         * Set the target value when the option is present on the command line.
         * In the case of a counter the target value is incremented by one.
         * \param[in] arg  Ignored (counters don't take arguments)
         */
        virtual void parse_value(const char * arg JEOD_UNUSED)
        {
            ++target;
        }
    };

    /**
     * A StringOptionValue represents a command line option that takes a string
     * as an argument.
     */
    class StringOptionValue : public BaseOptionValue
    {
        // Member functions
    public:
        /**
         * Non-default constructor.
         * \param[in]      opt_name    Option name
         * \param[in]      opt_default The default value for the option
         * \param[in,out]  opt_target  The target variable
         */
        StringOptionValue(const std::string & opt_name, const char * opt_default, const char *& opt_target)
            : BaseOptionValue(opt_name),
              default_value(opt_default),
              target(opt_target)
        {
        }

        /**
         * Does an option of the derived type take an argument?
         * \return true. Strings always take arguments.
         */
        virtual bool takes_argument()
        {
            return true;
        }

        /**
         * Set the target to the default value for the option.
         */
        virtual void set_default()
        {
            target = default_value;
        }

        /**
         * Set the target value when the option is present on the command line.
         * In the case of a string the target value is set to the command line
         * option argument.
         */
        virtual void parse_value(const char * arg)
        {
            target = arg;
        }

        // Member data
    protected:
        /**
         * The default for this option.
         */
        const char * default_value; //!< trick_units(--)

        /**
         * Where the parsed value will be stored.
         */
        const char *& target; //!< trick_units(--)
    };

    /**
     * An IntOptionValue represents a command line option that takes an integer
     * as an argument.
     */
    class IntOptionValue : public BaseIntOptionValue
    {
        // Member functions
    public:
        /**
         * Non-default constructor.
         * \param[in]      opt_name    Option name
         * \param[in]      opt_default The default value for the option
         * \param[in,out]  opt_target  The target variable
         */
        IntOptionValue(const std::string & opt_name, int opt_default, int & opt_target)
            : BaseIntOptionValue(opt_name, opt_target),
              default_value(opt_default)
        {
        }

        /**
         * Does an option of the derived type take an argument?
         * \return true. Integers always take arguments.
         */
        virtual bool takes_argument()
        {
            return true;
        }

        /**
         * Set the target to the default value for the option.
         */
        virtual void set_default()
        {
            target = default_value;
        }

        /**
         * Set the target value when the option is present on the command line.
         * In the case of an int the target value is parsed from the command line
         * option argument.
         */
        virtual void parse_value(const char * arg)
        {
            std::sscanf(arg, "%d", &target);
        }

        // Member data
    protected:
        /**
         * The default for this option.
         */
        int default_value; //!< trick_units(--)
    };

    /**
     * A DoubleOptionValue represents a command line option that takes a double
     * as an argument.
     */
    class DoubleOptionValue : public BaseOptionValue
    {
        // Member functions
    public:
        /**
         * Non-default constructor.
         * \param[in]      opt_name    Option name
         * \param[in]      opt_default The default value for the option
         * \param[in,out]  opt_target  The target variable
         */
        DoubleOptionValue(const std::string & opt_name, double opt_default, double & opt_target)
            : BaseOptionValue(opt_name),
              default_value(opt_default),
              target(opt_target)
        {
        }

        /**
         * Does an option of the derived type take an argument?
         * \return true. Doubles always take arguments.
         */
        virtual bool takes_argument()
        {
            return true;
        }

        /**
         * Set the target to the default value for the option.
         */
        virtual void set_default()
        {
            target = default_value;
        }

        /**
         * Set the target value when the option is present on the command line.
         * In the case of a double the target value is parsed from the command
         * line option argument.
         */
        virtual void parse_value(const char * arg)
        {
            std::sscanf(arg, "%lg", &target);
        }

        // Member data
    protected:
        /**
         * The default for this option.
         */
        double default_value; //!< trick_units(--)

        /**
         * Where the parsed value will be stored.
         */
        double & target; //!< trick_units(--)
    };

    /**
     * Contains the option names mapped to type.
     */
    typedef std::map<const std::string, BaseOptionValue *> OptionTable;

    // Member data
private:
    /**
     * All command line options and their types.
     */
    OptionTable option_table; //!< trick_io(**) trick_units(--)

    /**
     * Has the CmdlineParser::parse() been called?
     */
    bool parsed{}; //!< trick_units(--)
};

} // namespace jeod

#endif

/**
 * @}
 * @}
 */
