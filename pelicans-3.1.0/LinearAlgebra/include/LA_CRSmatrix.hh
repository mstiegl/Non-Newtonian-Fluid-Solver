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

#ifndef LA_CRS_MATRIX_HH
#define LA_CRS_MATRIX_HH

#include <LA_SeqMatrix.hh>
#include <doubleVector.hh>
#include <intVector.hh>
#include <size_t_vector.hh>

class LA_MatrixIterator ;
class LA_CRSmatrixIterator ;
class LA_DiagonalMatrix ;
class PEL_Communicator ;

/*
Sparse matrices with the Compressed Row Storage scheme.

These matrices are designed to be initialized by copying from existing
sparse matrix.

CRS matrix should be optimal for space consumption and product with
vector operator.

Up to now, methods that can extend sparsity pattern can be inhibited by use
of insertion_mode facility (see `::insertion_mode').

Implementation:
   The CRS implementation stores for all element:
      - its value in a table of name `VALUES'
      - its column number in a table of name `COL'
   The values are stored line per line, and the index in `VALUES' and `COL'
   of the first value of each line is stored in a table of name `START'
   (for convenience, the number of elements is stored at the end of `START',
    the index of the first element of the line `i' is then `START'(`i')
    and the last one is `START'(`i+1')-1).

Example:

   matrix:   [ 0 1 2 ]
             [ 0 0 1 ]
             [ 3 0 1 ]

   `VALUES' is [ 1 2 1 3 1 ]
   `COL'    is [ 1 2 2 0 2 ]
   `START'  is [ 0 2 3 5 ]

Hierarchical Data Structure for instantiation :

   MODULE LA_Matrix
      concrete_name = "LA_CRSmatrix"
      [ insertion_mode = <true|false> ]    default is false
   END MODULE LA_Matrix

PUBLISHED
*/

class PEL_EXPORT LA_CRSmatrix : public LA_SeqMatrix
{
   public: //-----------------------------------------------------------

   //-- Instance delivery and initialization

      // Create a copy of `other' (copying all stored coefficients of `other').
      static LA_CRSmatrix* create( PEL_Object* a_owner,
                                   LA_SeqMatrix const* other ) ;

      virtual LA_CRSmatrix* create_copy( PEL_Object* a_owner,
                                         LA_SeqMatrix const* other ) const ;

      virtual LA_CRSmatrix* create_matrix( PEL_Object* a_owner ) const ;

   //-- Characteristics

      virtual size_t allocated_memory( void ) const ;
      
   //-- Distributed processing

      virtual bool is_desynchronizable( void ) const ;
      
   //-- Access

      virtual LA_MatrixIterator* create_stored_item_iterator(
                                               PEL_Object* a_owner ) const ;

      virtual size_t nb_stored_items( void ) const ;

      virtual size_t nb_rows( void ) const ;

      virtual size_t nb_cols( void ) const ;

      // much slower than accessing through `LA_MatrixIterator::' objects
      virtual double item( size_t i, size_t j ) const ;

      virtual void extract_diag( LA_Vector* diag ) const ;

   //-- Element change

      virtual void nullify_row( size_t i ) ;

      virtual void set_stored_items( double val ) ;

      virtual void set_item( size_t i, size_t j, double x ) ;

      virtual void add_to_item( size_t i, size_t j, double x ) ;

      virtual void scale( double alpha ) ;

      // Can new items be added to existing pattern ?
      bool insertion_mode( void ) const ;

      // Modify insertion mode.
      void set_insertion_mode( bool allowed ) ;

   //-- BLAS level 2 : matrix-vector operators

      virtual void multiply_vec_then_add(
                           LA_Vector const* x, LA_Vector* y,
                           double alpha = 1.0, double beta = 0.0 ) const ;


   //-- BLAS level 3 : matrix-matrix operators

      virtual void set( LA_Matrix const* A, bool same_pattern=false ) ;

   //-- Distributed processing

      void send( PEL_Communicator const* com, size_t dest,
                 bool in_place=false ) const ;

      bool is_sending_in_place( void ) const ;

      void wait_send( PEL_Communicator const* com ) const ;

      static LA_CRSmatrix* receive( PEL_Object* a_owner,
                                    PEL_Communicator const* com,
                                    size_t src ) ;

   //-- System factorization

      virtual void factorize_MILU0( bool modified, double piv_min ) ;

      virtual void solve_LU( LA_SeqVector const* rhs,
                             LA_SeqVector* sol ) const ;

      virtual void relax( double omega,
                          LA_SeqMatrix::relaxation_mode mode,
                          LA_SeqVector const* omega_inv_diag,
                          LA_SeqVector const* rhs,
                          LA_SeqVector* sol ) const ;

   //-- Input - Output

      virtual void print( std::ostream& os, size_t indent_width ) const ;

      virtual void readMM( std::string const& file ) ;

      virtual void restore( PEL_ModuleExplorer const* exp ) ;

   protected: //--------------------------------------------------------

   private: //----------------------------------------------------------

     ~LA_CRSmatrix( void ) ;
      LA_CRSmatrix( LA_CRSmatrix const& other ) ;
      LA_CRSmatrix& operator=( LA_CRSmatrix const& other ) ;

      LA_CRSmatrix( PEL_Object* a_owner,
                    size_t a_nb_rows,
                    size_t a_nb_cols) ;

      LA_CRSmatrix( PEL_Object* a_owner,
                    LA_SeqMatrix const* other ) ;

      LA_CRSmatrix( PEL_Object* a_owner,
                    PEL_Communicator const* com,
                    size_t src ) ;

      virtual void re_initialize_with_global_sizes( size_t a_nb_rows,
                                                    size_t a_nb_cols ) ;

   //-- Plug in

      LA_CRSmatrix( void ) ;

      virtual LA_CRSmatrix* create_replica(
                                        PEL_Object* a_owner,
                                        PEL_ModuleExplorer const* exp ) const ;

      virtual void insert( size_t i, size_t j, double x, size_t pos ) ;

   //-- Friends

      friend class LA_CRSmatrixIterator ;

   //-- Preconditions, Postconditions, Invariant

      virtual bool invariant( void ) const ;

   //-- Class attributes

      static LA_CRSmatrix const* PROTOTYPE ;

   //-- Attributes

      // Matrix dimensions :
      size_t NB_ROWS ;
      size_t NB_COLS ;

      doubleVector VALUES ;
      size_t_vector DIAG ;
      intVector COL ;
      intVector START ;
      size_t NB_ELEMS ;
      bool INSERT ;
      mutable int DIMS[3] ;
      mutable void* REQUEST[4] ;
} ;

#endif
