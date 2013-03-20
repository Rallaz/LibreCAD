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

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "dwgreader.h"
#include "drw_textcodec.h"
#include "libdwgr.h"


dwgReader::~dwgReader(){
//    std::map<int, DRW_LType*> ltypemap;
//    std::map<int, DRW_Layer*> layermap;
    delete buf;
}

void dwgReader::parseAttribs(DRW_Entity* e){
    if (e != NULL){
        duint32 ltref =e->lTypeH.ref;
        duint32 lyref =e->layerH.ref;
        std::map<int, DRW_LType*>::iterator lt_it = ltypemap.find(ltref);
        if (lt_it != ltypemap.end()){
            e->lineType = (lt_it->second)->name;
        }
        std::map<int, DRW_Layer*>::iterator ly_it = layermap.find(lyref);
        if (ly_it != layermap.end()){
            e->layer = (ly_it->second)->name;
        }
    }
}

bool dwgReader15::readFileHeader() {
    version = parent->getVersion();
    decoder.setVersion(version);
    DBG("dwgReader15::readFileHeader\n");
    if (! buf->setPosition(13))
        return false;
    seekerImageData = buf->getRawLong32();
    DBG("seekerImageData= "); DBG(seekerImageData); DBG("\n");
    /* UNKNOUWN SECTION 2 bytes*/
    duint16 uk =    buf->getRawShort16();
    DBG("UNKNOUWN SECTION= "); DBG(uk); DBG("\n");
    duint16 cp = buf->getRawShort16();
    DBG("codepage= "); DBG(cp); DBG("\n");
    if (cp == 30)
        decoder.setCodePage("ANSI_1252");
    duint32 count = buf->getRawLong32();
    DBG("count records= ");
    DBG(count);
    DBG("\n");
    for (unsigned int i = 0; i < count; i++) {
        duint8 rec = buf->getRawChar8();
        duint32 seek = buf->getRawLong32();
        duint32 size = buf->getRawLong32();
        if (rec == 0) {
            DBG("Section HEADERS seek= ");
            DBG(seek); DBG(" size= "); DBG(size);  DBG("\n");
            sections["HEADERS"] = std::pair<duint32,duint32>(seek,size);
        } else if (rec == 1) {
            DBG("Section CLASSES seek= ");
            DBG(seek); DBG(" size= "); DBG(size);  DBG("\n");
            sections["CLASSES"] = std::pair<duint32,duint32>(seek,size);
        } else if (rec == 2) {
            DBG("Section OBJECTS seek= ");
            DBG(seek); DBG(" size= "); DBG(size);  DBG("\n");
            sections["OBJECTS"] = std::pair<duint32,duint32>(seek,size);
        } else if (rec == 3) {
            DBG("Section UNKNOWN seek= ");
            DBG(seek); DBG(" size= "); DBG(size);  DBG("\n");
            sections["UNKNOWN"] = std::pair<dint32,dint32>(seek,size);
        } else if (rec == 4) {
            DBG("Section R14DATA seek= ");
            DBG(seek); DBG(" size= "); DBG(size);  DBG("\n");
            sections["R14DATA"] = std::pair<dint32,dint32>(seek,size);
        } else if (rec == 5) {
            DBG("Section R14REC5 seek= ");
            DBG(seek); DBG(" size= "); DBG(size);  DBG("\n");
            sections["R14REC5"] = std::pair<dint32,dint32>(seek,size);
        } else {
            std::cerr << "Unsupported section number\n";
        }
    }
    if (! buf->isGood())
        return false;
    DBG("position after read section locator records= "); DBG(buf->getPosition());
    DBG(", bit are= "); DBG(buf->getBitPos()); DBG("\n");
    duint32 ckcrc = buf->crc8(0,0,buf->getPosition());
    DBG("file header crc8 0 result= "); DBG(ckcrc); DBG("\n");
    switch (count){
    case 3:
        ckcrc = ckcrc ^ 0xA598;
        break;
    case 4:
        ckcrc = ckcrc ^ 0x8101;
        break;
    case 5:
        ckcrc = ckcrc ^ 0x3CC4;
        break;
    case 6:
        ckcrc = ckcrc ^ 0x8461;
    }
    DBG("file header crc8 xor result= "); DBG(ckcrc); DBG("\n");
    DBG("file header CRC= "); DBG(buf->getRawShort16()); DBG("\n");
    DBG("file header sentinel= ");
#ifdef DRWG_DBG
    for (int i=0; i<16;i++) {
        DBGH(buf->getRawChar8()); DBG(" ");
    }
#endif

    DBG("position after read file header sentinel= "); DBG(buf->getPosition());
    DBG(", bit are= "); DBG(buf->getBitPos()); DBG("\n");

    DBG("dwgReader15::readFileHeader END\n");
    return true;
}

/*********** objects map ************************/
/** Note: object map are split in sections with max size 2035?
 *  heach section are 2 bytes size + data bytes + 2 bytes crc
 *  size value are data bytes + 2 and to calculate crc are used
 *  2 bytes size + data bytes
 *  last section are 2 bytes size + 2 bytes crc (size value always 2)
**/
bool dwgReader15::readDwgObjectOffsets() {
    DBG("dwgReader15::readDwgObjectOffsets\n");
    dint32 offset = sections["OBJECTS"].first;
    dint32 maxPos = offset + sections["OBJECTS"].second;
    DBG("Section OBJECTS offset= "); DBG(offset); DBG("\n");
    DBG("Section OBJECTS size= "); DBG(sections["OBJECTS"].second); DBG("\n");
    DBG("Section OBJECTS maxPos= "); DBG(maxPos); DBG("\n");
    if (!buf->setPosition(offset))
        return false;
    DBG("Section OBJECTS buf->curPosition()= "); DBG(buf->getPosition()); DBG("\n");

    int startPos = offset;

    while (maxPos > buf->getPosition()) {
        int pppp = buf->getPosition();
        DBG("start object section buf->curPosition()= "); DBG(pppp); DBG("\n");
        duint16 size = buf->getBERawShort16();
        DBG("object map section size= "); DBG(size); DBG("\n");
        buf->setPosition(startPos);
        char byteStr[size];
        buf->getBytes(byteStr, size);
        dwgBuffer buff(byteStr, size, &decoder);
        if (size != 2){
            buff.setPosition(2);
            int lastHandle = 0;
            int lastLoc = 0;
            //read data
            while(buff.getPosition()< size){
                pppp = buff.getPosition();
                lastHandle += buff.getModularChar();
                DBG("object map lastHandle= "); DBG(lastHandle); DBG("\n");
                lastLoc += buff.getModularChar();
                DBG("object map lastLoc= "); DBG(lastLoc); DBG("\n");
                ObjectMap.push_back(objHandle(0, lastHandle, lastLoc));
            }
        }
        //verify crc
        duint16 crcCalc = buff.crc8(0xc0c1,0,size);
        duint16 crcRead = buf->getBERawShort16();
        DBG("object map section crc8 read= "); DBG(crcRead); DBG("\n");
        DBG("object map section crc8 calculated= "); DBG(crcCalc); DBG("\n");
        pppp = buf->getPosition();
        DBG("object section buf->curPosition()= "); DBG(pppp); DBG("\n");
        startPos = buf->getPosition();
    }

    bool ret = buf->isGood();

    //read object types
    DBG("readDwgObjects() Total objects: "); DBG(ObjectMap.size()); DBG("\n");
    for (std::list<objHandle>::iterator it=ObjectMap.begin(); it != ObjectMap.end(); ++it){
        DBG("object map Handle= "); DBG(it->handle); DBG(" "); DBG(it->loc); DBG("\n");
        ret = buf->setPosition(it->loc); //loc == offset
        if (!ret){
            ret = false;
            continue;
        }
        int size = buf->getModularShort();
        DBG("readDwgObject size: "); DBG(size); DBG("\n");
        it->type = buf->getBitShort();
        DBG("readDwgObject object type: "); DBG(it->type); DBG("\n");
    }

    return ret;
}

/*********** objects ************************/
/**
 * Reads all the object referenced in the object map section of the DWG file
 * (using their object file offsets)
 */
bool dwgReader15::readDwgTables() {
    bool ret = true;
    bool ret2 = true;
    std::list<objHandle>ObjectControlMap;
    std::list<objHandle>LineTypeMap;
    std::list<objHandle>LayerMap;
    std::list<objHandle>BlockRecordMap;

//separate control object, layers and linetypes
    for (std::list<objHandle>::iterator it=ObjectMap.begin(); it != ObjectMap.end(); /*++it*/){
//        DBG("object map Handle= "); DBG(it->handle); DBG(" "); DBG(it->loc); DBG("\n");
        if (it->type == 0x30 || it->type == 0x32 || it->type == 0x34 || it->type == 0x38 || it->type == 0x3C
                || it->type == 0x3E || it->type == 0x40 || it->type == 0x42 || it->type == 0x44 || it->type == 0x46){
            ObjectControlMap.push_back(*it);
            it = ObjectMap.erase(it);
        } else if (it->type == 0x39){
            LineTypeMap.push_back(*it);
            it = ObjectMap.erase(it);
        } else if (it->type == 0x33){
            LayerMap.push_back(*it);
            it = ObjectMap.erase(it);
        } else if (it->type == 0x31 || it->type == 0x4 || it->type == 0x5){
//        } else if (it->type == 0x31){
            BlockRecordMap.push_back(*it);
            it = ObjectMap.erase(it);
        } else
            it++;
    }
    //parse object controls
    //parse linetypes
    for (std::list<objHandle>::iterator it=LineTypeMap.begin(); it != LineTypeMap.end(); ++it){
        DBG("LineTypeMap map Handle= "); DBG(it->handle); DBG(" "); DBG(it->loc); DBG("\n");
        DRW_LType *lt = new DRW_LType();
        buf->setPosition(it->loc);
        int size = buf->getModularShort();
        char byteStr[size];
        buf->getBytes(byteStr, size);
        dwgBuffer buff(byteStr, size, &decoder);
        ret2 = lt->parseDwg(version, &buff);
        ltypemap[lt->handle] = lt;
        if(ret)
            ret = ret2;
    }
//TODO: clear LineTypeMap

    //parse layers
    for (std::list<objHandle>::iterator it=LayerMap.begin(); it != LayerMap.end(); ++it){
        DBG("LayerMap map Handle= "); DBG(it->handle); DBG(" "); DBG(it->loc); DBG("\n");
        DRW_Layer *la = new DRW_Layer();
        buf->setPosition(it->loc);
        int size = buf->getModularShort();
        char byteStr[size];
        buf->getBytes(byteStr, size);
        dwgBuffer buff(byteStr, size, &decoder);
        ret2 = la->parseDwg(version, &buff);
        layermap[la->handle] = la;
        if(ret)
            ret = ret2;
    }
//TODO: clear LayerMap

    //set linetype in layer
    for (std::map<int, DRW_Layer*>::iterator it=layermap.begin(); it!=layermap.end(); ++it) {
        DRW_Layer *ly = it->second;
        duint32 ref =ly->lTypeH.ref;
        std::map<int, DRW_LType*>::iterator lt_it = ltypemap.find(ref);
        if (lt_it != ltypemap.end()){
            ly->lineType = (lt_it->second)->name;
        }
    }

    //parse blocks records
    std::map<int, DRW_Block*> tmpBlockmap;
    for (std::list<objHandle>::iterator it=BlockRecordMap.begin(); it != BlockRecordMap.end(); ++it){
        DBG("BlockMap map Handle= "); DBG(it->handle); DBG(" "); DBG(it->loc); DBG("\n");
        buf->setPosition(it->loc);
        int size = buf->getModularShort();
        char byteStr[size];
        buf->getBytes(byteStr, size);
        dwgBuffer buff(byteStr, size, &decoder);
        switch (it->type){
        case 4: {
            DRW_Block *e= new DRW_Block();
            ret2 = e->parseDwg(version, &buff);
            parseAttribs(e);
            tmpBlockmap[e->handle] = e;
            break; }
        case 5: {
            DRW_Block e;
            e.isEnd = true;
            ret2 = e.parseDwg(version, &buff);
            parseAttribs(&e);
            break; }
        case 49: {
            DRW_Block_Record *br = new DRW_Block_Record();
            ret2 = br->parseDwg(version, &buff);
            block_recmap[br->handle] = br;
            break; }
        default:
            break;
        }
        if(ret)
            ret = ret2;
    }
//TODO: clear BlockRecordMap & tmpBlockmap

    //complete block entity with block record data
    for (std::map<int, DRW_Block_Record*>::iterator it=block_recmap.begin(); it!=block_recmap.end(); ++it) {
        DRW_Block_Record* bkR = it->second;
        std::map<int, DRW_Block*>::iterator bkit = tmpBlockmap.find(bkR->handleBlock);
        if (bkit == tmpBlockmap.end()){//fail, set error
            if(ret)
                ret = ret2;
        } else {
            DRW_Block *bk = bkit->second;
            parseAttribs(bk);
            bk->basePoint = bkR->basePoint;
            bk->flags = bkR->flags;
//            bk->handleBlock = bkR->handle;
            if (bk->handleBlock==0){
                char ppp = bk->name[1];
                if (ppp=='P'){
//                if (bk->name[1]=='P')
                    bk->handleBlock==-1;}
            }
//            if (bk->handleBlock==0 && bk->name[1]=='P'){
//                bk->handleBlock==-1;
//            }
            blockmap[bk->handleBlock] = bk;
        }

    }

    return ret;
}

/**
 * Reads a dwg drawing entity (dwg object entity) given its offset in the file
 */
bool dwgReader15::readDwgEntity(objHandle& obj, DRW_Interface& intfa){
    bool ret = true;

        buf->setPosition(obj.loc);
        int size = buf->getModularShort();
        char byteStr[size];
        buf->getBytes(byteStr, size);
        dwgBuffer buff(byteStr, size, &decoder);
//            ARC, CIRCLE, LINE, POINT, ELLIPSE, BLOCK
//            INSERT, LWPOLYLINE, TEXT,
/*            POLYLINE, VERTEX, SPLINE, HATCH, MTEXT, E3DFACE, IMAGE,
            LEADER, DIMENSION, DIMALIGNED, DIMLINEAR, DIMRADIAL, DIMDIAMETRIC,
            DIMANGULAR, DIMANGULAR3P, DIMORDINATE, TRACE, SOLID, RAY, XLINE, VIEWPORT,*/

        switch (obj.type){
        case 17: {
            DRW_Arc e;
            ret = e.parseDwg(version, &buff);
            if (e.handleBlock != currBlock) {
                currBlock = e.handleBlock;
                intfa.setBlock(e.handleBlock);
            }
            parseAttribs(&e);
            intfa.addArc(e);
            break; }
        case 18: {
            DRW_Circle e;
            ret = e.parseDwg(version, &buff);
            if (e.handleBlock != currBlock) {
                currBlock = e.handleBlock;
                intfa.setBlock(e.handleBlock);
            }
            parseAttribs(&e);
            intfa.addCircle(e);
            break; }
        case 19:{
            DRW_Line e;
            ret = e.parseDwg(version, &buff);
            if (e.handleBlock != currBlock) {
                currBlock = e.handleBlock;
                intfa.setBlock(e.handleBlock);
            }
            parseAttribs(&e);
            intfa.addLine(e);
            break;}
        case 27: {
            DRW_Point e;
            ret = e.parseDwg(version, &buff);
            if (e.handleBlock != currBlock) {
                currBlock = e.handleBlock;
                intfa.setBlock(e.handleBlock);
            }
            parseAttribs(&e);
            intfa.addPoint(e);
            break; }
        case 35: {
            DRW_Ellipse e;
            ret = e.parseDwg(version, &buff);
            if (e.handleBlock != currBlock) {
                currBlock = e.handleBlock;
                intfa.setBlock(e.handleBlock);
            }
            parseAttribs(&e);
            intfa.addEllipse(e);
            break; }
/*        case 7: {//minsert = 8
            DRW_Insert e;
            ret = e.parseDwg(version, &buff);
            if (e.handleBlock != currBlock) {
                currBlock = e.handleBlock;
                intfa.setBlock(e.handleBlock);
            }
            parseAttribs(&e);
            intfa.addInsert(e);
            break; }*/
/*        case 77: {
            DRW_LWPolyline e;
            e.isEnd = true;
            ret = e.parseDwg(version, &buff);
            if (e.handleBlock != currBlock) {
                currBlock = e.handleBlock;
                intfa.setBlock(e.handleBlock);
            }
            parseAttribs(&e);
            intfa.addLWPolyline();
            break; }*/
        case 1: {
            DRW_Text e;
            ret = e.parseDwg(version, &buff);
            if (e.handleBlock != currBlock) {
                currBlock = e.handleBlock;
                intfa.setBlock(e.handleBlock);
            }
            parseAttribs(&e);
            intfa.addText(e);
            break; }
        default:
            break;
        }

    return ret;
}
//        } else if (it->type == 0x31 || it->type == 0x4 || it->type == 0x5){


///////////////////////////////////////////////////////////////////////

/*bool dwgReader15::readDwgHeader() {
return false;
}*/

/*bool dwgReader15::readDwgClasses() {
return false;
}*/


/*********** version 2004 ***************/
bool dwgReader18::readFileHeader() {
//    return (filestr->good());
    return false;
}

bool dwgReader18::readDwgEntity(objHandle& obj, DRW_Interface& intfa) {
    DRW_UNUSED(obj);
    DRW_UNUSED(intfa);
    //    return (filestr->good());
    return false;
}

