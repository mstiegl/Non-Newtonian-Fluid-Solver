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

#include <intArray2D.hh>

#include <size_t_array2D.hh>
#include <iostream>

#ifdef OUTLINE
#define inline
#include <intArray2D.icc>
#undef inline
#endif

//----------------------------------------------------------------------
intArray2D:: intArray2D( size_t dim0, size_t dim1, int val )
//----------------------------------------------------------------------
   : vector( dim0*dim1, val )
   , d0( dim0 )
   , d1( dim1 )
{
   PEL_LABEL( "intArray2D:: intArray2D( size_t, size_t )" ) ;
   PEL_CHECK_POST( index_bound(0) == dim0 ) ;
   PEL_CHECK_POST( index_bound(1) == dim1 ) ;
   PEL_CHECK_POST( FORALL( ( size_t i0=0 ; i0<index_bound(0) ; ++i0 ),
                   FORALL( ( size_t i1=0 ; i1<index_bound(1) ; ++i1 ),
                   operator()(i0,i1) == val ) ) ) ;
}

//----------------------------------------------------------------------
intArray2D:: ~intArray2D( void )
//----------------------------------------------------------------------
{
}

//----------------------------------------------------------------------
intArray2D:: intArray2D( intArray2D const& other )
//----------------------------------------------------------------------
   : vector( other.vector )
   , d0( other.d0 )
   , d1( other.d1 )
{
   PEL_LABEL( "intArray2D:: intArray2D( intArray2D const& )" ) ;
   PEL_CHECK_POST( *this == other ) ;
}

//----------------------------------------------------------------------
intArray2D const&
intArray2D:: operator=( intArray2D const& other )
//----------------------------------------------------------------------
{
   PEL_LABEL( "intArray2D:: operator=" ) ;

   if( this != &other )
   {
      vector = other.vector ;
      d0 = other.d0 ;
      d1 = other.d1 ;
   }

   intArray2D const& result = *this ;

   PEL_CHECK_POST( result == other ) ;
   return( result ) ;
}

//----------------------------------------------------------------------
bool
intArray2D:: operator==( intArray2D const& other ) const
//----------------------------------------------------------------------
{
   PEL_LABEL( "intArray2D:: operator==" ) ;

   bool result = ( d0==other.d0 &&
                   d1==other.d1 &&
                   vector==other.vector ) ;

   PEL_CHECK_POST(
      IMPLIES( result,
               index_bound(0)==other.index_bound(0) &&
               index_bound(1)==other.index_bound(1) ) ) ;
   PEL_CHECK_POST(
      !result ||
      FORALL( ( size_t i=0 ; i<index_bound(0) ; ++i ),
              FORALL( ( size_t j=0 ; j<index_bound(1) ; ++j ),
                      operator()(i,j) == other(i,j) ) ) ) ;
   return( result ) ;
}

//----------------------------------------------------------------------
bool
intArray2D:: operator!=( intArray2D const& other ) const
//----------------------------------------------------------------------
{
   PEL_LABEL( "intArray2D:: operator!=" ) ;
  
   bool result = !operator==( other ) ;
   
   PEL_CHECK_POST( EQUIVALENT( result, !operator==(other) ) ) ;
   return( result ) ;   
}

//----------------------------------------------------------------------
void
intArray2D:: re_initialize( size_t dim0, size_t dim1, int val ) 
//----------------------------------------------------------------------
{
   PEL_LABEL( "intArray2D:: re_initialize" ) ;
   
   d0 = dim0 ;
   d1 = dim1 ;
   vector.re_initialize( d0*d1, val ) ;

   PEL_CHECK_POST( index_bound(0) == dim0 ) ;
   PEL_CHECK_POST( index_bound(1) == dim1 ) ;
   PEL_CHECK_POST( FORALL( ( size_t i0=0 ; i0<index_bound(0) ; ++i0 ),
                   FORALL( ( size_t i1=0 ; i1<index_bound(1) ; ++i1 ),
                   operator()(i0,i1) == val ) ) ) ;
}

//----------------------------------------------------------------------
void
intArray2D:: raise_first_index_bound( size_t dim0, int val ) 
//----------------------------------------------------------------------
{
   PEL_LABEL( "intArray2D:: raise_first_index_bound" ) ;
   PEL_CHECK_PRE( dim0 > index_bound(0) ) ;
   PEL_CHECK_PRE( index_bound(1) > 0 ) ;
   PEL_SAVEOLD( size_t, index_bound, index_bound(0) ) ;

   d0 = dim0 ;
   vector.resize( d0*d1, val ) ;

   PEL_CHECK_POST( index_bound(0) == dim0 ) ;
   PEL_CHECK_POST( 
      FORALL( ( size_t i0=OLD(index_bound) ; i0<index_bound(0) ; ++i0 ),
         FORALL( ( size_t i1=0 ; i1<index_bound(1) ; ++i1 ),
            operator()(i0,i1) == val ) ) ) ;
}

//----------------------------------------------------------------------
size_t
intArray2D:: index_bound( size_t an_index ) const
//----------------------------------------------------------------------
{
   PEL_LABEL( "intArray2D:: index_bound" ) ;
   PEL_CHECK_PRE( an_index < 2 ) ;
   
   return ( ( an_index==0 ) ? d0 : d1 ) ;
}

//----------------------------------------------------------------------
void
intArray2D:: set( int val )
//----------------------------------------------------------------------
{
   PEL_LABEL( "intArray2D:: set" ) ;

   vector.set( val ) ;

   PEL_CHECK_POST( FORALL( ( size_t i0=0 ; i0<index_bound(0) ; ++i0 ),
                   FORALL( ( size_t i1=0 ; i1<index_bound(1) ; ++i1 ),
                   operator()(i0,i1) == val ) ) ) ;
}

//----------------------------------------------------------------------
void
intArray2D:: set( size_t_array2D const& other )
//----------------------------------------------------------------------
{
   PEL_LABEL( "size_t_array2D:: set(intArray2D const&)" ) ;

   size_t other_d0 = other.index_bound( 0 ) ;
   size_t other_d1 = other.index_bound( 1 ) ;
   size_t nvs = other_d0 * other_d1 ;
   if( d0*d1 != nvs )
   {
      vector.re_initialize( nvs ) ;
   }
   d0 = other_d0 ;
   d1 = other_d1 ; 
   for( size_t i0=0 ; i0<d0 ; ++i0 )
   {
      for( size_t i1=0 ; i1<d1 ; ++i1 )
      {
         operator()(i0,i1) = (size_t)( other( i0, i1 ) ) ;
      }
   }

   PEL_CHECK_POST( 
      FORALL( ( size_t i0=0 ; i0<index_bound(0) ; ++i0 ),
         FORALL( ( size_t i1=0 ; i1<index_bound(1) ; ++i1 ),
            operator()(i0,i1) == (int)( other( i0, i1 ) ) ) ) ) ;
}

//----------------------------------------------------------------------
void
intArray2D:: set_section( size_t an_index, size_t index_value, 
                            intVector const& x )
//----------------------------------------------------------------------
{
   PEL_LABEL( "intArray2D:: set_section" ) ;
   PEL_CHECK_PRE( an_index < 2 ) ;
   PEL_CHECK_PRE( index_value < index_bound(an_index) ) ;
   PEL_CHECK_PRE( IMPLIES( an_index==0, x.size() == index_bound(1) ) ) ;
   PEL_CHECK_PRE( IMPLIES( an_index==1, x.size() == index_bound(0) ) ) ;
   
   if( an_index == 0 )
   {
      for( size_t k=0 ; k<d1 ; k++)
      {
         operator()(index_value,k) = x(k) ;
      }
   }
   else if( an_index == 1 )
   {
      for( size_t k=0 ; k<d0 ; k++ )
      {
         operator()(k,index_value) = x(k) ;
      }
   }

   PEL_CHECK_POST( an_index==1 ||
                   FORALL( ( size_t i1=0 ; i1<index_bound(1) ; ++i1 ),
                      operator()(index_value,i1) == x(i1) ) ) ;
   PEL_CHECK_POST( an_index==0 ||
                   FORALL( ( size_t i0=0 ; i0<index_bound(0) ; ++i0 ),
                      operator()(i0,index_value) == x(i0) ) ) ;
}

//----------------------------------------------------------------------
void
intArray2D:: extract_section( size_t an_index, size_t index_value,
                              intVector& x ) const
//----------------------------------------------------------------------
{
   PEL_LABEL( "intArray2D:: extract_section" ) ;
   PEL_CHECK_PRE( an_index < 2 ) ;
   PEL_CHECK_PRE( index_value < index_bound(an_index) ) ;
   PEL_CHECK_PRE( IMPLIES( an_index==0, x.size() == index_bound(1) ) ) ;
   PEL_CHECK_PRE( IMPLIES( an_index==1, x.size() == index_bound(0) ) ) ;
   
   if( an_index == 0 )
   {
      PEL_CHECK( x.size() == d1 ) ;
      for( size_t k=0 ; k<d1 ; k++)
      {
         x(k) = operator()(index_value,k) ;
      }
   }
   else if( an_index == 1 )
   {
      PEL_CHECK( x.size() == d0 ) ;
      for( size_t k=0 ; k<d0 ; k++ )
      {
         x(k) = operator()(k,index_value) ;
      }
   }

   PEL_CHECK_POST( an_index==1 || 
                   FORALL( ( size_t i1=0 ; i1<index_bound(1) ; ++i1 ),
                           x(i1) == operator()(index_value,i1) ) ) ;
   PEL_CHECK_POST( an_index==0 || 
                   FORALL( ( size_t i0=0 ; i0<index_bound(0) ; ++i0 ),
                           x(i0) == operator()(i0,index_value) ) ) ;
}

//----------------------------------------------------------------------
std::ostream&
operator<<( std::ostream& out,intArray2D const& a )
//----------------------------------------------------------------------
{
   PEL_LABEL( "operator<<( std::ostream&, intArray2D const& )" ) ;
   for( size_t i = 0 ; i<a.d0 ; i++ )
   {
      for( size_t j = 0 ; j<a.d1 ; j++ )
      {
         out << a( i, j ) << " " ;
      }
      out << std::endl ;
   }
   return( out ) ;
}
