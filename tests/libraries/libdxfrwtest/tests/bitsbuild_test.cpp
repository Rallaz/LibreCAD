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

TEST(bitsbuild, addBits) {
	char buff[128] = {0};
	
	int crt_off = 0;
	
	crt_off = addBits(crt_off,buff,_ 1,_ 1,_ BITS_STOP_MARKER);
	EXPECT_EQ((unsigned char)buff[0],(unsigned char)128);
	crt_off = addBits(crt_off,buff,_ 1,_ 1,_ BITS_STOP_MARKER);
	EXPECT_EQ((unsigned char)buff[0],(unsigned char)192);
	crt_off = addBits(crt_off,buff,_ 2,_ 3,_ BITS_STOP_MARKER);
	EXPECT_EQ((unsigned char)buff[0],(unsigned char)240);
	
	crt_off = 0;
	buff[0] = 0;
	crt_off = addBits(crt_off,buff,
					  _ 1,_ 1,
					  _ 1,_ 1,
					  _ 2,_ 3,
					  _ BITS_STOP_MARKER);
	EXPECT_EQ((unsigned char)buff[0],(unsigned char)240);
	
}

TEST(bitsbuild, addRawBytes) {
	char buff[64] = {0};
	char buff_src[64] = {0};
	int crt_off = 0;

	for ( int i = 0; i < 64; i++)
	{
		buff_src[i] = (char)i;
	}

	crt_off = addRawBytes( crt_off, buff, buff_src, 64 );
	EXPECT_EQ(crt_off,64*8);
	for ( int i = 0; i < 64; i++)
	{
		EXPECT_EQ(buff[i],buff_src[i]);
	}
}

