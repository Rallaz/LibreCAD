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

#include <iostream>
#include <math.h>
#include "drw_objects.h"
#include "dxfreader.h"
#include "dxfwriter.h"
#include "libdwgr.h"// for debug

//! Base class for tables entries
/*!
*  Base class for tables entries
*  @author Rallaz
*/
void DRW_TableEntry::parseCode(int code, dxfReader *reader){
    switch (code) {
    case 5:
        handle = reader->getHandleString();
        break;
    case 330:
        handleBlock = reader->getHandleString();
        break;
    case 2:
        name = reader->getUtf8String();
        break;
    case 70:
        flags = reader->getInt32();
        break;
    default:
        break;
    }
}

bool DRW_TableEntry::parseDwg(DRW::Version version, dwgBuffer *buf){
    duint32 objSize;
//    duint8 ltFlags; //BB
//    duint8 plotFlags; //BB
DBG("\n***************************** parsing table entry *********************************************\n");
    /*dint16 oType =*/ buf->getBitShort();
    if (version > DRW::AC1014) {//2000+
        objSize = buf->getRawLong32();  //RL 32bits size in bits
    }
    dwgHandle ho = buf->getHandle();
    handle = ho.ref;
    DBG("TableEntry Handle: "); DBG(ho.code); DBG(".");
    DBG(ho.size); DBG("."); DBG(ho.ref);
    dint16 extDataSize = buf->getBitShort(); //BS
    DBG(" ext data size: "); DBG(extDataSize);
    while (extDataSize>0 && buf->isGood()) {
        /* RLZ: TODO */
        dwgHandle ah = buf->getHandle();
        DBG("App Handle: "); DBG(ah.code); DBG(".");
        DBG(ah.size); DBG("."); DBG(ah.ref);
        char byteStr[extDataSize];
        buf->getBytes(byteStr, extDataSize);
        dwgBuffer buff(byteStr, extDataSize, buf->decoder);
        int pos = buff.getPosition();
        int bpos = buff.getBitPos();
        DBG("ext data pos: "); DBG(pos); DBG("."); DBG(bpos); DBG("\n");
        duint8 dxfCode = buff.getRawChar8();
        DBG(" dxfCode: "); DBG(dxfCode);
        switch (dxfCode){
        case 0:{
            duint8 strLength = buff.getRawChar8();
            DBG(" strLength: "); DBG(strLength);
            duint16 cp = buff.getBERawShort16();
            DBG(" str codepage: "); DBG(cp);
            for (int i=0;i< strLength+1;i++) {//string length + null terminating char
                duint8 dxfChar = buff.getRawChar8();
                DBG(" dxfChar: "); DBG(dxfChar);
            }
            break;
        }
        default:
            /* RLZ: TODO */
            break;
        }
        DBG("ext data pos: "); DBG(buff.getPosition()); DBG("."); DBG(buff.getBitPos()); DBG("\n");
        extDataSize = buf->getBitShort(); //BS
        DBG(" ext data size: "); DBG(extDataSize);
    } //end parsing extData (EED)
    if (version < DRW::AC1015) {//14-
        objSize = buf->getRawLong32();  //RL 32bits size in bits
    }
    DBG(" objSize in bits: "); DBG(objSize);

    numReactors = buf->getBitLong(); //BL
    DBG(", numReactors: "); DBG(numReactors); DBG("\n");
    if (version > DRW::AC1015) {//2004+
        /*duint8 xDictFlag =*/ buf->getBit();
    }
    return buf->isGood();
}

//! Class to handle dimstyle entries
/*!
*  Class to handle ldim style symbol table entries
*  @author Rallaz
*/
void DRW_Dimstyle::parseCode(int code, dxfReader *reader){
    switch (code) {
    case 105:
        handle = reader->getHandleString();
        break;
    case 3:
        dimpost = reader->getUtf8String();
        break;
    case 4:
        dimapost = reader->getUtf8String();
        break;
    case 5:
        dimblk = reader->getUtf8String();
        break;
    case 6:
        dimblk1 = reader->getUtf8String();
        break;
    case 7:
        dimblk2 = reader->getUtf8String();
        break;
    case 40:
        dimscale = reader->getDouble();
        break;
    case 41:
        dimasz = reader->getDouble();
        break;
    case 42:
        dimexo = reader->getDouble();
        break;
    case 43:
        dimdli = reader->getDouble();
        break;
    case 44:
        dimexe = reader->getDouble();
        break;
    case 45:
        dimrnd = reader->getDouble();
        break;
    case 46:
        dimdle = reader->getDouble();
        break;
    case 47:
        dimtp = reader->getDouble();
        break;
    case 48:
        dimtm = reader->getDouble();
        break;
    case 140:
        dimtxt = reader->getDouble();
        break;
    case 141:
        dimcen = reader->getDouble();
        break;
    case 142:
        dimtsz = reader->getDouble();
        break;
    case 143:
        dimaltf = reader->getDouble();
        break;
    case 144:
        dimlfac = reader->getDouble();
        break;
    case 145:
        dimtvp = reader->getDouble();
        break;
    case 146:
        dimtfac = reader->getDouble();
        break;
    case 147:
        dimgap = reader->getDouble();
        break;
    case 148:
        dimaltrnd = reader->getDouble();
        break;
    case 71:
        dimtol = reader->getInt32();
        break;
    case 72:
        dimlim = reader->getInt32();
        break;
    case 73:
        dimtih = reader->getInt32();
        break;
    case 74:
        dimtoh = reader->getInt32();
        break;
    case 75:
        dimse1 = reader->getInt32();
        break;
    case 76:
        dimse2 = reader->getInt32();
        break;
    case 77:
        dimtad = reader->getInt32();
        break;
    case 78:
        dimzin = reader->getInt32();
        break;
    case 79:
        dimazin = reader->getInt32();
        break;
    case 170:
        dimalt = reader->getInt32();
        break;
    case 171:
        dimaltd = reader->getInt32();
        break;
    case 172:
        dimtofl = reader->getInt32();
        break;
    case 173:
        dimsah = reader->getInt32();
        break;
    case 174:
        dimtix = reader->getInt32();
        break;
    case 175:
        dimsoxd = reader->getInt32();
        break;
    case 176:
        dimclrd = reader->getInt32();
        break;
    case 177:
        dimclre = reader->getInt32();
        break;
    case 178:
        dimclrt = reader->getInt32();
        break;
    case 179:
        dimadec = reader->getInt32();
        break;
    case 270:
        dimunit = reader->getInt32();
        break;
    case 271:
        dimdec = reader->getInt32();
        break;
    case 272:
        dimtdec = reader->getInt32();
        break;
    case 273:
        dimaltu = reader->getInt32();
        break;
    case 274:
        dimalttd = reader->getInt32();
        break;
    case 275:
        dimaunit = reader->getInt32();
        break;
    case 276:
        dimfrac = reader->getInt32();
        break;
    case 277:
        dimlunit = reader->getInt32();
        break;
    case 278:
        dimdsep = reader->getInt32();
        break;
    case 279:
        dimtmove = reader->getInt32();
        break;
    case 280:
        dimjust = reader->getInt32();
        break;
    case 281:
        dimsd1 = reader->getInt32();
        break;
    case 282:
        dimsd2 = reader->getInt32();
        break;
    case 283:
        dimtolj = reader->getInt32();
        break;
    case 284:
        dimtzin = reader->getInt32();
        break;
    case 285:
        dimaltz = reader->getInt32();
        break;
    case 286:
        dimaltttz = reader->getInt32();
        break;
    case 287:
        dimfit = reader->getInt32();
        break;
    case 288:
        dimupt = reader->getInt32();
        break;
    case 289:
        dimatfit = reader->getInt32();
        break;
    case 340:
        dimtxsty = reader->getUtf8String();
        break;
    case 341:
        dimldrblk = reader->getUtf8String();
        break;
    case 342:
        dimblk = reader->getUtf8String();
        break;
    case 343:
        dimblk1 = reader->getUtf8String();
        break;
    case 344:
        dimblk2 = reader->getUtf8String();
        break;
    default:
        DRW_TableEntry::parseCode(code, reader);
        break;
    }
}

//! Class to handle line type entries
/*!
*  Class to handle line type symbol table entries
*  @author Rallaz
*/
void DRW_LType::parseCode(int code, dxfReader *reader){
    switch (code) {
    case 3:
        desc = reader->getUtf8String();
        break;
    case 73:
        size = reader->getInt32();
        path.reserve(size);
        break;
    case 40:
        length = reader->getDouble();
        break;
    case 49:
        path.push_back(reader->getDouble());
        pathIdx++;
        break;
/*    case 74:
        haveShape = reader->getInt32();
        break;*/
    default:
        DRW_TableEntry::parseCode(code, reader);
        break;
    }
}

//! Update line type
/*!
*  Update the size and length of line type acording to the path
*  @author Rallaz
*/
/*TODO: control max length permited */
void DRW_LType::update(){
    double d =0;
    size = path.size();
    for (int i = 0;  i< size; i++){
        d += fabs(path.at(i));
    }
    length = d;
}

bool DRW_LType::parseDwg(DRW::Version version, dwgBuffer *buf){
    bool ret = DRW_TableEntry::parseDwg(version, buf);
    DBG("\n***************************** parsing line type *********************************************\n");
    if (!ret)
        return ret;
    if (version > DRW::AC1018) {//2007+
        name = buf->getVariableText();
    } else {//2004-
        name = buf->getVariableUtf8Text();
    }
    DBG("linetype name: "); DBG(name.c_str()); DBG("\n");
    flags = buf->getBit()<< 6;
    DBG("flags: "); DBG(flags);
    dint16 xrefindex = buf->getBitShort();
    DBG(" xrefindex: "); DBG(xrefindex);
    duint8 xdep = buf->getBit();
    DBG(" xdep: "); DBG(xdep);
    flags |= xdep<< 4;
    DBG(" flags: "); DBG(flags);
    desc = buf->getVariableUtf8Text();
    DBG(" desc: "); DBG(desc.c_str());
    length = buf->getBitDouble();
    DBG(" length: "); DBG(length);
    char align = buf->getRawChar8();
    DBG(" align: "); DBG(align);
    size = buf->getRawChar8();
    DBG(" num dashes, size: "); DBG(size);
    DBG("\n    dashes:\n");
    for (int i=0; i< size; i++){
        path.push_back(buf->getBitDouble());
        /*int bs1 =*/ buf->getBitShort();
        /*double d1= */buf->getRawDouble();
        /*double d2=*/ buf->getRawDouble();
        /*double d3= */buf->getBitDouble();
        /*double d4= */buf->getBitDouble();
        /*int bs2 = */buf->getBitShort();
    }
    DBG("\n Remaining bytes: "); DBG(buf->numRemainingBytes()); DBG("\n");
    if (version < DRW::AC1021) { //2004-
        char strarea[256];
        buf->getBytes(strarea, 256);
        DBG("string area 256 bytes:\n"); DBG(strarea); DBG("\n");
    } else { //2007+
        char strarea[512];
        buf->getBytes(strarea, 512);
        DBG("string area 512 bytes:\n"); DBG(strarea); DBG("\n");
    }

//    DBG("\n Remaining bytes: "); DBG(buf->numRemainingBytes()); DBG("\n");
    dwgHandle ltControlH = buf->getHandle();
    DBG("linetype control Handle: "); DBG(ltControlH.code); DBG(".");
    DBG(ltControlH.size); DBG("."); DBG(ltControlH.ref);
    DBG("\n Remaining bytes: "); DBG(buf->numRemainingBytes()); DBG("\n");
    dwgHandle reactorsH = buf->getHandle();
    DBG(" reactorsH control Handle: "); DBG(reactorsH.code); DBG(".");
    DBG(reactorsH.size); DBG("."); DBG(reactorsH.ref); DBG("\n");
    dwgHandle XDicObjH = buf->getHandle();
    DBG(" XDicObj control Handle: "); DBG(XDicObjH.code); DBG(".");
    DBG(XDicObjH.size); DBG("."); DBG(XDicObjH.ref); DBG("\n");
    if(size>0){
        dwgHandle XRefH = buf->getHandle();
        DBG(" XRefH control Handle: "); DBG(XRefH.code); DBG(".");
        DBG(XRefH.size); DBG("."); DBG(XRefH.ref); DBG("\n");
        dwgHandle shpHandle = buf->getHandle();
        DBG(" shapeFile Handle: "); DBG(shpHandle.code); DBG(".");
        DBG(shpHandle.size); DBG("."); DBG(shpHandle.ref); DBG("\n");
    }

    DBG("\n Remaining bytes: "); DBG(buf->numRemainingBytes()); DBG("\n");
//    RS crc;   //RS */
    return buf->isGood();
}

//! Class to handle layer entries
/*!
*  Class to handle layer symbol table entries
*  @author Rallaz
*/
void DRW_Layer::parseCode(int code, dxfReader *reader){
    switch (code) {
    case 6:
        lineType = reader->getUtf8String();
        break;
    case 62:
        color = reader->getInt32();
        break;
    case 290:
        plotF = reader->getBool();
        break;
    case 370:
        lWeight = DRW_LW_Conv::dxfInt2lineWidth(reader->getInt32());
        break;
    case 390:
        handlePlotS = reader->getString();
        break;
    case 347:
        handlePlotM = reader->getString();
        break;
    default:
        DRW_TableEntry::parseCode(code, reader);
        break;
    }
}

bool DRW_Layer::parseDwg(DRW::Version version, dwgBuffer *buf){
    bool ret = DRW_TableEntry::parseDwg(version, buf);
    DBG("\n***************************** parsing layer *********************************************\n");
    if (!ret)
        return ret;
    if (version > DRW::AC1018) {//2007+
        name = buf->getVariableText();
    } else {//2004-
        name = buf->getVariableUtf8Text();
    }
    DBG("layer name: "); DBG(name.c_str());

    flags |= buf->getBit()<< 6;//layer have entity
    /*dint16 xrefindex =*/ buf->getBitShort();
    flags |= buf->getBit() << 4;//is refx dependent
    if (version < DRW::AC1015) {//14-
        flags |= buf->getBit(); //layer frozen
        /*flags |=*/ buf->getBit(); //unused, negate the color
        flags |= buf->getBit() << 1;//frozen in new
        flags |= buf->getBit()<< 3;//locked
    }
    if (version > DRW::AC1014) {//2000+
        dint16 f = buf->getBitShort();//bit2 are layer on
        DBG(", flags 2000+: "); DBG(f); DBG("\n");
        flags |= f & 0x0001; //layer frozen
        flags |= ( f>> 1) & 0x0002;//frozen in new
        flags |= ( f>> 1) & 0x0004;//locked
        plotF = ( f>> 4) & 0x0001;
        lWeight = DRW_LW_Conv::dwgInt2lineWidth( (f & 0x03E0) >> 5 );
    }
    color = buf->getBitShort(); //BS or CMC //ok for R14 or negate
    DBG(", entity color: "); DBG(color); DBG("\n");

    dwgHandle layerControlH = buf->getHandle();
    DBG("layer control Handle: "); DBG(layerControlH.code); DBG(".");
    DBG(layerControlH.size); DBG("."); DBG(layerControlH.ref);

    dwgHandle XDicObjH = buf->getHandle();
    DBG(" XDicObj control Handle: "); DBG(XDicObjH.code); DBG(".");
    DBG(XDicObjH.size); DBG("."); DBG(XDicObjH.ref); DBG("\n");
    dwgHandle XRefH = buf->getHandle();
    DBG(" XRefH control Handle: "); DBG(XRefH.code); DBG(".");
    DBG(XRefH.size); DBG("."); DBG(XRefH.ref); DBG("\n");
    if (version > DRW::AC1014) {//2000+
        dwgHandle plotStyH = buf->getHandle();
        DBG(" PLot style control Handle: "); DBG(plotStyH.code); DBG(".");
        DBG(plotStyH.size); DBG("."); DBG(plotStyH.ref); DBG("\n");
    }
    //lineType handle
    lTypeH = buf->getHandle();
    DBG("line type Handle: "); DBG(lTypeH.code); DBG(".");
    DBG(lTypeH.size); DBG("."); DBG(lTypeH.ref);
    DBG("\n Remaining bytes: "); DBG(buf->numRemainingBytes()); DBG("\n");
//    RS crc;   //RS */
    return buf->isGood();
}

bool DRW_Block_Record::parseDwg(DRW::Version version, dwgBuffer *buf){
    bool ret = DRW_TableEntry::parseDwg(version, buf);
    DBG("\n***************************** parsing block record ******************************************\n");
    if (!ret)
        return ret;
    duint32 insertCount = 0;//only 2000+
    duint32 objectCount = 0; //only 2004+

    if (version > DRW::AC1018) {//2007+
        name = buf->getVariableText();
    } else {//2004-
        name = buf->getVariableUtf8Text();
    }
    DBG("block record name: "); DBG(name.c_str());DBG("\n");

    flags |= buf->getBit()<< 6;//referenced external reference, block code 70, bit 7 (64)
    /*dint16 xrefindex =*/ buf->getBitShort();
    flags |= buf->getBit() << 4;//is refx dependent, block code 70, bit 5 (16)
    flags |= buf->getBit(); //if is anonimous block (*U) block code 70, bit 1 (1)
    flags |= buf->getBit() << 1; //if block contains attdefs, block code 70, bit 2 (2)
    bool blockIsXref = buf->getBit(); //if is a Xref, block code 70, bit 3 (4)
    bool xrefOverlaid = buf->getBit(); //if is a overlaid Xref, block code 70, bit 4 (8)
    flags |= blockIsXref << 2; //if is a Xref, block code 70, bit 3 (4)
    flags |= xrefOverlaid << 3; //if is a overlaid Xref, block code 70, bit 4 (8)
    if (version > DRW::AC1014) {//2000+
        flags |= buf->getBit() << 5; //if is a loaded Xref, block code 70, bit 6 (32)
    }
    if (version > DRW::AC1015) {//2004+
        objectCount = buf->getBitLong(); //Number of objects owned by this
    }
    basePoint.x = buf->getBitDouble();
    basePoint.y = buf->getBitDouble();
    basePoint.z = buf->getBitDouble();
    DBG("insertion point X: "); DBG(basePoint.x); DBG(", Y: "); DBG(basePoint.y); DBG(", Z: "); DBG(basePoint.z); DBG("\n");
    if (version > DRW::AC1018) {//2007+
        UTF8STRING path = buf->getVariableText();
    } else {//2004-
        UTF8STRING path = buf->getVariableUtf8Text();
    }
    if (version > DRW::AC1014) {//2000+
        insertCount = 0;
        while (duint8 i = buf->getRawChar8() != 0)
            insertCount +=i;
        if (version > DRW::AC1018) {//2007+
            UTF8STRING bkdesc = buf->getVariableText();
        } else {//2004-
            UTF8STRING bkdesc = buf->getVariableUtf8Text();
        }
        duint32 prevData = buf->getBitLong();
        for (unsigned int i= 0; i < prevData; i++)
            buf->getRawChar8();
    }
    if (version > DRW::AC1018) {//2007+
        duint16 insUnits = buf->getBitShort();
        bool canExplode = buf->getBit(); //if block can be exploded
        duint8 bkScaling = buf->getRawChar8();
    }

    dwgHandle blockControlH = buf->getHandle();
    DBG("block control Handle: "); DBG(blockControlH.code); DBG(".");
    DBG(blockControlH.size); DBG("."); DBG(blockControlH.ref);

    for (int i=0; i<numReactors; i++){
        dwgHandle reactorH = buf->getHandle();
        DBG(" reactor Handle #"); DBG(i); DBG(": "); DBG(reactorH.code); DBG(".");
        DBG(reactorH.size); DBG("."); DBG(reactorH.ref); DBG("\n");
    }
    dwgHandle XDicObjH = buf->getHandle();
    DBG(" XDicObj control Handle: "); DBG(XDicObjH.code); DBG(".");
    DBG(XDicObjH.size); DBG("."); DBG(XDicObjH.ref); DBG("\n");
    dwgHandle NullH = buf->getHandle();
    DBG(" NullH control Handle: "); DBG(NullH.code); DBG(".");
    DBG(NullH.size); DBG("."); DBG(NullH.ref); DBG("\n");
    dwgHandle blockH = buf->getHandle();
    DBG(" blockH control Handle: "); DBG(blockH.code); DBG(".");
    DBG(blockH.size); DBG("."); DBG(blockH.ref); DBG("\n");
    handleBlock = blockH.ref;

    if (version > DRW::AC1015) {//2004+
        for (unsigned int i=0; i< objectCount; i++){
            dwgHandle entityH = buf->getHandle();
            DBG(" entityH Handle #"); DBG(i); DBG(": "); DBG(entityH.code); DBG(".");
            DBG(entityH.size); DBG("."); DBG(entityH.ref); DBG("\n");
        }
    } else {//2000-
        if(!blockIsXref && !xrefOverlaid){
            dwgHandle firstH = buf->getHandle();
            DBG(" firstH entity Handle: "); DBG(firstH.code); DBG(".");
            DBG(firstH.size); DBG("."); DBG(firstH.ref); DBG("\n");
            dwgHandle lastH = buf->getHandle();
            DBG(" lastH entity Handle: "); DBG(lastH.code); DBG(".");
            DBG(lastH.size); DBG("."); DBG(lastH.ref); DBG("\n");
        }
    }
    dwgHandle endBlockH = buf->getHandle();
    DBG(" endBlockHl Handle: "); DBG(endBlockH.code); DBG(".");
    DBG(endBlockH.size); DBG("."); DBG(endBlockH.ref); DBG("\n");

    if (version > DRW::AC1014) {//2000+
        for (unsigned int i=0; i< insertCount; i++){
            dwgHandle insertsH = buf->getHandle();
            DBG(" insertsH Handle #"); DBG(i); DBG(": "); DBG(insertsH.code); DBG(".");
            DBG(insertsH.size); DBG("."); DBG(insertsH.ref); DBG("\n");
        }
        dwgHandle layoutH = buf->getHandle();
        DBG(" layoutH Handle: "); DBG(layoutH.code); DBG(".");
        DBG(layoutH.size); DBG("."); DBG(layoutH.ref); DBG("\n");
    }
    DBG("\n Remaining bytes: "); DBG(buf->numRemainingBytes()); DBG("\n");
//    RS crc;   //RS */
    return buf->isGood();
}

//! Class to handle text style entries
/*!
*  Class to handle text style symbol table entries
*  @author Rallaz
*/
void DRW_Textstyle::parseCode(int code, dxfReader *reader){
    switch (code) {
    case 3:
        font = reader->getUtf8String();
        break;
    case 4:
        bigFont = reader->getUtf8String();
        break;
    case 40:
        height = reader->getDouble();
        break;
    case 41:
        width = reader->getDouble();
        break;
    case 50:
        oblique = reader->getDouble();
        break;
    case 42:
        lastHeight = reader->getDouble();
        break;
    case 71:
        genFlag = reader->getInt32();
        break;
    case 1071:
        fontFamily = reader->getInt32();
        break;
    default:
        DRW_TableEntry::parseCode(code, reader);
        break;
    }
}

//! Class to handle vport entries
/*!
*  Class to handle vport symbol table entries
*  @author Rallaz
*/
void DRW_Vport::parseCode(int code, dxfReader *reader){
    switch (code) {
    case 10:
        lowerLeft.x = reader->getDouble();
        break;
    case 20:
        lowerLeft.y = reader->getDouble();
        break;
    case 11:
        UpperRight.x = reader->getDouble();
        break;
    case 21:
        UpperRight.y = reader->getDouble();
        break;
    case 12:
        center.x = reader->getDouble();
        break;
    case 22:
        center.y = reader->getDouble();
        break;
    case 13:
        snapBase.x = reader->getDouble();
        break;
    case 23:
        snapBase.y = reader->getDouble();
        break;
    case 14:
        snapSpacing.x = reader->getDouble();
        break;
    case 24:
        snapSpacing.y = reader->getDouble();
        break;
    case 15:
        gridSpacing.x = reader->getDouble();
        break;
    case 25:
        gridSpacing.y = reader->getDouble();
        break;
    case 16:
        viewDir.x = reader->getDouble();
        break;
    case 26:
        viewDir.y = reader->getDouble();
        break;
    case 36:
        viewDir.z = reader->getDouble();
        break;
    case 17:
        viewTarget.x = reader->getDouble();
        break;
    case 27:
        viewTarget.y = reader->getDouble();
        break;
    case 37:
        viewTarget.z = reader->getDouble();
        break;
    case 40:
        height = reader->getDouble();
        break;
    case 41:
        ratio = reader->getDouble();
        break;
    case 42:
        lensHeight = reader->getDouble();
        break;
    case 43:
        frontClip = reader->getDouble();
        break;
    case 44:
        backClip = reader->getDouble();
        break;
    case 50:
        snapAngle = reader->getDouble();
        break;
    case 51:
        twistAngle = reader->getDouble();
        break;
    case 71:
        viewMode = reader->getInt32();
        break;
    case 72:
        circleZoom = reader->getInt32();
        break;
    case 73:
        fastZoom = reader->getInt32();
        break;
    case 74:
        ucsIcon = reader->getInt32();
        break;
    case 75:
        snap = reader->getInt32();
        break;
    case 76:
        grid = reader->getInt32();
        break;
    case 77:
        snapStyle = reader->getInt32();
        break;
    case 78:
        snapIsopair = reader->getInt32();
        break;
    default:
        DRW_TableEntry::parseCode(code, reader);
        break;
    }
}

void DRW_ImageDef::parseCode(int code, dxfReader *reader){
    switch (code) {
    case 1:
        name = reader->getUtf8String();
        break;
    case 5:
        handle = reader->getString();
        break;
    case 10:
        u = reader->getDouble();
        break;
    case 20:
        v = reader->getDouble();
        break;
    case 11:
        up = reader->getDouble();
        break;
    case 12:
        vp = reader->getDouble();
        break;
    case 21:
        vp = reader->getDouble();
        break;
    case 280:
        loaded = reader->getInt32();
        break;
    case 281:
        resolution = reader->getInt32();
        break;
    default:
        break;
    }
}

void DRW_Header::addComment(string c){
    if (!comments.empty())
        comments += '\n';
    comments += c;
}

void DRW_Header::parseCode(int code, dxfReader *reader){
    switch (code) {
    case 9:
        curr = new DRW_Variant();
        name = reader->getString();
        if (version < DRW::AC1015 && name == "$DIMUNIT")
            name="$DIMLUNIT";
        vars[name]=curr;
        break;
    case 1:
        curr->addString(reader->getUtf8String());
        if (name =="$ACADVER") {
            reader->setVersion(curr->content.s);
            version = reader->getVersion();
        }
        curr->code = code;
        break;
    case 2:
        curr->addString(reader->getUtf8String());
        curr->code = code;
        break;
    case 3:
        curr->addString(reader->getUtf8String());
        if (name =="$DWGCODEPAGE") {
            reader->setCodePage(curr->content.s);
            curr->addString(reader->getCodePage());
        }
        curr->code = code;
        break;
    case 6:
        curr->addString(reader->getUtf8String());
        curr->code = code;
        break;
    case 7:
        curr->addString(reader->getUtf8String());
        curr->code = code;
        break;
    case 8:
        curr->addString(reader->getUtf8String());
        curr->code = code;
        break;
    case 10:
        curr->addCoord(new DRW_Coord());
        curr->setCoordX(reader->getDouble());
        curr->code = code;
        break;
    case 20:
        curr->setCoordY(reader->getDouble());
        break;
    case 30:
        curr->setCoordZ(reader->getDouble());
        curr->code = code;
        break;
    case 40:
        curr->addDouble(reader->getDouble());
        curr->code = code;
        break;
    case 50:
        curr->addDouble(reader->getDouble());
        curr->code = code;
        break;
    case 62:
        curr->addInt(reader->getInt32());
        curr->code = code;
        break;
    case 70:
        curr->addInt(reader->getInt32());
        curr->code = code;
        break;
    case 280:
        curr->addInt(reader->getInt32());
        curr->code = code;
        break;
    case 290:
        curr->addInt(reader->getInt32());
        curr->code = code;
        break;
    case 370:
        curr->addInt(reader->getInt32());
        curr->code = code;
        break;
    case 380:
        curr->addInt(reader->getInt32());
        curr->code = code;
        break;
    case 390:
        curr->addString(reader->getUtf8String());
        curr->code = code;
        break;
    default:
        break;
    }
}

void DRW_Header::write(dxfWriter *writer, DRW::Version ver){
/*RLZ: TODO complete all vars to AC1024*/
    double varDouble;
    int varInt;
    std::string varStr;
    DRW_Coord varCoord;
    writer->writeString(2, "HEADER");
    writer->writeString(9, "$ACADVER");
    switch (ver) {
    case DRW::AC1006: //unsupported version acad 10
    case DRW::AC1009: //acad 11 & 12
        varStr = "AC1009";
        break;
    case DRW::AC1012: //unsupported version acad 13
    case DRW::AC1014: //acad 14
        varStr = "AC1014";
        break;
    case DRW::AC1015: //acad 2000
        varStr = "AC1015";
        break;
    case DRW::AC1018: //acad 2004
        varStr = "AC1018";
        break;
/*    case DRW::AC1021: //acad 2007
        varStr = "AC1021";
        break;*/
    case DRW::AC1024: //acad 2010
        varStr = "AC1024";
        break;
    default: //acad 2007 default version
        varStr = "AC1021";
        break;
    }
    writer->writeString(1, varStr);
    writer->setVersion(&varStr);

    if (ver > DRW::AC1012) {
        writer->writeString(9, "$HANDSEED");
//RLZ        dxfHex(5, 0xFFFF);
        writer->writeString(5, "20000");
    }
    if (!getStr("$DWGCODEPAGE", &varStr)) {
        varStr = "ANSI_1252";
    }
    writer->writeString(9, "$DWGCODEPAGE");
    writer->setCodePage(&varStr);
    writer->writeString(3, writer->getCodePage() );
    writer->writeString(9, "$INSBASE");
    if (getCoord("$INSBASE", &varCoord)) {
        writer->writeDouble(10, varCoord.x);
        writer->writeDouble(20, varCoord.y);
        writer->writeDouble(30, varCoord.z);
    } else {
        writer->writeDouble(10, 0.0);
        writer->writeDouble(20, 0.0);
        writer->writeDouble(30, 0.0);
    }
    writer->writeString(9, "$EXTMIN");
    if (getCoord("$EXTMIN", &varCoord)) {
        writer->writeDouble(10, varCoord.x);
        writer->writeDouble(20, varCoord.y);
        writer->writeDouble(30, varCoord.z);
    } else {
        writer->writeDouble(10, 1.0000000000000000E+020);
        writer->writeDouble(20, 1.0000000000000000E+020);
        writer->writeDouble(30, 1.0000000000000000E+020);
    }
    writer->writeString(9, "$EXTMAX");
    if (getCoord("$EXTMAX", &varCoord)) {
        writer->writeDouble(10, varCoord.x);
        writer->writeDouble(20, varCoord.y);
        writer->writeDouble(30, varCoord.z);
    } else {
        writer->writeDouble(10, -1.0000000000000000E+020);
        writer->writeDouble(20, -1.0000000000000000E+020);
        writer->writeDouble(30, -1.0000000000000000E+020);
    }
    writer->writeString(9, "$LIMMIN");
    if (getCoord("$LIMMIN", &varCoord)) {
        writer->writeDouble(10, varCoord.x);
        writer->writeDouble(20, varCoord.y);
    } else {
        writer->writeDouble(10, 0.0);
        writer->writeDouble(20, 0.0);
    }
    writer->writeString(9, "$LIMMAX");
    if (getCoord("$LIMMAX", &varCoord)) {
        writer->writeDouble(10, varCoord.x);
        writer->writeDouble(20, varCoord.y);
    } else {
        writer->writeDouble(10, 420.0);
        writer->writeDouble(20, 297.0);
    }
    writer->writeString(9, "$ORTHOMODE");
    if (getInt("$ORTHOMODE", &varInt))
        writer->writeInt16(70, varInt);
    else
        writer->writeInt16(70, 0);
    writer->writeString(9, "$LTSCALE");
    if (getDouble("$LTSCALE", &varDouble))
        writer->writeDouble(40, varDouble);
    else
        writer->writeDouble(40, 1.0);
    writer->writeString(9, "$TEXTSTYLE");
    if (getStr("$TEXTSTYLE", &varStr))
        if (ver == DRW::AC1009)
            writer->writeUtf8Caps(7, varStr);
        else
            writer->writeUtf8String(7, varStr);
    else
        writer->writeString(7, "STANDARD");

    writer->writeString(9, "$DIMASZ");
    if (getDouble("$DIMASZ", &varDouble))
        writer->writeDouble(40, varDouble);
    else
        writer->writeDouble(40, 2.5);
    writer->writeString(9, "$DIMSCALE");
    if (getDouble("$DIMSCALE", &varDouble))
        writer->writeDouble(40, varDouble);
    else
        writer->writeDouble(40, 1.0);
    writer->writeString(9, "$DIMEXO");
    if (getDouble("$DIMEXO", &varDouble))
        writer->writeDouble(40, varDouble);
    else
        writer->writeDouble(40, 0.625);
    writer->writeString(9, "$DIMEXE");
    if (getDouble("$DIMEXE", &varDouble))
        writer->writeDouble(40, varDouble);
    else
        writer->writeDouble(40, 1.25);
    writer->writeString(9, "$DIMTXT");
    if (getDouble("$DIMTXT", &varDouble))
        writer->writeDouble(40, varDouble);
    else
        writer->writeDouble(40, 2.5);
    writer->writeString(9, "$DIMTSZ");
    if (getDouble("$DIMTSZ", &varDouble))
        writer->writeDouble(40, varDouble);
    else
        writer->writeDouble(40, 0.0);
    if (ver > DRW::AC1009) {
        writer->writeString(9, "$DIMAUNIT");
        if (getInt("$DIMAUNIT", &varInt))
            writer->writeInt16(70, varInt);
        else
            writer->writeInt16(70, 0);
        writer->writeString(9, "$DIMADEC");
        if (getInt("$DIMADEC", &varInt))
            writer->writeInt16(70, varInt);
        else
            writer->writeInt16(70, 0);
    }
    //verify if exist "$DIMLUNIT" or obsolete "$DIMUNIT" (pre v2000)
    if ( !getInt("$DIMLUNIT", &varInt) ){
        if (!getInt("$DIMUNIT", &varInt))
            varInt = 2;
    }
    //verify valid values from 1 to 6
    if (varInt<1 || varInt>6)
        varInt = 2;
    if (ver > DRW::AC1014) {
        writer->writeString(9, "$DIMLUNIT");
        writer->writeInt16(70, varInt);
    } else {
        writer->writeString(9, "$DIMUNIT");
        writer->writeInt16(70, varInt);
    }
    writer->writeString(9, "$DIMSTYLE");
    if (getStr("$DIMSTYLE", &varStr))
        if (ver == DRW::AC1009)
            writer->writeUtf8Caps(2, varStr);
        else
            writer->writeUtf8String(2, varStr);
    else
        writer->writeString(2, "STANDARD");
    writer->writeString(9, "$DIMGAP");
    if (getDouble("$DIMGAP", &varDouble))
        writer->writeDouble(40, varDouble);
    else
        writer->writeDouble(40, 0.625);
    writer->writeString(9, "$DIMTIH");
    if (getInt("$DIMTIH", &varInt))
        writer->writeInt16(70, varInt);
    else
        writer->writeInt16(70, 0);

    writer->writeString(9, "$LUNITS");
    if (getInt("$LUNITS", &varInt))
        writer->writeInt16(70, varInt);
    else
        writer->writeInt16(70, 2);
    writer->writeString(9, "$LUPREC");
    if (getInt("$LUPREC", &varInt))
        writer->writeInt16(70, varInt);
    else
        writer->writeInt16(70, 4);
    writer->writeString(9, "$AUNITS");
    if (getInt("$AUNITS", &varInt))
        writer->writeInt16(70, varInt);
    else
        writer->writeInt16(70, 0);
    writer->writeString(9, "$AUPREC");
    if (getInt("$AUPREC", &varInt))
        writer->writeInt16(70, varInt);
    else
        writer->writeInt16(70, 2);
    if (ver > DRW::AC1009) {
    writer->writeString(9, "$SPLINESEGS");
    if (getInt("$SPLINESEGS", &varInt)) {
        writer->writeInt16(70, varInt);
    } else
        writer->writeInt16(70, 8);
    }
/* RLZ: moved to active VPORT, but can write in header if present*/
    if (getInt("$GRIDMODE", &varInt)) {
        writer->writeString(9, "$GRIDMODE");
        writer->writeInt16(70, varInt);
    }
    if (getInt("$SNAPSTYLE", &varInt)) {
        writer->writeString(9, "$SNAPSTYLE");
        writer->writeInt16(70, varInt);
    }
    if (getCoord("$GRIDUNIT", &varCoord)) {
        writer->writeString(9, "$GRIDUNIT");
        writer->writeDouble(10, varCoord.x);
        writer->writeDouble(20, varCoord.y);
    }
    if (getCoord("$VIEWCTR", &varCoord)) {
        writer->writeString(9, "$VIEWCTR");
        writer->writeDouble(10, varCoord.x);
        writer->writeDouble(20, varCoord.y);
    }
/* RLZ: moved to active VPORT, but can write in header if present*/

    if (ver > DRW::AC1009) {
        writer->writeString(9, "$PINSBASE");
        if (getCoord("$PINSBASE", &varCoord)) {
            writer->writeDouble(10, varCoord.x);
            writer->writeDouble(20, varCoord.y);
            writer->writeDouble(30, varCoord.z);
        } else {
            writer->writeDouble(10, 0.0);
            writer->writeDouble(20, 0.0);
            writer->writeDouble(30, 0.0);
        }
    }
    writer->writeString(9, "$PLIMMIN");
    if (getCoord("$PLIMMIN", &varCoord)) {
        writer->writeDouble(10, varCoord.x);
        writer->writeDouble(20, varCoord.y);
    } else {
        writer->writeDouble(10, 0.0);
        writer->writeDouble(20, 0.0);
    }
    writer->writeString(9, "$PLIMMAX");
    if (getCoord("$PLIMMAX", &varCoord)) {
        writer->writeDouble(10, varCoord.x);
        writer->writeDouble(20, varCoord.y);
    } else {
        writer->writeDouble(10, 297.0);
        writer->writeDouble(20, 210.0);
    }
    if (ver > DRW::AC1014) {
        writer->writeString(9, "$INSUNITS");
        if (getInt("$INSUNITS", &varInt))
            writer->writeInt16(70, varInt);
        else
            writer->writeInt16(70, 0);
    }
    if (ver > DRW::AC1009) {
        writer->writeString(9, "$PSVPSCALE");
        if (getDouble("$PSVPSCALE", &varDouble))
            writer->writeDouble(40, varDouble);
        else
            writer->writeDouble(40, 0.0);
    }
    std::map<std::string,DRW_Variant *>::const_iterator it;
    for ( it=vars.begin() ; it != vars.end(); it++ ){
//        QString key = QString::fromStdString((*it).first);
        std::cerr << (*it).first << std::endl;
    }

}

bool DRW_Header::getDouble(string key, double *varDouble){
    bool result = false;
    std::map<std::string,DRW_Variant *>::iterator it;
    it=vars.find( key);
    if (it != vars.end()) {
        DRW_Variant *var = (*it).second;
        if (var->type == DRW_Variant::DOUBLE) {
            *varDouble = var->content.d;
            result = true;
        }
        vars.erase (it);
    }
    return result;
}

bool DRW_Header::getInt(string key, int *varInt){
    bool result = false;
    std::map<std::string,DRW_Variant *>::iterator it;
    it=vars.find( key);
    if (it != vars.end()) {
        DRW_Variant *var = (*it).second;
        if (var->type == DRW_Variant::INTEGER) {
            *varInt = var->content.i;
            result = true;
        }
        vars.erase (it);
    }
    return result;
}

bool DRW_Header::getStr(string key, std::string *varStr){
    bool result = false;
    std::map<std::string,DRW_Variant *>::iterator it;
    it=vars.find( key);
    if (it != vars.end()) {
        DRW_Variant *var = (*it).second;
        if (var->type == DRW_Variant::STRING) {
            *varStr = *var->content.s;
            result = true;
        }
        vars.erase (it);
    }
    return result;
}

bool DRW_Header::getCoord(string key, DRW_Coord *varCoord){
    bool result = false;
    std::map<std::string,DRW_Variant *>::iterator it;
    it=vars.find( key);
    if (it != vars.end()) {
        DRW_Variant *var = (*it).second;
        if (var->type == DRW_Variant::COORD) {
            *varCoord = *var->content.v;
            result = true;
        }
        vars.erase (it);
    }
    return result;
}
