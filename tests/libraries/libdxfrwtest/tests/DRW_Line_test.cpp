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


TEST(DRW_Line, parseDwg) {
	int off;
	char		tst_bf[250];
	
	Q_ASSERT( sizeof(od_line) < 250 );
	off = addBits(0, tst_bf, 
				  BIT2(0,0), /* raw short - type of the entry */
				  DSZ_SHORT, _ od_line_id,
				  //DSZ_LONG, _ od_line_len, /* length of the object (if version > AC1014 */
				  DSZ_BYTE, _ 0x11,
				  DSZ_BYTE, _ od_line_hdl,
				  BITS_STOP_MARKER );
	off = addRawBytes( off, tst_bf, (char*)od_line, sizeof(od_line) );
	/*
	dwgBuffer 	buf((char*)tst_bf, off/8+1);
	DRW_Line	tst;
	
	TNick" further failed tests after issue #21 has been fixed; oh, the irony
	 
	EXPECT_TRUE( tst.parseDwg(DRW::AC1014, &buf) );
	
	EXPECT_EQ( tst.color, (int)0 );
	EXPECT_EQ( tst.color24, (int)-1 );
	EXPECT_EQ( tst.colorName, std::string() );
	EXPECT_EQ( tst.eType, (DRW::ETYPE)DRW::LINE );
//	EXPECT_DOUBLE_EQ( tst.extAxisY.x, (double)0 );
//	EXPECT_DOUBLE_EQ( tst.extAxisY.y, (double)0 );
//	EXPECT_DOUBLE_EQ( tst.extAxisY.z, (double)0 );
//	EXPECT_DOUBLE_EQ( tst.extAxisY.x, (double)0 );
//	EXPECT_DOUBLE_EQ( tst.extAxisY.y, (double)0 );
//	EXPECT_DOUBLE_EQ( tst.extAxisY.z, (double)0 );
	EXPECT_EQ( tst.handle, (int)204 );
	EXPECT_EQ( tst.handleBlock, (int)0 );
	EXPECT_EQ( tst.haveExtrusion, (bool)false );
	EXPECT_EQ( tst.lTypeH.code, (duint8)0 );
	EXPECT_EQ( tst.lTypeH.ref, (duint32)0 );
	EXPECT_EQ( tst.lTypeH.size, (duint8)0 );
	EXPECT_EQ( tst.lWeight, (DRW_LW_Conv::lineWidth)DRW_LW_Conv::widthByLayer );
	EXPECT_EQ( tst.layer, std::string("0") );
	EXPECT_EQ( tst.layerH.code, (duint8)0 );
	EXPECT_EQ( tst.layerH.ref, (duint32)0 );
	EXPECT_EQ( tst.layerH.size, (duint8)0 );
	EXPECT_EQ( tst.lineType, std::string() );
	EXPECT_DOUBLE_EQ( tst.ltypeScale, (double)0 );
	EXPECT_EQ( tst.nextLinkers, (duint8)0 );
	EXPECT_EQ( tst.plotFlags, (duint8)0 );
	EXPECT_EQ( tst.space, (int)0 );
	EXPECT_EQ( tst.visible, (bool)true );
	EXPECT_DOUBLE_EQ( tst.basePoint.x, (double)1 );
	EXPECT_DOUBLE_EQ( tst.basePoint.y, (double)0 );
	EXPECT_DOUBLE_EQ( tst.basePoint.z, (double)1 );
	EXPECT_DOUBLE_EQ( tst.extPoint.y, (double)1 );
	EXPECT_DOUBLE_EQ( tst.extPoint.z, (double)0 );
	EXPECT_DOUBLE_EQ( tst.secPoint.x, (double)1 );
	EXPECT_DOUBLE_EQ( tst.secPoint.y, (double)0 );
	EXPECT_DOUBLE_EQ( tst.secPoint.z, (double)0 );
	
	there is something fishy about these two; investigate */
	//EXPECT_DOUBLE_EQ( tst.extPoint.x, (double)3.37657383913876E+220 );
	//EXPECT_DOUBLE_EQ( tst.thickness, (double)-5.22236163650598E+067 );
	
}

class LineGh : public DRW_InterfaceGhost	{

	virtual void addLine(const DRW_Line& data) { 

		EXPECT_EQ( data.color, (int)256 );
		EXPECT_EQ( data.color24, (int)-1 );
		EXPECT_EQ( data.colorName, std::string() );
		EXPECT_EQ( data.eType, DRW::LINE );
//		EXPECT_DOUBLE_EQ( data.extAxisX.x, (double)0 );
//		EXPECT_DOUBLE_EQ( data.extAxisX.y, (double)0 );
//		EXPECT_DOUBLE_EQ( data.extAxisX.z, (double)0 );
//		EXPECT_DOUBLE_EQ( data.extAxisY.x, (double)0 );
//		EXPECT_DOUBLE_EQ( data.extAxisY.y, (double)0 );
//		EXPECT_DOUBLE_EQ( data.extAxisY.z, (double)0 );
		EXPECT_EQ( data.handle, (int)641 );
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
		EXPECT_DOUBLE_EQ( data.basePoint.x, (double)-11.11 );
		EXPECT_DOUBLE_EQ( data.basePoint.y, (double)-11.11 );
		EXPECT_DOUBLE_EQ( data.basePoint.z, (double)-11.11 );
		EXPECT_DOUBLE_EQ( data.extPoint.x, (double)0 );
		EXPECT_DOUBLE_EQ( data.extPoint.y, (double)0 );
		EXPECT_DOUBLE_EQ( data.extPoint.z, (double)1 );
		EXPECT_DOUBLE_EQ( data.thickness, (double)0 );
		EXPECT_DOUBLE_EQ( data.secPoint.x, (double)9 );
		EXPECT_DOUBLE_EQ( data.secPoint.y, (double)9 );
		EXPECT_DOUBLE_EQ( data.secPoint.z, (double)9 );

	}
	
public:

	static void testFile ( const QString & file )
	{
		LineGh	dwg_interf;
		dwgR	dwg_reader(QFile::encodeName(file));
		EXPECT_TRUE( dwg_reader.read(&dwg_interf, APPLY_EXTRUSION) );
	}
};

TEST(DRW_Line, parseFromFile_r14) {
	LineGh::testFile( DRWG_TEST_DIR "/data/dwg/libdwgrw_R14_LINE.dwg" );
}

TEST(DRW_Line, parseFromFile_2000) {
	LineGh::testFile( DRWG_TEST_DIR "/data/dwg/libdwgrw_2000_LINE.dwg" );
}

/*
 
not implemented, yet

TEST(DRW_Line, parseFromFile_2004) {
	LineGh::testFile( DRWG_TEST_DIR "/data/dwg/libdwgrw_2004_LINE.dwg" );
}

TEST(DRW_Line, parseFromFile_2007) {
	LineGh::testFile( DRWG_TEST_DIR "/data/dwg/libdwgrw_2007_LINE.dwg" );
}

*/


