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

#include	"../support/bitsbuild.h"
#include	"../support/drw_interface_ghost.h"
#include	"../support/odsample.h"

TEST(DRW_Text, parseDwg) {
	int off;
	DRW_Text	tst;
	char		tst_bf[256];
	
	Q_ASSERT( sizeof(od_text) < 250 );
	off = addBits(0, tst_bf, 
			BIT2(0,0), /* raw short - type of the entry */
			DSZ_SHORT, _ od_text_id, /* text in this case */
			//DSZ_LONG, _ od_text_len, /* length of the object (if version > AC1014 */
			DSZ_BYTE, _ 0x11,
			DSZ_BYTE, _ od_text_hdl,
			BITS_STOP_MARKER );
	off = addRawBytes( off, tst_bf, (char*)od_text, sizeof(od_text) );
	dwgBuffer 	buf((char*)tst_bf, off/8+1);

	EXPECT_TRUE( tst.parseDwg(DRW::AC1014, &buf) );

	/* DRW_Entity */
	EXPECT_EQ( tst.color, 256 );
	EXPECT_EQ( tst.color24,-1 );
	EXPECT_EQ( tst.colorName, std::string() );
	EXPECT_EQ( tst.eType, DRW::TEXT );
//	EXPECT_DOUBLE_EQ( tst.extAxisX.x, 0 );
//	EXPECT_DOUBLE_EQ( tst.extAxisX.y, 0 );
//	EXPECT_DOUBLE_EQ( tst.extAxisX.z, 0 );
//	EXPECT_DOUBLE_EQ( tst.extAxisY.x, 0 );
//	EXPECT_DOUBLE_EQ( tst.extAxisY.y, 0 );
//	EXPECT_DOUBLE_EQ( tst.extAxisY.z, 0 );
	EXPECT_EQ( tst.handle, 76 );
	EXPECT_EQ( tst.handleBlock, 0 );
	EXPECT_EQ( tst.haveExtrusion, false );
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
	EXPECT_DOUBLE_EQ( tst.ltypeScale, 1 );
	EXPECT_EQ( tst.nextLinkers, (duint32)0 );
	EXPECT_EQ( tst.plotFlags, (duint8)0 );
	EXPECT_EQ( tst.space, 0 );
	EXPECT_EQ( tst.visible, true );
	
	/* DRW_Point */
	EXPECT_DOUBLE_EQ( tst.basePoint.x, 3 );
	EXPECT_DOUBLE_EQ( tst.basePoint.y, 4 );
	EXPECT_DOUBLE_EQ( tst.basePoint.z, 0 );
	EXPECT_DOUBLE_EQ( tst.extPoint.x, 0 );
	EXPECT_DOUBLE_EQ( tst.extPoint.y, 0 );
	EXPECT_DOUBLE_EQ( tst.extPoint.z, 1 );
	EXPECT_DOUBLE_EQ( tst.thickness, 0 );
	
	/* DRW_Line */
	EXPECT_DOUBLE_EQ( tst.secPoint.x, 0 );
	EXPECT_DOUBLE_EQ( tst.secPoint.y, 0 );
	EXPECT_DOUBLE_EQ( tst.secPoint.z, 0 );

	/* DRW_Text */
	EXPECT_EQ( tst.alignH, DRW_Text::HLeft );
	EXPECT_EQ( tst.alignV, DRW_Text::VBaseLine );
	EXPECT_DOUBLE_EQ( tst.angle, 0.0 );
	EXPECT_DOUBLE_EQ( tst.height, 0.2 );
	EXPECT_DOUBLE_EQ( tst.oblique, 0.0 );
	EXPECT_EQ( QString::compare( 
				   QString::fromStdString( tst.style ),
				   QString("STANDARD"), 
				   Qt::CaseInsensitive ), 0 );
	EXPECT_EQ( tst.styleH.code, (duint8)5 );
	EXPECT_EQ( tst.styleH.ref, (duint32)16 );
	EXPECT_EQ( tst.styleH.size, (duint8)1 );
	EXPECT_EQ( tst.text, std::string("this is text") );
	EXPECT_EQ( tst.textgen, 0 );
	EXPECT_DOUBLE_EQ( tst.widthscale, 1 );
	
}

class TextGh : public DRW_InterfaceGhost	{

	virtual void addText(const DRW_Text& data) { 
		
		/* DRW_Entity */
		EXPECT_EQ( data.color, 256 );
		EXPECT_EQ( data.color24,-1 );
		EXPECT_EQ( data.colorName, std::string() );
		EXPECT_EQ( data.eType, DRW::TEXT );
		//	EXPECT_DOUBLE_EQ( data.extAxisX.x, 0 );
		//	EXPECT_DOUBLE_EQ( data.extAxisX.y, 0 );
		//	EXPECT_DOUBLE_EQ( data.extAxisX.z, 0 );
		//	EXPECT_DOUBLE_EQ( data.extAxisY.x, 0 );
		//	EXPECT_DOUBLE_EQ( data.extAxisY.y, 0 );
		//	EXPECT_DOUBLE_EQ( data.extAxisY.z, 0 );
		//EXPECT_EQ( data.handle, 76 );
		EXPECT_EQ( data.handleBlock, 0 );
		EXPECT_EQ( data.haveExtrusion, false );
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
		EXPECT_DOUBLE_EQ( data.ltypeScale, 1 );
		EXPECT_EQ( data.nextLinkers, (duint32)0 );
		EXPECT_EQ( data.plotFlags, (duint8)0 );
		EXPECT_EQ( data.space, 0 );
		EXPECT_EQ( data.visible, true );
		
		/* DRW_Point */
		EXPECT_DOUBLE_EQ( data.basePoint.x, 15.5 );
		EXPECT_DOUBLE_EQ( data.basePoint.y, 16.5 );
		EXPECT_DOUBLE_EQ( data.basePoint.z, 0 );
		EXPECT_DOUBLE_EQ( data.extPoint.x, 0 );
		EXPECT_DOUBLE_EQ( data.extPoint.y, 0 );
		EXPECT_DOUBLE_EQ( data.extPoint.z, 1 );
		EXPECT_DOUBLE_EQ( data.thickness, 0 );
		
		/* DRW_Line */
		/**
		@todo R14 and 2000 give different results,
		either (0,0,0) or (15.5, 16.5, 0)
		 
		EXPECT_DOUBLE_EQ( data.secPoint.x, 15.5 );
		EXPECT_DOUBLE_EQ( data.secPoint.y, 16.5 );
		*/
		EXPECT_DOUBLE_EQ( data.secPoint.z, 0 );
		
		/* DRW_Text */
		EXPECT_EQ( data.alignH, DRW_Text::HLeft );
		EXPECT_EQ( data.alignV, DRW_Text::VBaseLine );
		EXPECT_DOUBLE_EQ( data.angle, (1.5*M_PI/180.0) );
		EXPECT_DOUBLE_EQ( data.height, 0.54 );
		EXPECT_DOUBLE_EQ( data.oblique, 0.0 );
		EXPECT_EQ( QString::compare( 
					   QString::fromStdString( data.style ),
					   QString("STANDARD"), 
					   Qt::CaseInsensitive ), 0 );
		EXPECT_EQ( data.styleH.code, (duint8)5 );
		//EXPECT_EQ( data.styleH.ref, (duint32)16 );
		EXPECT_EQ( data.styleH.size, (duint8)1 );
		EXPECT_EQ( data.text, std::string("123-test string-456") );
		EXPECT_EQ( data.textgen, 0 );
		EXPECT_DOUBLE_EQ( data.widthscale, 1 );
	}
	
public:

	static void testFile ( const QString & file )
	{
		TextGh	dwg_interf;
		dwgR	dwg_reader(QFile::encodeName(file));
		EXPECT_TRUE( dwg_reader.read(&dwg_interf, APPLY_EXTRUSION) );
	}
};

TEST(DRW_Text, parseFromFile_r14) {
	TextGh::testFile( DRWG_TEST_DIR "/data/dwg/libdwgrw_R14_TEXT.dwg" );
}

TEST(DRW_Text, parseFromFile_2000) {
	TextGh::testFile( DRWG_TEST_DIR "/data/dwg/libdwgrw_2000_TEXT.dwg" );
}



/*
 
not implemented, yet

TEST(DRW_Text, parseFromFile_2004) {
	TextGh::testFile( DRWG_TEST_DIR "/data/dwg/libdwgrw_2004_TEXT.dwg" );
}

TEST(DRW_Text, parseFromFile_2007) {
	TextGh::testFile( DRWG_TEST_DIR "/data/dwg/libdwgrw_2007_TEXT.dwg" );
}

*/
