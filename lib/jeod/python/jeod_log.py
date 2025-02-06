"""
This python module defines the class jeod_log.Logger,
which provides functions that add items to the Trick log file specification.
"""
import trick.sim_services

class Logger (object) :
   """
   The class jeod_log.Logger provides mechanisms for creating a Trick data
   recording group, adding items to be logged to the recording group,
   and adding the populated recording group to the data recorder.

   Methods:
      __init__ (data_record_dispatcher)
         Construct an instance of a jeod_log.Logger.
         A new instance of a Logger is typically created by
            logger = jeod_log.Logger (data_record.drd)
         where data_record.drd is the global namespace variable that represents
         the simulation's one instance of the Trick::DataRecordDispatcher class.

      open_group (log_cycle, group_name)
         Create a data recording group with the given name that will be
         logged at the given rate.

      close_group ()
         Add the data recording group to the data recorder specified at object
         construction time. The user of the logger should add items to the
         group before calling close_group (but after calling open_group).

      log_XXX (item, <other arguments>)
         Several methods, each of which adds data specifications to the
         current recording group.

   Sample usage:
      # Create an instance of a Logger.
      logger = jeod_log.Logger ()

      # Create a binary (.trk) recording group named "data" that records
      # data at the specified cycle rate
      logger.open_group (log_cycle, "data")

      # Log sim object veh.body's composite body frame and external forces.
      logger.log_ref_frame ("veh.body.composite_body")
      logger.log_vector ("veh.extern_force")

      # Close the "data" group.
      logger.close_group ()
     
   """


   def __stringify (id) :
      if id is None :
         return "(default)"
      else :
         return '"%s"' % (id)


   def open_group (self, log_cycle, group_name, group_format='trk') :
      """
      Create a data recording group for use in subsequent log operations.
      The optional format argument must be one of
        - 'trk' for a Trick binary file,
        - 'hd5' for a HDF5 file,
        - 'csv' for a comma-separated ASCII file.
      The default is to create a .trk file.

      Sample usage:
        logger = jeod_log.Logger (data_record_dispatcher)
        logger.open_group (log_cycle, "data")
        logger.log_ref_frame ("veh.body.composite_body")
        logger.log_vector ("veh.extern_force")
        logger.close_group ()
      """

      # There had better not be a currently-open data recording group.
      if self.__dr_group is not None :
         raise IOError ("Attempt to open new group " +
                         __stringify(group_name) +
                        " before closing existing group " + 
                         __stringify(self.__group_name))

      # Map the specified format to the function that creates the group.
      format_handler = {}
      format_type = { 'trk' : 'DRBinary',
                      'hd5' : 'DRHDF5',
                      'csv' : 'DRAscii' }
      for key, value in format_type.items():
        try:
          format_handler[key] = getattr(trick.sim_services, value)
        except AttributeError:
          continue

      # Check that the specified format is valid.
      if group_format not in format_handler :
         raise KeyError ("Invalid group_format " + group_format)

      self.__group_name = group_name
      self.__dr_group = format_handler[group_format] (group_name)
      self.__dr_group.thisown = 0
      self.__dr_group.set_cycle (log_cycle)
      self.__dr_group.freq = trick.sim_services.DR_Always


   def close_group (self) :
      """
      Close the currently open data recording group.
      """
      if self.__dr_group is None :
         warn ("There is no open recording group to close")
         return

      trick.add_data_record_group(self.__dr_group)
      self.__dr_group = None
      self.__group_name = None


   def log_scalar (self, spec, ignored_third_arg=None) :
      """
      Add the scalar(s) specified by spec to the data recording group.
      The spec can either be a string that specifies a scalar to be logged
      or can be a list of strings each of which specifies a scalar.
      """
      if isinstance(spec, str) :
         self.__dr_group.add_variable (spec)
      elif isinstance(spec, list) or isinstance(spec, tuple) :
         for item in spec :
            self.log_scalar (item)
      else :
         raise TypeError ("Expected string or list of strings")


   def log_vector (self, spec, size=None) :
      """
      Add the vector(s) specified by spec to the data recording group.
      The spec can either be a string that specifies a vector to be logged
      or can be a list of strings each of which specifies a vector.
      """
      if size is None :
         size = 3

      if isinstance(spec, str) :
         self.log_scalar (
            ['%s[%d]' % (spec, ii) for ii in range(0,size)])
      elif isinstance(spec, list) or isinstance(spec, tuple) :
         for item in spec :
            self.log_vector (item)
      else :
         raise TypeError ("Expected string or list of strings")


   def log_matrix (self, spec, sizeM=3, sizeN=3) :
      """
      Add the MxN matrix(ces) specified by spec to the data recording group.
      The spec can either be a string that specifies a matrix to be logged
      or can be a list of strings each of which specifies a matrix.
      """
      if sizeM is None :
         sizeM = 3
         sizeN = 3
      elif isinstance(sizeM, list) or isinstance(sizeM, tuple) :
         if len(sizeM) == 2 :
            sizeN = sizeM[1]
            sizeM = sizeM[0]
         else :
            raise TypeError ("Size array is not list of two elements")

      if isinstance(spec, str) :
         self.log_scalar (
            [( ['%s[%d][%d]' % (spec, ii, jj) for jj in range(0,sizeN)] ) for ii in range(0,sizeM)] 
         )
      elif isinstance(spec, list) or isinstance(spec, tuple) :
         for item in spec :
            self.log_matrix (item, sizeM, sizeN)
      else :
         raise TypeError ("Expected string or list of strings")


   def log_quaternion (self, spec, ignored_third_arg=None) :
      """
      Add the quaternion(s) specified by spec to the data recording group.
      The spec can either be a string that specifies a quaternion to be logged
      or can be a list of strings each of which specifies a quaternion.
      """
      if isinstance(spec, str) :
         self.log_scalar (spec + '.scalar')
         self.log_vector (spec + '.vector')
      elif isinstance(spec, list) or isinstance(spec, tuple) :
         for item in spec :
            self.log_quaternion (item)
      else :
         raise TypeError ("Expected string or list of strings")


   def log_trans_state (self, spec, select=None) :
      """
      Add the trans. states specified by spec to the data recording group.
      The spec can either be a string that specifies a trans. state to be logged
      or can be a list of strings each of which specifies a trans. state.
      """
      if select is None :
         select=['position', 'velocity']

      if isinstance(spec, str) :
         if 'position' in select :
            self.log_vector (spec + '.position')
         if 'velocity' in select :
            self.log_vector (spec + '.velocity')
      elif isinstance(spec, list) or isinstance(spec, tuple) :
         for item in spec :
            self.log_trans_state (item, select)
      else :
         raise TypeError ("Expected string or list of strings")


   def log_rot_state (self, spec, select=None) :
      """
      Add the rot. states specified by spec to the data recording group.
      The spec can either be a string that specifies a rot. state to be logged
      or can be a list of strings each of which specifies a rot. state.
      The optional select specifies the parts of a rot. state to be recorded.
      The default omits the magnitude/direction form of the angular velocity.
      """
      if select is None :
         select=['quat', 'matrix', 'rate']

      if isinstance(spec, str) :
         if ('Q_inertial_body' in select) or ('quat' in select) :
            self.log_quaternion (spec + '.Q_inertial_body')
         if ('T_inertial_body' in select) or 'matrix' in select :
            self.log_matrix (spec + '.T_inertial_body')
         if ('ang_vel_this' in select) or ('rate' in select) :
            self.log_vector (spec + '.ang_vel_this')
         if ('ang_vel_mag' in select) or ('aux' in select) :
            self.log_scalar (spec + '.ang_vel_mag')
         if ('ang_vel_unit' in select) or ('aux' in select) :
            self.log_vector (spec + '.ang_vel_unit')
      elif isinstance(spec, list) or isinstance(spec, tuple) :
         for item in spec :
            self.log_rot_state (item, select)
      else :
         raise TypeError ("Expected string or list of strings")


   def log_ref_frame_trans_state (self, spec, select=None) :
      """
      Add the trans. states specified by spec to the data recording group.
      The spec can either be a string that specifies a ref frame to be logged
      or can be a list of strings each of which specifies a ref frame.
      """
      if select is None :
         select=['position', 'velocity']

      if isinstance(spec, str) :
         self.log_trans_state (spec + '.state.trans', select)
      elif isinstance(spec, list) or isinstance(spec, tuple) :
         for item in spec :
            self.log_ref_frame_trans_state (item, select)
      else :
         raise TypeError ("Expected string or list of strings")


   def log_ref_frame_rot_state (self, spec, select=None) :
      """
      Add the rot. states specified by spec to the data recording group.
      The spec can either be a string that specifies a ref frame to be logged
      or can be a list of strings each of which specifies a ref frame.
      The optional select specifies the parts of the rot. state to be recorded.
      The default omits the magnitude/direction form of the angular velocity.
      """
      if select is None :
         select=['quat', 'matrix', 'rate']

      if isinstance(spec, str) :
         self.log_rot_state (spec + '.state.rot', select)
      elif isinstance(spec, list) or isinstance(spec, tuple) :
         for item in spec :
            self.log_ref_frame_rot_state (item, select)
      else :
         raise TypeError ("Expected string or list of strings")


   def log_ref_frame (self, spec, select=None) :
      """
      Add the states specified by spec to the data recording group.
      The spec can either be a string that specifies a ref frame to be logged
      or can be a list of strings each of which specifies a ref frame.
      The optional select specifies the parts of the rot. state to be recorded.
      The default omits the magnitude/direction form of the angular velocity.
      """
      if select is None :
         select=['position', 'velocity', 'quat', 'matrix', 'rate']

      if isinstance(spec, str) :
         self.log_ref_frame_trans_state (spec, select)
         self.log_ref_frame_rot_state (spec, select)
      elif isinstance(spec, list) or isinstance(spec, tuple) :
         for item in spec :
            self.log_ref_frame (item, select)
      else :
         raise TypeError ("Expected string or list of strings")


   def __log_set (self, key, spec, member, opt_data=None) :
      """
      Low-level handler for log_set.
      """
      if isinstance(member, str) :
         item_name = '.'.join((spec, member))
         self.__log_set_dictionary[key] (item_name, opt_data)
      elif isinstance(member, list) or isinstance(member, tuple) :
         for item in member :
            self.__log_set (key, spec, item, opt_data)
      else :
         raise TypeError ("Expected string or list of strings (member)")


   def log_set (self, spec, select) :
      """
      Add a set of data members from spec to the data recording group.
      The spec can either be a string that specifies an object to be logged
      or can be a list of strings each of which specifies an object.
      The select argument is list (or tuple), each element of which is a two or
      three element list (or tuple) of the form
        (member_spec, data_type <, optional_data>)
      The member_spec is a string (or list/tuple of strings) that identify
      data members to be logged. The data_type is one of 'scalar', 'vector',
      etc. -- the XXX in the above log_XXX method names sans the 'log_' prefix.
      The optional_data, if present, is optional data to be sent to the
      log_XXX method.

      Sample usage:
      Assume my.var contains data members
      - foo, a scalar;
      - bar, a 3-vector; and
      - baz, a 3x3 matrix.
      The following will result in the contents of my.variable being logged:
        logger.log_set ("my.var",
                        {"foo":"scalar",
                         "bar":"vector",
                         "baz":"matrix"})
      """
      if isinstance(spec, str) :
         for item in select :
            if (isinstance(item, list) or isinstance(item, tuple)) and \
               (len(item) == 2) or (len(item) == 3) :
               member = item[0]
               key = item[1]
               if len(item) == 2 :
                  opt_data = None
               else :
                  opt_data = item[2]
               if key not in self.__log_set_dictionary :
                  raise KeyError ('Invalid type "%s"' % (key))
               self.__log_set (key, spec, member, opt_data)
            else :
               raise TypeError ('select argument "%s" has wrong structure' % \
                                (str(select)))
      elif isinstance(spec, list) or isinstance(spec, tuple) :
         for item in spec :
            self.log_set (item, select)
      else :
         raise TypeError ("Expected string or list of strings")


   def __bind (self, unbound_method) :
      return unbound_method.__get__(self)

   def __init__ (self) :
      self.__dr_group = None
      self.__group_name = None

      # The __log_set_dictionary maps type names to handlers.
      # MAINTENANCE NOTE: When you add a new log_XXX function, add it to
      # this list as well if you want log_set to use the new capability.
      self.__log_set_dictionary = {
         'scalar'                : self.__bind(Logger.log_scalar),
         'vector'                : self.__bind(Logger.log_vector),
         'matrix'                : self.__bind(Logger.log_matrix),
         'quaternion'            : self.__bind(Logger.log_quaternion),
         'trans_state'           : self.__bind(Logger.log_trans_state),
         'rot_state'             : self.__bind(Logger.log_rot_state),
         'ref_frame_trans_state' : self.__bind(Logger.log_ref_frame_trans_state),
         'ref_frame_rot_state'   : self.__bind(Logger.log_ref_frame_rot_state),
         'ref_frame'             : self.__bind(Logger.log_ref_frame),
         'set'                   : self.__bind(Logger.log_set) }
