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

#ifndef PEL_COMMUNICATOR_HH
#define PEL_COMMUNICATOR_HH

#include <PEL_Object.hh>
#include <PEL_Data.hh>

class PEL_DoubleComparator ;
class PEL_ObjectRegister ;

class size_t_vector ;
class size_t_array2D ;
class intVector ;
class intArray2D ;
class doubleVector ;
class doubleArray2D ;
class boolVector ;
class boolArray2D ;
class stringVector ;

#ifdef max
#undef max
#endif
#ifdef min
#undef min
#endif

/*
Inter-process communicators.

Processes within a communicator are assigned numbers from 0 to 
`::nb_ranks()'-1.
*/

class PEL_EXPORT PEL_Communicator : public PEL_Object
{
   public: //-----------------------------------------------------------

   //-- Instance delivery and initialization

      // Return instance of self of name `a_name'.
      static PEL_Communicator const* object( std::string const& a_name ) ;
      
   //-- Characteristics
      
      // size of the world
      virtual size_t nb_ranks( void ) const = 0 ;
      
      // rank of `self' between 0 to `::nb_ranks()'-1
      virtual size_t rank( void ) const = 0 ;
      
      // name of `self'
      std::string const& name( void ) const ;
      
  //-- Point-to-point blocking communication(500.)
      
      void send( size_t dest, size_t value ) const ;
      void receive( size_t src, size_t& value ) const ;

      void send( size_t dest, size_t_vector const& value ) const ;
      void receive( size_t src, size_t_vector& value ) const ;

      void send( size_t dest, size_t_array2D const& value ) const ;
      void receive( size_t src, size_t_array2D& value ) const ;

      void send( size_t dest, int value ) const ;
      void receive( size_t src, int& value ) const ;

      void send( size_t dest, intVector const& value ) const ;
      void receive( size_t src, intVector& value ) const ;
      
      void send( size_t dest, intArray2D const& value ) const ;
      void receive( size_t src, intArray2D& value ) const ;

      void send( size_t dest, double value ) const ;
      void receive( size_t src, double& value ) const ;

      void send( size_t dest, doubleVector const& value ) const ;
      void receive( size_t src, doubleVector& value ) const ;
      
      void send( size_t dest, doubleArray2D const& value ) const ;
      void receive( size_t src, doubleArray2D& value ) const ;

      void send( size_t dest, bool value ) const ;
      void receive( size_t src, bool& value ) const ;

      void send( size_t dest, boolVector const& value ) const ;
      void receive( size_t src, boolVector& value ) const ;

      void send( size_t dest, boolArray2D const& value ) const ;
      void receive( size_t src, boolArray2D& value ) const ;

      void send(  size_t dest, std::string const& value ) const ;
      void receive(  size_t src, std::string& value ) const ;
      
      void send(  size_t dest, stringVector const& value ) const ;
      void receive(  size_t src, stringVector& value ) const ;
      
      virtual void send( size_t dest, int const* value, int nb ) const  = 0 ;
      virtual void receive( size_t src, int* value, int nb ) const = 0 ;

      virtual void send( size_t dest, double const* value, int nb ) const = 0 ;
      virtual void receive( size_t src, double* value, int nb ) const  = 0 ;
      
      virtual void send( size_t dest, char const* value, int nb ) const = 0 ;
      virtual void receive( size_t src, char* value, int nb ) const  = 0 ;

   //-- Point-to-point non-blocking communication(501.)
      
      virtual void* Isend( size_t dest, int const* value, int nb ) const = 0 ;
      virtual void* Ireceive( size_t src, int* value, int nb ) const = 0 ;
      
      virtual void* Isend( size_t dest, double const* value, int nb ) const = 0 ;
      virtual void* Ireceive( size_t src, double* value, int nb ) const = 0 ;

   //-- Collective communication:�broadcast(502.)

      // The process `root' sends a message containing its value of `value'
      // to all other processes (including itself). This message is received
      // by all the processes in `value'.
      // The argument `root' must have identical value on all processes. 
      void broadcast( size_t& value, size_t root=0 ) const ;
      
      // The process `root' sends a message containing its value of `values'
      // to all other processes (including itself). This message is received
      // by all the processes in `values'.
      // The argument `root' must have identical value on all processes. 
      void broadcast( size_t_vector& values, size_t root=0 ) const ;
      
      // The process `root' sends a message containing its value of `values'
      // to all other processes (including itself). This message is received
      // by all the processes in `values'.
      // The argument `root' must have identical value on all processes. 
      void broadcast( size_t_array2D& values, size_t root=0 ) const ;
      
      // The process `root' sends a message containing its value of `value'
      // to all other processes (including itself). This message is received
      // by all the processes in `value'.
      // The argument `root' must have identical value on all processes. 
      void broadcast( int& value, size_t root=0 ) const ;
      
      // The process `root' sends a message containing its value of `values'
      // to all other processes (including itself). This message is received
      // by all the processes in `values'.
      // The argument `root' must have identical value on all processes. 
      void broadcast( intVector& values, size_t root=0 ) const ;
      
      // The process `root' sends a message containing its value of `values'
      // to all other processes (including itself). This message is received
      // by all the processes in `values'.
      // The argument `root' must have identical value on all processes. 
      void broadcast( intArray2D& values, size_t root=0 ) const ;
      
      // The process `root' sends a message containing its value of `value'
      // to all other processes (including itself). This message is received
      // by all the processes in `value'.
      // The argument `root' must have identical value on all processes. 
      void broadcast( double& value, size_t root=0 ) const ;
      
      // The process `root' sends a message containing its value of `values'
      // to all other processes (including itself). This message is received
      // by all the processes in `values'.
      // The argument `root' must have identical value on all processes. 
      void broadcast( doubleVector& values, size_t root=0 ) const ;
      
      // The process `root' sends a message containing its value of `values'
      // to all other processes (including itself). This message is received
      // by all the processes in `values'.
      // The argument `root' must have identical value on all processes. 
      void broadcast( doubleArray2D& values, size_t root=0 ) const ;
    
      // The process `root' sends a message containing its value of `value'
      // to all other processes (including itself). This message is received
      // by all the processes in `value'.
      // The argument `root' must have identical value on all processes. 
      void broadcast( bool& value, size_t root=0 ) const ;
      
      // The process `root' sends a message containing its value of `values'
      // to all other processes (including itself). This message is received
      // by all the processes in `values'.
      // The argument `root' must have identical value on all processes. 
      void broadcast( boolVector& values, size_t root=0 ) const ;
      
      // The process `root' sends a message containing its value of `values'
      // to all other processes (including itself). This message is received
      // by all the processes in `values'.
      // The argument `root' must have identical value on all processes. 
      void broadcast( boolArray2D& values, size_t root=0 ) const ;
      
      // The process `root' sends a message containing its value of `value'
      // to all other processes (including itself). This message is received
      // by all the processes in `value'.
      // The argument `root' must have identical value on all processes. 
      void broadcast( std::string& value, size_t root=0 ) const ;
      
      // The process `root' sends a message containing its value of `value'
      // to all other processes (including itself). This message is received
      // by all the processes in `value'.
      // The argument `root' must have identical value on all processes. 
      void broadcast( stringVector& value, size_t root=0 ) const ;
      
   //-- Collective communication:�gather(503.)
      
      // Each process sends a message containing its value of `value'.
      // The emitted `::nb_ranks()' messages are concatenated in rank order, 
      // and the result is received by the process of rank `root' in `result' 
      // (`result'(i) is the content of `value' on the process of rank i).
      // All arguments are significant for the process of rank `root'.
      // The argument `result' is not significant and ignored for processes
      // of rank other than `root'.
      // The argument `root' must have identical value on all processes. 
      void gather( double value, doubleVector& result, size_t root=0 ) const ;

      // Each process sends a message containing its value of `value'.
      // The emitted `::nb_ranks()' messages are concatenated in rank order, 
      // and the result is received by the process of rank `root' in `result' 
      // (`result'(i) is the content of `value' on the process of rank i).
      // All arguments are significant for the process of rank `root'.
      // The argument `result' is not significant and ignored for processes
      // of rank other than `root'.
      // The argument `root' must have identical value on all processes. 
      void gather( int value, intVector& result, size_t root=0 ) const ;
      
      // Each process sends a message containing its value of `values'. 
      // The emitted `::nb_ranks()' messages are concatenated in rank order, 
      // and the result is received by the process of rank `root' in `result' 
      // All arguments are significant for the process of rank `root'.
      // The argument `result' is not significant and ignored for processes
      // of rank other than `root'.
      // The argument `root' must have identical value on all processes. 
      void gather( doubleVector const& values,
                   doubleVector& result,
                   size_t root=0 ) const ;
      
      // Each process sends a message containing its value of `values'. 
      // The emitted `::nb_ranks()' messages are concatenated in rank order, 
      // and the result is received by the process of rank `root' in `result' 
      // All arguments are significant for the process of rank `root'.
      // The argument `result' is not significant and ignored for processes
      // of rank other than `root'.
      // The argument `root' must have identical value on all processes. 
      void gather( intVector const& values,
                   intVector& result,
                   size_t root=0 ) const ;
      
   //-- Collective communication: gather to all(504.)

      // Each process sends a message containing its value of `value'.
      // The emitted `::nb_ranks()' messages are concatenated in rank order, 
      // and the result is received by all the processes in `result' 
      // (`result'(i) is the content of `value' on the process of rank i).
      void all_gather( int value, intVector& result ) const ;

      // Each process sends a message containing its value of `values'.
      // The emitted `::nb_ranks()' messages are concatenated in rank order, 
      // and the result is received by all the processes in `result'.
      void all_gather( intVector const& values, intVector& result ) const ;
      
      virtual void all_gather_v( double const* values,
                                 size_t nb,
                                 double* result,
                                 intVector const& partition,
                                 intVector const& start  ) const = 0 ;

   //-- Collective communication: all to all scatter/gather(511.)

      // Each process sends distinct data to each of the receivers.
      // Each process receives the table `result' where `result'(i) is
      // the `::rank'()-th element of the table `values' sent by the
      // process of rank i .
      void all_to_all( intVector const& values, intVector& result ) const ;

   //-- Collective communication: reductions(512.)
      
      // the logical and (operator &&) of all the `::nb_ranks()' data provided 
      // in the input `value' of each process 
      virtual bool boolean_and( bool value ) const ;

      // the logical or (operator ||) of all the `::nb_ranks()' data provided 
      // in the input `value' of each process 
      virtual bool boolean_or( bool value ) const ;
      
      // the sum (operator +) of all the `::nb_ranks()' data provided 
      // in the input `value' of each process 
      virtual double sum( double value ) const ;

      // Each process sends a message containing its value of `vec'
      // and receives the sum (operator +) item by item of all the  
      // `::nb_ranks()' vectors that were sent: on output, `vec'(i) is the
      // sum of the `::nb_ranks()' values of `vec'(i) sent by all the
      // processes.
      void sum_vector( doubleVector& vec ) const ;
      
      // Each process sends a message containing its value of `array'
      // and receives the sum (operator +) item by item of all the  
      // `::nb_ranks()' arrays that were sent: on output, `array'(i,j) is the
      // sum of the `::nb_ranks()' values of `array'(i,j) sent by all the
      // processes.
      void sum_array( doubleArray2D& array ) const ;
      
      // the smallest of all the `::nb_ranks()' data provided 
      // in the input `value' of each process 
      virtual double min( double value ) const ;

      // the smallest of all the `::nb_ranks()' data provided 
      // in the input `value' of each process 
      virtual size_t min( size_t value ) const ;

      // the largest of all the `::nb_ranks()' data provided 
      // in the input `value' of each process 
      virtual double max( double value ) const ;

      // the largest of all the `::nb_ranks()' data provided 
      // in the input `value' of each process 
      virtual size_t max( size_t value ) const ;

      // Is `val' equal to the same value in each process ?
      virtual bool same_value_everywhere( double val ) const = 0 ;

      // Is `val' equal to the same value in each process ?
      virtual bool same_value_everywhere( int val ) const = 0 ;
      
      /* 
      The columns provided by the input `coord' of each process are merged 
      and lexicographically sorted (starting from the second index). 
      The resulting array is received in `coord' by the process of rank 0
      (and not by the other processes). The position in this array of the
      columns that were sent are received by each process in `idx'.
      - `coord' should have the same first index limit on each process.
      - `coord' is emitted by each process, and is received only on 
                the process of rank 0.
      - `idx' is reinitialized on each process.
      - on output, 
        `idx'(j) = column index in `coord' received by the process of rank 0
                   of the j-th column of `coord' sent by the current process.
      */ 
      void merge( PEL_DoubleComparator const* dbl_comp,
                  doubleArray2D& coord, size_t_vector& idx ) const ;

   //-- Collective communication: synchronization(513.)
      
      virtual void barrier( void ) const = 0 ;

      virtual void wait( void* request ) const ;

   //-- Input - Output

      static void do_trace( void ) ;

      
   protected: //--------------------------------------------------------

   //-- Plug in

      virtual ~PEL_Communicator( void ) ;

      // registration of `self', calling it `a_name'.
      PEL_Communicator( std::string const& a_name ) ;

   //-- Collective communication:�broadcast
         
      virtual void broadcast( int* value, size_t nb, size_t root ) const = 0 ;
      
      virtual void broadcast( double* value, size_t nb, size_t root ) const = 0 ;
      
      virtual void broadcast( char* value, size_t nb, size_t root ) const = 0 ;
      
   //-- Collective communication:�gather
      
      virtual void gather( double const* value, size_t nb,
                           double* result, size_t root ) const = 0 ;
      
      virtual void gather( int const* value, size_t nb,
                           int* result, size_t root ) const = 0 ;
      
   //-- Collective communication: gather to all
            
      virtual void all_gather( int const* value, size_t nb,
                               int* result  ) const = 0 ;
      
   //-- Collective communication: all to all scatter/gather
      
      virtual void all_to_all( int const* value, size_t nb,
                               int* result  ) const = 0 ;
      
   //-- Collective communication: reductions

      virtual void sum_vector( double* values, int nb ) const ;
      
   //-- Preconditions, Postconditions, Invariant
      
      virtual bool nb_ranks_POST( size_t result ) const ;
      
      virtual bool rank_POST( size_t result ) const ;
      
      virtual bool send_PRE( size_t dest, int const* value, int nb ) const ;
      
      virtual bool send_PRE( size_t dest, double const* value, int nb ) const ;
      
      virtual bool send_PRE( size_t dest, char const* value, int nb ) const ;

      virtual bool receive_PRE( size_t src, int const* value, int nb ) const ;
      
      virtual bool receive_PRE( size_t src, double const* value, int nb ) const ;
      
      virtual bool receive_PRE( size_t src, char const* value, int nb ) const ;
      
      virtual bool min_POST( double result, double value ) const ;
      virtual bool min_POST( size_t result, size_t value ) const ;

      virtual bool max_POST( double result, double value ) const ;
      virtual bool max_POST( size_t result, size_t value ) const ;

      virtual bool sum_vector_PRE( double const* values, int nb ) const ;
      
   //-- Trace
      
      static bool trace( void ) ;
      
   private: //----------------------------------------------------------

      PEL_Communicator( void ) ;
      PEL_Communicator( PEL_Communicator const& other ) ;
      PEL_Communicator& operator=( PEL_Communicator const& other ) ;

      static PEL_ObjectRegister* plugins_map( void ) ;

      void convert( size_t_vector const& src, intVector& dest ) const ;
      void convert( intVector const& src, size_t_vector& dest ) const ;
      
      void convert( size_t_array2D const& src, intArray2D& dest ) const ;
      void convert( intArray2D const& src, size_t_array2D& dest ) const ;
      
      void convert( boolVector const& src, intVector& dest ) const ;
      void convert( intVector const& src, boolVector& dest ) const ;
      
      void convert( boolArray2D const& src, intArray2D& dest ) const ;
      void convert( intArray2D const& src, boolArray2D& dest ) const ;
      
   //-- Trace

      void print_method( void ) const ;
      
   //-- Attributes

      std::string MY_NAME ;
      static bool TRACE ;
} ;

#endif



