/***************************************************************************//**
 * \file SimulationParameters.h
 * \author Anush Krishnan (anush@bu.edu)
 * \brief Definition of the class `SimulationParameters`.
 */


#if !defined(SIMULATION_PARAMETERS_H)
#define SIMULATION_PARAMETERS_H

#include "types.h"

#include <iostream>
#include <string>
#include <vector>

#include <petscsys.h>


/**
 * \class SimulationParameters
 * \brief Stores various parameters used in the simulation
 */
class SimulationParameters
{
public:
  /**
   * \class TimeScheme
   * \brief Stores info about the numerical scheme to use.
   */
  class TimeIntegration
  {
  public:
    TimeScheme scheme;                   ///< type of time-stepping scheme
    std::vector<PetscReal> coefficients; ///< coefficients of integration
  }; // TimeIntegration

  std::string directory; ///< directory of the simulation

  PetscReal dt; ///< time-increment
  
  PetscInt startStep, ///< initial time-step 
           nt,        ///< number of time steps
           nsave,     ///< data-saving interval
           nrestart;  ///< data-saving interval for the convective terms
  
  std::string outputFormat;  ///< output format to use
  PetscBool outputFlux,     ///< boolean to output the flux components
            outputVelocity; ///< boolean to output the velocity components

  IBMethod ibm; ///< type of system to be solved
  
  // parameters for decoupled solver (Li et al., 2016)
  PetscInt decoupling_algorithm;      ///< decoupled IBPM: algo index for order of decoupling
  PetscInt decoupling_forceEstimator; ///< decoupled IBPM: scheme index to estimate momentum forcing
  PetscReal decoupling_atol,          ///< decoupled IBPM: absolute tolerance for sub-iterative process
            decoupling_rtol;          ///< decoupled IBPM: relative tolerance for sub-iterative process
  PetscInt decoupling_maxIters;       ///< decoupled IBPM: maximum number of iterations for sub-iterative process
  PetscBool decoupling_printStats;    ///< decoupled IBPM: prints L2 norm and relative L2 norms when using sub-iterative process

  TimeIntegration convection, ///< time-scheme for the convection term
                  diffusion;  ///< time-scheme for the diffusion term

  ExecuteType vSolveType, ///< hardware to use for the velocity solver
              pSolveType; ///< hardware to use for the Poisson solver

  // constructors
  SimulationParameters();
  SimulationParameters(std::string dir, std::string filePath);
  // destructor
  ~SimulationParameters();
  // parse input file and store simulation parameters
  void initialize(std::string filePath);
  // print simulation parameters
  PetscErrorCode printInfo();

}; // SimulationParameters

#endif