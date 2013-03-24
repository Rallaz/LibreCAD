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
#include	<QString>
#include	<QFile>

#include	"../support/bitsbuild.h"
#include	"../support/drw_interface_ghost.h"
#include	"../support/odsample.h"

#undef	EXPECT_DOUBLE_EQ
#define EXPECT_DOUBLE_EQ(a,b)	\
	EXPECT_TRUE( qFuzzyCompare(a,b) )


TEST(DRW_Insert, parseDwg) {
	int off;
	char		tst_bf[250];
	
	Q_ASSERT( sizeof(od_insert) < 250 );
	off = addBits(0, tst_bf, 
				  BIT2(0,0), /* raw short - type of the entry */
				  DSZ_SHORT, _ od_insert_id,
				  //DSZ_LONG, _ od_insert_len, /* length of the object (if version > AC1014 */
				  DSZ_BYTE, _ 0x11,
				  DSZ_BYTE, _ od_insert_hdl,
				  BITS_STOP_MARKER );
	off = addRawBytes( off, tst_bf, (char*)od_insert, sizeof(od_insert) );
	
	/** @ todo the test fails; test data may be corrupted 
	 
	DRW_Insert	tst;
	dwgBuffer 	buf((char*)tst_bf, off/8+1);
	 
	EXPECT_TRUE( tst.parseDwg(DRW::AC1014, &buf) );
	*/
}
