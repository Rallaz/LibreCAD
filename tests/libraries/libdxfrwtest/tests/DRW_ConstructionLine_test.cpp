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
#include	<dwgbuffer.h>

static unsigned char constructionline_OD[] = {
	0x0
};

TEST(DRW_ConstructionLine, parseDwg) {
//	DRW_ConstructionLine	tst;
	dwgBuffer 	buf((char*)constructionline_OD, sizeof(constructionline_OD) );
	//tst.parseDwg( DRW::AC1014, &buf );
	
}
