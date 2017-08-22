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

#include <intArray3D.hh>

#include <iostream>

#ifdef OUTLINE
#define inline
#include <intArray3D.icc>
#undef inline
#endif

//----------------------------------------------------------------------
intArray3D:: intArray3D( size_t dim0, size_t dim1, size_t dim2, int val )
//----------------------------------------------------------------------
   : vector( dim0*dim1*dim2, val )
   , d0( dim0 )
   , d1( dim1 )
   , d2( dim2 )
{
   PEL_LABEL( "intArray3D:: intArray3D" ) ;

   PEL_CHECK_POST( index_bound(0) == dim0 ) ;
   PEL_CHECK_POST( index_bound(1) == dim1 ) ;
   PEL_CHECK_POST( index_bound(2) == dim2 ) ;
   PEL_CHECK_POST( FORALL( ( size_t i0=0 ; i0<index_bound(0) ; ++i0 ),
                   FORALL( ( size_t i1=0 ; i1<index_bound(1) ; ++i1 ),
                   FORALL( ( size_t i2=0 ; i2<index_bound(2) ; ++i2 ),
                      operator()(i0,i1,i2) == val )))) ;
}

//----------------------------------------------------------------------
intArray3D:: intArray3D( intArray3D const& other )
//----------------------------------------------------------------------
   : vector( other.d0*other.d1*other.d2 )
   , d0( other.d0 )
   , d1( other.d1 )
   , d2( other.d2 )
{
   PEL_LABEL( "intArray3D:: intArray3D" ) ;
   for( size_t i0=0 ; i0<index_bound(0) ; ++i0 )
      for( size_t i1=0 ; i1<index_bound(1) ; ++i1 )
         for( size_t i2=0 ; i2<index_bound(2) ; ++i2 )
            operator()(i0,i1,i2) = other(i0,i1,i2) ;
   

   PEL_CHECK_POST( *this == other ) ;
}

//----------------------------------------------------------------------
intArray3D:: ~intArray3D( void )
//----------------------------------------------------------------------
{
}

//----------------------------------------------------------------------
intArray3D&
intArray3D:: operator=( intArray3D const& other )
//----------------------------------------------------------------------
{
   PEL_LABEL( "intArray3D:: operator=" ) ;
   if( this != &other )
   {
      vector = other.vector ;
      d0 = other.d0 ;
      d1 = other.d1 ;
      d2 = other.d2 ;
   }

   intArray3D& result = *this ;

   PEL_CHECK_POST( result == other ) ;
   return( result ) ;
}

//----------------------------------------------------------------------
bool
intArray3D:: operator==( intArray3D const& other ) const
//----------------------------------------------------------------------
{
   PEL_LABEL( "intArray3D:: operator==" ) ;

   
   bool result = ( d0==other.d0 &&
                   d1==other.d1 &&
                   d2==other.d2 &&
                   vector==other.vector ) ;

   PEL_CHECK_POST(
      IMPLIES( result,
               index_bound(0)==other.index_bound(0) &&
               index_bound(1)==other.index_bound(1) &&
               index_bound(2)==other.index_bound(2) ) ) ;
   PEL_CHECK_POST(
      !result ||
      FORALL( ( size_t i=0 ; i<index_bound(0) ; ++i ),
              FORALL( ( size_t j=0 ; j<index_bound(1) ; ++j ),
                      FORALL( ( size_t k=0 ; k<index_bound(2) ; ++k ),
                              operator()(i,j,k) == other(i,j,k) ) ) ) ) ;
   return( result ) ;
}

//----------------------------------------------------------------------
bool
intArray3D:: operator!=( intArray3D const& other ) const
//----------------------------------------------------------------------
{
   PEL_LABEL( "intArray3D:: operator!=" ) ;
  
   bool result = !operator==( other ) ;
   
   PEL_CHECK_POST( EQUIVALENT( result, !operator==(other) ) ) ;
   return( result ) ;   
}

//----------------------------------------------------------------------
void
intArray3D:: re_initialize( size_t dim0, size_t dim1, size_t dim2,
                            int val ) 
//----------------------------------------------------------------------
{
   PEL_LABEL( "intArray3D:: re_initialize" ) ;

   d0 = dim0 ;
   d1 = dim1 ;
   d2 = dim2 ;
   vector.re_initialize( d0*d1*d2, val ) ;
   
   PEL_CHECK_POST( index_bound(0) == dim0 ) ;
   PEL_CHECK_POST( index_bound(1) == dim1 ) ;
   PEL_CHECK_POST( index_bound(2) == dim2 ) ;
   PEL_CHECK_POST( FORALL( ( size_t i0=0 ; i0<index_bound(0) ; ++i0 ),
                   FORALL( ( size_t i1=0 ; i1<index_bound(1) ; ++i1 ),
                   FORALL( ( size_t i2=0 ; i2<index_bound(2) ; ++i2 ),
                      operator()(i0,i1,i2) == val )))) ;
}

//----------------------------------------------------------------------
void
intArray3D:: raise_first_index_bound( size_t dim0, int val ) 
//----------------------------------------------------------------------
{
   PEL_LABEL( "intArray3D:: raise_first_index_bound" ) ;
   PEL_CHECK_PRE( dim0 > index_bound(0) ) ;
   PEL_CHECK_PRE( index_bound(1) > 0 &&
                  index_bound(2) > 0 ) ;
   PEL_SAVEOLD( size_t, index_bound, index_bound(0) ) ;

   d0 = dim0 ;
   vector.resize( d0*d1*d2, val ) ;

   PEL_CHECK_POST( index_bound(0)==dim0 ) ;
   PEL_CHECK_POST( 
         FORALL( ( size_t i0=OLD(index_bound) ; i0<index_bound(0) ; ++i0 ),
                   FORALL( ( size_t i1=0 ; i1<index_bound(1) ; ++i1 ),
                   FORALL( ( size_t i2=0 ; i2<index_bound(2) ; ++i2 ),
                      operator()(i0,i1,i2) == val )))) ;
}

//----------------------------------------------------------------------
void
intArray3D:: set( int val )
//----------------------------------------------------------------------
{
   PEL_LABEL( "intArray3D:: set" ) ;

   vector.set( val ) ;

   PEL_CHECK_POST( FORALL( ( size_t i0=0 ; i0<index_bound(0) ; ++i0 ),
                   FORALL( ( size_t i1=0 ; i1<index_bound(1) ; ++i1 ),
                   FORALL( ( size_t i2=0 ; i2<index_bound(2) ; ++i2 ),
                      operator()(i0,i1,i2) == val )))) ;
}

//----------------------------------------------------------------------
std::ostream&
operator<<( std::ostream& out, intArray3D const& a )
//----------------------------------------------------------------------
{
   PEL_LABEL( "operator<<( std::ostream&, intArray3D const& )" ) ;
   out << a.d0 << " " << a.d1 << " " << a.d2 << " : " << std::endl ;
   
   for( size_t i = 0 ; i<a.d0 ; i++ )
   {
      for( size_t j = 0 ; j<a.d1 ; j++ )
      {
         for( size_t k = 0 ; k<a.d2 ; k++ )
         {
            out << i << " " << j << " " << k
                << " " << a( i, j, k ) << std::endl ;
         }
      }
   }
   return( out ) ;
}
