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
#include	"../support/bitsbuild.h"

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

TEST(dwgBuffer, getRawDouble) {
	char tst[64];

	dwgBuffer 	buf((char*)tst, sizeof(tst) );
	double * pd;
	RESET_BUFF; pd = (double*)&tst[0];
	
	*pd = 0.0; buf.setPosition(0);
	EXPECT_DOUBLE_EQ( buf.getRawDouble(), 0.0 );
	*pd = 1.0; buf.setPosition(0);
	EXPECT_DOUBLE_EQ( buf.getRawDouble(), 1.0 );
	*pd = -1.0; buf.setPosition(0);
	EXPECT_DOUBLE_EQ( buf.getRawDouble(), -1.0 );
	*pd = 1.11; buf.setPosition(0);
	EXPECT_DOUBLE_EQ( buf.getRawDouble(), 1.11 );
	*pd = -1.11; buf.setPosition(0);
	EXPECT_DOUBLE_EQ( buf.getRawDouble(), -1.11 );
	*pd = 1.62523e-319; buf.setPosition(0);
	EXPECT_DOUBLE_EQ( buf.getRawDouble(), 1.62523e-319 );
	
	buf.setPosition(0);
	*pd = 0.0; pd++;
	*pd = 1.0; pd++;
	*pd = -1.0; pd++;
	EXPECT_DOUBLE_EQ( buf.getRawDouble(), 0.0 );
	EXPECT_DOUBLE_EQ( buf.getRawDouble(), 1.0 );
	EXPECT_DOUBLE_EQ( buf.getRawDouble(), -1.0 );
}

TEST(dwgBuffer, getRawLong32) {
	char tst[64];

	dwgBuffer 	buf((char*)tst, sizeof(tst) );
	
	duint32 * pd;
	RESET_BUFF; pd = (duint32*)&tst[0];
	
	*pd = 0; buf.setPosition(0);
	EXPECT_EQ( buf.getRawLong32(), (duint32)0 );
	*pd = 1; buf.setPosition(0);
	EXPECT_EQ( buf.getRawLong32(), (duint32)1 );
	*pd = 10; buf.setPosition(0);
	EXPECT_EQ( buf.getRawLong32(), (duint32)10 );
	*pd = 100; buf.setPosition(0);
	EXPECT_EQ( buf.getRawLong32(), (duint32)100 );
	*pd = 1000; buf.setPosition(0);
	EXPECT_EQ( buf.getRawLong32(), (duint32)1000 );
	*pd = 10000; buf.setPosition(0);
	EXPECT_EQ( buf.getRawLong32(), (duint32)10000 );
	*pd = 100000; buf.setPosition(0);
	EXPECT_EQ( buf.getRawLong32(), (duint32)100000 );
	*pd = 0xFFFFFFFF; buf.setPosition(0);
	EXPECT_EQ( buf.getRawLong32(), (duint32)0xFFFFFFFF );

}

TEST(dwgBuffer, getModularChar) {
	char tst[64];

	dwgBuffer 	buf((char*)tst, sizeof(tst) );
	
	RESET_BUFF;
	tst[0] = 0; // ending marker
	EXPECT_EQ( buf.getModularChar(), (dint32)0 );
	
	RESET_BUFF;
	tst[0] = 0x80 | 0;
	tst[1] = 0x80 | 0;
	tst[2] = 0; // ending marker
	EXPECT_EQ( buf.getModularChar(), (dint32)0 );
	
	RESET_BUFF;
	tst[0] = 0x80 | 0;
	tst[1] = 0x80 | 0;
	tst[2] = 0x80 | 0;
	tst[3] = 0; // ending marker
	EXPECT_EQ( buf.getModularChar(), (dint32)0 );
	
	RESET_BUFF;
	tst[0] = 0x80 | 1;
	tst[1] = 0x80 | 0;
	tst[2] = 0x80 | 0;
	tst[3] = 0; // ending marker
	EXPECT_EQ( buf.getModularChar(), (dint32)1 );
	
	RESET_BUFF;
	tst[0] = 0x80 | 1;
	tst[1] = 0x80 | 0;
	tst[2] = 0x80 | 0;
	tst[3] = 0x40; // ending marker and negative marker
	EXPECT_EQ( buf.getModularChar(), (dint32)-1 );
	
	RESET_BUFF;
	tst[0] = 0x80 | 1;
	tst[1] = 0x80 | 1;
	tst[2] = 0x80 | 1;
	tst[3] = 0; // ending marker
	EXPECT_EQ( buf.getModularChar(), (dint32)16513 );
	
	RESET_BUFF;
	tst[0] = 0x80 | 1;
	tst[1] = 0x80 | 1;
	tst[2] = 0x80 | 1;
	tst[3] = 0x05; // ending marker
	EXPECT_EQ( buf.getModularChar(), (dint32)10502273 );
	
}

TEST(dwgBuffer, getModularShort) {
	char tst[64];
	
	dwgBuffer 	buf((char*)tst, sizeof(tst) );

	RESET_BUFF;
	tst[0] =        0; tst[1] = 0;  // ending marker
	EXPECT_EQ( buf.getModularShort(), (dint32)0 );

	RESET_BUFF;
	tst[0] = 0; tst[1] = 0x80 | 0;
	tst[2] = 0; tst[3] =        0;  // ending marker
	EXPECT_EQ( buf.getModularShort(), (dint32)0 );
	
	RESET_BUFF;
	tst[0] = 0; tst[1] = 0x80 | 0;
	tst[2] = 0; tst[3] = 0x80 | 0;
	tst[4] = 0; tst[5] =        0;  // ending marker
	EXPECT_EQ( buf.getModularShort(), (dint32)0 );
	
	RESET_BUFF;
	tst[0] = 1; tst[1] = 0x80 | 0;
	tst[2] = 0; tst[3] = 0x80 | 0;
	tst[4] = 0; tst[5] =        0;  // ending marker
	EXPECT_EQ( buf.getModularShort(), (dint32)1 );
	
	RESET_BUFF;
	tst[0] = 1; tst[1] = 0x80 | 1;
	tst[2] = 0; tst[3] = 0x80 | 0;
	tst[4] = 0; tst[5] =        0;  // ending marker
	EXPECT_EQ( buf.getModularShort(), (dint32)257 );
}

TEST(dwgBuffer, getHandle) {
	char tst[64];
	
	dwgBuffer 	buf((char*)tst, sizeof(tst) );
	dwgHandle	hdl;

	RESET_BUFF;
	tst[0] = 0x00 | 0x00; // code and size
	hdl = buf.getHandle();
	EXPECT_EQ( hdl.code, (duint32)0 );
	EXPECT_EQ( hdl.size, (duint32)0 );
	EXPECT_EQ( hdl.ref, (duint32)0 );

	RESET_BUFF;
	tst[0] = 0x10 | 0x00; // code and size
	hdl = buf.getHandle();
	EXPECT_EQ( hdl.code, (duint32)1 );
	EXPECT_EQ( hdl.size, (duint32)0 );
	EXPECT_EQ( hdl.ref, (duint32)0 );

	RESET_BUFF;
	tst[0] = 0x10 | 0x01; // code and size
	tst[1] = 0;
	hdl = buf.getHandle();
	EXPECT_EQ( hdl.code, (duint32)1 );
	EXPECT_EQ( hdl.size, (duint32)1 );
	EXPECT_EQ( hdl.ref, (duint32)0 );

	RESET_BUFF;
	tst[0] = 0x10 | 0x01; // code and size
	tst[1] = 5;
	hdl = buf.getHandle();
	EXPECT_EQ( hdl.code, (duint32)1 );
	EXPECT_EQ( hdl.size, (duint32)1 );
	EXPECT_EQ( hdl.ref, (duint32)5 );

	RESET_BUFF;
	tst[0] = 0x10 | 0x02; // code and size
	tst[1] = 0x11;
	tst[2] = 0x55;
	hdl = buf.getHandle();
	EXPECT_EQ( hdl.code, (duint32)1 );
	EXPECT_EQ( hdl.size, (duint32)2 );
	EXPECT_EQ( hdl.ref, (duint32)0x1155 );
}

TEST(dwgBuffer, getVariableText) {
	char tst[64];
	int off;
	dwgBuffer 	buf((char*)tst, sizeof(tst) );

	RESET_BUFF;
	addBits(0, tst, 
			BIT2(1,0),/* encodes 0 => empty string */
			BITS_STOP_MARKER
			);
	EXPECT_EQ( buf.getVariableText(), std::string() );

	RESET_BUFF;
	off = addBits(0, tst, 
				  BIT2(0,0), /* encodes a raw short */
				  BIT8(0,0,0,0,0,1,1,1), /* 7 */
				  BIT8(0,0,0,0,0,0,0,0), 
				  BITS_STOP_MARKER
				  );
	addRawBytes( off, tst, "abcdefg", 7 );
	EXPECT_EQ( buf.getVariableText(), std::string( "abcdefg" ) );
}

TEST(dwgBuffer, getVariableUtf8Text) {
	char tst[64];
	int off;
	dwgBuffer 	buf((char*)tst, sizeof(tst) );

	RESET_BUFF;
	addBits(0, tst, 
			BIT2(1,0),/* encodes 0 => empty string */
			BITS_STOP_MARKER
			);
	EXPECT_EQ( buf.getVariableUtf8Text(), std::string() );

	RESET_BUFF;
	off = addBits(0, tst, 
				  BIT2(0,0), /* encodes a raw short */
				  BIT8(0,0,0,0,0,1,1,1), /* 7 */
				  BIT8(0,0,0,0,0,0,0,0), 
				  BITS_STOP_MARKER
				  );
	addRawBytes( off, tst, "abcdefg", 7 );
	EXPECT_EQ( buf.getVariableUtf8Text(), std::string( "abcdefg" ) );
}

TEST(dwgBuffer, getExtrusion) {
	char tst[64];
	DRW_Coord crd;
	dwgBuffer 	buf((char*)tst, sizeof(tst) );

	RESET_BUFF;
	addBits(0, tst, 
			BIT1(1), /* encodes 0,0,1 */
			BITS_STOP_MARKER
			);
	crd = buf.getExtrusion(true);
	EXPECT_DOUBLE_EQ( crd.x, 0.0 );
	EXPECT_DOUBLE_EQ( crd.y, 0.0 );
	EXPECT_DOUBLE_EQ( crd.z, 1.0 );

}

TEST(dwgBuffer, getThickness) {
	char tst[64];
	
	dwgBuffer 	buf((char*)tst, sizeof(tst) );

	RESET_BUFF;
	addBits(0, tst, 
			BIT1(1), /* encodes 0.0 */
			BITS_STOP_MARKER
			);
	EXPECT_DOUBLE_EQ( buf.getThickness(true), 0.0 );
	
	RESET_BUFF;
	addBits(0, tst, 
			BIT1(0), /* encodes NOT 0.0 */
			BIT2(0,1), /* encodes 1.0 */
			BITS_STOP_MARKER
			);
	EXPECT_DOUBLE_EQ( buf.getThickness(true), 1.0 );
		
	RESET_BUFF;
	addBits(0, tst, 
			BIT1(0), /* encodes NOT 0.0 */
			BIT2(1,0), /* encodes 0.0 */
			BITS_STOP_MARKER
			);
	EXPECT_DOUBLE_EQ( buf.getThickness(true), 0.0 );

	RESET_BUFF;
	addBits(0, tst, 
			BIT1(0), /* encodes NOT 0.0 */
			BIT2(0,0), /* encodes a number */
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
	EXPECT_DOUBLE_EQ( buf.getThickness(true), 1.62523e-319 );
	
}

/* Other methods that need love:

	getDefaultDouble
	getBERawShort16
	getBytes
	crc8
*/
