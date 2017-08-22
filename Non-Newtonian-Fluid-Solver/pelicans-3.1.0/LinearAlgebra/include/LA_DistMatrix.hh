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

#ifndef LA_DIST_MATRIX_HH
#define LA_DIST_MATRIX_HH

#include <LA_Matrix.hh>

#include <LA.hh>
#include <LA_DistVector.hh>
#include <size_t_vector.hh>


class PEL_DistributedPartition ;

class LA_BlockSeqMatrix ;
class LA_SeqVector ;
class LA_CRSmatrix ;
class LA_DistScatter ;
class LA_SeqMatrix ;
class LA_Vector ;

/*
  Built_in row-distributed matrices of PELICANS framework.

  This matrix distributes its rows over all processes, that means that each
  instance contains few rows but entire corresponding columns.

  It is implemented by using two layers of `LA_BlockSeqMatrix::', one for items it
  owns and another for items which are owned by other processes.

  The `LA_BlockSeqMatrix::' matrices use usually `LA_PelMatrix::' for each block
  sub-matrices but other kind of `LA_SeqMatrix::' can be set by data deck.

  Moreover, two different kinds of `LA_SeqMatrix::' can be used, one for the primary
  assembling and another for further calculation, after the first synchronization.

  Such a matrix can be instantiated in PELICANS datafiles with following module :

  MODULE LA_Matrix

     concrete_name = "LA_DistMatrix"

     [�nb_rows = <n> ]

     [ nb_cols = <m> ]

     [ local_row = <true|false> ]

     [ local_col = <true|false> ]

     [ MODULE initial_block_prototype
         ... a valid LA_SeqMatrix
       END MODULE initial_block_prototype ]

     [ MODULE final_block_prototype
         ... a valid LA_SeqMatrix
       END MODULE final_block_prototype ]

  END MODULE LA_Matrix

PUBLISHED
*/

class PEL_EXPORT LA_DistMatrix : public LA_Matrix
{
   public: //-----------------------------------------------------------

   //-- Instance delivery and initialization

      static LA_DistMatrix* create( PEL_Object* a_owner,
                              size_t a_nb_rows, size_t a_nb_cols,
                              size_t a_nb_local_rows, size_t a_nb_local_cols,
                              LA::DistributionStrategy dist_strat,
                              bool verbose = false ) ;

      virtual void re_initialize(
                              size_t a_nb_rows, size_t a_nb_cols,
                              size_t a_nb_local_rows = PEL::bad_index(),
                              size_t a_nb_local_cols = PEL::bad_index() ) ;

      virtual LA_DistMatrix* create_matrix( PEL_Object* a_owner ) const ;

      virtual LA_DistVector* create_vector( PEL_Object* a_owner ) const ;

   //-- Characteristics

      virtual LA_Implementation const* implementation( void ) const ;

      // memory allocated by `self'
      size_t allocated_memory( void ) const ;

   //-- Distributed processing

      virtual bool is_desynchronizable( void ) const ;

      virtual void synchronize( void ) ;

      virtual void stop_local_modifs( void ) ;

      virtual PEL_DistributedPartition const* row_distribution( void ) const ;
      virtual PEL_DistributedPartition const* col_distribution( void ) const ;

      // prototype of sparse matrix used to store local items
      LA_SeqMatrix const* block_prototype( void ) const ;

      // Modify the prototype of sparse matrix used to store local items.
      void set_block_prototype( LA_SeqMatrix const* a_proto ) ;

      LA_SeqMatrix* diagonal_block_matrix( void ) const ;

   //-- Access

      virtual size_t nb_stored_items( void ) const ;

      virtual size_t nb_rows( void ) const ;

      virtual size_t nb_cols( void ) const ;

      virtual double item( size_t i, size_t j ) const ;

      virtual void extract_diag( LA_Vector* diag ) const ;

      virtual LA_SeqMatrix* create_local_matrix( PEL_Object* a_owner ) const ;

      virtual LA_MatrixIterator* create_stored_item_iterator(
                                                 PEL_Object* a_owner ) const ;

   //-- Element change

      virtual void nullify( void ) ;

      virtual void set_item( size_t i, size_t j, double x ) ;

      virtual void add_to_item( size_t i, size_t j, double x ) ;

      virtual void scale( double alpha ) ;

      double two_norm( void ) const ;

   //-- BLAS level 2 : matrix-vector operators

      virtual void multiply_vec_then_add(
                               LA_Vector const* x,
                               LA_Vector* y,
                               double alpha = 1.0, double beta = 0.0 ) const ;

      virtual void tr_multiply_vec_then_add(
                               LA_Vector const* x,
                               LA_Vector* y,
                               double alpha = 1.0, double beta = 0.0 ) const ;

      virtual void scale_as_diag_mat_mat( LA_Vector const* lvec ) ;

      virtual void scale_as_mat_diag_mat( LA_Vector const* rvec ) ;

      virtual void add_to_diag( LA_Vector const* vec ) ;

   //-- BLAS level 3 : matrix-matrix operators

      // Reinitialize by copying all coefficients of `A'.
      virtual void set( LA_Matrix const* A, bool same_pattern=false ) ;

      virtual void add_Mat( LA_Matrix const* A,
                            double alpha = 1.0,
                            bool same_pattern=false ) ;

      virtual void add_tMat( LA_Matrix const* A, double alpha = 1.0 ) ;

      virtual void add_Mat_Mat( LA_Matrix const* A, LA_Matrix const* B,
                                double alpha = 1.0 ) ;

   //-- Input - Output

      virtual void print( std::ostream& os, size_t indent_width ) const ;

   protected: //--------------------------------------------------------

   private: //----------------------------------------------------------

      LA_DistMatrix( LA_DistMatrix const& other ) ;
      LA_DistMatrix& operator=( LA_DistMatrix const& other ) ;

      LA_DistMatrix( PEL_Object* a_owner,
                     size_t a_nb_rows, size_t a_nb_cols,
                     size_t a_nb_local_rows, size_t a_nb_local_cols,
                     LA::DistributionStrategy dist_strat,
                     LA_SeqMatrix* initial_prototype,
                     LA_SeqMatrix* final_prototype,
                     bool verbose ) ;

      LA_DistMatrix( PEL_Object* a_owner,
                     LA_DistMatrix const* other ) ;

     ~LA_DistMatrix( void ) ;

    //-- Plug in

      LA_DistMatrix( void ) ;

      virtual LA_DistMatrix* create_replica(
         PEL_Object* a_owner, PEL_ModuleExplorer const* exp ) const ;

   //-- Distributed processing

      static PEL_Communicator const* communicator( void ) ;

      static LA_CRSmatrix const* convert_if_needed( LA_SeqMatrix const* mat ) ;

      void send_submatrix( size_t N, size_t i ) const ;
      void wait_send_submatrix( size_t N, size_t i ) const ;
      void receive_submatrix( size_t N, size_t i,
                              LA::SyncState effective_mode ) ;

      void build_scatter( void ) const ;

   //-- BLAS level 2 : matrix-vector operators

      void multiply_vec_then_add_IMP(
                               LA_DistVector const* x,
                               LA_DistVector* y,
                               double alpha = 1.0, double beta = 0.0 ) const ;

      void tr_multiply_vec_then_add_IMP(
                               LA_DistVector const* x,
                               LA_DistVector* y,
                               double alpha = 1.0, double beta = 0.0 ) const ;

   //-- Preconditions, Postconditions, Invariant

      virtual bool invariant( void ) const ;

      virtual bool implementation_POST(
                                LA_Implementation const* result ) const ;

   //-- class attributes

      static LA_DistMatrix const* PROTOTYPE ;

   //-- Attributes

      bool const VERBOSE ;

      size_t NB_ROWS ;
      size_t NB_COLS ;

      // Distributed partition:
      // (only rows between FIRST_LOCAL_ROW and LAST_LOCAL_ROW are owned
      //  by self and stored in local sequential matrix, others are owned
      //  by other processes)
      PEL_DistributedPartition* const ROW_DIST ;
      PEL_DistributedPartition* const COL_DIST ;
      size_t FIRST_LOCAL_ROW ;
      size_t  LAST_LOCAL_ROW ;

      // Local matrix owned/stored by current process:
      LA_SeqMatrix* LOCAL_DIAG_MATRIX ;
      LA_SeqMatrix* LOCAL_NODIAG_MATRIX ;
      LA_BlockSeqMatrix* NON_LOCAL_MATRIX ;

      // Scatter:
      LA_SeqVector* GLOBAL_VEC ;
      mutable bool SCATTER_OK ;
      LA_DistScatter* SCATTER ;

      // Sequential block prototype for local elements:
      LA_SeqMatrix const* SEQ_PROTO ;
      LA_SeqMatrix const* const INITIAL_PROTO ;
      LA_SeqMatrix const* const FINAL_PROTO ;
      bool HAS_SUBST_PROTO ;

      bool INITIALIZED ;
      bool IN_PLACE ;
      bool SQUARE ;
} ;

#endif
