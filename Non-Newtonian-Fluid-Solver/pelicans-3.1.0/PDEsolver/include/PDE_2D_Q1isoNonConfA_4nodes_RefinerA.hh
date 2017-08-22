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

#ifndef PDE_2D_Q1_ISO_NON_CONF_A_4_NODES_REFINER_A_HH
#define PDE_2D_Q1_ISO_NON_CONF_A_4_NODES_REFINER_A_HH

#include <PDE_ReferenceElementRefiner.hh>

/*
Refiner for the reference element `PDE_2D_Q1isoNonConfA_4nodes::' 
proposed by Stefan Turek.
This refiner is valid only for rectangles.

Ref : Turek Stefan, "Efficient Solvers for Incompressible Flow Problems: 
                     An Algorithmic and Computational Approach" 
      Series: Lecture Notes in Computational Science and Engineering, 
      Springer, pp.199--206, vol.6, 1999.
*/

class PEL_EXPORT PDE_2D_Q1isoNonConfA_4nodes_RefinerA : public PDE_ReferenceElementRefiner
{
   public: //-----------------------------------------------------------

      static PDE_2D_Q1isoNonConfA_4nodes_RefinerA const* object( void ) ;

   protected: //--------------------------------------------------------

   private: //----------------------------------------------------------

      PDE_2D_Q1isoNonConfA_4nodes_RefinerA( void ) ;
     ~PDE_2D_Q1isoNonConfA_4nodes_RefinerA( void ) ;
      PDE_2D_Q1isoNonConfA_4nodes_RefinerA( 
                     PDE_2D_Q1isoNonConfA_4nodes_RefinerA const& other ) ;
      PDE_2D_Q1isoNonConfA_4nodes_RefinerA& operator=( 
                     PDE_2D_Q1isoNonConfA_4nodes_RefinerA const& other ) ;

   //-- Attributes

} ;

#endif
