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

#include <FE_SplitSystem.hh>

#include <PEL_Error.hh>
#include <PEL_List.hh>
#include <PEL_ListIterator.hh>
#include <PEL_ModuleExplorer.hh>
#include <PEL_assertions.hh>

#include <PDE_DomainAndFields.hh>
#include <PDE_SetOfDomains.hh>

#include <FE_TimeIterator.hh>

#include <iostream>

using std::endl ;
using std::string ;

FE_SplitSystem const* FE_SplitSystem::PROTOTYPE = new FE_SplitSystem() ;

//-------------------------------------------------------------------------
FE_SplitSystem:: FE_SplitSystem( void )
//-------------------------------------------------------------------------
   : FE_OneStepIteration( "FE_SplitSystem" )
{
}

//-------------------------------------------------------------------------
FE_SplitSystem*
FE_SplitSystem:: create_replica( PEL_Object* a_owner,
                                 PDE_DomainAndFields const* dom,
                                 FE_SetOfParameters const* prms,
                                 PEL_ModuleExplorer* exp ) const
//-------------------------------------------------------------------------
{
   PEL_LABEL( "FE_SplitSystem:: create_replica" ) ;
   PEL_CHECK( create_replica_PRE( a_owner, dom, prms, exp ) ) ;

   FE_SplitSystem* result = new FE_SplitSystem( a_owner, dom, prms, exp ) ;

   PEL_CHECK( create_replica_POST( result, a_owner, dom, prms, exp ) ) ;
   return( result ) ;
}

//-------------------------------------------------------------------------
FE_SplitSystem:: FE_SplitSystem( PEL_Object* a_owner,
                                 PDE_DomainAndFields const* dom,
                                 FE_SetOfParameters const* prms,
                                 PEL_ModuleExplorer const* exp )
//-------------------------------------------------------------------------
   : FE_OneStepIteration( a_owner, dom, exp )
   , CMPS( PEL_List::create( this ) )
   , IT( 0 )
{
   PEL_ModuleExplorer* e = exp->create_subexplorer( 0, 
                                             "list_of_FE_OneStepIteration" ) ;
   e->start_module_iterator() ;
   for( ; e->is_valid_module() ; e->go_next_module() )
   {
      PEL_ModuleExplorer* ee = e->create_subexplorer( 0 ) ; 
      FE_OneStepIteration* cmp = 
                           FE_OneStepIteration::make( CMPS, dom, prms, ee ) ;
      CMPS->append( cmp ) ;
      ee->destroy() ; ee = 0 ;
   }
   e->destroy() ; e = 0 ;

   IT = PEL_ListIterator::create( this, CMPS ) ;
}

//-------------------------------------------------------------------------
FE_SplitSystem*
FE_SplitSystem:: create_replica( PEL_Object* a_owner,
                                 PDE_SetOfDomains const* sdoms,
                                 FE_SetOfParameters const* prms,
                                 PEL_ModuleExplorer* exp ) const
//-------------------------------------------------------------------------
{
   PEL_LABEL( "FE_SplitSystem:: create_replica" ) ;
   PEL_CHECK( create_replica_PRE( a_owner, sdoms, prms, exp ) ) ;

   FE_SplitSystem* result = 0 ;

   if( exp->has_entry( "domain" ) )
   {
      string const& dom_name = exp->string_data( "domain" ) ;
      PDE_DomainAndFields const* dom = sdoms->domain( dom_name ) ;
      result = create_replica( a_owner, dom, prms, exp ) ;
   }
   else
   {
      result = new FE_SplitSystem( a_owner, sdoms, prms, exp ) ;
   }

   PEL_CHECK( create_replica_POST( result, a_owner, sdoms, prms, exp ) ) ;
   return( result ) ;
}

//-------------------------------------------------------------------------
FE_SplitSystem:: FE_SplitSystem( PEL_Object* a_owner,
                                 PDE_SetOfDomains const* sdoms,
                                 FE_SetOfParameters const* prms,
                                 PEL_ModuleExplorer const* exp )
//-------------------------------------------------------------------------
   : FE_OneStepIteration( a_owner, sdoms, exp )
   , CMPS( PEL_List::create( this ) )
   , IT( 0 )
{
   PEL_ModuleExplorer* e = exp->create_subexplorer( 0, 
                                             "list_of_FE_OneStepIteration" ) ;
   e->start_module_iterator() ;
   for( ; e->is_valid_module() ; e->go_next_module() )
   {
      PEL_ModuleExplorer* ee = e->create_subexplorer( 0 ) ; 
      FE_OneStepIteration* cmp = 
                           FE_OneStepIteration::make( CMPS, sdoms, prms, ee ) ;
      CMPS->append( cmp ) ;
      ee->destroy() ; ee = 0 ;
   }
   e->destroy() ; e = 0 ;

   IT = PEL_ListIterator::create( this, CMPS ) ;
}

//-------------------------------------------------------------------------
FE_SplitSystem:: ~FE_SplitSystem( void ) 
//-------------------------------------------------------------------------
{
}

//-------------------------------------------------------------------------
void
FE_SplitSystem:: do_before_time_stepping( FE_TimeIterator const* t_it )
//-------------------------------------------------------------------------
{
   PEL_LABEL( "FE_SplitSystem:: do_before_time_stepping" ) ;
   PEL_CHECK_PRE( do_before_time_stepping_PRE( t_it ) ) ;

   start_total_timer( "FE_SplitSystem:: do_before_time_stepping" ) ;
   
   for( IT->start() ; IT->is_valid() ; IT->go_next() )
   {
      FE_OneStepIteration* pb = 
                           static_cast<FE_OneStepIteration*>( IT->item() ) ;
      pb->do_before_time_stepping( t_it ) ;
   }

   stop_total_timer() ;
}

//-------------------------------------------------------------------------
void
FE_SplitSystem:: do_before_inner_iterations_stage( 
                                             FE_TimeIterator const* t_it )
//-------------------------------------------------------------------------
{
   PEL_LABEL( "FE_SplitSystem:: do_before_inner_iterations_stage" ) ;
   PEL_CHECK_PRE( do_before_inner_iterations_stage_PRE( t_it ) ) ;

   start_total_timer( "FE_SplitSystem:: do_before_inner_iterations_stage" ) ;

   for( IT->start() ; IT->is_valid() ; IT->go_next() )
   {
      FE_OneStepIteration* pb = 
                           static_cast<FE_OneStepIteration*>( IT->item() ) ;
      pb->do_before_inner_iterations_stage( t_it ) ;
   }

   stop_total_timer() ;

   PEL_CHECK_POST( do_before_inner_iterations_stage_POST( t_it ) ) ;
}

//-------------------------------------------------------------------------
void
FE_SplitSystem:: do_inner_iterations_stage( FE_TimeIterator const* t_it )
//-------------------------------------------------------------------------
{
   PEL_LABEL( "FE_SplitSystem:: do_inner_iterations_stage" ) ;
   PEL_CHECK_PRE( do_inner_iterations_stage_PRE( t_it ) ) ;

   start_total_timer( "FE_SplitSystem:: do_inner_iterations_stage" ) ;

   IT->start() ;
   for( ; !inner_iterations_stage_failed() && IT->is_valid() ; IT->go_next() )
   {
      FE_OneStepIteration* pb = 
                           static_cast<FE_OneStepIteration*>( IT->item() ) ;
      pb->do_inner_iterations_stage( t_it ) ;
      if( pb->inner_iterations_stage_failed() )
      {
         notify_inner_iterations_stage_failure() ;
      }
   }

   stop_total_timer() ;
}

//-------------------------------------------------------------------------
void
FE_SplitSystem:: do_one_inner_iteration( FE_TimeIterator const* t_it )
//-------------------------------------------------------------------------
{
   PEL_LABEL( "FE_SplitSystem:: do_one_inner_iteration" ) ;
   PEL_CHECK_PRE( do_one_inner_iteration_PRE( t_it ) ) ;

   PEL_Error::object()->raise_plain( "should not be called" ) ;
}

//-------------------------------------------------------------------------
bool
FE_SplitSystem:: inner_iterations_are_completed( 
                                         FE_TimeIterator const* t_it ) const
//-------------------------------------------------------------------------
{
   PEL_LABEL( "FE_SplitSystem:: inner_iterations_are_completed" ) ;
   PEL_CHECK_PRE( inner_iterations_are_completed_PRE( t_it ) ) ;

   return( true ) ;
}

//-------------------------------------------------------------------------
void
FE_SplitSystem:: do_after_inner_iterations_stage( FE_TimeIterator const* t_it )
//-------------------------------------------------------------------------
{
   PEL_LABEL( "FE_SplitSystem:: do_after_inner_iterations_stage" ) ;
   PEL_CHECK_PRE( do_after_inner_iterations_stage_PRE( t_it ) ) ;

   start_total_timer( "FE_SplitSystem:: do_after_inner_iterations_stage" ) ;
   
   for( IT->start() ; IT->is_valid() ; IT->go_next() )
   {
      FE_OneStepIteration* pb = 
                           static_cast<FE_OneStepIteration*>( IT->item() ) ;
      pb->do_after_inner_iterations_stage( t_it ) ;
   }
   
   stop_total_timer() ;
}

//-------------------------------------------------------------------------
void
FE_SplitSystem:: do_after_time_adaptation( FE_TimeIterator const* t_it )
//-------------------------------------------------------------------------
{
   PEL_LABEL( "FE_SplitSystem:: do_after_time_adaptation" ) ;
   PEL_CHECK_PRE( do_after_time_adaptation_PRE( t_it ) ) ;

   start_total_timer( "FE_SplitSystem:: do_after_time_adaptation" ) ;
   
   for( IT->start() ; IT->is_valid() ; IT->go_next() )
   {
      FE_OneStepIteration* pb =  
                           static_cast<FE_OneStepIteration*>( IT->item() ) ;
      pb->do_after_time_adaptation( t_it ) ;
   }
   
   stop_total_timer() ;
}

//-------------------------------------------------------------------------
void
FE_SplitSystem:: do_additional_savings( FE_TimeIterator const* t_it,
                                        PDE_ResultSaver* rs )
//-------------------------------------------------------------------------
{
   PEL_LABEL( "FE_SplitSystem:: do_additional_savings" ) ;
   PEL_CHECK_PRE( do_additional_savings_PRE( t_it, rs ) ) ;

   start_total_timer( "FE_SplitSystem:: do_additional_savings" ) ;

   for( IT->start() ; IT->is_valid() ; IT->go_next() )
   {
      FE_OneStepIteration* pb = 
                           static_cast<FE_OneStepIteration*>( IT->item() ) ;
      pb->do_additional_savings( t_it, rs ) ;
   }

   stop_total_timer() ;
}

//-------------------------------------------------------------------------
void
FE_SplitSystem:: do_after_time_stepping( void )
//-------------------------------------------------------------------------
{
   PEL_LABEL( "FE_SplitSystem:: do_after_time_stepping" ) ;

   start_total_timer( "FE_SplitSystem:: do_after_time_stepping" ) ;

   for( IT->start() ; IT->is_valid() ; IT->go_next() )
   {
      FE_OneStepIteration* pb = 
                           static_cast<FE_OneStepIteration*>( IT->item() ) ;
      pb->do_after_time_stepping() ;
   }

   stop_total_timer() ;
}

//----------------------------------------------------------------------------
void
FE_SplitSystem:: print_additional_times( std::ostream& os,
                                         size_t indent_width ) const
//----------------------------------------------------------------------------
{
   PEL_LABEL( "FE_SplitSystem:: print_additional_times" ) ;

   for( IT->start() ; IT->is_valid() ; IT->go_next() )
   {
      FE_OneStepIteration* pb = 
                           static_cast<FE_OneStepIteration*>( IT->item() ) ;
      pb->print_additional_times( os, indent_width ) ;
   }
}

//----------------------------------------------------------------------------
void
FE_SplitSystem:: notify_inner_iterations_stage_failure( void )
//----------------------------------------------------------------------------
{
   PEL_LABEL( "FE_SplitSystem:: notify_inner_iterations_stage_failure" ) ;
   PEL_CHECK_PRE( notify_inner_iterations_stage_failure_PRE() ) ;

   FE_OneStepIteration::notify_inner_iterations_stage_failure() ;
   PEL_ListIterator* it = PEL_ListIterator::create( 0, CMPS ) ;
   for( it->start() ; it->is_valid() ; it->go_next() )
   {
      FE_OneStepIteration* pb = 
                           static_cast<FE_OneStepIteration*>( it->item() ) ;
      if( !pb->inner_iterations_stage_failed() )
      {
         pb->notify_inner_iterations_stage_failure() ;
      }
   }
   it->destroy() ; it = 0 ;
   
   PEL_CHECK_POST( notify_inner_iterations_stage_failure_POST() ) ;
}

//----------------------------------------------------------------------------
void
FE_SplitSystem:: reset_after_inner_iterations_stage_failure( void )
//----------------------------------------------------------------------------
{
   PEL_LABEL( "FE_SplitSystem:: reset_after_inner_iterations_stage_failure" ) ;
   PEL_CHECK( reset_after_inner_iterations_stage_failure_PRE() ) ;

   FE_OneStepIteration::reset_after_inner_iterations_stage_failure() ;
   for( IT->start() ; IT->is_valid() ; IT->go_next() )
   {
      FE_OneStepIteration* pb = 
                           static_cast<FE_OneStepIteration*>( IT->item() ) ;
      pb->reset_after_inner_iterations_stage_failure() ;
   }
   
   PEL_CHECK_POST( reset_after_inner_iterations_stage_failure_POST() ) ;
}

//----------------------------------------------------------------------------
void
FE_SplitSystem:: adapt_time_iterator( FE_TimeIteratorAdapter* t_adapter )
//----------------------------------------------------------------------------
{
   PEL_LABEL( "FE_SplitSystem:: adapt_time_iterator" ) ;
   PEL_CHECK_PRE( adapt_time_iterator_PRE( t_adapter ) ) ;

   for( IT->start() ; IT->is_valid() ; IT->go_next() )
   {
      FE_OneStepIteration* pb = 
                           static_cast<FE_OneStepIteration*>( IT->item() ) ;
      pb->adapt_time_iterator( t_adapter ) ;
   }
}

//-------------------------------------------------------------------------
void
FE_SplitSystem:: add_storable_objects( PEL_ListIdentity* list )
//-------------------------------------------------------------------------
{
   PEL_LABEL( "FE_SplitSystem:: add_storable_objects" ) ;

   for( IT->start() ; IT->is_valid() ; IT->go_next() )
   {
      FE_OneStepIteration* pb = 
                           static_cast<FE_OneStepIteration*>( IT->item() ) ;
      pb->add_storable_objects( list ) ;
   }
}

//-------------------------------------------------------------------------
void
FE_SplitSystem:: print( std::ostream& os, size_t indent_width ) const
//-------------------------------------------------------------------------
{
   PEL_LABEL( "FE_SplitSystem:: print" ) ;

   FE_OneStepIteration:: print( os, indent_width ) ;

   for( IT->start() ; IT->is_valid() ; IT->go_next() )
   {
      FE_OneStepIteration* pb = 
                           static_cast<FE_OneStepIteration*>( IT->item() ) ;
      pb->print( os, indent_width+3 ) ;
   }
}