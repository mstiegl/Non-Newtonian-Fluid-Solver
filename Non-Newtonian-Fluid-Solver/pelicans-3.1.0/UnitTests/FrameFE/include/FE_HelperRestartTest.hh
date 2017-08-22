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

#ifndef FE_HELPER_RESTART_TEST_HH
#define FE_HELPER_RESTART_TEST_HH

#include <FE_OneStepIteration.hh>

class PEL_EXPORT FE_HelperRestartTest : public FE_OneStepIteration
{
   public: //-----------------------------------------------------------------

   //-- Substeps of the step by step progression
      
      virtual void do_before_time_stepping( FE_TimeIterator const* t_it ) ;

      virtual void do_one_inner_iteration( FE_TimeIterator const* t_it ) ;

   protected: //--------------------------------------------------------------
      
   private: //----------------------------------------------------------------

     ~FE_HelperRestartTest( void ) ;
      FE_HelperRestartTest( FE_HelperRestartTest const& other ) ;
      FE_HelperRestartTest& operator=(
                            FE_HelperRestartTest const& other ) ;

      FE_HelperRestartTest( PEL_Object* a_owner,
                            PDE_DomainAndFields const* dom,
                            FE_SetOfParameters const* prms,
                            PEL_ModuleExplorer const* exp ) ;

   //-- Plug in

      FE_HelperRestartTest( void ) ;

      virtual FE_HelperRestartTest* create_replica( 
                              PEL_Object* a_owner,
                              PDE_DomainAndFields const* dom,
                              FE_SetOfParameters const* prms,
                              PEL_ModuleExplorer* exp ) const ;
      
   //-- Internals
      
      void modify_field( double xx ) ;
      
   //-- Class attributes

      static FE_HelperRestartTest const* PROTOTYPE ;      

   //-- Attributes

      PDE_DiscreteField* const FIELD ;
      size_t const FIELD_LEVEL ;
      double COEF ;
      double MIN_VAL ;
      double MAX_VAL ;
} ;

#endif
