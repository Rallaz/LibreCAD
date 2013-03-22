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

#define FROM_BINARY(a1,a2,a3,a4,a5,a6,a7,a8)	( \
	( a1 << 7 ) | \
	( a2 << 6 ) | \
	( a3 << 5 ) | \
	( a4 << 4 ) | \
	( a5 << 3 ) | \
	( a6 << 2 ) | \
	( a7 << 1 ) | \
	( a8 << 0 ) )


TEST(dwgBuffer, initFromChar) {
	unsigned char tst[] = { 0x00, 0x01 };
	dwgBuffer 	buf((char*)tst, sizeof(tst) );
	
	// test right after initialization
	EXPECT_EQ( buf.size(), (duint32)2 );
	EXPECT_EQ( buf.getPosition(), 0 );
	EXPECT_EQ( buf.getBitPos(), 0 );
	EXPECT_TRUE( buf.isGood() );
	EXPECT_EQ( buf.numRemainingBytes(), 2 );
}

TEST(dwgBuffer, setPosition) {
	unsigned char tst[] = { 0x00, 0x01 };
	dwgBuffer 	buf((char*)tst, sizeof(tst) );

	EXPECT_TRUE( buf.setPosition(0) );
	EXPECT_EQ( buf.getPosition(), 0 );
	EXPECT_EQ( buf.getBitPos(), 0 );
	EXPECT_TRUE( buf.setPosition(1) );
	EXPECT_EQ( buf.getPosition(), 1 );
	EXPECT_EQ( buf.getBitPos(), 0 );
	EXPECT_TRUE( buf.setPosition(2) );
	EXPECT_EQ( buf.getPosition(), 2 );
	EXPECT_EQ( buf.getBitPos(), 0 );
	EXPECT_FALSE( buf.setPosition(3) );
	EXPECT_EQ( buf.getPosition(), 2 );
	EXPECT_FALSE( buf.setPosition(500) );
	EXPECT_EQ( buf.getPosition(), 2 );
	EXPECT_FALSE( buf.setPosition(-500) );
	EXPECT_EQ( buf.getPosition(), 2 );
	EXPECT_FALSE( buf.setPosition(-1) );
	EXPECT_EQ( buf.getPosition(), 2 );
	EXPECT_TRUE( buf.setPosition(0) );
	EXPECT_EQ( buf.getPosition(), 0 );
	EXPECT_EQ( buf.getBitPos(), 0 );
}


TEST(dwgBuffer, setBitPos) {
	unsigned char tst[] = { 0x00, 0x01 };
	dwgBuffer 	buf((char*)tst, sizeof(tst) );

	buf.setBitPos(0);
	EXPECT_EQ( buf.getBitPos(), 0 );
	buf.setBitPos(1);
	EXPECT_EQ( buf.getBitPos(), 1 );
	buf.setBitPos(-1);
	EXPECT_EQ( buf.getBitPos(), 1 );
	buf.setBitPos(2);
	EXPECT_EQ( buf.getBitPos(), 2 );
	buf.setBitPos(8); // above
	EXPECT_EQ( buf.getBitPos(), 2 );
	buf.setBitPos(7);
	EXPECT_EQ( buf.getBitPos(), 7 );
	buf.setBitPos(0);
	EXPECT_EQ( buf.getBitPos(), 0 );
}

TEST(dwgBuffer, getBytes) {
	unsigned char tst[] = { 0x00, 0x01 };
	dwgBuffer 	buf((char*)tst, sizeof(tst) );

	// get back two bytes
	unsigned char get_tst[] = { 0xFF, 0xFF };
	EXPECT_TRUE( buf.getBytes((char*)get_tst,sizeof(get_tst)) );
	EXPECT_EQ( get_tst[0], tst[0] );
	EXPECT_EQ( get_tst[1], tst[1] );
	EXPECT_EQ( buf.numRemainingBytes(), 0 );
	EXPECT_TRUE( buf.isGood() );
}

TEST(dwgBuffer, getBit) {
	unsigned char tst[] = { 0xFF /*11111111b*/, 0x00 /*00000000b*/ };
	dwgBuffer 	buf((char*)tst, sizeof(tst) );

	for ( int i = 0; i < 8; i++ )
	{
		EXPECT_EQ( buf.getBit(), 1 );
	}
	for ( int i = 0; i < 8; i++ )
	{
		EXPECT_EQ( buf.getBit(), 0 );
	}	
}

TEST(dwgBuffer, get2Bits) {
	unsigned char tst[] = { 0xFF /*11111111b*/, 0x00 /*00000000b*/ };
	dwgBuffer 	buf((char*)tst, sizeof(tst) );

	for ( int i = 0; i < 4; i++ )
	{
		EXPECT_EQ( buf.get2Bits(), 3 );
	}
	for ( int i = 0; i < 4; i++ )
	{
		EXPECT_EQ( buf.get2Bits(), 0 );
	}
}

#define RESET_BUFF		\
	buf.setPosition(0);	\
	memset(tst,0,16)

TEST(dwgBuffer, getBitShort) {
	char tst[64];
	 
	dwgBuffer 	buf((char*)tst, sizeof(tst) );
	
	RESET_BUFF;
	tst[0] = FROM_BINARY(
	  1,1, /* encodes 256 */
	  0,0,0,0,0,0
	);
	EXPECT_EQ( buf.getBitShort(), (dint16)256 );
	
	RESET_BUFF;
	tst[0] = FROM_BINARY(
	  1,0, /* encodes 0 */
	  0,0,0,0,0,0
	);
	EXPECT_EQ( buf.getBitShort(), (dint16)0 );
	
	RESET_BUFF;
	addBits(0, tst, 
			BIT2(0,1), /* encodes a raw char */
			BIT8(0,1,1,1,1,1,1,1), /* 127 */
			BITS_STOP_MARKER
	);
	EXPECT_EQ( buf.getBitShort(), (dint16)127 );
	
	RESET_BUFF;
	addBits(0, tst, 
			BIT2(0,1), /* encodes a raw char */
			BIT8(0,0,1,1,1,1,1,1), /* 63 */
			BITS_STOP_MARKER
	);
	EXPECT_EQ( buf.getBitShort(), (dint16)63 );
	
	RESET_BUFF;
	addBits(0, tst, 
			BIT2(0,1), /* encodes a raw char */
			BIT8(0,0,1,1,1,1,1,0), /* 62 */
			BITS_STOP_MARKER
	);
	EXPECT_EQ( buf.getBitShort(), (dint16)62 );
	
	
	RESET_BUFF;
	addBits(0, tst, 
			BIT2(0,0), /* encodes a raw short */
			BIT8(0,0,1,1,1,1,1,0), /* 62 */
			BIT8(0,0,0,0,0,0,0,0), 
			BITS_STOP_MARKER
	);
	EXPECT_EQ( buf.getBitShort(), (dint16)62 );
	
	RESET_BUFF;
	addBits(0, tst, 
			BIT2(0,0), /* encodes a raw short */
			BIT8(0,0,1,1,1,1,1,1), /* 63 */
			BIT8(0,0,0,0,0,0,0,0), 
			BITS_STOP_MARKER
	);
	EXPECT_EQ( buf.getBitShort(), (dint16)63 );
	
	RESET_BUFF;
	addBits(0, tst, 
			BIT2(0,0), /* encodes a raw short */
			BIT8(0,1,1,1,1,1,1,1), /* 127 */
			BIT8(0,0,0,0,0,0,0,0), 
			BITS_STOP_MARKER
	);
	EXPECT_EQ( buf.getBitShort(), (dint16)127 );
	
	RESET_BUFF;
	addBits(0, tst, 
			BIT2(0,0), /* encodes a raw short */
			BIT8(0,1,1,1,1,1,1,1), /* 0x807F */
			BIT8(1,0,0,0,0,0,0,0), 
			BITS_STOP_MARKER
	);
	EXPECT_EQ( buf.getBitShort(), (dint16)0x807F );
	
}

TEST(dwgBuffer, getBitLong) {
	char tst[64];
	 
	dwgBuffer 	buf((char*)tst, sizeof(tst) );
	
	RESET_BUFF;
	tst[0] = FROM_BINARY(
	  1,0, /* encodes 0 */
	  0,0,0,0,0,0
	);
	EXPECT_EQ( buf.getBitLong(), (dint16)0 );
	

	RESET_BUFF;
	addBits(0, tst, 
			BIT2(0,1), /* encodes a raw char */
			BIT8(0,1,1,1,1,1,1,1), /* 127 */
			BITS_STOP_MARKER
	);
	EXPECT_EQ( buf.getBitLong(), (dint16)127 );
	
	RESET_BUFF;
	addBits(0, tst, 
			BIT2(0,1), /* encodes a raw char */
			BIT8(0,0,1,1,1,1,1,1), /* 63 */
			BITS_STOP_MARKER
	);
	EXPECT_EQ( buf.getBitLong(), (dint16)63 );
	
	RESET_BUFF;
	addBits(0, tst, 
			BIT2(0,1), /* encodes a raw char */
			BIT8(0,0,1,1,1,1,1,0), /* 62 */
			BITS_STOP_MARKER
	);
	EXPECT_EQ( buf.getBitLong(), (dint16)62 );
	

	RESET_BUFF;
	addBits(0, tst, 
			BIT2(0,0), /* encodes a raw long */
			BIT8(0,0,1,1,1,1,1,0), /* 62 */
			BIT8(0,0,0,0,0,0,0,0), 
			BIT8(0,0,0,0,0,0,0,0), 
			BIT8(0,0,0,0,0,0,0,0), 
			BITS_STOP_MARKER
	);
	EXPECT_EQ( buf.getBitLong(), (dint16)62 );
	
	RESET_BUFF;
	addBits(0, tst, 
			BIT2(0,0), /* encodes a raw long */
			BIT8(0,0,1,1,1,1,1,1), /* 63 */
			BIT8(0,0,0,0,0,0,0,0), 
			BIT8(0,0,0,0,0,0,0,0), 
			BIT8(0,0,0,0,0,0,0,0), 
			BITS_STOP_MARKER
	);
	EXPECT_EQ( buf.getBitLong(), (dint16)63 );
	
	RESET_BUFF;
	addBits(0, tst, 
			BIT2(0,0), /* encodes a raw long */
			BIT8(0,1,1,1,1,1,1,1), /* 127 */
			BIT8(0,0,0,0,0,0,0,0), 
			BIT8(0,0,0,0,0,0,0,0), 
			BIT8(0,0,0,0,0,0,0,0), 
			BITS_STOP_MARKER
	);
	EXPECT_EQ( buf.getBitLong(), (dint16)127 );
	
	RESET_BUFF;
	addBits(0, tst, 
			BIT2(0,0), /* encodes a raw long */
			BIT8(0,1,1,1,1,1,1,1), /* 0x807F = 32895*/
			BIT8(1,0,0,0,0,0,0,0), 
			BIT8(0,0,0,0,0,0,0,0), 
			BIT8(0,0,0,0,0,0,0,0), 
			BITS_STOP_MARKER
	);
	EXPECT_EQ( (dint16)buf.getBitLong(), (dint16)32895 );
}

TEST(dwgBuffer, getBitDouble) {
	char tst[64];
	 
	dwgBuffer 	buf((char*)tst, sizeof(tst) );

	RESET_BUFF;
	tst[0] = FROM_BINARY(
	  0,1, /* encodes 1.0 */
	  0,0,0,0,0,0
	);
	EXPECT_EQ( buf.getBitDouble(), 1.0 );

	RESET_BUFF;
	tst[0] = FROM_BINARY(
	  1,0, /* encodes 0.0 */
	  0,0,0,0,0,0
	);
	EXPECT_EQ( buf.getBitDouble(), 0.0 );

	RESET_BUFF;
	addBits(0, tst, 
			BIT2(0,0), /* encodes a double */
			BIT8(0,1,1,1,1,1,1,1),
			BIT8(1,0,0,0,0,0,0,0), 
			BIT8(0,0,0,0,0,0,0,0), 
			BIT8(0,0,0,0,0,0,0,0), 
			BIT8(0,0,0,0,0,0,0,0), 
			BIT8(0,0,0,0,0,0,0,0), 
			BIT8(0,0,0,0,0,0,0,0), 
			BIT8(0,0,0,0,0,0,0,0), 
			BITS_STOP_MARKER
	);
	EXPECT_DOUBLE_EQ( buf.getBitDouble(), 1.62523e-319 );

	/* more tests are welcomed */
}

TEST(dwgBuffer, getRawChar8) {
	char tst[64];

	dwgBuffer 	buf((char*)tst, sizeof(tst) );
	RESET_BUFF;
	tst[0] = 0;
	tst[1] = 1;
	tst[2] = 2;
	tst[3] = 3;
	tst[4] = 255;
	EXPECT_EQ( buf.getRawChar8(), 0 );
	EXPECT_EQ( buf.getRawChar8(), 1 );
	EXPECT_EQ( buf.getRawChar8(), 2 );
	EXPECT_EQ( buf.getRawChar8(), 3 );
	EXPECT_EQ( buf.getRawChar8(), 255 );
}

TEST(dwgBuffer, getRawShort16) {
	char tst[64];

	dwgBuffer 	buf((char*)tst, sizeof(tst) );
	RESET_BUFF;
	tst[0] = 0;
	tst[1] = 0;
	tst[2] = 1;
	tst[3] = 0;
	tst[4] = 2;
	tst[5] = 0;
	tst[6] = 3;
	tst[7] = 0;
	EXPECT_EQ( buf.getRawShort16(), 0 );
	EXPECT_EQ( buf.getRawShort16(), 1 );
	EXPECT_EQ( buf.getRawShort16(), 2 );
	EXPECT_EQ( buf.getRawShort16(), 3 );
}








