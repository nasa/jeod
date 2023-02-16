/**
 * @addtogroup Tools
 * @{
 * @addtogroup TestHarness
 * @{
 *
 * @file tools/test_harness/src/cmdline_parser.cc
 * Implement the CmdlineParser class.
 */

/*******************************************************************************

Purpose:
  ()

 

*******************************************************************************/

// System includes
#include <cassert>
#include <cstddef>
#include <cstdio>
#include <cstring>
#include <list>
#include <sstream>
#include <getopt.h>

// JEOD includes
#include "utils/message/include/message_handler.hh"

// Model includes
#include "../include/cmdline_parser.hh"
#include "../include/test_harness_messages.hh"

/**
 * A list of integers.
 */
typedef std::list<int> IntList;


//! Namespace jeod
namespace jeod {

/**
 * Default constructor.
 */
CmdlineParser::CmdlineParser (
   void)
:
   option_table(),
   parsed(false)
{ }


/**
 * Destructor.
 */
CmdlineParser::~CmdlineParser (
   void)
{
   for (OptionTable::iterator iter = option_table.begin();
        iter != option_table.end();
        ++iter) {
      delete iter->second;
      iter->second = NULL;
   }
}


/**
 * Add a switch to the list of recognized command line options.
 * \param[in]     option_name   The option name, sans the leading hyphen.
 * \param[in,out] target        The receiver of the default and parsed value.
 */
void
CmdlineParser::add (
   const char * option_name,
   bool * target)
{
   add_switch (option_name, target);
}


/**
 * Add a switch to the list of recognized command line options.
 * \param[in]     option_name   The option name, sans the leading hyphen.
 * \param[in,out] target        The receiver of the default and parsed value.
 */
void
CmdlineParser::add_switch (
   const char * option_name,
   bool * target)
{
   // Convert the target pointer to a reference.
   bool & target_ref = *target;

   // Check prerequisites: Before parse && name not registered.
   if (check_prereqs (__FILE__, __LINE__, "add_switch", option_name)) {

      SwitchOptionValue * opt_val =
         new SwitchOptionValue (option_name, target_ref);

      option_table.insert (OptionTable::value_type (option_name, opt_val));
   }
}


/**
 * Add a toggle to the list of recognized command line options.
 * \param[in]     option_name   The option name, sans the leading hyphen.
 * \param[in]     default_value The default value for this option.
 * \param[in,out] target        The receiver of the default and parsed value.
 */
void
CmdlineParser::add_toggle (
   const char * option_name,
   bool default_value,
   bool * target)
{
   // Convert the target pointer to a reference.
   bool & target_ref = *target;

   // Form the name of the command line option that clears the switch.
   std::string negated_name("no");
   negated_name += option_name;

   // Add the toggle if it is OK to do so.
   if (check_toggle_prereqs (__FILE__, __LINE__, "add_toggle",
                             option_name, negated_name)) {

      ToggleOptionValue * opt_val =
         new ToggleOptionValue (option_name, default_value, target_ref);
      NegatedOptionValue * neg_val =
         new NegatedOptionValue (negated_name, target_ref);

      option_table.insert (OptionTable::value_type (option_name, opt_val));
      option_table.insert (OptionTable::value_type (negated_name, neg_val));
   }
}


/**
 * Add a switch to the list of recognized command line options.
 * \param[in]     option_name   The option name, sans the leading hyphen.
 * \param[in,out] target        The receiver of the default and parsed value.
 */
void
CmdlineParser::add_counter (
   const char * option_name,
   int * target)
{
   // Convert the target pointer to a reference.
   int & target_ref = *target;

   // Check prerequisites: Before parse && name not registered.
   if (check_prereqs (__FILE__, __LINE__, "add_counter", option_name)) {

      CounterOptionValue * opt_val =
         new CounterOptionValue (option_name, target_ref);

      option_table.insert (OptionTable::value_type (option_name, opt_val));
   }
}


/**
 * Add a string option to the list of recognized command line options.
 * \param[in]     option_name   The option name, sans the leading hyphen.
 * \param[in]     default_value The default value for this option.
 * \param[in,out] target        The receiver of the default and parsed value.
 */
void
CmdlineParser::add_string (
   const char * option_name,
   const char * default_value,
   const char ** target)
{
   // Convert the target pointer to a reference.
   const char *& target_ref = *target;

   // Check prerequisites: Before parse && name not registered.
   if (check_prereqs (__FILE__, __LINE__, "add_string", option_name)) {

      StringOptionValue * opt_val =
         new StringOptionValue (option_name, default_value, target_ref);

      option_table.insert (OptionTable::value_type (option_name, opt_val));
   }
}


/**
 * Add an integer option to the list of recognized command line options.
 * \param[in]     option_name   The option name, sans the leading hyphen.
 * \param[in]     default_value The default value for this option.
 * \param[in,out] target        The receiver of the default and parsed value.
 */
void
CmdlineParser::add_int (
   const char * option_name,
   int default_value,
   int * target)
{
   // Convert the target pointer to a reference.
   int & target_ref = *target;

   // Check prerequisites: Before parse && name not registered.
   if (check_prereqs (__FILE__, __LINE__, "add_int", option_name)) {

      IntOptionValue * opt_val =
         new IntOptionValue (option_name, default_value, target_ref);

      option_table.insert (OptionTable::value_type (option_name, opt_val));
   }
}


/**
 * Add a double option to the list of recognized command line options.
 * \param[in]     option_name   The option name, sans the leading hyphen.
 * \param[in]     default_value The default value for this option.
 * \param[in,out] target        The receiver of the default and parsed value.
 */
void
CmdlineParser::add_double (
   const char * option_name,
   double default_value,
   double * target)
{
   // Convert the target pointer to a reference.
   double & target_ref = *target;

   // Check prerequisites: Before parse && name not registered.
   if (check_prereqs (__FILE__, __LINE__, "add_double", option_name)) {

      DoubleOptionValue * opt_val =
         new DoubleOptionValue (option_name, default_value, target_ref);

      option_table.insert (OptionTable::value_type (option_name, opt_val));
   }
}


/**
 * Parse the command line.
 * \return              Parse status; zero=OK, non-zero=error.
 * \param[in,out] argc  The number of command line entries.
 * \param[in,out] argv  The command line entries.
 */
int
CmdlineParser::parse (
   int & argc,
   char ** argv)
{
   IntList delete_list;
   int status = 0;

   if (parsed) {
      MessageHandler::error (
         __FILE__, __LINE__, TestHarnessMessages::inconsistent_setup,
        "CmdlineParser::parse() has already been called.");
      return 1;
   }

   unsigned int nopts = option_table.size();
   unsigned int iopt = 0;
   option * options = new option[nopts+1];

   // Establish the options array based on the options added via
   // the previous calls to CmdlineParser::add_XXX.
   for (OptionTable::iterator iter = option_table.begin();
        iter != option_table.end();
        ++iter) {
      iter->second->set_default ();

      BaseOptionValue * iter_opt = iter->second;
      options[iopt].name    = iter_opt->name.c_str();
      options[iopt].has_arg = iter->second->takes_argument() ?
                              required_argument : no_argument;
      options[iopt].flag    = NULL;
      options[iopt].val     = 1;
      ++iopt;
   }

   // The options array must be terminated with a null entry.
   options[iopt].name    = NULL;
   options[iopt].has_arg = 0;
   options[iopt].flag    = NULL;
   options[iopt].val     = 0;

   // Parse the command line.
   while (status == 0) {
      int cur_index = optind;
      int option_index;
      int optc = getopt_long_only (argc, argv, "", options, &option_index);

      // At the end: Exit the loop.
      if (optc == -1) {
         break;
      }

      // Unrecognized / ambiguous option: Report.
      if (optc == '?') {
         MessageHandler::warn (
            __FILE__, __LINE__, TestHarnessMessages::invalid_option,
            "Unrecognized command line option '%s'", argv[cur_index]);
         status = 2;
      }

      // Missing argument: Report.
      else if (optc == ':') {
         MessageHandler::warn (
            __FILE__, __LINE__, TestHarnessMessages::invalid_option,
            "Missing argument for option '%s'", argv[cur_index]);
         status = 3;
      }

      // getopt_long_only parse was OK: Process the option.
      else {
         OptionTable::iterator iter (
            option_table.find(options[option_index].name));
         assert (iter != option_table.end());
         if (options[option_index].has_arg == no_argument) {
            iter->second->parse_value (NULL);
            delete_list.push_front (cur_index);
         }
         else {
            iter->second->parse_value (argv[cur_index+1]);
            delete_list.push_front (cur_index);
            delete_list.push_front (cur_index+1);
         }
      }
   }

   // Delete the parsed options from argv, leaving only the unparsed items.
   if (status == 0) {
      for (IntList::iterator iter = delete_list.begin();
           iter != delete_list.end();
           ++iter) {
         for (int darg = *iter; darg < argc; ++darg) {
            argv[darg] = argv[darg+1];
         }
         --argc;
      }
   }

   // Cleanup: Mark the command line as parsed and delete temporaries.
   parsed = true;

   delete [] options;
   return status;
}


/**
 * Check whether it is OK to call the named method.
 * \param[in] file          __FILE_
 * \param[in] line          __LINE_
 * \param[in] method_name  The method name, used for error reporting.
 * \param[in] option_name  The option name, sans the leading hyphen.
 */
void
CmdlineParser::check_timing (
   const char * file,
   int line,
   const char * method_name,
   const std::string & option_name)
{
   // Calling an add_XXX() method after calling parse() is verboten.
   if (parsed) {
      MessageHandler::fail (
         file, line, TestHarnessMessages::inconsistent_setup,
         "CmdlineParser::%s(\"%s\") called after the command line was parsed.",
         method_name, option_name.c_str());
   }
}


/**
 * Ensure that the option name has not yet been registered.
 * \param[in] file          __FILE_
 * \param[in] line          __LINE_
 * \param[in] method_name   The method name, used for error reporting.
 * \param[in] option_name   The option name, sans the leading hyphen.
 */
bool
CmdlineParser::check_registration (
   const char * file,
   int line,
   const char * method_name,
   const std::string & option_name)
{
   bool is_ok;

   // The option name must not be a duplicate entry.
   // Failure is a non-fatal error; this lets the programmers see if they have
   // goofed up elsewhere.
   if (option_table.find (option_name) != option_table.end()) {
      MessageHandler::error (
         file, line, TestHarnessMessages::inconsistent_setup,
         "CmdlineParser::%s(\"%s\") error: Name is already registered.",
         method_name, option_name.c_str());

      is_ok = false;
   }

   // Lookup is past end of table => option name has not been registered.
   else {
      is_ok = true;
   }

   return is_ok;
}


/**
 * Check timing and registration prerequisites.
 * \param[in] file          __FILE_
 * \param[in] line          __LINE_
 * \param[in] method_name   The method name, used for error reporting.
 * \param[in] option_name   The option name, sans the leading hyphen.
 */
bool
CmdlineParser::check_prereqs (
   const char * file,
   int line,
   const char * method_name,
   const std::string & option_name)
{
   check_timing (file, line, method_name, option_name);

   return check_registration (file, line, method_name, option_name);
}


/**
 * Check timing and registration prerequisites.
 * \param[in] file          __FILE_
 * \param[in] line          __LINE_
 * \param[in] method_name   The method name, used for error reporting.
 * \param[in] option_name   The option name, sans the leading hyphen.
 * \param[in] negated_name  The option name prefixed with "no".
 */
bool
CmdlineParser::check_toggle_prereqs (
   const char * file,
   int line,
   const char * method_name,
   const std::string & option_name,
   const std::string & negated_name)
{
   check_timing (file, line, method_name, option_name);

   return check_registration (file, line, method_name, option_name) &&
          check_registration (file, line, method_name, negated_name);
}

} // End JEOD namespace

/**
 * @}
 * @}
 */
