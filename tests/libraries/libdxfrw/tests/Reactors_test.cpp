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
#include	<libdxfrw.h>
#include	<QString>
#include	<QFile>

#include	"../support/bitsbuild.h"
#include	"../support/drw_interface_ghost.h"
#include	"../support/odsample.h"

#undef	EXPECT_DOUBLE_EQ
#define EXPECT_DOUBLE_EQ(a,b)	\
	EXPECT_TRUE( qFuzzyCompare(a,b) )

class ReactorGh : public DRW_InterfaceGhost	{
	bool	b_is_dwg_;

/*
	virtual void addReactor(const DRW_Reactor& data) { 
	}
*/
public:

	static void testFile ( const QString & file )
	{
		ReactorGh	dwg_interf;
		dwgR	dwg_reader(QFile::encodeName(file));
		dwg_interf.b_is_dwg_ = true;
		EXPECT_TRUE( dwg_reader.read(&dwg_interf, APPLY_EXTRUSION) );
	}
	static void testDxfFile ( const QString & file )
	{
		ReactorGh	dxf_interf;
		dxfRW	dxf_reader(QFile::encodeName(file));
		dxf_interf.b_is_dwg_ = false;
		EXPECT_TRUE( dxf_reader.read(&dxf_interf, APPLY_EXTRUSION) );
	}
};

TEST(Reactors, parseFromFile_r14) {
	ReactorGh::testFile( DRWG_TEST_DIR "/data/dwg/libdwgrw_R14_REACTOR.dwg" );
}

TEST(Reactors, parseFromFile_2000) {
	ReactorGh::testFile( DRWG_TEST_DIR "/data/dwg/libdwgrw_2000_REACTOR.dwg" );
}

/*
 
not implemented, yet

TEST(Reactors, parseFromFile_2004) {
	ReactorGh::testFile( DRWG_TEST_DIR "/data/dwg/libdwgrw_2004_REACTOR.dwg" );
}

TEST(Reactors, parseFromFile_2007) {
	ReactorGh::testFile( DRWG_TEST_DIR "/data/dwg/libdwgrw_2007_REACTOR.dwg" );
}

*/

TEST(Reactors, parseDxfFromFile_r12) {
	/* R12 does not have MTEXT */
	ReactorGh::testDxfFile(DRWG_TEST_DIR "/data/dxf/libdwgrw_R12_REACTOR.dxf" );
}

TEST(Reactors, parseDxfFromFile_2000) {
	ReactorGh::testDxfFile(DRWG_TEST_DIR "/data/dxf/libdwgrw_2000_REACTOR.dxf" );
}

TEST(Reactors, parseDxfFromFile_2004) {
	ReactorGh::testDxfFile(DRWG_TEST_DIR "/data/dxf/libdwgrw_2004_REACTOR.dxf" );
}

TEST(Reactors, parseDxfFromFile_2007) {
	ReactorGh::testDxfFile(DRWG_TEST_DIR "/data/dxf/libdwgrw_2007_REACTOR.dxf" );
}

