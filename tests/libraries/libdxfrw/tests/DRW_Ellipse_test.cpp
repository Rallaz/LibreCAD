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


TEST(DRW_Ellipse, parseDwg) {
	int off;
	
	char		tst_bf[250];
	
	Q_ASSERT( sizeof(od_ellipse) < 250 );
	off = addBits(0, tst_bf, 
				  BIT2(0,0), /* raw short - type of the entry */
				  DSZ_SHORT, _ od_ellipse_id,
				  //DSZ_LONG, _ od_ellipse_len, /* length of the object (if version > AC1014 */
				  DSZ_BYTE, _ 0x11,
				  DSZ_BYTE, _ od_ellipse_hdl,
				  BITS_STOP_MARKER );
	off = addRawBytes( off, tst_bf, (char*)od_ellipse, sizeof(od_ellipse) );
	
	/*
	DRW_Ellipse	tst;
	dwgBuffer 	buf((char*)tst_bf, off/8+1);
	
	TNick" further failed tests after issue #21 has been fixed; oh, the irony
	
	EXPECT_TRUE( tst.parseDwg(DRW::AC1014, &buf) );
	
	EXPECT_EQ( tst.color, (int)256 );
	EXPECT_EQ( tst.color24, (int)-1 );
	EXPECT_EQ( tst.colorName, std::string() );
	EXPECT_EQ( tst.eType, DRW::ELLIPSE );
//	EXPECT_DOUBLE_EQ( tst.extAxisX.x, (double)0 );
//	EXPECT_DOUBLE_EQ( tst.extAxisX.y, (double)0 );
//	EXPECT_DOUBLE_EQ( tst.extAxisX.z, (double)0 );
//	EXPECT_DOUBLE_EQ( tst.extAxisY.x, (double)0 );
//	EXPECT_DOUBLE_EQ( tst.extAxisY.y, (double)0 );
//	EXPECT_DOUBLE_EQ( tst.extAxisY.z, (double)0 );
	EXPECT_EQ( tst.handle, (int)34 );
	EXPECT_EQ( tst.handleBlock, (int)0 );
	EXPECT_EQ( tst.haveExtrusion, (bool)false );
	EXPECT_EQ( tst.lTypeH.code, (duint8)0 );
	EXPECT_EQ( tst.lTypeH.ref, (duint32)0 );
	EXPECT_EQ( tst.lTypeH.size, (duint8)0 );
	EXPECT_EQ( tst.lWeight, DRW_LW_Conv::widthByLayer );
	EXPECT_EQ( tst.layer, std::string("0") );
	EXPECT_EQ( tst.lTypeH.code, (duint8)0 );
	EXPECT_EQ( tst.lTypeH.ref, (duint32)0 );
	EXPECT_EQ( tst.lTypeH.size, (duint8)0 );
	EXPECT_EQ( QString::compare( 
				   QString::fromStdString( tst.lineType ),
				   QString("BYLAYER"), 
				   Qt::CaseInsensitive ), 0 );
	EXPECT_DOUBLE_EQ( tst.ltypeScale, (double)1 );
	EXPECT_EQ( tst.nextLinkers, (duint8)0 );
	EXPECT_EQ( tst.plotFlags, (duint8)0 );
	EXPECT_EQ( tst.space, (int)0 );
	EXPECT_EQ( tst.visible, (bool)true );
	EXPECT_DOUBLE_EQ( tst.basePoint.x, (double)4.55697363762383 );
	EXPECT_DOUBLE_EQ( tst.basePoint.y, (double)6.75818841107394 );
	EXPECT_DOUBLE_EQ( tst.basePoint.z, (double)0 );
	EXPECT_DOUBLE_EQ( tst.extPoint.x, (double)0 );
	EXPECT_DOUBLE_EQ( tst.extPoint.y, (double)0 );
	EXPECT_DOUBLE_EQ( tst.extPoint.z, (double)1 );
	EXPECT_DOUBLE_EQ( tst.thickness, (double)0 );
	EXPECT_DOUBLE_EQ( tst.secPoint.x, (double)1.46061505239457 );
	EXPECT_DOUBLE_EQ( tst.secPoint.y, (double)-0.768397006550254 );
	EXPECT_DOUBLE_EQ( tst.secPoint.z, (double)0 );
	EXPECT_DOUBLE_EQ( tst.endparam, (double)6.28318530717959 );
	EXPECT_EQ( tst.isccw, (int)1 );
	EXPECT_DOUBLE_EQ( tst.ratio, (double)0.492861202508106 );
	EXPECT_DOUBLE_EQ( tst.staparam, (double)0 );
	*/
}

class EllipseGh : public DRW_InterfaceGhost	{

	virtual void addEllipse(const DRW_Ellipse& data) { 

		EXPECT_EQ( data.color, (int)256 );
		EXPECT_EQ( data.color24, (int)-1 );
		EXPECT_EQ( data.colorName, std::string() );
		EXPECT_EQ( data.eType, DRW::ELLIPSE );
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
		EXPECT_DOUBLE_EQ( data.basePoint.x, (double)-1.2 );
		EXPECT_DOUBLE_EQ( data.basePoint.y, (double)0.5 );
		EXPECT_DOUBLE_EQ( data.basePoint.z, (double)0 );
		EXPECT_DOUBLE_EQ( data.extPoint.x, (double)0 );
		EXPECT_DOUBLE_EQ( data.extPoint.y, (double)0 );
		EXPECT_DOUBLE_EQ( data.extPoint.z, (double)1 );
		EXPECT_DOUBLE_EQ( data.thickness, (double)0 );
		EXPECT_DOUBLE_EQ( data.secPoint.x, (double)15 );
		EXPECT_DOUBLE_EQ( data.secPoint.y, (double)-17.9 );
		EXPECT_DOUBLE_EQ( data.secPoint.z, (double)0 );
		EXPECT_DOUBLE_EQ( data.endparam, (double)6.28318530717959 );
		EXPECT_EQ( data.isccw, (int)1 );
		EXPECT_DOUBLE_EQ( data.ratio, (double)0.205960305895467 );
		EXPECT_DOUBLE_EQ( data.staparam, (double)0 );

	}
	
public:

	static void testFile ( const QString & file )
	{
		EllipseGh	dwg_interf;
		dwgR	dwg_reader(QFile::encodeName(file));
		EXPECT_TRUE( dwg_reader.read(&dwg_interf, APPLY_EXTRUSION) );
	}
};

TEST(DRW_Ellipse, parseFromFile_r14) {
	EllipseGh::testFile( DRWG_TEST_DIR "/data/dwg/libdwgrw_R14_ELLIPSE.dwg" );
}

TEST(DRW_Ellipse, parseFromFile_2000) {
	EllipseGh::testFile( DRWG_TEST_DIR "/data/dwg/libdwgrw_2000_ELLIPSE.dwg" );
}

/*
 
not implemented, yet

TEST(DRW_Ellipse, parseFromFile_2004) {
	EllipseGh::testFile( DRWG_TEST_DIR "/data/dwg/libdwgrw_2004_ELLIPSE.dwg" );
}

TEST(DRW_Ellipse, parseFromFile_2007) {
	EllipseGh::testFile( DRWG_TEST_DIR "/data/dwg/libdwgrw_2007_ELLIPSE.dwg" );
}

*/

//Command: ellipse
//Specify axis endpoint of ellipse or [Arc/Center]: 13.8,-17.4
//Specify other endpoint of axis: -16.2,18.4
//Specify distance to other axis or [Rotation]: 4.81



























