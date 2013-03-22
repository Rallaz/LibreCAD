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
#include	"bitsbuild.h"

static unsigned char text_OD[] = {
/* OBJECT */
//*       */ 0x00, 0x01,                                     /* type = text */
//*       */ 0x49,                                           /* len */
//*       */ 0x4C,                                           /* handle */
/* 00559 */ 0x49, 0x00,                                     /* I.       0100 1001 0000 0000 */
/* 0055B */ 0x40, 0x40, 0x53, 0x20, 0x58, 0x10, 0x00, 0x05, /* @@S X... 0100 0000 0100 0000 0101 0011 0010 0000 0101 1000 0001 0000 0000 0000 0000 0101 */
/* 00563 */ 0x5B, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, /* [@...... 0101 1011 0100 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0001 */
/* 0056B */ 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, /* ........ 0000 1000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0010 */
/* 00573 */ 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ 0000 1000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 */
/* 0057B */ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 */
/* 00583 */ 0x00, 0x14, 0xD4, 0x4D, 0x4C, 0xCC, 0xCC, 0xCC, /* ...ML... 0000 0000 0001 0100 1101 0100 0100 1101 0100 1100 1100 1100 1100 1100 1100 1100 */
/* 0058B */ 0xCC, 0xE4, 0x9F, 0xA8, 0x63, 0xA3, 0x43, 0x4B, /* ....c.CK 1100 1100 1110 0100 1001 1111 1010 1000 0110 0011 1010 0011 0100 0011 0100 1011 */
/* 00593 */ 0x99, 0x03, 0x4B, 0x99, 0x03, 0xA3, 0x2B, 0xC3, /* ..K...+. 1001 1001 0000 0011 0100 1011 1001 1001 0000 0011 1010 0011 0010 1011 1100 0011 */
/* 0059B */ 0xA5, 0x46, 0x0A, 0x21, 0xE8, 0x08, 0x0A, 0x22, /* .F.!..." 1010 0101 0100 0110 0000 1010 0010 0001 1110 1000 0000 1000 0000 1010 0010 0010 */
/* 005A3 */ 0x00,                                           /* .        0000 0000 */
/* 005A4 */ 0xC9, 0x72                                      /* crc */
/* ENDOBJECT */
};

TEST(DRW_Text, parseDwg) {
	int off;
	DRW_Text	tst;
	char		tst_bf[128];
	
	off = addBits(0, tst_bf, 
			BIT2(0,0), /* raw short - type of the entry */
			DSZ_SHORT, _ 1, /* text in this case */
			//DSZ_LONG, _ 0x49, /* length of the object (if version > AC1014 */
			DSZ_BYTE, _ 0x11,
			DSZ_BYTE, _ 0x4C, /** @todo TNick: there is sth wrong with handle */
			BITS_STOP_MARKER );
	off = addRawBytes( off, tst_bf, (char*)text_OD, sizeof(text_OD) );
	dwgBuffer 	buf((char*)tst_bf, off/8+1);

	/* DRW_Entity */
	EXPECT_TRUE( tst.parseDwg(DRW::AC1014, &buf) );
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
	EXPECT_EQ( tst.lineType, std::string("BYLAYER") );
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
	EXPECT_EQ( tst.style, std::string("STANDARD") );
	EXPECT_EQ( tst.styleH.code, (duint8)5 );
	EXPECT_EQ( tst.styleH.ref, (duint32)16 );
	EXPECT_EQ( tst.styleH.size, (duint8)1 );
	EXPECT_EQ( tst.text, std::string("this is text") );
	EXPECT_EQ( tst.textgen, 0 );
	EXPECT_DOUBLE_EQ( tst.widthscale, 1 );
	
}
