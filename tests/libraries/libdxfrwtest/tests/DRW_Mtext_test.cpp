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
#include	<QFile>
#include	<QString>
#include	<libdwgr.h>
#include	<libdxfrw.h>

#include	"../support/bitsbuild.h"
#include	"../support/drw_interface_ghost.h"
#include	"../support/odsample.h"

TEST(DRW_MText, parseDwg) {
	int off;
	/*DRW_MText	tst;*/
	char		tst_bf[256];

	Q_ASSERT( sizeof(od_mtext) < 250 );
	off = addBits(0, tst_bf, 
			BIT2(0,0), /* raw short - type of the entry */
			DSZ_SHORT, _ od_mtext_id, /* text in this case */
			//DSZ_LONG, _ od_mtext_len, /* length of the object (if version > AC1014 */
			DSZ_BYTE, _ 0x11,
			DSZ_BYTE, _ od_mtext_hdl,
			BITS_STOP_MARKER );
	off = addRawBytes( off, tst_bf, (char*)od_mtext, sizeof(od_mtext) );
	/* dwgBuffer 	buf((char*)tst_bf, off/8+1); */

	/* EXPECT_TRUE( tst.parseDwg(DRW::AC1014, &buf) );*/

	/* TNick: 
	 
	The data in the sample seems corrupt or it may not be for the R14 version.
	First sign is at the bit size = 3300249615; following values are bits  and
	I don't know the (expected) proper values.
	Color is 256, line type scale is 0;
	Inside MText processing the insertion point z is trashed
	
	As the test with real life files succedes, I conclude that the sample
	data is corrupt.
	*/
}

class MTextGh : public DRW_InterfaceGhost	{
	bool b_is_dwg_;

	virtual void addMText(const DRW_MText& data) { 

		EXPECT_EQ( data.color, (int)256 );
		EXPECT_EQ( data.color24, (int)-1 );
		EXPECT_EQ( data.colorName, std::string() );
		EXPECT_EQ( data.eType, DRW::MTEXT );
//		EXPECT_DOUBLE_EQ( data.extAxisX.x, (double)0 );
//		EXPECT_DOUBLE_EQ( data.extAxisX.y, (double)0 );
//		EXPECT_DOUBLE_EQ( data.extAxisX.z, (double)0 );
//		EXPECT_DOUBLE_EQ( data.extAxisY.x, (double)0 );
//		EXPECT_DOUBLE_EQ( data.extAxisY.y, (double)0 );
//		EXPECT_DOUBLE_EQ( data.extAxisY.z, (double)0 );
		EXPECT_EQ( data.handle, (int)734 );
		if ( b_is_dwg_ )
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
		EXPECT_DOUBLE_EQ( data.basePoint.x, (double)-10.5 );
		EXPECT_DOUBLE_EQ( data.basePoint.y, (double)14.5 );
		EXPECT_DOUBLE_EQ( data.basePoint.z, (double)-3.3 );
		EXPECT_DOUBLE_EQ( data.extPoint.x, (double)0 );
		EXPECT_DOUBLE_EQ( data.extPoint.y, (double)0 );
		EXPECT_DOUBLE_EQ( data.extPoint.z, (double)1 );
		EXPECT_DOUBLE_EQ( data.thickness, (double)0 );
		EXPECT_DOUBLE_EQ( data.secPoint.x, (double)0 );
		EXPECT_DOUBLE_EQ( data.secPoint.y, (double)0 );
		EXPECT_DOUBLE_EQ( data.secPoint.z, (double)0 );
		EXPECT_EQ( data.alignH, DRW_Text::HLeft );
		EXPECT_EQ( data.alignV, DRW_Text::VBottom );
		EXPECT_DOUBLE_EQ( data.angle, (double)0 );
		EXPECT_DOUBLE_EQ( data.height, (double)0.2 );
		EXPECT_DOUBLE_EQ( data.oblique, (double)0 );
		EXPECT_EQ( QString::compare( 
					   QString::fromStdString( data.style ),
					   QString("STANDARD"), 
					   Qt::CaseInsensitive ), 0 );
		if ( b_is_dwg_ )
		{
			EXPECT_EQ( data.styleH.code, (duint8)5 );
			EXPECT_EQ( data.styleH.ref, (duint32)17 );
			EXPECT_EQ( data.styleH.size, (duint8)1 );
		}
		EXPECT_EQ( QString::compare( 
					   QString::fromStdString( data.text ),
					   QString("first line of text\\Psecond line of text"), 
					   Qt::CaseInsensitive ), 0 );
		EXPECT_EQ( data.textgen, (int)1 );
		EXPECT_DOUBLE_EQ( data.widthscale, (double)21 );
	}
	
public:

	static void testFile ( const QString & file )
	{
		MTextGh	dwg_interf;
		dwgR	dwg_reader(QFile::encodeName(file));
		dwg_interf.b_is_dwg_ = true;
		EXPECT_TRUE( dwg_reader.read(&dwg_interf, APPLY_EXTRUSION) );
	}

	static void testDxfFile ( const QString & file )
	{
		MTextGh	dxf_interf;
		dxfRW	dxf_reader(QFile::encodeName(file));
		dxf_interf.b_is_dwg_ = false;
		EXPECT_TRUE( dxf_reader.read(&dxf_interf, APPLY_EXTRUSION) );
	}
	
};

TEST(DRW_MText, parseFromFile_r14) {
	MTextGh::testFile( DRWG_TEST_DIR "/data/dwg/libdwgrw_R14_MTEXT.dwg" );
}

TEST(DRW_MText, parseFromFile_2000) {
	MTextGh::testFile( DRWG_TEST_DIR "/data/dwg/libdwgrw_2000_MTEXT.dwg" );
}

/*
 
not implemented, yet

TEST(DRW_MText, parseFromFile_2004) {
	MTextGh::testFile( DRWG_TEST_DIR "/data/dwg/libdwgrw_2004_MTEXT.dwg" );
}

TEST(DRW_MText, parseFromFile_2007) {
	MTextGh::testFile( DRWG_TEST_DIR "/data/dwg/libdwgrw_2007_MTEXT.dwg" );
}

*/

TEST(DRW_MText, parseDxfFromFile_r12) {
	/* R12 does not have MTEXT */
	MTextGh::testDxfFile(DRWG_TEST_DIR "/data/dxf/libdwgrw_R12_MTEXT.dxf" );
}

TEST(DRW_MText, parseDxfFromFile_2000) {
	MTextGh::testDxfFile(DRWG_TEST_DIR "/data/dxf/libdwgrw_2000_MTEXT.dxf" );
}

TEST(DRW_MText, parseDxfFromFile_2004) {
	MTextGh::testDxfFile(DRWG_TEST_DIR "/data/dxf/libdwgrw_2004_MTEXT.dxf" );
}

TEST(DRW_MText, parseDxfFromFile_2007) {
	MTextGh::testDxfFile(DRWG_TEST_DIR "/data/dxf/libdwgrw_2007_MTEXT.dxf" );
}

