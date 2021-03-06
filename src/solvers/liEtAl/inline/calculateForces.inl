/*! Implementation of the method to calculate the forces acting on a immersed body.
 * \file calculateForces.inl
 */


/*!
 * \brief Computes the forces acting on the immersed bodies.
 * 
 * Sum directly over the Lagrangian forces.
 */
template <PetscInt dim>
PetscErrorCode LiEtAlSolver<dim>::calculateForces()
{
  PetscErrorCode ierr;

  PetscFunctionBeginUser;

  // get access to the global body forces vector
  PetscReal **f;
  ierr = DMDAVecGetArrayDOF(bda, fTilde, &f); CHKERRQ(ierr);

  DMDALocalInfo info;
  ierr = DMDAGetLocalInfo(bda, &info); CHKERRQ(ierr);

  PetscReal localForces[dim];
  PetscInt start = 0, end;
  PetscMPIInt rank;
  ierr = MPI_Comm_rank(PETSC_COMM_WORLD, &rank); CHKERRQ(ierr);
  for (auto &body : bodies)
  {
    end = start + body.numPointsOnProcess[rank];
    for (PetscInt d=0; d<dim; d++)
    {
      localForces[d] = 0.0;
      for (PetscInt i=0; i<info.xm; i++)
      {
        if (start <= i && i < end)
        {
          localForces[d] += f[info.xs + i][d];
        }
      }
    }
    ierr = MPI_Reduce(localForces, body.forces, dim, MPIU_REAL, MPI_SUM, 0, PETSC_COMM_WORLD); CHKERRQ(ierr);
    start = end;
  }

  ierr = DMDAVecRestoreArrayDOF(bda, fTilde, &f); CHKERRQ(ierr);

  PetscFunctionReturn(0);
} // calculateForces
