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

#ifndef PEL_GROUP_EXP_HH
#define PEL_GROUP_EXP_HH

#include <PEL_Expression.hh>
#include <doubleVector.hh>

/* 
Expressions grouping items according a given subdivision of a given set.

---
name      : unit_sort
arguments : Double, Double, Double, Int[, Bool]
type      : Int

unit_sort( x, x1, x2, N ) splits [x1,x2] into N intervals of equal
diameters and returns the index of the interval containing x. The lower
bound of indices is zero (included) and the upper bound is the
number of intervals (excluded).

example :  
   unit_sort( 0.0, 0.0, 1.0, 2 ) : value is 0 (first interval)
   unit_sort( 0.9, 0.0, 1.0, 2 ) : value is 1 (second interval)

The last optional argument (default is false) shifts the intervals of
half an interval.

example:
   unit_sort( 0.0000, 0.0, 1.0, 2, true ) : value is 0 (first interval)
   unit_sort( 0.2499, 0.0, 1.0, 2, true ) : value is 0 (first interval)
   unit_sort( 0.2501, 0.0, 1.0, 2, true ) : value is 1 (second interval)
   unit_sort( 0.5000, 0.0, 1.0, 2, true ) : value is 1 (second interval)
   unit_sort( 0.7499, 0.0, 1.0, 2, true ) : value is 1 (second interval)
   unit_sort( 0.7501, 0.0, 1.0, 2, true ) : value is 0 (first interval)
   unit_sort( 1.0000, 0.0, 1.0, 2, true ) : value is 0 (first interval)
   
---
name      : segm_sort
arguments : Double, doubleVector, Int[, Bool]
type      : Int

segm_sort( x, vec, N ) splits vec into N intervals of equal
number of values and returns the index of the interval containing x. The lower
bound of indices is zero (included) and the upper bound is the
number of intervals (excluded).

example :  
   segm_sort( 0.0, <0. 0.5 0.75 1.>, 2 ) : value is 0 (first interval)
   segm_sort( 0.9, <0. 0.5 0.75 1.>, 2 ) : value is 1 (second interval)
 
The last optional argument (default is false) shifts the intervals of
half an interval.
   
---
name      : segm2D_sort
arguments : 2D doubleVector, doubleVector1, Int1, doubleVector2, Int2[, Bool]
type      : Int

segm2D_sort( x, vec1, N1, vec2, N2 ) splits vec1 into N1 intervals of equal
number of values, vec2 into N2 intervals of equal number of values and returns
the index of the interval containing x.
The lower bound of indices is zero (included) and the upper bound is N1*N2
(excluded).
    with i1 = segm_sort(x(0),vec1, N1) (in [0...N1-1])
         i2 = segm_sort(x(1),vec2, N2) (in [0...N2-1])
    returns : i2*N1+i1 (in [0...N1*N2-1])
  
The last optional argument (default is false) shifts the intervals of
half an interval.

---
name      : segm3D_sort
arguments : 3D doubleVector, doubleVector1, Int1, doubleVector2, Int2, doubleVector3, Int3[, Bool]
type      : Int

segm3D_sort( x, vec1, N1, vec2, N2, vec3, N3 ) splits vec1 into N1 intervals
of equal number of values, vec2 into N2 intervals of equal number of values,
vec3 into N3 intervals of equal number of values, and returns the index of the
interval containing x.
The lower bound of indices is zero (included) and the upper bound is N1*N2*N3
(excluded).
    with i1 = segm_sort(x(0),vec1, N1) (in [0...N1-1])
         i2 = segm_sort(x(1),vec2, N2) (in [0...N2-1])
         i3 = segm_sort(x(2),vec3, N3) (in [0...N3-1])
    returns : i3*N1*N2+i2*N1+i1 (in [0...N1*N2*N3-1])

The last optional argument (default is false) shifts the intervals of
half an interval.
    
---

PUBLISHED
*/

class PEL_EXPORT PEL_GroupExp : public PEL_Expression
{
   public: //--------------------------------------------------------------

   //-- Instance delivery and initialization

      /* Enable optimized evaluation:
          - some inner tables are set at the first called to `::to_int' function
          - WARNING: tables vec, vec1, vec2, vec3, and sizes N, N1, N2, N3
                     are supposed to be fixed between two calls of the function
                     (only x is varying).
          - then:
               initialization is in O( size(vec) ), performed only onces
               `::to_int' function is then in O(log(N)).
      */
      static void set_optimized_evaluation( void ) ;

      /* Disable optimized evaluation:
          - `::to_int' function is then in O( size(vec)+log(N) ).
      */
      static void unset_optimized_evaluation( void ) ;
      
   //-- Type
      
      virtual PEL_Data::Type data_type( void ) const ;
      
   //-- Value
      
      virtual int to_int( PEL_Context const* ct ) const ;
      
         
   protected: //----------------------------------------------------------
            
   private: //------------------------------------------------------------

      PEL_GroupExp( void ) ;
     ~PEL_GroupExp( void ) ;
      PEL_GroupExp( PEL_GroupExp const& other ) ;
      PEL_GroupExp& operator=( PEL_GroupExp const& other ) ;

      enum GroupOp { unit_sort, segm_sort, segm2D_sort, segm3D_sort } ;
      
      PEL_GroupExp( PEL_Object* a_owner,
	            std::string const& a_name,
		    PEL_Sequence const* argument_list,
                    GroupOp a_op ) ;

      void initialize( std::string const& v_arg, doubleVector const& v,
                       std::string const& n_arg, int n,
                       doubleVector& x_table ) const ;

      int index( double x, doubleVector const& x_table, bool shift ) const ;
      
   //-- Plug in

      PEL_GroupExp( std::string const& a_name, GroupOp a_op ) ;

      virtual PEL_GroupExp* create_replica( 
                                  PEL_Object * a_owner,
				  PEL_Sequence const* argument_list ) const ;

   //-- Characteristics

      virtual std::string const& usage( void ) const ;

      virtual bool valid_arguments( PEL_Sequence const* some_arguments ) const ;
      
   //-- Class attributes

      static bool OPT_EVAL ;
      
      static PEL_GroupExp const* PROTOTYPE_UNIT_SORT ;
      static PEL_GroupExp const* PROTOTYPE_SEGM_SORT ;
      static PEL_GroupExp const* PROTOTYPE_SEGM2D_SORT ;
      static PEL_GroupExp const* PROTOTYPE_SEGM3D_SORT ;
            
   //-- Attributes
      
      GroupOp const OP ;

      mutable bool INITIALIZED ;
      mutable doubleVector X ;
      mutable doubleVector Y ;
      mutable doubleVector Z ;
} ;

#endif
