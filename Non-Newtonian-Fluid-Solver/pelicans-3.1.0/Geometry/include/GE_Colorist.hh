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

#ifndef GE_COLORIST_HH
#define GE_COLORIST_HH

#include <PEL_Object.hh>

class PEL_ContextSimple ;
class PEL_DoubleVector ;
class PEL_Vector ;
class PEL_ModuleExplorer ;
class GE_Color ;
class GE_Meshing ;
class doubleVector ;
class size_t_vector ;

class PEL_EXPORT GE_Colorist : public PEL_Object
{
   public: //------------------------------------------------------------

      static GE_Colorist* create( PEL_Object* a_owner,
                                  PEL_ModuleExplorer const* exp ) ;

      void initialize( GE_Meshing* meshing ) ;

      bool is_initialized( void ) const ;

      GE_Color const* vertex_color( doubleVector const& coordinates ) const ;

      GE_Color const* cell_color( size_t_vector const& vertex_indices ) const ;

      GE_Color const* face_color( size_t_vector const& vertex_indices ) const ;

   protected: //---------------------------------------------------------

   private: //-----------------------------------------------------------

      GE_Colorist( void ) ;
     ~GE_Colorist( void ) ;
      GE_Colorist( GE_Colorist const& other ) ;
      GE_Colorist const& operator=( GE_Colorist const& other ) ;

      GE_Colorist( PEL_Object* a_owner, PEL_ModuleExplorer const* exp ) ;

      void read_colors( PEL_ModuleExplorer const* exp,
                        std::string const& module_name,
                        PEL_Vector* colors, PEL_Vector* expressions ) const ;

      void compute_center( size_t_vector const& vertex_indices,
			   doubleVector& result ) const ;

      GE_Color const* entity_color( doubleVector const& coordinates,
				    PEL_Vector const* colors,
				    PEL_Vector const* expressions ) const ;

      void check_module( PEL_Module const* mod ) const ;

   //-- Attributes

      size_t DIM ;
      PEL_Vector* VERTICES ;
      PEL_Vector* V_COLORS ;
      PEL_Vector* V_EXPRES ;
      PEL_Vector* C_COLORS ;
      PEL_Vector* C_EXPRES ;
      PEL_Vector* S_COLORS ;
      PEL_Vector* S_EXPRES ;
      PEL_ContextSimple* CONTEXT ;
      PEL_DoubleVector* COORDS ;

} ;

#endif
