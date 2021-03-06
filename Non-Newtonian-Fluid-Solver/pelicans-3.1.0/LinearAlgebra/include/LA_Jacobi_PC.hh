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

#ifndef LA_JACOBI_PC_HH
#define LA_JACOBI_PC_HH

#include <LA_Preconditioner.hh>

/*
Jacobi preconditioners. 

A Jacobi preconditioner M related to a matrix A is defined as follows : 
M is diagonal, such that the i-th diagonal element of the inverse 
of M is equal to the inverse of the i-th diagonal element of A if it 
is non-zero, or is equal to 1 if it is zero. 
*/

class PEL_EXPORT LA_Jacobi_PC : public LA_Preconditioner
{

   public: //----------------------------------------------------------

   //-- Instance delivery and initialization
      
      /*
      Create and return an instance.
      The diagonal coefficients of the matrix from which `self' is built
      will be considered zero provided that their absolute value is 
      lower `smallest_inverted_item'.
      */
      static LA_Jacobi_PC* create( PEL_Object* a_owner, 
                                   double smallest_inverted_item ) ;

      virtual LA_Jacobi_PC* create_clone( PEL_Object* a_owner ) const ;

   //-- Status

      virtual bool is_valid( void ) const ;

      virtual size_t dimension( void ) const ;

   //-- Building

      virtual void build( LA_Matrix const* mat ) ;
      
      virtual void unbuild( void ) ;
      
   //-- Linear system solution
      
      virtual void solve( LA_Vector const* rhs,
                          LA_Vector* sol ) ;
      virtual bool successful_solve( void ) const ;

   protected: //-------------------------------------------------------

   private: //---------------------------------------------------------

     ~LA_Jacobi_PC( void ) ; 
      LA_Jacobi_PC( LA_Jacobi_PC const& other ) ;
      LA_Jacobi_PC& operator=( LA_Jacobi_PC const& other ) ;

      LA_Jacobi_PC( PEL_Object* a_owner, double smallest_inverted_item ) ;

   //-- Plug in

      LA_Jacobi_PC( void ) ;

      virtual LA_Jacobi_PC* create_replica( 
                                    PEL_Object* a_owner,
				    PEL_ModuleExplorer const* exp ) const ;

   //-- Class attributes

      static LA_Jacobi_PC const* PROTOTYPE ;

   //-- Attributes

      double const MIN_DIAG ;
      
      LA_Vector* INV_DIAG ;
      bool BUILD_OK ;
      bool SOLVE_OK ;
} ;

#endif

        
