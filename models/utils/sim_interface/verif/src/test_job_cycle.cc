/********************************* TRICK HEADER ***************************

PURPOSE:
  (dummy function that calls the get_job_cycle_time)

REFERENCE:
  (((none)))

ASSUMPTIONS AND LIMITATIONS:
  (none)

CLASS:
  (scheduled)

LIBRARY DEPENDENCY:
  ((test_job_cycle.o))

 
************************************************************************/
// System includes
#include<iostream>

// JEOD includes.
#include "../include/test_job_cycle.hh"

//! Namespace jeod
namespace jeod {

JobCycleDriver::JobCycleDriver () :
active (false),
total_job_cycle_time (0.0)
{
// nothing to do
}

JobCycleDriver::~JobCycleDriver()
{
// nothing to destroy
}

void JobCycleDriver::update ( // void
)
{
if (!active)
return;

double delta=JeodSimulationInterface::get_job_cycle();

std::cout << "Total job cycle time = "<< total_job_cycle_time << "\n";
total_job_cycle_time+=delta;
std::cout<<"get_job_cycle returned "<<
delta <<"\n";
}
} // End JEOD namespace
