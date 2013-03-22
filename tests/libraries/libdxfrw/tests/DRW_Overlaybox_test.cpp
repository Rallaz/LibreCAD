/******************************************************************************
**  libDXFrw - Library to read/write DXF files (ascii & binary)              **
**                                                                           **
**  Copyright (C) 2013 TNick, nicu.tofan@gmail.com                             **
**                                                                           **
**  This library is free software, licensed under the terms of the GNU       **
**  General Public License as published by the Free Software Foundation,     **
**  either version 2 of the License, or (at your option) any later version.  **
**  You should have received a copy of the GNU General Public License        **
**  along with this program.  If not, see <http://www.gnu.org/licenses/>.    **
******************************************************************************/

#include	<gtest/gtest.h>
#include	<libdwgr.h>

static unsigned char overlaybox_OD[] = {
	0x0
};

TEST(DRW_Overlaybox, parseDwg) {
//	DRW_Overlaybox	tst;
	dwgBuffer 	buf((char*)overlaybox_OD, sizeof(overlaybox_OD) );
//	//tst.parseDwg( DRW::AC1014, &buf );
	
}
