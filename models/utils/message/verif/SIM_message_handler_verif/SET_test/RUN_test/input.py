#
# Purpose:
#   (Test the JEOD message handling system.
#    The verification driver generates messages based on the message number:
#      case 0: No message.
#      case 1: Failure.
#      case 2: Error.
#      case 3: Warning.
#      case 4: Information.
#      case 5: Debug.
#      case 6: Generic message interface (severity=6)
#    This driver repeatedly uses cases 2-6, testing various combinations
#    of the message suppression capabilities and terminates with a failure.)
#
#
#

import sys

outFile = open('{0}/run.out'.format(trick.command_line_args_get_output_dir()), 'w')

def outPut(msg):
  global outFile
  outFile.write(msg)
  print(msg)

read = 1
trick.add_read(read, '''
outPut("""***************************************************
Tests 1-5: Tests with default suppression settings

Test 1: send_message()  (Message not suppressed)
""")
verif.driver.message_number = 6
''')

read = 2
trick.add_read(read, '''
outPut("""Test 2: debug() (Message suppressed)
""")
verif.driver.message_number = 5
''')

read = 3
trick.add_read(read, '''
outPut("""Test 3: inform() (Message suppressed)
""")

verif.driver.message_number = 4
''')

read = 4
trick.add_read(read, '''
outPut("""Test 4: warn()  (Message not suppressed)
""")
verif.driver.message_number = 3
''')

read = 5
trick.add_read(read, '''
outPut("""Test 5: error()  (Message not suppressed)
""")
verif.driver.message_number = 2
''')

read = 6
trick.add_read(read, '''
outPut("""
***************************************
Tests 6-10: Suppression level set to 6

Test 6: send_message() (Not suppressed)
""")
trick.MessageHandler_set_suppression_level(6)
verif.driver.message_number = 6
''')

read = 7
trick.add_read(read, '''
outPut("""Test 7: debug() (Message suppressed)
""")
verif.driver.message_number = 5
''')

read = 8
trick.add_read(read, '''
outPut("""Test 8: inform() (Message suppressed)
""")
verif.driver.message_number = 4
''')

read = 9
trick.add_read(read, '''
outPut("""Test 9: warn() (Message suppressed)
""")
verif.driver.message_number = 3
''')

read = 10
trick.add_read(read, '''
outPut("""Test 10: error() (Not suppressed)
""")
verif.driver.message_number = 2
''')

read = 11
trick.add_read(read, '''
outPut("""
**************************************
Tests 11-12: Supression level set to trick.MessageHandler.Error

Test 11: send_message() (Message suppressed)
""")
trick.MessageHandler_set_suppression_level(trick.MessageHandler.Error)
verif.driver.message_number = 6
''')

read = 12
trick.add_read(read, '''
outPut("""Test 12: error() (Not suppressed)
""")
verif.driver.message_number = 2
''')

read = 13
trick.add_read(read, '''
outPut("""
**********************************************
Tests 13-15: Tests of suppressing ID, location

Test 13: error() (ID suppressed)
""")
trick.MessageHandler_set_suppression_level(1)
trick.MessageHandler_set_suppress_id(True)
verif.driver.message_number = 2
''')

read = 14
trick.add_read(read, '''
outPut("""Test 14: error() (location suppressed)
""")
trick.MessageHandler_set_suppress_id(False)
trick.MessageHandler_set_suppress_location(True)
verif.driver.message_number = 2
''')

read = 15
trick.add_read(read, '''
outPut("""Test 15: error() (ID and location suppressed)
""")
trick.MessageHandler_set_suppress_id(True)
trick.MessageHandler_set_suppress_location(True)
verif.driver.message_number = 2
''')

read = 16
trick.add_read(read, '''
outPut("""
******************************************
Tests 16-20: Suppression level set to 9999

Test 16: send_message() (Not suppressed)
""")
trick.MessageHandler_set_suppression_level(trick.MessageHandler.Debug)
trick.MessageHandler_set_suppress_id(False)
trick.MessageHandler_set_suppress_location(False)
verif.driver.message_number = 6
''')

read = 17
trick.add_read(read, '''
outPut("""Test 17: debug() (Not suppressed)
""")
verif.driver.message_number = 5
''')

read = 18
trick.add_read(read, '''
outPut("""Test 18: inform() (Not suppressed)
""")
verif.driver.message_number = 4
''')

read = 19
trick.add_read(read, '''
outPut("""Test 19: warn() (Not suppressed)
""")
verif.driver.message_number = 3
''')

read = 20
trick.add_read(read, '''
outPut("""Test 20: error() (Not suppressed)
""")
verif.driver.message_number = 2
''')

read = 21
trick.add_read(read, '''
outPut("""
*************************************************
Tests 21-25: Suppress utils/message/verif/message

Test 21: send_message() (Message suppressed)
""")
trick.MessageHandler_add_suppressed_code("utils/message/verif/message")
trick.MessageHandler_set_suppression_level(trick.MessageHandler.Warning)
verif.driver.message_number = 6
''')

read = 22
trick.add_read(read, '''
outPut("""Test 22: custom error() (Not suppressed)
""")
trick.MessageHandler_error(
   "input.py", 201,
   "utils/message/verif/message",
   "MessageHandler::error() called from input file")
''')

read = 23
trick.add_read(read, '''
outPut("""Test 23: inform() (Message suppressed)
""")
verif.driver.message_number = 4
''')

read = 24
trick.add_read(read, '''
outPut("""Test 24: warn() (Not suppressed)
""")
verif.driver.message_number = 3
''')

read = 25
trick.add_read(read, '''
outPut("""Test 25: error() (Not suppressed)
""")
verif.driver.message_number = 2
''')


read = 99
trick.add_read(read, '''
outPut("""Last test: fail() (Never suppressed)
""")
verif.driver.message_number = 1
''')


read = 999
trick.add_read(read, '''
outPut("""Getting here is an error
""")
verif.driver.message_number = 2
''')

trick.exec_set_terminate_time(1000)
