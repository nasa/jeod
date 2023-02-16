#!/usr/bin/env python3
"""
description:
Processes the specified JEOD simulation input and attempts to identify 
obsolete and/or migrated code which requires attention. By default, potential 
issues are printed via stdout. Recommend logging to file via the terminal stream
operator (e.g. ./code_converter.py [flags] source_file > log_file)"
"""
# TODO support for "-from" and "-to" arguments to support conversion between
#      different versions

###
#  Includes
###

import regex # supports operations with groups of unfixed size, unlike 're'
import os 
import sys
import glob
import errno
from itertools import islice
from argparse import ArgumentParser as ArgParser
from argparse import RawTextHelpFormatter as TxtFmt
import pathlib

###
#  Defaults
###

script_dir = os.path.dirname(os.path.realpath(__file__))
# jeod_dir   = os.path.abspath(os.path.join(script_dir,'../..'))
searchpath = []
searchfile = []
forcefix   = False
rq         = r'["' + r"']" # regex quote: either ' or "
cpp_ext    = ('.cpp','.cxx','.c++','.cc','.c',
              '.hpp','.hxx','.h++','.hh','.h')
trick_ext  = ('S_define','.sm')
python_ext = ('.py','.input')
mass_props = ['composite_properties',
              'compute_inverse_inertia',
              'core_properties',
              'structure_point',
              'compute_point_mass_inertia',
              'attach_validate',
              'detach_validate',
              'find_mass_point',
              'get_mass_properties_initialized',
              'initialize_mass',
              'is_progeny_of',
              'mass_points_size',
              'print_body',
              'print_tree',
              'reattach',
              'set_update_flag',
              'update_mass_properties']
time_conv = ['time_type',
             'a_to_b_runtime',
             'a_to_b_initialization',
             'b_to_a_runtime',
             'b_to_a_initialization']
time_conv_new = dict(zip(time_conv, 
                  [None,
                   'trick.A_TO_B_UPDATE',
                   'trick.A_TO_B_INIT',
                   'trick.B_TO_A_UPDATE',
                   'trick.B_TO_A_INIT']))


###
#  Subroutines
###
def print_warning(filename,line_num,msg):
    msg = msg.replace('\n','\n\t         ')
    print('%s:%d\n\t%s' % (filename,line_num+1,msg) )

def find_file(filename):
    global searchpath
    for pp in searchpath:
        for ff in  pathlib.Path(pp).rglob( os.path.basename(filename) ):
            return str(ff) # return first not null

    

def determine_filetype(filename):
    if filename.endswith(cpp_ext):
        return 'cpp'
    elif filename.endswith(trick_ext):
        return 'trick'
    elif filename.endswith(python_ext):
        return 'python'
    else:
        raise IOERROR(errno.EIO, 'Unrecognized filetype: ".%s"\n' 
                        % filename.split('.')[-1] )
    return None # unreached

def find_all_python(filename):
    global searchfile
    if filename in searchfile:
        return
    searchfile += [filename]
    with open(filename,'r') as fin:
        for ii, line in enumerate(fin):
            if not line or line.isspace() or line.strip()[0]=='#':
                continue
            lineNoSpaces = ''.join(line.split())
            if lineNoSpaces.startswith('execfile(') or lineNoSpaces.startswith('exec('):
               ss = lineNoSpaces.replace('"', '\'').split('\'')
               for part in ss:
                  if part.endswith(python_ext):
                     if not part in searchfile:
                        fullPath = find_file(part)
                        if fullPath:
                           find_all_python(fullPath)

            match = regex.search(r'exec\s*\(\s*open\s*\(\s*'+rq+r'([\w\/\.]+)'+rq+r'\s*\)\s*\.\s*read\s*\(\s*\)\s*\)', line)
            if match:
                sub =  find_file(match.groups()[0])
                if sub: 
                    if not sub in searchfile:
                        find_all_python(sub)
                else:
                    print('"%s" not in search path. Skipping..."' % match.groups()[0])
                continue
            match = regex.search(r'exec\s*\(\s*compile\s*\(\s*open\s*\(\s*'+rq+r'([\w\/\.]+)'+rq+r'\s*\)\s*\.\s*read\s*\(\s*\)\s*\)', line)
            if match:
                sub =  find_file(match.groups()[0])
                if sub: 
                    if not sub in searchfile:
                        find_all_python(sub)
                else:
                    print('"%s" not in search path. Skipping..."' % match.groups()[0])
                continue

            match = regex.search(r'execfile\s*\(\s*'+rq+r'([\w\/\.]+)'+rq+r'\s*\)',line)
            if match:
                sub =  find_file(match.groups()[0])
                if sub: 
                    if not sub in searchfile:
                        find_all_python(sub)
                else:
                    print('"%s" not in search path. Skipping..."' % match.groups()[0])
                continue
    return
                
def find_all_cpp(filename):
    # Does not search system path includes <>
    global searchfile
    if filename in searchfile:
        return
    searchfile += [filename]
    ret = []
    with open(filename,'r') as fin:
        in_comment = False
        for ii, line in enumerate(fin):
            if not line or line.isspace():
                continue
            # if in comment block, keep reading until end
            if in_comment:
                if '*/' in line:
                    in_comment = False
                else:
                    continue
            elif '//' in line:
                if line.strip()[0:1] == '//':
                    continue
            elif '/*' in line:
                if line.strip()[0:1]=='/*':
                    in_comment = True
                    continue
                else:
                    in_comment = True # in comment after reading this line
            #includes 
            match = regex.search(r'#include\s+\"([\w\.\/]+)\"', line)
            if match:
                sub =  find_file(match.groups()[0])
                if sub: 
                    if not sub in searchfile:
                        find_all_cpp(sub)
                else:
                    print('"%s" not in search path. Skipping..."' % match.groups()[0])
    # Search for source corresponding to header
    name,hdr_ext = os.path.splitext(filename)
    if hdr_ext in ('.hpp','.hxx','.h++','.hh','.h'):
        for src_ext in ('.cpp','.cxx','.c++','.cc','.c'):
            sub = find_file(name+src_ext)
            if sub: 
                if not sub in searchfile:
                    find_all_cpp(sub)
                break
    return

def find_all_trick(filename):
    global searchfile
    if filename in searchfile:
        return
    searchfile += [filename]
    with open(filename,'r') as fin:
        in_comment = False
        for ii, line in enumerate(fin):
            if not line or line.isspace():
                continue
            # if in comment block, keep reading until end
            if in_comment:
                if '*/' in line:
                    in_comment = False
                else:
                    continue
            elif '//' in line:
                if line.strip()[0:1] == '//':
                    continue
            elif '/*' in line:
                if line.strip()[0:1]=='/*':
                    in_comment = True
                    continue
                else:
                    in_comment = True # in comment after reading this line
            #includes 
            match = regex.search(r'#include\s+\"([\w\.\/]+)\"', line)
            if match:
                sub =  find_file(match.groups()[0])
                if sub: 
                    if not sub in searchfile:
                        find_all_trick(sub)
                else:
                    print('"%s" not in search path. Skipping..."' % match.groups()[0])
                continue
            ##includes 
            match = regex.search(r'##include\s+\"([\w\.\/]+)\"', line)
            if match:
                sub =  find_file(match.groups()[0])
                if sub: 
                    if not sub in searchfile:
                        find_all_cpp(sub)
                else:
                    print('"%s" not in search path. Skipping..."' % match.groups()[0])
                continue
    return

def find_all_like(filename):
    filetype = determine_filetype(filename)
    if filetype == 'cpp':
        return find_all_cpp(filename)
    if filetype == 'trick':
        return find_all_trick(filename)
    if filetype == 'python':
        return find_all_python(filename)

def bodyaction_fixup_pointers(line, origVarName):
    (varPath, _, refName) = line.partition('=')
    refName = refName.strip().rstrip()
    remainder = line.rpartition(refName)[2]
    varPath = varPath.rstrip()
    pos = varPath.rfind('.dyn_{0}'.format(origVarName))
    if pos != -1:
       varPath = varPath[0:pos] + '.set_{0}_body( {1} ){2}'.format(origVarName, refName, remainder)
    else:
       pos = varPath.rfind('.{0}'.format(origVarName))
       if pos != -1:
          varPath = varPath[0:pos] + '.set_{0}_body( {1} ){2}'.format(origVarName, refName, remainder)
       else:
          print('Unable to replace pointer assignments for {0}.\n\t{1}'.format(origVarName, varPath))
    line = varPath
    return line

def lint_python(filename):
    global forcefix
    global mass_props
    global time_conv
    print('... Searching %s ...' % filename )
    with open(filename,'r') as fin:
        filedata = str()
        for ii, line in enumerate(fin):
            if not line or line.isspace() or line.strip()[0]=='#':
                filedata += line
                continue
            lineCleanedSpaces = ' '.join(line.split())
            lineNoSpaces = ''.join(lineCleanedSpaces.split())
            # MassBasicPoint collapsed
            if ('mass_basic_point' in line) \
            or ('MassBasicPoint' in line):
                if forcefix:
                    line = line.replace('mass_basic_point','mass_point')
                    line = line.replace('MassBasicPoint','MassPoint')
                print_warning(filename,ii,"MassBasicPoint collapsed into MassPoint")
            # BodyAction's renamed
            if ('mass_body_attach' in line) \
            or ('MassBodyAttach'   in line) \
            or ('mass_body_detach' in line) \
            or ('MassBodyDetach'   in line):
                if forcefix:
                    line = line.replace('MassBodyAttach','BodyAttach')
                    line = line.replace('mass_body_attach','body_attach')
                    line = line.replace('MassBodyDetach','BodyDetach')
                    line = line.replace('mass_body_detach','body_detach')
                    line = line.replace('MassBodyReattach','BodyReattach')
                    line = line.replace('mass_body_attach','body_attach')
                print_warning(filename,ii,
                    "BodyActions refactored to account for "
                    "separation of DynBody and MassBody\n"
                    "** See jeod/models/dynamics/body_action for class names and documentation\n"
                    "** Use set_<variable_name>_body instead of pointer assignments. ")
            # BodyAction change to set_subject_body for DynBody and MassBody assignments
            if ('.subject=' in lineNoSpaces) \
            or ('.dyn_subject=' in lineNoSpaces):
               if forcefix:
                  line = bodyaction_fixup_pointers(line, 'subject')
               print_warning(filename,ii,"BodyAction classes use set_subject_body to set the subject")
            
            # BodyAttach change to set_parent_body for DynBody and MassBody assignments
            elif ('.parent=' in lineNoSpaces) \
            or ('.dyn_parent=' in lineNoSpaces):
               if forcefix:
                  line = bodyaction_fixup_pointers(line, 'parent')
               print_warning(filename,ii,"BodyAttach classes use set_subject_body to set the parent")

            # BodyDetachSpecific change to set_detach_from_body for DynBody and MassBody assignments
            elif ('.detach_from=' in lineNoSpaces) \
            or ('.dyn_detach_from=' in lineNoSpaces):
               if forcefix:
                  line = bodyaction_fixup_pointers(line, 'detach_from')
               print_warning(filename,ii,"BodyDetachSpecific classes use set_detach_from_body to set the detach_from body")

            # Simple6Dof removed, collapsed into DynBody
            if ('simple_6dof_dyn_body' in line) \
            or ('Simple6DofDynBody' in line):
                if forcefix:
                    line = line.replace('Simple6DofDynBody','DynBody')
                    line = line.replace('simple_6dof_dyn_body','dyn_body')
                print_warning(filename,ii,"Simple6DofDynBody collapsed into DynBody")
            # DynBody no longer inherits from MassBody (elif so not redundant with Simple6Dof)
            elif ('body' in line):
                for attr in mass_props:
                    if regex.search('body\.'+attr,line):
                        if forcefix: 
                            line = line.replace('body.'+attr,'body.mass.'+attr)
                        print_warning(filename,ii,"DynBody no longer inherits from MassBody.\n"
                                "Access through MassBody DynBody::mass\n"
                                "e.g., dyn_body.composite_properties -> dyn_body.mass.composite_properties")
            # Gravity models renamed
            if ('GravityModel' in line) \
            or ('grav_model' in line) \
            or ('gravity_model' in line):
                if forcefix:
                    line = line.replace('GravityModel','GravityManager')
                    line = line.replace('grav_model','grav_manager')
                    line = line.replace('gravity_model','gravity_manager')
                print_warning(filename,ii,"GravityModel -> GravityManager")
            if ('GravityBody' in line) \
            or ('grav_body' in line) \
            or ('gravity_body' in line):
                if forcefix:
                    line = line.replace('GravityBody','GravitySource')
                    line = line.replace('grav_body','grav_body')
                    line = line.replace('gravity_body','gravity_body')
                print_warning(filename,ii,"GravityBody -> GravitySource")
            # Time refactor
            if 'register_type' in line:
                if forcefix:
                    line = line.replace('register_type','register_time')
                print_warning(filename,ii,"TimeManager::register_type -> register_time")
            elif any(attr in line for attr in time_conv):
                print_warning(filename,ii,
                    "JEOD time converter a_to_b / b_to_a bools replaced with PROTECTED member\n"
                    "That must be defined IN SOURCE by TimeUDE-inherited class constructor:\n"
                    "Direction JeodBaseTime::valid_directions\n"
                    "e.g., a_to_b_runtime = true;\n"
                    "      a_to_b_initialization = false;\n"
                    "      b_to_a_runtime = false;\n"
                    "      b_to_a_initialization = true;\n"
                    "  ->  valid_directions = A_TO_B_UPDATE | B_TO_A_INIT")
            # Ephemeris refactor
            if ('De4xx' in line) \
            or ('de4xx' in line) \
            or ('De405' in line) \
            or ('de405' in line) \
            or ('De421' in line) \
            or ('de421' in line) \
            or ('De440' in line) \
            or ('de440' in line):
                print_warning(filename,ii,"JEOD Ephemeris refactor:\n"
                                          "1) Removes EPHEM_DE4## macro interface and replaces with \n"
                                          "   env.de4xx.set_model_number(4##)\n"
                                          "2) Removes default_data type classes")
            filedata += line
    with open(filename,'w') as fout:
        fout.write(filedata)

def lint_cpp(filename):
    global forcefix
    global mass_props
    global time_conv
    print('... Searching %s ...' % filename )
    found_namespace = False
    with open(filename,'r') as fin:
        filedata = str()
        for ii, line in enumerate(fin):
            if not line or line.isspace():
                filedata += line
                continue
            lineCleanedSpaces = ' '.join(line.split())
            lineNoSpaces = ''.join(lineCleanedSpaces.split())
            # jeod_namespace
            if not found_namespace and 'jeod::' in line:
                found_namespace = True
            # MassBasicPoint collapsed
            if ('mass_basic_point' in line) \
            or ('MassBasicPoint' in line):
                if forcefix:
                    line = line.replace('mass_basic_point','mass_point')
                    line = line.replace('MassBasicPoint','MassPoint')
                print_warning(filename,ii,"MassBasicPoint collapsed into MassPoint")
            # BodyAction's renamed
            if ('mass_body_attach' in line) \
            or ('MassBodyAttach'   in line) \
            or ('mass_body_detach' in line) \
            or ('MassBodyDetach'   in line):
                if forcefix:
                    line = line.replace('MassBodyAttach','BodyAttach')
                    line = line.replace('mass_body_attach','body_attach')
                    line = line.replace('MassBodyDetach','BodyDetach')
                    line = line.replace('mass_body_detach','body_detach')
                    line = line.replace('MassBodyReattach','BodyReattach')
                    line = line.replace('mass_body_attach','body_attach')
                print_warning(filename,ii,"BodyActions refactored to account for "
                    "separation of DynBody and MassBody\n"
                    "** See jeod/models/dynamics/body_action for class names and documentation\n"
                    "** Use set_<variable_name>_body instead of pointer assignments. ")
            # BodyAction change to set_subject_body for DynBody and MassBody assignments
            if ('.subject=' in lineNoSpaces) \
            or ('.dyn_subject=' in lineNoSpaces):
               if forcefix:
                  refName = lineNoSpaces.partition('=')[2]
                  line = line.replace(refName, 'set_subject_body({0})'.format(refName))
               print_warning(filename,ii,"BodyAction classes use set_subject_body to set the subject")
            
            # BodyAttach change to set_parent_body for DynBody and MassBody assignments
            elif ('.parent=' in lineNoSpaces) \
            or ('.dyn_parent=' in lineNoSpaces):
               if forcefix:
                  refName = lineNoSpaces.partition('=')[2]
                  line = line.replace(refName, 'set_parent_body({0})'.format(refName))
               print_warning(filename,ii,"BodyAttach classes use set_subject_body to set the parent")

            # BodyDetachSpecific change to set_detach_from_body for DynBody and MassBody assignments
            elif ('.detach_from=' in lineNoSpaces) \
            or ('.dyn_detach_from=' in lineNoSpaces):
               if forcefix:
                  refName = lineNoSpaces.partition('=')[2]
                  line = line.replace(refName, 'set_detach_from_body({0})'.format(refName))
               print_warning(filename,ii,"BodyDetachSpecific classes use set_detach_from_body to set the detach_from body")
            # Simple6Dof removed, collapsed into DynBody
            if ('simple_6dof_dyn_body' in line) \
            or ('Simple6DofDynBody' in line):
                if forcefix:
                    line = line.replace('Simple6DofDynBody','DynBody')
                    line = line.replace('simple_6dof_dyn_body','dyn_body')
                print_warning(filename,ii,"Simple6DofDynBody collapsed into DynBody")
            # DynBody no longer inherits from MassBody
            if ('body' in line) \
            or ('DynBody::' in line):
                for attr in mass_props:
                    if regex.search('body\.'+attr,line) \
                    or regex.search('DynBody::'+attr,line):
                        if forcefix: 
                            line = line.replace('body.'+attr,'body.mass.'+attr)
                        print_warning(filename,ii,"DynBody no longer inherits from MassBody.\n"
                            "Access through MassBody DynBody::mass\n"
                            "e.g., dyn_body.composite_properties -> dyn_body.mass.composite_properties")
            # Gravity models renamed
            if ('GravityModel' in line) \
            or ('grav_model' in line) \
            or ('gravity_model' in line):
                if forcefix:
                    line = line.replace('GravityModel','GravityManager')
                    line = line.replace('grav_model','grav_manager')
                    line = line.replace('gravity_model','gravity_manager')
                print_warning(filename,ii,"GravityModel -> GravityManager")
            if ('GravityBody' in line) \
            or ('grav_body' in line) \
            or ('gravity_body' in line):
                if forcefix:
                    line = line.replace('GravityBody','GravitySource')
                    line = line.replace('grav_body','grav_body')
                    line = line.replace('gravity_body','gravity_body')
                print_warning(filename,ii,"GravityBody -> GravitySource")
            # Time refactor
            if 'register_type' in line:
                if forcefix:
                    line = line.replace('register_type','register_time')
                print_warning(filename,ii,"TimeManager::register_type -> register_time")
            elif 'initialize_from_name' in line:
                print_warning(filename,ii,"JeodBaseTime::initialize_from_name moved from char* to std::string")
            elif 'update_from_name' in line:
                print_warning(filename,ii,"JeodBaseTime::initialize_from_name moved from char* to std::string")
            elif 'a_name' in line:
                print_warning(filename,ii,"TimeConverter::a_name moved from char* to std::string")
            elif 'b_name' in line:
                print_warning(filename,ii,"TimeConverter::b_name moved from char* to std::string")
            elif any(attr in line for attr in time_conv):
                print_warning(filename,ii,"JEOD time converter a_to_b / b_to_a bools replaced with "
                                          "Direction JeodBaseTime::valid_directions\n"
                                          "e.g., a_to_b_runtime = true;\n"
                                          "      a_to_b_initialization = false;\n"
                                          "      b_to_a_runtime = false;\n"
                                          "      b_to_a_initialization = true;\n"
                                          "  ->  valid_directions = A_TO_B_UPDATE | B_TO_A_INIT")
            # Ephemeris refactor
            if ('De4xx' in line) \
            or ('de4xx' in line):
                print_warning(filename,ii,"JEOD Ephemeris refactor:\n"
                                          "1) Removes EPHEM_DE4## macro interface and replaces with \n"
                                          "   env.de4xx.set_model_number(4##)\n"
                                          "2) Removes default_data type classes")
            filedata += line
    # jeod namespace
    if not found_namespace:
        if forcefix:
            pass
        else:
            print_warning(filename,ii,
                          "Beware: all JEOD model classes placed in namespace jeod\n"
                          "Recommend 'using namespace jeod;' or prepend 'jeod::'")

    with open(filename,'w') as fout:
        fout.write(filedata)

def lint_trick(filename):
    with open(filename,'r') as fin:
        for ii, line in enumerate(fin):
            if not line or line.isspace():
                continue
            # mac OS no more mac_S_overrides.mk
            if 'mac_S_overrides.mk' in line:
                print_warning(filename,ii,'mac_S_overrides.mk removed from JEOD repo')
            # Ephemeris macros
            if regex.search('#.+EPHEM_DE\d+', line):
                print_warning(filename,ii,
                              "Ephemeris model no longer uses "
                              "macro switches.\n"
                              "Set De4xxFileSpec::denum instead.")
    lint_cpp(filename)
    return 

def lint_like(files):
    ret = []
    for ff in files:
        ff = str(ff)
        filetype = determine_filetype(ff)
        if filetype == 'cpp':
            ret.append( lint_cpp(ff)    )
        if filetype == 'trick':
            ret.append( lint_trick(ff)  )
        if filetype == 'python':
            ret.append( lint_python(ff) )
    return ret


###
#  main routine
###

# Parse Arguments
parser = ArgParser(description=__doc__,formatter_class=TxtFmt)
parser.add_argument('input_file',type=str,\
    help="The target trick, C++, or python source file for which to "
         "search for potential compatiblity issues")
parser.add_argument('-r',action='store_true',dest="recurse",\
    help="Include to recursively searching files called/included within the input_file\n"
         "Off by default.")
parser.add_argument('-f',action='store_true',dest="force",\
    help="Include to force changes to unarbitrary code (e.g., JEOD #include's).\n"
         "Does not force changes to arbitrary code (e.g., MassBody and/or DynBody operations).\n"
         "Off by default.")
parser.add_argument('-I',type=str,action='append',nargs=1,dest="searchpath",\
    help="Include directory to recursively* search for  linked files.\n"
         "* This is a top level directory, exact pathing is not needed\n"
         "**Advise against including jeod or a folder containing jeod as a subdirectory")

clargs, errargs = parser.parse_known_args()
for a in errargs: # Lazy error handle: report first and exit.
    raise parser.exit(errno.EINVAL,"Unknown input argument '%s'\n" % str(a))
if clargs.searchpath:
    # flatten path list 
    clargs.searchpath = [ os.path.expanduser(item) \
                          for sublist in clargs.searchpath \
                              for item in sublist] 
    searchpath += clargs.searchpath
searchpath += [os.path.abspath(os.path.dirname(clargs.input_file))]
forcefix = clargs.force

# Find all files
filetype = determine_filetype(clargs.input_file)
if clargs.recurse:
    find_all_like(clargs.input_file)
else:
    searchfile = [clargs.input_file]
print('-------------------')

# Lint file contents
lint_like(searchfile)
