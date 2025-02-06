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
  ((test_job_cycle.cc))


************************************************************************/
// System includes
#include <iostream>

// JEOD includes.
#include "../include/test_job_cycle.hh"

//! Namespace jeod
namespace jeod
{

void JobCycleDriver::update()
{
    if(!active)
    {
        return;
    }

    double delta = JeodSimulationInterface::get_job_cycle();

    std::cout << "Total job cycle time = " << total_job_cycle_time << "\n";
    total_job_cycle_time += delta;
    std::cout << "get_job_cycle returned " << delta << "\n";
}
} // namespace jeod
