/******************************************************************************
**  libDXFrw - Library to read/write DXF files (ascii & binary)              **
**                                                                           **
**  Copyright (C) 2011 Rallaz, rallazz@gmail.com                             **
**                                                                           **
**  This library is free software, licensed under the terms of the GNU       **
**  General Public License as published by the Free Software Foundation,     **
**  either version 2 of the License, or (at your option) any later version.  **
**  You should have received a copy of the GNU General Public License        **
**  along with this program.  If not, see <http://www.gnu.org/licenses/>.    **
******************************************************************************/

#ifndef DRW_CLASSES_H
#define DRW_CLASSES_H


#include "drw_base.h"
#include "libdwgr.h"

class dxfReader;
class dxfWriter;
class dwgBuffer;

//! Class to handle header entries
/*!
*  Class to handle layer symbol table entries
*  @author Rallaz
*/
class DRW_Class {
public:
    DRW_Class() {
    }
    ~DRW_Class() {
    }

    void parseCode(int code, dxfReader *reader);
    void write(dxfWriter *writer, DRW::Version ver);
    bool parseDwg(DRW::Version version, dwgBuffer *buf);

private:
    void toDwgType();
public:
    UTF8STRING recNane;      /*!< record name, code 1 */
    UTF8STRING className;    /*!< C++ class name, code 2 */
    UTF8STRING appName;       /*!< app name, code 3 */
    int proxyCapabilities;    /*!< proxy capabilities, code 90 */
    int proxyFlag;         /*!< proxy flag (app loaded on save), code 280 */
    int entityFlag;        /*!< entity flag, code 281 (0 object, 1 entity)*/
public: //only for read dwg
    duint16 classNum;
    int dwgType;
};

#endif

// EOF

