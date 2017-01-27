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

#ifndef BOOL_ARRAY_3D_HH
#define BOOL_ARRAY_3D_HH

#include <boolVector.hh>

/*
sequences of values, all of type bool, ordered according to three indices
in contiguous intervals
*/

class PEL_EXPORT boolArray3D
{

   public: //-----------------------------------------------------------

   //-- Instance delivery and initialization

      // Contruction of an instance whose all index intervals admit 0 as an
      // inclusive lower limit and respectively `dim0', `dim1' and `dim2'  
      // as an exclusive upper limit.
      boolArray3D( size_t dim0, size_t dim1, size_t dim2,
                   bool val=false ) ;

      boolArray3D( boolArray3D const& other ) ;

      boolArray3D const& operator=( boolArray3D const& other ) ;

      // Reinitialize the internal state, as if the constructor with the
      // same argument list was just completed. 
      void re_initialize( size_t dim0, size_t dim1, size_t dim2,
                          bool val=false ) ;

   //-- Termination

     ~boolArray3D( void ) ;

   //-- Comparison

     bool operator==( boolArray3D const& other ) const ;
     
     bool operator!=( boolArray3D const& other ) const ;
      
   //-- Access

      // exclusive upper limit of the `an_index'-th index interval
      size_t index_bound( size_t an_index ) const ;

      // item of indices `i0', 'i1' and `i2'
      bool const& operator()( size_t i0, size_t i1, size_t i2 ) const ;

   //-- Element change

      // Increase the exclusive upper limit of the first index interval 
      // to `dim0'.
      void raise_first_index_bound( size_t dim0, bool val=false ) ;
  
      // Assign `val' to all items.
      void set( bool val ) ; 

      // item of indices `i0', 'i1' and `i2'
      bool& operator()( size_t i0, size_t i1, size_t i2 ) ;

   //-- Input - Output
     
      friend std::ostream& operator<<( std::ostream& out, 
                                       boolArray3D const& a ) ;
      
   protected: //--------------------------------------------------------

   private: //----------------------------------------------------------

      boolArray3D( void ) ;
      
   //-- Attributes
    
      boolVector vector ;
      size_t d0 ; 
      size_t d1 ;
      size_t d2 ;
             
} ;


#ifndef OUTLINE
#include <boolArray3D.icc>
#endif

#endif 
