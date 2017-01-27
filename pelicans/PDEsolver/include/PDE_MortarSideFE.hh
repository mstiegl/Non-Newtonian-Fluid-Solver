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

#ifndef PDE_MORTAR_SIDE_FE_HH
#define PDE_MORTAR_SIDE_FE_HH

#include <PDE_MeshFE.hh>

#include <PDE_BasisFunctionMortarSide.hh>

class PEL_Vector ;

class PDE_BoundFE ;

class PEL_EXPORT PDE_MortarSideFE : public PDE_MeshFE
{
   public: //-----------------------------------------------------------

      static PDE_MortarSideFE* create( PEL_Object* a_owner,
                                       size_t a_number,
                                       GE_Mpolyhedron* a_polyhedron,
                                       GE_Color const* a_color,
                                       size_t a_refinement_level,
                                       PDE_DiscOnMeshFE const* a_disc ) ;

   //-- Adjacent domains

      void append_domain_bound( size_t domain_id, PDE_BoundFE* bd ) ;

      PEL_Vector const* domain_bounds( size_t domain_id ) const ;

   //-- Basis functions

      void set_basis_function( size_t ee, 
                               size_t ln, 
                               PDE_BasisFunctionMortarSide* bf ) ;

      virtual PDE_BasisFunctionMortarSide* basis_function( size_t ee, 
                                                           size_t ln ) const ;

   //-- Adaptation

      virtual PDE_MortarSideFE* parent( void ) const ;

      virtual bool is_active( void ) const ;

   //-- Input - Output

      virtual void print( std::ostream& os, size_t indent_width ) const ;

   protected: //--------------------------------------------------------

   private: //----------------------------------------------------------

      PDE_MortarSideFE( void ) ;
     ~PDE_MortarSideFE( void ) ;
      PDE_MortarSideFE( PDE_MortarSideFE const& other ) ;
      PDE_MortarSideFE& operator=( PDE_MortarSideFE const& other ) ;

      PDE_MortarSideFE( PEL_Object* a_owner,
                        size_t a_number,
                        GE_Mpolyhedron* a_polyhedron,
                        GE_Color const* a_color,
                        size_t a_refinement_level,
                        PDE_DiscOnMeshFE const* a_disc ) ;

      virtual PDE_DiscOnMeshFE const* disc( void ) const ;
      
   //-- Attributes

      PDE_DiscOnMeshFE const* DISC ;

      PEL_Vector* BOUNDS_0 ;
      PEL_Vector* BOUNDS_1 ;

      std::vector< std::vector< PDE_BasisFunctionMortarSide* > > BFS ;
} ;

#endif