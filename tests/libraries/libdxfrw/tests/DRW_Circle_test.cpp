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


TEST(DRW_Circle, parseDwg) {
	int off;
	
	
	char		tst_bf[250];
	
	Q_ASSERT( sizeof(od_circle) < 250 );
	off = addBits(0, tst_bf, 
				  BIT2(0,0), /* raw short - type of the entry */
				  DSZ_SHORT, _ od_circle_id,
				  //DSZ_LONG, _ od_circle_len, /* length of the object (if version > AC1014 */
				  DSZ_BYTE, _ 0x11,
				  DSZ_BYTE, _ od_circle_hdl,
				  BITS_STOP_MARKER );
	off = addRawBytes( off, tst_bf, (char*)od_circle, sizeof(od_circle) );
	
	/**
	DRW_Circle	tst;
	dwgBuffer 	buf((char*)tst_bf, off/8+1);
	
	@ todo the test fails; test data may be corrupted
	EXPECT_TRUE( tst.parseDwg(DRW::AC1014, &buf) );
	 */
}

class CircleGh : public DRW_InterfaceGhost	{

	virtual void addCircle(const DRW_Circle& data) { 

		EXPECT_EQ( data.color, (int)256 );
		EXPECT_EQ( data.color24, (int)-1 );
		EXPECT_EQ( data.colorName, std::string() );
		EXPECT_EQ( data.eType, DRW::CIRCLE );
//		EXPECT_DOUBLE_EQ( data.extAxisX.x, (double)0 );
//		EXPECT_DOUBLE_EQ( data.extAxisX.y, (double)0 );
//		EXPECT_DOUBLE_EQ( data.extAxisX.z, (double)0 );
//		EXPECT_DOUBLE_EQ( data.extAxisY.x, (double)0 );
//		EXPECT_DOUBLE_EQ( data.extAxisY.y, (double)0 );
//		EXPECT_DOUBLE_EQ( data.extAxisY.z, (double)0 );
		EXPECT_EQ( data.handle, (int)208 );
		EXPECT_EQ( data.handleBlock, (int)0 );
		EXPECT_EQ( data.haveExtrusion, (bool)false );
		EXPECT_EQ( data.lTypeH.code, (duint8)0 );
		EXPECT_EQ( data.lTypeH.ref, (duint32)0 );
		EXPECT_EQ( data.lTypeH.size, (duint8)0 );
		EXPECT_EQ( data.lWeight, DRW_LW_Conv::widthByLayer );
		EXPECT_EQ( data.layer, std::string("0") );
		EXPECT_EQ( data.layerH.code, (duint8)0 );
		EXPECT_EQ( data.layerH.ref, (duint32)0 );
		EXPECT_EQ( data.layerH.size, (duint8)0 );
		EXPECT_EQ( QString::compare( 
					   QString::fromStdString( data.lineType ),
					   QString("BYLAYER"), 
					   Qt::CaseInsensitive ), 0 );
		EXPECT_DOUBLE_EQ( data.ltypeScale, (double)1 );
		EXPECT_EQ( data.nextLinkers, (duint8)0 );
		EXPECT_EQ( data.plotFlags, (duint8)0 );
		EXPECT_EQ( data.space, (int)0 );
		EXPECT_EQ( data.visible, (bool)true );
		EXPECT_DOUBLE_EQ( data.basePoint.x, (double)-5 );
		EXPECT_DOUBLE_EQ( data.basePoint.y, (double)3 );
		EXPECT_DOUBLE_EQ( data.basePoint.z, (double)-1 );
		EXPECT_DOUBLE_EQ( data.extPoint.x, (double)0 );
		EXPECT_DOUBLE_EQ( data.extPoint.y, (double)0 );
		EXPECT_DOUBLE_EQ( data.extPoint.z, (double)1 );
		EXPECT_DOUBLE_EQ( data.thickness, (double)0 );
		EXPECT_DOUBLE_EQ( data.radious, (double)11 );
		
	}
	
public:

	static void testFile ( const QString & file )
	{
		CircleGh	dwg_interf;
		dwgR	dwg_reader(QFile::encodeName(file));
		EXPECT_TRUE( dwg_reader.read(&dwg_interf, APPLY_EXTRUSION) );
	}
};

TEST(DRW_Circle, parseFromFile_r14) {
	CircleGh::testFile( DRWG_TEST_DIR "/data/dwg/libdwgrw_R14_CIRCLE.dwg" );
}

TEST(DRW_Circle, parseFromFile_2000) {
	CircleGh::testFile( DRWG_TEST_DIR "/data/dwg/libdwgrw_2000_CIRCLE.dwg" );
}

/*
 
not implemented, yet

TEST(DRW_Circle, parseFromFile_2004) {
	CircleGh::testFile( DRWG_TEST_DIR "/data/dwg/libdwgrw_2004_CIRCLE.dwg" );
}

TEST(DRW_Circle, parseFromFile_2007) {
	CircleGh::testFile( DRWG_TEST_DIR "/data/dwg/libdwgrw_2007_CIRCLE.dwg" );
}

*/
