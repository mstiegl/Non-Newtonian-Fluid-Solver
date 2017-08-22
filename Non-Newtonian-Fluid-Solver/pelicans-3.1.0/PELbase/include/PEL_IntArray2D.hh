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

#ifndef PEL_IntArray2D_HH
#define PEL_IntArray2D_HH

#include <PEL_Data.hh>

#include <intArray2D.hh>

class size_t_array2D ;

class PEL_EXPORT PEL_IntArray2D : public PEL_Data
{
      
   public: //---------------------------------------------------------------

   //-- Instance delivery and initialization

      static PEL_IntArray2D* create( PEL_Object* a_owner,
                                     intArray2D const& val ) ;

      static PEL_IntArray2D* create( PEL_Object* a_owner,
                                     size_t_array2D const& val ) ;

      // Build array from list of vectors.
      // Returns 0 if entry vectors are inconsistent.
      static PEL_IntArray2D* create( PEL_Object* a_owner,
                                        PEL_List const* list ) ;
      
      virtual PEL_IntArray2D* create_clone( PEL_Object* a_owner ) const ;

   //-- Type
      
      virtual PEL_Data::Type data_type( void ) const ;

   //-- Value
      
      virtual intArray2D const& to_int_array2D( PEL_Context const* ct = 0 ) const ;
      
   //-- Input - Output
      
      virtual void print( std::ostream& os, size_t indent_width ) const ;
      
   protected: //------------------------------------------------------------

   private: //--------------------------------------------------------------

      PEL_IntArray2D( void ) ;
     ~PEL_IntArray2D( void ) ;
      PEL_IntArray2D( PEL_IntArray2D const & other ) ;
      PEL_IntArray2D& operator=( PEL_IntArray2D const& other ) ;

      PEL_IntArray2D( PEL_Object* a_owner, intArray2D const& val ) ;

      PEL_IntArray2D( PEL_Object* a_owner, size_t_array2D const& val ) ;
            
   //-- Attributes

      intArray2D myValue ;
      
} ;


#endif
