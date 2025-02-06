#!/usr/bin/python

import os, sys
from datetime import datetime
import calendar

planet = 'Earth'
model = 'GGM05C'
tide_free_delta = '4.173E-9'
dataFile = sys.argv[1]

scriptPath = os.path.realpath(__file__)
scriptDir = os.path.dirname(scriptPath)
dataDir = os.path.realpath( os.path.join(scriptDir, '..'))
incDir = os.path.join(dataDir, 'include')
srcDir = os.path.join(dataDir, 'src')

className = planet.lower() + '_' + model
headerFileName = className + '.hh'
srcFileName = className + '.cc'
includeFile = os.path.join(incDir, headerFileName)
srcFile = os.path.join(srcDir, srcFileName)

headerOut = open(includeFile, 'w')
commentHeader = """
/******************************************************************************
Purpose:
  ()

Library dependencies:
  (({0}.o))
******************************************************************************/

#ifndef JEOD_SPHERICALHARMONICSGRAVITYBODY_{1}_DEFAULT_DATA_H
#define JEOD_SPHERICALHARMONICSGRAVITYBODY_{1}_DEFAULT_DATA_H

#include "spherical_harmonics_gravity_source_default_data.hh"

//! Namespace jeod 
namespace jeod {{

class SphericalHarmonicsGravitySource_{0}_default_data :
   public SphericalHarmonicsGravitySource_default_data {{
 public:
   virtual void initialize (SphericalHarmonicsGravitySource*);
}};

}} // End JEOD namespace

#endif
""".format(className, className.upper() )
headerOut.write(commentHeader)
headerOut.close()

def parseCol(line, size):
  global startIdx
  ret = line[startIdx:startIdx+size]
  startIdx = startIdx+size
  return ret
 

## Read data from file
dataIn = open(dataFile, 'r')
# Read first line - format for following strings
dataIn.readline()
# Read header/description
dataDescr = dataIn.readline()
dataDescr = ' '.join(dataDescr.split()).split()
(dataName, mu, radius) = dataDescr[0:3]
# Read third line - format for following strings
colFormatRaw = dataIn.readline()
res = colFormatRaw.partition('(')[2].rpartition(')')[0]
colFormatStr = res.split(',')
nameLen = int(colFormatStr[0].split('A')[1])
coeffIdxLen = int(colFormatStr[1].split('2I')[1])
coeffLen = int(colFormatStr[2].split('2D')[1].split('.')[0])

# Read and parse values
coeffs = []
maxSvals = {}
for line in dataIn:
  startIdx = 0
  name = parseCol(line, nameLen)
  if not name.isalpha():
     continue
  row = parseCol(line, coeffIdxLen)
  col = parseCol(line, coeffIdxLen)
  Cval = parseCol(line,coeffLen)
  Sval = parseCol(line,coeffLen)
  Cval = Cval.replace('D', 'E')
  Sval = Sval.replace('D', 'E')

  irow = int(row)
  icol = int(col)

  if irow not in maxSvals:
     maxSvals[irow] = 0

  if maxSvals[irow] < icol:
     maxSvals[irow] = icol

  coeffs.append((row, col, Cval, Sval))

srcOut = open(srcFile, 'w')
today = datetime.today()
srcHeader = """
/*******************************************************************************

Purpose:
  (Gravity Recovery and Climate Experiment (GRACE) {0} {1} Gravity Field,
   {2}x{3} fully normalized gravity coefficients retrieved from
   http://www.csr.utexas.edu/grace/gravity in {4} {5})

*******************************************************************************/
""".format(model, planet, coeffs[-1][0], coeffs[-1][1], calendar.month_name[today.month], today.year)

srcOut.write(srcHeader)

srcBody = """

//! Namespace jeod 
namespace jeod {{
class SphericalHarmonicsGravitySource;
}}
#define JEOD_FRIEND_CLASS SphericalHarmonicsGravitySource_{0}_default_data

// JEOD includes
#include "environment/gravity/include/gravity_source.hh"
#include "environment/gravity/include/spherical_harmonics_gravity_source.hh"
#include "utils/memory/include/jeod_alloc.hh"
#include "utils/named_item/include/named_item.hh"

// Unsupported includes
#include "../include/{0}.hh"



//! Namespace jeod 
namespace jeod {{

void
SphericalHarmonicsGravitySource_{0}_default_data::initialize (
   SphericalHarmonicsGravitySource * SphericalHarmonicsGravitySource_ptr)
{{
   SphericalHarmonicsGravitySource_ptr->planet_name = "{1}";

   SphericalHarmonicsGravitySource_ptr->degree = {2};
   SphericalHarmonicsGravitySource_ptr->order = {3};
   SphericalHarmonicsGravitySource_ptr->mu = {4};
   SphericalHarmonicsGravitySource_ptr->radius = {5};

   SphericalHarmonicsGravitySource_ptr->tide_free = false;
   // C20 includes the permanent tide
   SphericalHarmonicsGravitySource_ptr->tide_free_delta = {6};

   SphericalHarmonicsGravitySource_ptr->Cnm =
      JEOD_ALLOC_PRIM_ARRAY ({2} + 1, double*);
   SphericalHarmonicsGravitySource_ptr->Snm =
      JEOD_ALLOC_PRIM_ARRAY ({3} + 1, double*);

   /* FULLY NORMALIZED GRAVITY COEFFICIENTS (unitless) */"""
srcBody = srcBody.format(className, planet, coeffs[-1][0], coeffs[-1][1], mu, radius, tide_free_delta)
srcOut.write(srcBody)

prevRow = -1
for coeff in coeffs:
  irow = int(coeff[0])
  icol = int(coeff[1])
  if prevRow != irow:
     srcOut.write("""
   SphericalHarmonicsGravitySource_ptr->Cnm[{0}] =
      JEOD_ALLOC_PRIM_ARRAY ({1}, double);""".format(coeff[0], int(maxSvals[irow])+1))
     prevRow = irow

  srcOut.write("""
   SphericalHarmonicsGravitySource_ptr->Cnm[{0}][{1}] ={2};""".format(coeff[0], coeff[1], coeff[2]))
  
prevRow = -1
for coeff in coeffs:
  irow = int(coeff[0])
  icol = int(coeff[1])
  if prevRow != irow:
     srcOut.write("""
   SphericalHarmonicsGravitySource_ptr->Snm[{0}] =
      JEOD_ALLOC_PRIM_ARRAY ({1}, double);""".format(coeff[0], int(maxSvals[irow])+1))
     prevRow = irow

  srcOut.write("""
   SphericalHarmonicsGravitySource_ptr->Snm[{0}][{1}] ={2};""".format(coeff[0], coeff[1], coeff[3]))
 
srcOut.write("""
}
} // End JEOD namespace
""")

srcOut.close()
