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


TEST(DRW_Arc, parseDwg) {
	int off;
	DRW_Arc	tst;
	
	char		tst_bf[250];
	
	Q_ASSERT( sizeof(od_arc) < 250 );
	off = addBits(0, tst_bf, 
				  BIT2(0,0), /* raw short - type of the entry */
				  DSZ_SHORT, _ od_arc_id,
				  //DSZ_LONG, _ od_arc_len, /* length of the object (if version > AC1014 */
				  DSZ_BYTE, _ 0x11,
				  DSZ_BYTE, _ od_arc_hdl,
				  BITS_STOP_MARKER );
	off = addRawBytes( off, tst_bf, (char*)od_arc, sizeof(od_arc) );
	dwgBuffer 	buf((char*)tst_bf, off/8+1);
	
	EXPECT_TRUE( tst.parseDwg(DRW::AC1014, &buf) );
	
	EXPECT_EQ( tst.color, (int)256 );
	EXPECT_EQ( tst.color24, (int)-1 );
	EXPECT_EQ( tst.colorName, std::string() );
	EXPECT_EQ( tst.eType, DRW::ARC );
//	EXPECT_DOUBLE_EQ( tst.extAxisX.x, (double)0 );
//	EXPECT_DOUBLE_EQ( tst.extAxisX.y, (double)0 );
//	EXPECT_DOUBLE_EQ( tst.extAxisX.z, (double)0 );
//	EXPECT_DOUBLE_EQ( tst.extAxisY.x, (double)0 );
//	EXPECT_DOUBLE_EQ( tst.extAxisY.y, (double)0 );
//	EXPECT_DOUBLE_EQ( tst.extAxisY.z, (double)0 );
	EXPECT_EQ( tst.handle, (int)100 );
	EXPECT_EQ( tst.handleBlock, (int)0 );
	EXPECT_EQ( tst.haveExtrusion, (bool)false );
	EXPECT_EQ( tst.lTypeH.code, (duint8)0 );
	EXPECT_EQ( tst.lTypeH.ref, (duint32)0 );
	EXPECT_EQ( tst.lTypeH.size, (duint8)0 );
	EXPECT_EQ( tst.lWeight, DRW_LW_Conv::widthByLayer );
	EXPECT_EQ( tst.layer, std::string("0") );
	EXPECT_EQ( tst.layerH.code, (duint8)0 );
	EXPECT_EQ( tst.layerH.ref, (duint32)0 );
	EXPECT_EQ( tst.layerH.size, (duint8)0 );
	EXPECT_EQ( QString::compare( 
				   QString::fromStdString( tst.lineType ),
				   QString("BYLAYER"), 
				   Qt::CaseInsensitive ), 0 );
	EXPECT_DOUBLE_EQ( tst.ltypeScale, (double)1 );
	EXPECT_EQ( tst.nextLinkers, (duint8)0 );
	EXPECT_EQ( tst.plotFlags, (duint8)0 );
	EXPECT_EQ( tst.space, (int)0 );
	EXPECT_EQ( tst.visible, (bool)true );
	EXPECT_DOUBLE_EQ( tst.basePoint.x, (double)2.60027896851687 );
	EXPECT_DOUBLE_EQ( tst.basePoint.y, (double)7.32642136576277 );
	EXPECT_DOUBLE_EQ( tst.basePoint.z, (double)0 );
	EXPECT_DOUBLE_EQ( tst.extPoint.x, (double)0 );
	EXPECT_DOUBLE_EQ( tst.extPoint.y, (double)0 );
	EXPECT_DOUBLE_EQ( tst.extPoint.z, (double)1 );
	EXPECT_DOUBLE_EQ( tst.thickness, (double)0 );
	EXPECT_DOUBLE_EQ( tst.radious, (double)1.34933027990935 );
	EXPECT_DOUBLE_EQ( tst.endangle, (double)3.91290304387565 );
	EXPECT_EQ( tst.isccw, (int)1 );
	EXPECT_DOUBLE_EQ( tst.staangle, (double)2.11127815440514 );

}














