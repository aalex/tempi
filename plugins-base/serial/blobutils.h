/*
 * Copyright (C) 2011 Alexandre Quessy
 * Copyright (C) 2011 Michal Seta
 * Copyright (C) 2012 Nicolas Bouillot
 * Copyright (C) 2012 Emmanuel Durand
 *
 * This file is part of Tempi-plugins-base.
 *
 * This program is free software; you can redistither version 3 of the License, or
 * (at your option) any later version.
 * 
 * Tempi is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with Tempi.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * @file
 * Some utilities to deal with blobs.
 */
#ifndef __TEMPI_SERIAL_BLOBUTILS_H__
#define __TEMPI_SERIAL_BLOBUTILS_H__

#include <atom/blobvalue.h>

namespace tempi {
namespace plugins_base {
namespace blobutils {

atom::BlobValue::ptr createEmptyBlob();

} // end of namespace
} // end of namespace
} // end of namespace

#endif // ifndef

