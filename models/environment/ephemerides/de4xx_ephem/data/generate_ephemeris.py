#!/usr/bin/env python
"""
description:
Processes the Jet Propulsions Laboratory Developmental ephemerics (JPL DE)
raw ASCII tables into Johnson Space Center Engineering Orbital Dynamics (JEOD) 
compatible C++ source files.
"""

###
#  Includes
###

import regex
import os 
import sys
import glob
import errno
from itertools import islice
from argparse import ArgumentParser as ArgParser
from argparse import RawTextHelpFormatter as TxtFmt


###
#  Defaults
###

script_dir = os.path.dirname(os.path.realpath(__file__))
in_dir     = os.path.join(script_dir,'../ascii_full_set')
out_dir    = os.path.join(script_dir,'data_src')


###
#  Structures
###
class HeaderData:
    """Container for processed Header information"""
    def __init__(self):
        # Global data
        self.ksize = -1
        self.ncoeff = -1
        self.number_segments = -1
        self.group_line_dict = dict()
        # title GROUP 1010
        self.title = str()
        # time era GROUP 1030
        self.start_epoch = -1
        self.stop_epoch = -1
        self.delta_epoch = -1
        # Constants GROUP 1040 & 1041
        self.consts = dict()
        self.nconsts = -1
        # Ephemeris Item metadata GROUP 1050
        self.coef_start_index = list()
        self.num_coef_per_component = list()
        self.num_coef_sets = list()
        self.num_items = -1

class AscpData:
    """Container for processed Coefficient information"""
    def __init__(self):
        # Global data
        self.coefs = []
        self.num_records = -1
        self.record_length = list()
        self.record_start_epoch = list()
        self.record_stop_epoch = list()
        self.record_delta_epoch = list()

###
#  Subroutines
###
def find_n(full, sub, n=1):
    found = full.find(sub)
    ii = 1
    while found >= 0 and ii < n:
        found = full.find(sub,found+1)
        ii = ii + 1
    return found


def parse_header_file(filename):
    """
    Parse header.<denum> and return structure of contents
    """
    data = HeaderData()
    with open(filename,'r') as fin:
        # ksize and ncoeff
        line = fin.readline()
        match = regex.search(r'KSIZE\s*?=\s*?(\d+)', line)
        if match:
            data.ksize = int(match.groups()[0])
        else:
            raise IOError(errno.EIO,'Header missing required entry KSIZE') 
        match = regex.search(r'NCOEFF\s*?=\s*?(\d+)', line)
        if match:
            data.ncoeff = int(match.groups()[0])
        else:
            raise IOError(errno.EIO,'Header missing required entry NCOEFF') 
        if (2*data.ncoeff) != data.ksize:
            raise IOError(errno.EIO,'Header has unrecognized NCOEFF and/or KSIZE')
        # Find groups
        pattern = regex.compile(r'GROUP\s+?(\d+)')
        in_group = ''
        ii_start = -1
        for ii, line in enumerate(fin):
            match = regex.search(pattern, line)
            if not match:
                continue
            elif in_group: # Found match while in group: end group
                data.group_line_dict[in_group] = (ii_start, ii)
            # Found match, start new group
            in_group = match.groups()[0]
            ii_start = ii
        # Group 1010
        fin.seek(0) # Reset file cursor
        lines = islice(fin, data.group_line_dict['1010'][0],\
                         data.group_line_dict['1010'][1] )
        for line in lines:
            # Grab first nonblank, non-ID line as title
            if not line.isspace() and not regex.search(r'GROUP\s+\d+',line):
                data.title = str(line).strip()
                break;
        # Group 1030
        fin.seek(0) # Reset file cursor
        lines = islice(fin, data.group_line_dict['1030'][0],\
                            data.group_line_dict['1030'][1] )
        for line in lines:
            # grab if not ID line or blank lines
            if not line.isspace() and not regex.search(r'GROUP\s+\d+',line):
                line = regex.sub(r'(?<=[+-]?\d\.\d+)D(?=[+-]?\d+)','E',line)
                line = regex.findall(r'(^|\s+)([+-]?\d+(\.\d+([eE][+-]?\d+)*)*)',line)
                data.start_epoch = line[0][1]
                data.stop_epoch  = line[1][1]
                data.delta_epoch = line[2][1]
                break
            pass
            
        # Group 1040
        fin.seek(0) # Reset file cursor
        keys = []
        lines = islice(fin, data.group_line_dict['1040'][0],\
                            data.group_line_dict['1040'][1] )
        for line in lines:
            # Grab skip ID line and blank lines
            if line.isspace() or regex.search(r'GROUP\s+\d+',line):
                continue
            # Grab int number of entries
            if data.nconsts < 0:
                match = regex.search(r'(^|\s+?)(\d+)(\s+?)',line)
                if match:
                    data.nconsts = int(match.groups()[1])
            # Grab constants
            keys = keys + regex.findall(r'([a-zA-z]\w*)',line)
        # Group 1041
        fin.seek(0) # Reset file cursor
        values = []
        lines = islice(fin, data.group_line_dict['1041'][0],\
                            data.group_line_dict['1041'][1] )
        for line in lines:
            # Grab skip ID line and blank lines
            if line.isspace() or regex.search(r'GROUP\s+\d+',line):
                continue
            # Grab int number of entries
            match = regex.search(r'(^|\s+?)(\d+)(\s+?)',line)
            if match:
                if int(match.groups()[1]) != data.nconsts:
                    raise IOError(errno.EIO,\
                    'Header disagreement between number of constants:\n'
                    'GROUP 1040: %d\nGROUP 1041: %d'\
                    % (data.nconsts, int(match.groups()[0]) ) )
            # replace 1.0D+01 -> 1.0E+01
            line = regex.sub(r'(?<=[+-]*\d\.\d+)D(?=[+-]\d+)','E',line)
            # Grab constants
            line = regex.findall(r'([+-]*\d\.\d+[eE][+-]\d+)',line)
            line = [x for x in line]
            values = values + line
        if len(keys) != len(values):
            raise IOError(errno.EIO,'Header mismatched constants:\n'
                    '# of constant names:  %d\n'
                    '# of constant values: %d'\
                    % (len(keys), len(values)) )
        data.consts = {keys[x]:values[x] for x in range(len(values))}

        # Group 1050
        fin.seek(0) # Reset file cursor
        arr = []
        lines = islice(fin, data.group_line_dict['1050'][0],\
                            data.group_line_dict['1050'][1] )
        for line in lines:
            # Grab skip ID line and blank lines
            if line.isspace() or regex.search(r'GROUP\s+\d+',line):
                continue
            # Grab int metadata
            arr.append(regex.findall(r'(\d+)',line))
        for ii in range(len(arr[0])):
            # If included in header, but absent from files, warn
            if (int(arr[1][ii])==0) or (int(arr[2][ii])==0):
                print("! Warning: item %d has no associated poly coefs !" % ii )
        data.coef_start_index = [int(x) for x in arr[0]]
        data.num_coef_per_component = [int(x) for x in arr[1]]
        data.num_coef_sets = [int(x) for x in arr[2]]
        data.num_items = len(data.num_coef_sets)

    # Print summary
    print('KSIZE= %d\tNCOEFF=%d' % (data.ksize, data.ncoeff) )
    print('"%s"' % data.title )
    print('Start Epoch = %s' % data.start_epoch )
    print('Stop  Epoch = %s' % data.stop_epoch )
    print('Delta Epoch = %s' % data.delta_epoch )
    print('Processed %d constants' % len(data.consts) )
    print('Processing records for %d ephemeris items' % data.num_items )
    
    return data






def parse_data_file(mdata, filename):
    """
    Parse an individual data file using metadata from header and return data
    """
    poly = AscpData()
    poly.coefs = []
    ii = 0
    with open(filename,'r') as fin:
        line_no = 0
        inRecord = False
        for line in fin:
            line_no = line_no + 1
            if line.isspace():
                continue
            elems = line.replace('D','E').replace('\n', '').split()
            if not elems:
               continue
            if inRecord:
               coefsToAdd = elems[:elemsRequired]
               coefs_ii.extend(coefsToAdd)
               elemsRequired -= len(coefsToAdd)
               if elemsRequired <= 0:
                  inRecord=False
                  if len(coefs_ii) != mdata.ncoeff:
                     raise IOError(errno.EIO,
                         "%s:%d\n\tmisaligned record at index %d" %
                            ( filename, line_no, ii+1 ) )
                  poly.coefs.append(coefs_ii)
                  poly.record_length.append(elemsRequired)
                  poly.record_start_epoch.append(coefs_ii[0])
                  poly.record_stop_epoch.append(coefs_ii[1])
            else:
               elemsRequired = int(elems[1])
               inRecord = True
               coefs_ii = []
    poly.num_records = len(poly.coefs)
    return poly


def write_out_file(filename,data,meta,subset=None):
    """
    Write the output C++ source file for JEOD using data from parsed files
    """
    tab = ' '*4
    with open(os.path.join(script_dir, 'ascp_stencil.cc.template'), 'r') as fin:
        fmt = fin.read()
    if subset is not None:
        if isinstance(subset,int):
            subset = [subset]

    fout = open( filename, 'w' )
    header_block = fmt.find('jeod::EphemerisDataSetMeta')
    fout.write(fmt[:header_block])

    fmt = fmt[header_block:]
    
    # jeod::EphemerisDataSetMeta
    val = [ meta.num_items,
            meta.start_epoch,
            meta.stop_epoch,
            meta.delta_epoch,
            meta.number_segments,
            meta.ncoeff ]

    # double de_constants
    block_val = (
        meta.consts['DENUM'],
        meta.consts['LENUM'],
        meta.consts['AU'],
        meta.consts['EMRAT'],
        meta.consts['CLIGHT'],
        meta.consts['GM1'],
        meta.consts['GM2'],
        meta.consts['GMB'],
        meta.consts['GM4'],
        meta.consts['GM5'],
        meta.consts['GM6'],
        meta.consts['GM7'],
        meta.consts['GM8'],
        meta.consts['GM9'],
        meta.consts['GMS']  )
    block_start = fmt.find('de_constants')
    block_start += fmt[block_start:].find('{')
    block_end   = fmt.find('}',block_start)+1
    # Build format
    block_fmt = '{\n' + tab*2 + (('%s,\n'+tab*2)*(len(block_val)-1)) + '%s' + tab + '\n}'
    # Pipe entries
    block = ( block_fmt % block_val )
    fmt = fmt[:block_start] + block + fmt[block_end:]

    # jeod::EphemerisDataItemMeta
    val = val + [meta.num_items]
    block_start = fmt.find('jeod::EphemerisDataItemMeta')
    block_start += find_n(fmt[block_start:],'{',2)
    block_end   = fmt.find('}',block_start)+1
    block_fmt = tab +fmt[block_start:block_end]
    block = str()
    for ii in range(meta.num_items):
        # Write values
        block_val = (
            meta.coef_start_index[ii],
            meta.num_coef_per_component[ii],
            meta.num_coef_sets[ii] )
        # Modify format
        block = block + ( block_fmt % block_val )
        # Look ahead, place comma
        if ii !=(meta.num_items-1):
            block = block + ',\n'
    fmt = fmt[:block_start] + block + fmt[block_end:]

    # jeod::EphemerisDataSegmentMeta
    val = val + [meta.number_segments]
    block_start = fmt.find('jeod::EphemerisDataSegmentMeta')
    block_start += find_n(fmt[block_start:],'{',2)
    block_end   = fmt.find('}',block_start)+1
    block_fmt = tab + fmt[block_start:block_end]
    block = str()
    for ii in range(len(data)):
        # Write values
        block_val = (
            data[ii].num_records,
            data[ii].record_start_epoch[0],
            data[ii].record_stop_epoch[-1] )
            
        # Modify format
        block = block + ( block_fmt % block_val )
        # Look ahead, place comma
        if ii !=(len(data)-1):
            block = block + ',\n'
    fmt = fmt[:block_start] + block + fmt[block_end:]

    # Coefficients table
    """
    Really big table coming up, so modify approach used above. Instead, we need
    to write as we go.
        1) Find this block in the stencil template file.
        2) Write everything up until now into the output file so that we can
           start writing as we go without holding everything in memory
        3) Create the format for each record.
        4) Write each segment header, then loop to write each record
    """
    # 1)
    val = val + [ meta.title ]
    block_start = fmt.find('double segment_coeffs')
    # block_end unneeded, last part of in file
    # 2)
    fmt = fmt[:block_start]
    # Only write metadata to first subset file
    if subset is None or 0 in subset:
       fout.write( fmt % tuple(val) )
    # 3)
    newline = '\n'+tab+'  '
    sep = ', '
    coefs_per_line = 3
    block_fmt = str()
    block_fmt += tab + '{' + newline
    block_fmt += (('%s'+sep)*coefs_per_line+newline)*int(meta.ncoeff/coefs_per_line)
    # if no remainder, then remove trailing separator and close }
    if (meta.ncoeff%coefs_per_line)==0:
        block_fmt = block_fmt[:-(len(newline)+len(sep))] + '\n' +tab + '}'
    # If remainder, add, then remove trailing separator and close }
    else:
        block_fmt += ('%s'+sep)*(meta.ncoeff%coefs_per_line)
        block_fmt = block_fmt[:-(len(sep))] + '\n' + tab + '}'
    frec = 0
    for ii in subset:
            fout.write( ( ('double segment_coeffs_%d[%d][%d] = {\n')
                        % ( ii, data[ii].num_records, meta.ncoeff ) ) )
            for jj in range(data[ii].num_records):
                fout.write( ( block_fmt % tuple(data[ii].coefs[jj]) ) )
                # look ahead, see if we need a comma
                if jj!=(data[ii].num_records-1):
                    fout.write( ',' )
                fout.write( '\n' )
            fout.write( '};\n\n' )
            print('%6d records written from Julian dates %s to %s'
                    % ( data[ii].num_records,
                        data[ii].record_start_epoch[0], 
                        data[ii].record_stop_epoch[-1] ) )
            frec += data[ii].num_records
    return frec


###
#  main routine
###

# Parse Arguments
parser = ArgParser(description=__doc__,formatter_class=TxtFmt)
parser.add_argument('denum',nargs=1,type=str,\
    help="The unique DE version ID corresponding to the "
         "ASCII extensions\n"
         "(e.g., for DE440: generate_ephemeris.py 440)")
parser.add_argument('-i',type=str,default=in_dir,dest="indir",\
    help="Specify the relative or absolute path to the "
         "directory containing\nthe ASCII Ephemeris files:\n"
         "1) header.<model>    -- Header declaring constants and\n"
         "                        ephemeris item formatting\n"
         "2) ascp<segment>.<model> -- Set of segment files containing\n"
         "                            Chebychev polynomial coefficients \n"
         "                            for computing planetary ephemerides.\n")
parser.add_argument('-o',type=str,default=out_dir,dest="outdir",\
    help="Specify the relative or absolute path to the generated\n"
         "C++ files.\n"
         "(Default: -o ./data_src)")
parser.add_argument('-t0',type=int,default=-1,dest="t0",\
    help="Specify starting segment for which to generate C++ source (inclusive)")
parser.add_argument('-tf',type=int,default=-1,dest="tf",\
    help="Specify final segment for which to generate C++ source (exclusive)")

clargs, errargs = parser.parse_known_args()
for a in errargs: # Lazy error handle: report first and exit.
    raise parser.exit(errno.EINVAL,"Unknown input argument '%s'\n" % str(a))

denum = str(clargs.denum[0])
in_dir  = os.path.realpath(clargs.indir)
out_dir = os.path.realpath(clargs.outdir)
t0 = int(clargs.t0)
tf = int(clargs.tf)
ascp = glob.glob(os.path.join(in_dir, 'ascp*.'+denum))
hdr  = glob.glob(os.path.join(in_dir,'header.'+denum))
if len(ascp) == 0:
    raise IOError(errno.ENOENT, 
            'No such file:\n' + os.path.join(in_dir,'ascp*.'+denum))
elif len(hdr) != 1:
    raise IOError(errno.ENOENT, 
            'Could not find unique header:\n' + os.path.join(in_dir,'header.'+denum))
else:
    hdr = hdr[0]
segments = []
for f in ascp:
    match = regex.search(r'ascp(\d+)\.'+denum,f)
    if match:
        match = int(match.groups()[-1])
        if (t0>0) and (match<t0):
            continue
        elif (tf>0) and (match>=tf):
            continue
        else:
            segments.append(f)
if len(segments) == 0:
    raise IOError(errno.ENOENT, 
            'No ascp*.%s files between t0=%d and tf=%d' % (t0,tf) )
segments.sort()

print("----- Generating JEOD Ephemeris source for JPL DE%s -----" % denum)
print("Raw ASCII: %s" % in_dir)
print("C++ output: %s" % out_dir)

# Parse JPL DE files
meta = parse_header_file(hdr)
meta.number_segments = len(segments)
records = 0
poly = []
for fin in segments:
    poly.append(parse_data_file(meta, fin))
    frec = poly[-1].num_records
    records = records + frec
    print('%6d records found in %s' % (frec, os.path.basename(fin) ) )

# Remove old DE files
oldDeFiles  = glob.glob(os.path.join(out_dir,'de{0}*.cc'.format(denum)))
for oldDeFile in oldDeFiles:
    os.remove(oldDeFile)
    print("Removing old DE file '%s'" % oldDeFile)

# Write C++ified DE file
frec = 0
for ii in range(len(poly)):
    fout = os.path.join(out_dir,'de%s_%d.cc'%(denum,ii))
    frec += write_out_file(fout,poly,meta,ii)
if frec != records:
    raise IOERROR(errno.EIO, 
        "%d records found, but %d records published..."
        % (records, frec) )
print('----- %10d total records written into JEOD Ephemeris -----' % records)
