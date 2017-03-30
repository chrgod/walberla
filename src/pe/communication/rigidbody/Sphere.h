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
//! \file Sphere.h
//! \ingroup pe
//! \author Tobias Preclik
//! \author Sebastian Eibl <sebastian.eibl@fau.de>
//! \brief Marshalling of objects for data transmission or storage.
//
//======================================================================================================================

#pragma once


//*************************************************************************************************
// Includes
//*************************************************************************************************

#include "pe/communication/Instantiate.h"
#include "pe/communication/Marshalling.h"
#include "pe/rigidbody/Sphere.h"

namespace walberla {
namespace pe {
namespace communication {

struct SphereParameters : public GeomPrimitiveParameters {
   real_t radius_;
};

//*************************************************************************************************
/*!\brief Marshalling a sphere primitive.
 *
 * \param buffer The buffer to be filled.
 * \param obj The object to be marshalled.
 * \return void
 */
void marshal( mpi::SendBuffer& buffer, const Sphere& obj );
//*************************************************************************************************

//*************************************************************************************************
/*!\brief Unmarshalling a sphere primitive.
 *
 * \param buffer The buffer from where to read.
 * \param objparam The object to be reconstructed.
 * \param hasSuperBody False if body is not part of a union. Passed on to rigid body unmarshalling.
 * \return void
 */
void unmarshal( mpi::RecvBuffer& buffer, SphereParameters& objparam, bool hasSuperBody );
//*************************************************************************************************


template <>
inline BodyID instantiate<Sphere>( mpi::RecvBuffer& buffer, const math::AABB& domain, const math::AABB& block )
{
   SphereParameters subobjparam;
   unmarshal( buffer, subobjparam, false );
   correctBodyPosition(domain, block.center(), subobjparam.gpos_);
   SphereID obj = new Sphere( subobjparam.sid_, subobjparam.uid_, subobjparam.gpos_, subobjparam.rpos_, subobjparam.q_, subobjparam.radius_, subobjparam.material_, false, subobjparam.communicating_, subobjparam.infiniteMass_ );
   obj->setLinearVel( subobjparam.v_ );
   obj->setAngularVel( subobjparam.w_ );
   obj->MPITrait.setOwner( subobjparam.mpiTrait_.owner_ );
   return obj;
}

}  // namespace communication
}  // namespace pe
}  // namespace walberla