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
//! \file all.h
//! \ingroup core
//! \author Florian Schornbaum <florian.schornbaum@fau.de>
//! \brief Collective header file for module core
//
//======================================================================================================================

#pragma once

#include "Broadcast.h"
#include "BufferDataTypeExtensions.h"
#include "BufferSizeTrait.h"
#include "BufferSystem.h"
#include "Environment.h"
#include "Gather.h"
#include "Gatherv.h"
#include "MPIManager.h"
#include "MPITextFile.h"
#include "MPIWrapper.h"
#include "OpenMPBufferSystem.h"
#include "RecvBuffer.h"
#include "Reduce.h"
#include "SendBuffer.h"
#include "SetReduction.h"
#include "Tokenizing.h"