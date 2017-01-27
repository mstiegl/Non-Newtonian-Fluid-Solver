/*
 *  Copyright 1995-2010 by IRSN
 *
 *  This software is an application framework, with a set of integrated  
 *  reusable components, whose purpose is to simplify the task of developing 
 *  softwares of numerical mathematics and scientific computing.
 * 
 *  This software is governed by the CeCILL-C license under French law and 
 *  abiding by the rules of distribution of free software. You can use, modify 
 *  and/or redistribute the software under the terms of the CeCILL-C license  
 *  as circulated by CEA, CNRS and INRIA at the following URL 
 *  "http://www.cecill.info". 
 *
 *  As a counterpart to the access to the source code and rights to copy,  
 *  modify and redistribute granted by the license, users are provided only 
 *  with a limited warranty and the software's author, the holder of the  
 *  economic rights, and the successive licensors have only limited liability. 
 *
 *  In this respect, the user's attention is drawn to the risks associated  
 *  with loading, using, modifying and/or developing or reproducing the  
 *  software by the user in light of its specific status of free software,
 *  that may mean that it is complicated to manipulate, and that also  
 *  therefore means that it is reserved for developers and experienced 
 *  professionals having in-depth computer knowledge. Users are therefore 
 *  encouraged to load and test the software's suitability as regards their 
 *  requirements in conditions enabling the security of their systems and/or 
 *  data to be ensured and, more generally, to use and operate it in the same 
 *  conditions as regards security. 
 *
 *  The fact that you are presently reading this means that you have had 
 *  knowledge of the CeCILL-C license and that you accept its terms.
 */

#ifndef LA_SOLVER_HH
#define LA_SOLVER_HH

#include <PEL_Object.hh>

class PEL_ModuleExplorer ;
class PEL_ObjectRegister ;

class LA_Implementation ;
class LA_Matrix ;
class LA_Vector ;

/*
Solvers of linear systems that can be either direct or preconditioned
iterative.

PUBLISHED
*/

class PEL_EXPORT LA_Solver : public PEL_Object
{

   public: //-----------------------------------------------------------

   //-- Instance delivery and initialization
      
      // Is `solver_name' a registered solver ?
      static bool is_registered( std::string const solver_name ) ;
      
      static LA_Solver* make( PEL_Object* a_owner,
                              PEL_ModuleExplorer const* exp ) ;
      
      virtual LA_Solver* create_clone( PEL_Object* a_owner ) const = 0 ;
      
   //-- Setting

      // If `stop' is true, solver will stop on convergence default
      // or factorization problem. Default value is true.
      void set_stop_on_error( bool stop ) ;      

      // By default, iterative solvers assume an initial guess of zero by 
      // zeroing the initial value of the solution vector that is passed
      // to `::solve'. 
      // If `flg' is false, this default is confirmed. 
      // If `flg' is true, the solution vector that is passed to `::solve'
      // is used as a possible nonzero initial guess.
      void set_initial_guess_nonzero( bool flg ) ;
      
   //-- Instance characteristics

      // Will solver raise a fatal error on solving default ?
      bool stop_on_error( void ) const ;
      
      // Is a LHS matrix set ?
      bool matrix_is_set( void ) const ;

      // implementation of LHS matrix
      LA_Implementation const* matrix_implementation( void ) const ;
      
      // Is verbose mode activated ?
      bool is_verbose( void ) const ;
      
     // dimension of self
      size_t size( void ) const ;

      // Has solution been successfully computed ?
      bool solution_is_achieved( void ) const ;

      // for iterative solvers, number of iterations performed at `::solve'
      // completion
      size_t nb_iterations_achieved( void ) const ;

      // Is `self' an iterative solver or a direct one ?
      bool is_iterative( void ) const ;

      // if false, the incoming solution vector will be used as the initial 
      // guess; if true the initial guess will be assumed to be zero by 
      // zeroing the incoming solution vector (only relevant for iterative
      // solvers)
      bool zero_initial_guess( void ) const ;
      
      // matrix set
      LA_Matrix const* matrix( void ) const ;
      
   //-- Solution
      
      // Reset the LHS matrix which MUST have keept the same pattern as matrix
      // in call to `::set_matrix' method.
      // Value of numerical coefficients could (and should) have been modified.
      virtual void reset_matrix( void ) ;
      
      // Set the LHS matrix.
      // For direct solver, build inverse factorization of `mat'.
      // For iterative solvers, build preconditionner.
      void set_matrix( LA_Matrix const* mat ) ;

      // Free memory used by the LHS matrix.
      void unset_matrix( void ) ;

      // Solve the system where `b' is the RHS, and return the solution
      // into `x'.
      void solve( LA_Vector const* b, LA_Vector* x ) ;
      
   //-- Input - Output
      
      virtual void print( std::ostream& os, size_t indent_width ) const ;

   protected: //--------------------------------------------------------

      LA_Solver( PEL_Object* a_owner ) ;

      LA_Solver( PEL_Object* a_owner, LA_Solver const* other ) ;
      
      virtual ~LA_Solver( void ) ;
      
      void raise_fatal_error_if_not_sequential( void ) ;
      
   //-- Plug in
      
      LA_Solver( std::string const& a_name ) ;
      
      virtual LA_Solver* create_replica(
                                PEL_Object* a_owner,
                                PEL_ModuleExplorer const* exp ) const = 0 ;

      bool is_a_prototype( void ) const ;

   //-- Linear system resolution

      void set_iterative( bool iterative ) ;
      
      virtual void unset_matrix_self( void ) = 0 ;
      
      virtual void set_matrix_self( LA_Matrix const* mat,
                                    bool &ok, bool same_pattern ) = 0 ;

      virtual void solve_self( LA_Vector const* b, LA_Vector* x,
                               size_t &nb_iter, bool &ok ) = 0 ;
      
   //-- Preconditions, Postconditions, Invariant
      
      virtual bool create_clone_POST( LA_Solver const* result,
                                      PEL_Object* a_owner ) const ;
      
      virtual bool create_replica_PRE( PEL_Object* a_owner,
                                       PEL_ModuleExplorer const* exp ) const ;
      
      virtual bool create_replica_POST( LA_Solver const* result,
                                        PEL_Object* a_owner,
                                        PEL_ModuleExplorer const* exp ) const ;

      virtual bool unset_matrix_self_PRE( void ) const ;
      
      virtual bool set_matrix_self_PRE(  LA_Matrix const* mat,
                                         bool same_pattern  ) const ;
      virtual bool set_matrix_self_POST( LA_Matrix const* mat, bool ok ) const ;

      virtual bool solve_self_PRE( LA_Vector const* b, LA_Vector const* x ) const ;
      virtual bool solve_self_POST( LA_Vector const* b, LA_Vector const* x,
                                    size_t nb_iter, bool ok ) const ;
      
   private: //----------------------------------------------------------

      LA_Solver( void ) ;
      LA_Solver( LA_Solver const& other ) ;
      LA_Solver& operator=( LA_Solver const& other ) ;

      static PEL_ObjectRegister* plugins_map( void ) ;
      
   //-- Attributes

      bool const IS_PROTO ;
      std::string SOLVER_NAME ;
      bool ITERATIVE ;
      bool ZERO_INIT ;
      bool STOP ;
      bool VERBOSE ;
      
      size_t SIZE ;
      size_t NB_ITER ;
      bool SOL_ACHIEVED ;
      LA_Matrix const* MATRIX ;
      int SAVE_MATRIX_ITER_NB ;
} ; 

#endif
