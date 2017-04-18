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

#ifndef LA_PEL_MATRIX_HH
#define LA_PEL_MATRIX_HH

#include <LA_SeqMatrix.hh>

class LA_MatrixIterator ;
class LA_PelMatrixIterator ;
class LA_DiagonalMatrix ;

/*
   Sparse matrices with the following storage scheme :
      - the stored elements of each row are stored in linked lists ;
      - an array of pointers, of the same size as the number of rows of the
        matrix, stores the address of the head of each list.

   As a sequential matrix, a `LA_PelMatrix::' matrix is always synchronized
   by default. However, this can be switched by enabling unsynchronization
   and then testing distributed behavior (but of course on only one processor).

Data deck instanciation :

   MODULE LA_Matrix
      concrete_name = "LA_PelMatrix"
      [ is_desynchronizable = <true|false> ]    default is false
   END MODULE LA_Matrix

PUBLISHED
*/

class PEL_EXPORT LA_PelMatrix : public LA_SeqMatrix
{
   public: //-----------------------------------------------------------

   //-- Instance delivery and initialization

      // Create and return an instance.
      static LA_PelMatrix* create( PEL_Object* a_owner,
                                   size_t a_nb_rows,
                                   size_t a_nb_cols ) ;

      virtual LA_PelMatrix* create_matrix( PEL_Object* a_owner ) const ;

   //-- Characteristics

      virtual size_t allocated_memory( void ) const ;

   //-- Distributed processing

      virtual bool is_desynchronizable( void ) const ;

   //-- Access

      virtual size_t nb_stored_items( void ) const ;

      virtual size_t nb_rows( void ) const ;

      virtual size_t nb_cols( void ) const ;

      virtual double item( size_t i, size_t j ) const ;

      virtual LA_MatrixIterator* create_stored_item_iterator(
                                               PEL_Object* a_owner ) const ;
   //-- Element change

      virtual void nullify_row( size_t i ) ;

      virtual void set_stored_items( double val ) ;

      virtual void set_item( size_t i, size_t j, double x ) ;

      virtual void add_to_item( size_t i, size_t j, double x ) ;

      virtual void scale( double alpha ) ;

   //-- BLAS level 2 : matrix-vector operators

      virtual void multiply_vec_then_add(
                           LA_Vector const* x, LA_Vector* y,
                           double alpha = 1.0, double beta = 0.0 ) const ;

      virtual void tr_multiply_vec_then_add(
                           LA_Vector const* x, LA_Vector* y,
                           double alpha = 1.0, double beta = 0.0 ) const ;


   //-- BLAS level 3 : matrix-matrix operators//

      virtual void set( LA_Matrix const* A, bool same_pattern ) ;

      // IMPLEMENTATION : provided if `A' and `B' are both `LA_PelMatrix::'
      // objects. Use of `LA_SeqMatrix::' implementation otherwise.
      virtual void add_Mat_Mat( LA_Matrix const* A,
                                LA_Matrix const* B,
                                double alpha = 1.0 ) ;

      // IMPLEMENTATION : provided if `A' and `B' are `LA_PelMatrix::' objects,
      // use of `LA_SeqMatrix::' implementation otherwise.
      virtual void add_Mat_tMat( LA_Matrix const* A,
                                 LA_Matrix const* B,
                                 double alpha = 1.0 ) ;

   //-- System factorization

      virtual void factorize_MILU0( bool modified, double piv_min ) ;

      virtual void solve_LU( LA_SeqVector const* rhs,
                             LA_SeqVector* sol ) const ;

      virtual void relax( double omega,
                          LA_SeqMatrix::relaxation_mode mode,
                          LA_SeqVector const* omega_inv_diag,
                          LA_SeqVector const* rhs,
                          LA_SeqVector* sol ) const ;

   //-- Hidden

      //?????????????????????????????????????????????????????????????????
      // Ces definitions sont rendues publiques a cause d'un bug du
      // compilateur Sun CC5.0 (probleme de scope).
      //-----------------------------------------------------------------
      struct RowElm
      {
         size_t iCol ;
         double xVal ;
         RowElm* next ;
      } ;

   protected: //--------------------------------------------------------

   private: //----------------------------------------------------------

     ~LA_PelMatrix( void ) ;
      LA_PelMatrix( LA_PelMatrix const& other ) ;
      LA_PelMatrix& operator=( LA_PelMatrix const& other ) ;

      LA_PelMatrix( PEL_Object* a_owner,
                    size_t a_nb_rows,
                    size_t a_nb_cols ) ;

      virtual void re_initialize_with_global_sizes( size_t a_nb_rows,
                                                    size_t a_nb_cols ) ;

      // Bucket implementation of RowElm allocator
      // A RowElmBucket is a group of RowElm being allocated once time.
      // Then, create_element method distributes RowElm elements when needed.
      // Finally, the RowElm are group deleted by destroy_all_elements
      struct RowElmBucket
      {
         RowElm elem[128] ;
         RowElmBucket * next ;
         size_t used ;
      } ;

      RowElm* create_element( void ) ;

      void destroy_all_elements( void ) ;

      void init_allocating( void ) ;

   //-- Plug in

      LA_PelMatrix( void ) ;

      virtual LA_PelMatrix* create_replica(
                                        PEL_Object* a_owner,
                                        PEL_ModuleExplorer const* exp ) const ;

   //--BLAS level 3 : matrix-matrix operators

      virtual void add_Mat_Mat_IMP( LA_PelMatrix const* A,
                                    LA_PelMatrix const* B,
                                    double alpha ) ;

      virtual void add_Mat_tMat_IMP( LA_PelMatrix const* A,
                                     LA_PelMatrix const* B,
                                     double alpha ) ;

   //-- Distributed processing

      void make_desynchronizable( void ) ;

   //-- Friend

      friend class LA_PelMatrixIterator ;

   //-- Preconditions, Postconditions, Invariant

      virtual bool invariant( void ) const ;

   //-- Class attributes

      static LA_PelMatrix const* PROTOTYPE ;

   //-- Attributes

      bool UNSYNCHRO ;

      // Matrix dimensions :
      size_t NB_ROWS ;
      size_t NB_COLS ;

      RowElm** ROW_TABLE ;
      RowElmBucket* FIRST_BUCKET ;
      RowElmBucket* CURRENT_BUCKET ;

} ;

#endif
