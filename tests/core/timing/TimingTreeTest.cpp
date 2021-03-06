//======================================================================================================================
//
//  This file is part of waLBerla. waLBerla is free software: you can 
//  redistribute it and/or modify it under the terms of the GNU General Public
//  License as published by the Free Software Foundation, either version 3 of 
//  the License, or (at your option) any later version.
//  
//  waLBerla is distributed in the hope that it will be useful, but WITHOUT 
//  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or 
//  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License 
//  for more details.
//  
//  You should have received a copy of the GNU General Public License along
//  with waLBerla (see COPYING.txt). If not, see <http://www.gnu.org/licenses/>.
//
//! \file TimingPoolTest.cpp
//! \ingroup core
//! \author Martin Bauer <martin.bauer@fau.de>
//
//======================================================================================================================

#include "core/debug/TestSubsystem.h"
#include "core/logging/Logging.h"
#include "core/mpi/Environment.h"
#include "core/timing/StaticPolicy.h"
#include "core/timing/TimingTree.h"

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread/thread.hpp>

#include <iostream>

using namespace walberla;

void mssleep(unsigned int ms)
{
   timing::StaticPolicy::addTime( ms * 1e-3);
}

int main( int argc, char ** argv )
{
   debug::enterTestMode();

#ifdef WALBERLA_BUILD_WITH_BOOST_THREAD

   mpi::Environment mpiEnv(argc, argv);
   WALBERLA_UNUSED( mpiEnv );

   const unsigned int rank = static_cast<unsigned int> ( MPIManager::instance()->worldRank() );

   timing::TimingTree<timing::StaticPolicy> tt;
   timing::StaticPolicy::setTime(0);

   tt.start("A");
   mssleep(100 * rank);
   tt.start("AA");
   mssleep(100 * rank);
   tt.stop("AA");
   tt.start("AB");
   tt.start("ABA");
   mssleep(100 * rank);
   tt.stop("ABA");
   tt.start("ABB");
   mssleep(100 * rank);
   tt.stop("ABB");
   tt.stop("AB");
   tt.start("AC");
   mssleep(100 * rank);
   tt.start("ACA");
   mssleep(100 * rank);
   tt.stop("ACA");
   tt.stop("AC");
   tt.stop("A");

   // check copy constructor
   timing::TimingTree<timing::StaticPolicy> tt2(tt);
   // check assignment operator
   timing::TimingTree<timing::StaticPolicy> tt3;
   tt3 = tt;

   tt2 = tt.getReduced( timing::REDUCE_TOTAL, 0 );
   tt2 = tt.getReduced( timing::REDUCE_TOTAL, 1 );

   tt2 = tt.getReduced( timing::REDUCE_MIN, -1 );
   {
   const auto& data = tt2.getRawData();
   WALBERLA_CHECK_FLOAT_EQUAL( data.tree_.at("A").timer_.total(), (1.8) );
   WALBERLA_CHECK_FLOAT_EQUAL( data.tree_.at("A").tree_.at("AA").timer_.total(), (0.3) );

   WALBERLA_CHECK_FLOAT_EQUAL( tt["A.AB.ABB"].total(), (0.100 * rank), "total time: " << tt["A.AB.ABB"].total() );
   }

   tt2 = tt.getReduced( timing::REDUCE_MAX, -1 );
   {
   const auto& data = tt2.getRawData();
   WALBERLA_CHECK_FLOAT_EQUAL( data.tree_.at("A").timer_.total(), (1.8) );
   WALBERLA_CHECK_FLOAT_EQUAL( data.tree_.at("A").tree_.at("AA").timer_.total(), (0.3) );

   WALBERLA_CHECK_FLOAT_EQUAL( tt["A.AB.ABB"].total(), (0.100 * rank), "total time: " << tt["A.AB.ABB"].total() );
   }

   tt2 = tt.getReduced( timing::REDUCE_AVG, -1 );
   {
   const auto& data = tt2.getRawData();
   WALBERLA_CHECK_FLOAT_EQUAL( data.tree_.at("A").timer_.total(), (1.8) );
   WALBERLA_CHECK_FLOAT_EQUAL( data.tree_.at("A").tree_.at("AA").timer_.total(), (0.3) );

   WALBERLA_CHECK_FLOAT_EQUAL( tt["A.AB.ABB"].total(), (0.100 * rank), "total time: " << tt["A.AB.ABB"].total() );
   }

   tt2 = tt.getReduced( timing::REDUCE_TOTAL, -1 );
   {
   const auto& data = tt2.getRawData();
   WALBERLA_CHECK_FLOAT_EQUAL( data.tree_.at("A").timer_.total(), (1.8) );
   WALBERLA_CHECK_FLOAT_EQUAL( data.tree_.at("A").tree_.at("AA").timer_.total(), (0.3) );

   WALBERLA_CHECK_FLOAT_EQUAL( tt["A.AB.ABB"].total(), (0.100 * rank), "total time: " << tt["A.AB.ABB"].total() );
   }

   WALBERLA_ROOT_SECTION()
   {
//      std::cout << tt;
//      std::cout << tt2;
//      std::cout << tt3;
   }
#endif

   return 0;
}




