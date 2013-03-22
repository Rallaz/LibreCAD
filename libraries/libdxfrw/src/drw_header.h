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

#ifndef DRW_HEADER_H
#define DRW_HEADER_H


//#include <string>
//#include <vector>
#include <map>
#include "drw_base.h"
//#include "dwgbuffer.h" //RLZ TODO: move type defs to drw_base.h

//class dwgBuffer;
class dxfReader;
class dxfWriter;

//using std::string;


//! Class to handle header entries
/*!
*  Class to handle layer symbol table entries
*  @author Rallaz
*/
class DRW_Header {
public:
    DRW_Header() {
    }
    ~DRW_Header() {
        vars.clear();
    }

    void parseCode(int code, dxfReader *reader);
    void write(dxfWriter *writer, DRW::Version ver);
    void addComment(string c);
    string getComments() const {return comments;}
private:
    bool getDouble(string key, double *varDouble);
    bool getInt(string key, int *varInt);
    bool getStr(string key, std::string *varStr);
    bool getCoord(std::string key, DRW_Coord *varStr);

public:
    std::map<string,DRW_Variant*> vars;
private:
    std::string comments;
    std::string name;
    DRW_Variant *curr;
    int version; //to use on read
};

#endif

// EOF

