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
//! \file OpenMPBufferSystem.h
//! \ingroup core
//! \author Martin Bauer <martin.bauer@fau.de>
//
//======================================================================================================================

#pragma once

#include "BufferSystem.h"

#include <boost/function.hpp>
#include <map>


namespace walberla {
namespace mpi {



//**********************************************************************************************************************
/*! Wrapper around BufferSystem for OpenMP parallel MPI communication
*
* \ingroup mpi
*
* Manages a set of sending (packing) and receiving (unpacking) functions for all communication partners.
* These functions are executed in parallel and are not allowed to influence each other.
*
* When running multiple BufferSystems concurrently different MPI tags have to be used
* for the systems: the tag can be passed in the constructor.
*/
//**********************************************************************************************************************
class OpenMPBufferSystem
{
public:

   OpenMPBufferSystem( const MPI_Comm & communicator, int tag = 0,
                       bool _serialSends = false, bool _serialRecvs = false );


   void enforceSerialSends( bool val ) { serialSends_ = val; }
   void enforceSerialRecvs( bool val ) { serialRecvs_ = val; }


   void setReceiverInfo( bool _sizeChangesEverytime ) { dirty_ = true; sizeChangesEverytime_ = _sizeChangesEverytime; }

   bool isSizeCommunicatedInNextStep() const          { return bs_.isSizeCommunicatedInNextStep(); }

   void addReceivingFunction( MPIRank rank, boost::function<void ( RecvBuffer & buf ) > recvFunction );
   void addSendingFunction  ( MPIRank rank, boost::function<void ( SendBuffer & buf ) > sendFunction );

   void clearReceivingFunctions() { dirty_ = true; recvFunctions_.clear(); }
   void clearSendingFunctions()   { dirty_ = true; sendFunctions_.clear(); sendRanks_.clear(); }

   void startCommunication();
   void wait();

   bool sizeChangesEverytime() const { return sizeChangesEverytime_; }
   bool serialSends() const          { return serialSends_; }
   bool serialRecvs() const          { return serialRecvs_; }


private:
   BufferSystem bs_;

   bool dirty_;

   void setupBufferSystem();

   bool serialSends_;
   bool serialRecvs_;

   bool sizeChangesEverytime_;

   std::map<MPIRank, boost::function<void ( RecvBuffer & )> > recvFunctions_;


   std::vector<MPIRank> sendRanks_;
   std::vector< boost::function<void ( SendBuffer & )> > sendFunctions_;

   void startCommunicationOpenMP();
   void startCommunicationSerial();

   void waitOpenMP();
   void waitSerial();
};



} // namespace mpi
} // namespace walberla
