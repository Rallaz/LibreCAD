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

#include <cstdlib>
#include "drw_entities.h"
#include "dxfreader.h"
#include "dwgbuffer.h"
#include "libdwgr.h"// for debug


//! Calculate arbitary axis
/*!
*   Calculate arbitary axis for aplly extrusions
*  @author Rallaz
*/
void DRW_Entity::calculateAxis(DRW_Coord extPoint){
    if (fabs(extPoint.x) < 0.015625 && fabs(extPoint.y) < 0.015625) {
        extAxisX.x = extPoint.z;
        extAxisX.y = 0;
        extAxisX.z = -extPoint.x;
    } else {
        extAxisX.x = -extPoint.y;
        extAxisX.y = extPoint.x;
        extAxisX.z = 0;
    }
    extAxisX.unitize();
    extAxisY.x = (extPoint.y * extAxisX.z) - (extAxisX.y * extPoint.z);
    extAxisY.y = (extPoint.z * extAxisX.x) - (extAxisX.z * extPoint.x);
    extAxisY.z = (extPoint.x * extAxisX.y) - (extAxisX.x * extPoint.y);
    extAxisY.unitize();
}
//! Extrude a point using arbitary axis
/*!
*   apply extrusion in a point using arbitary axis (previous calculated)
*  @author Rallaz
*/
void DRW_Entity::extrudePoint(DRW_Coord extPoint, DRW_Coord *point){
    double px, py, pz;
    px = (extAxisX.x*point->x)+(extAxisY.x*point->y)+(extPoint.x*point->z);
    py = (extAxisX.y*point->x)+(extAxisY.y*point->y)+(extPoint.y*point->z);
    pz = (extAxisX.z*point->x)+(extAxisY.z*point->y)+(extPoint.z*point->z);

    point->x = px;
    point->y = py;
    point->z = pz;
}

void DRW_Entity::parseCode(int code, dxfReader *reader){
    switch (code) {
    case 5:
        handle = reader->getHandleString();
        break;
    case 330:
        handleBlock = reader->getHandleString();
        break;
    case 8:
        layer = reader->getUtf8String();
        break;
    case 6:
        lineType = reader->getUtf8String();
        break;
    case 62:
        color = reader->getInt32();
        break;
    case 370:
        lWeight = DRW_LW_Conv::dxfInt2lineWidth(reader->getInt32());
        break;
    case 48:
        ltypeScale = reader->getDouble();
        break;
    case 60:
        visible = reader->getBool();
        break;
    case 420:
        color24 = reader->getInt32();
        break;
    case 430:
        colorName = reader->getString();
        break;
    case 67:
        space = reader->getInt32();
        break;
    default:
        break;
    }
}

bool DRW_Entity::parseDwg(DRW::Version version, dwgBuffer *buf){
    duint32 objSize;
    duint8 ltFlags; //BB
    /*dint16 oType =*/ buf->getBitShort();
    DBG("\n***************************** parsing entity *********************************************\n");

    if (version > DRW::AC1014) {//2000+
        objSize = buf->getRawLong32();  //RL 32bits
    }
    dwgHandle ho = buf->getHandle();
    handle = ho.ref;
    DBG("Entity Handle: "); DBG(ho.code); DBG(".");
    DBG(ho.size); DBG("."); DBG(ho.ref);
    dint16 extDataSize = buf->getBitShort(); //BS
    DBG(" ext data size: "); DBG(extDataSize);
    if (extDataSize>0 && buf->isGood()) {
        dwgHandle ah = buf->getHandle();
        DBG("App Handle: "); DBG(ah.code); DBG(".");
        DBG(ah.size); DBG("."); DBG(ah.ref);
        duint8 dxfCode = buf->getRawChar8();
        DBG(" dxfCode: "); DBG(dxfCode);
        switch (dxfCode){
        case 0:{
            duint8 strLength = buf->getRawChar8();
            DBG(" strLength: "); DBG(strLength);
            duint16 cp = buf->getBERawShort16();
            DBG(" str codepage: "); DBG(cp);
            for (int i=0;i< strLength+1;i++) {//string length + null terminating char
                duint8 dxfChar = buf->getRawChar8();
                DBG(" dxfChar: "); DBG(dxfChar);
            }
            break;
        }
        default:
            /* RLZ: TODO */
            break;
        }
        extDataSize = buf->getBitShort(); //BS
        DBG(" ext data size: "); DBG(extDataSize);
    } //end parsing extData (EED)
    duint8 graphFlag = buf->getBit(); //B
    DBG(" graphFlag: "); DBG(graphFlag); DBG("\n");
    if (graphFlag) {
        duint32 graphData = buf->getRawLong32();  //RL 32bits
        DBG("graphData in bytes: "); DBG(graphData); DBG("\n");
// RLZ: TODO
    }
    if (version < DRW::AC1015) {//14-
        objSize = buf->getRawLong32();  //RL 32bits size in bits
    }
    DBG(" objSize in bits: "); DBG(objSize); DBG("\n");
    duint8 entmode = buf->get2Bits(); //BB
    if (entmode == 0)
        entmode = 2;
    else if(entmode ==2)
        entmode = 0;
    space = entmode;
    DBG("entmode: "); DBG(entmode);
    duint8 numReactors = buf->getBitLong(); //BL
    DBG(", numReactors: "); DBG(numReactors);

    if (version < DRW::AC1015) {//14-
        if(buf->getBit()) //is bylayer line type
            lineType = "BYLAYER";
        else
            lineType = "";
    }
    if (version > DRW::AC1015) {//2004+
        /*duint8 xDictFlag =*/ buf->getBit();
    }
    if (version > DRW::AC1015) {//2004+
        /*duint8 xDictFlag =*/ buf->getBit();
    }

    nextLinkers = buf->getBit(); //aka nolinks //B
    DBG(", nextLinkers: "); DBG(nextLinkers);

    color = buf->getBitShort(); //BS or CMC
    ltypeScale = buf->getBitDouble(); //BD
    DBG(" entity color: "); DBG(color);
    DBG(" ltScale: "); DBG(ltypeScale); DBG("\n");
    if (version > DRW::AC1014) {//2000+
        ltFlags = buf->get2Bits(); //BB
        if (ltFlags == 1)
            lineType = "byblock";
        else if (ltFlags == 2)
            lineType = "continuous";
        else if (ltFlags == 0)
            lineType = "bylayer";
        else //handle at end
            lineType = "";
        DBG("ltFlags: "); DBG(ltFlags);
        DBG(" lineType: "); DBG(lineType.c_str());
        plotFlags = buf->get2Bits(); //BB
        DBG(", plotFlags: "); DBG(plotFlags);
    }
    if (version > DRW::AC1018) {//2007+
        /*duint8 materialFlag =*/ buf->get2Bits(); //BB
        /*duint8 shadowFlag =*/ buf->getRawChar8(); //RC
    }
    dint16 invisibleFlag = buf->getBitShort(); //BS
    DBG(" invisibleFlag: "); DBG(invisibleFlag);
    if (version > DRW::AC1014) {//2000+
        lWeight = DRW_LW_Conv::dwgInt2lineWidth( buf->getRawChar8() ); //RC
        DBG(" lwFlag (lWeight): "); DBG(lWeight); DBG("\n");
    }
    return buf->isGood();
}

bool DRW_Entity::parseDwgEntHandle(DRW::Version version, dwgBuffer *buf){
    //    X handleAssoc;   //X
        DBG("X handleAssoc: \n");
        //lineType handle
        if(space == 2){//entity are in block
            dwgHandle ownerH = buf->getHandle();
            DBG("owner Handle: "); DBG(ownerH.code); DBG(".");
            DBG(ownerH.size); DBG("."); DBG(ownerH.ref); DBG("\n");
            DBG("   Remaining bytes: "); DBG(buf->numRemainingBytes()); DBG("\n");
            if (ownerH.code == 12)
                handleBlock = handle - ownerH.ref;
            else if (ownerH.code == 10)
                handleBlock = handle + ownerH.ref;
        }
        DBG(" Block Handle: "); DBG(handleBlock); DBG(".");
        dwgHandle XDicObjH = buf->getHandle();
        DBG(" XDicObj control Handle: "); DBG(XDicObjH.code); DBG(".");
        DBG(XDicObjH.size); DBG("."); DBG(XDicObjH.ref); DBG("\n");
        DBG("Remaining bytes: "); DBG(buf->numRemainingBytes()); DBG("\n");

        if (version > DRW::AC1014) {//2000+
            if (nextLinkers == 0) {
                for (int i=0; i<2;i++) {
                    dwgHandle nextLinkH = buf->getHandle();
                    DBG(" nextLinkers Handle: "); DBG(nextLinkH.code); DBG(".");
                    DBG(nextLinkH.size); DBG("."); DBG(nextLinkH.ref); DBG("\n");
                    DBG("\n Remaining bytes: "); DBG(buf->numRemainingBytes()); DBG("\n");
                }
            }
        }
        //layer handle
        dwgHandle layerH = buf->getHandle();
        DBG(" layer Handle: "); DBG(layerH.code); DBG(".");
        DBG(layerH.size); DBG("."); DBG(layerH.ref); DBG("\n");
        DBG("   Remaining bytes: "); DBG(buf->numRemainingBytes()); DBG("\n");
        //lineType handle
        if(lineType.empty()){
            dwgHandle ltypeH = buf->getHandle();
            DBG("linetype Handle: "); DBG(ltypeH.code); DBG(".");
            DBG(ltypeH.size); DBG("."); DBG(ltypeH.ref); DBG("\n");
            DBG("   Remaining bytes: "); DBG(buf->numRemainingBytes()); DBG("\n");
        }
        if (version < DRW::AC1015) {//14-
            if (nextLinkers == 0) {
                for (int i=0; i<2;i++) {
                    dwgHandle nextLinkH = buf->getHandle();
                    DBG(" nextLinkers Handle: "); DBG(nextLinkH.code); DBG(".");
                    DBG(nextLinkH.size); DBG("."); DBG(nextLinkH.ref); DBG("\n");
                    DBG("\n Remaining bytes: "); DBG(buf->numRemainingBytes()); DBG("\n");
                }
            }
        }
        if (version > DRW::AC1014) {//2000+
            if (plotFlags == 3) {
                dwgHandle plotStyleH = buf->getHandle();
                DBG(" plot style Handle: "); DBG(plotStyleH.code); DBG(".");
                DBG(plotStyleH.size); DBG("."); DBG(plotStyleH.ref); DBG("\n");
                DBG("\n Remaining bytes: "); DBG(buf->numRemainingBytes()); DBG("\n");
            }
        }
    return buf->isGood();
}

void DRW_Point::parseCode(int code, dxfReader *reader){
    switch (code) {
    case 10:
        basePoint.x = reader->getDouble();
        break;
    case 20:
        basePoint.y = reader->getDouble();
        break;
    case 30:
        basePoint.z = reader->getDouble();
        break;
    case 39:
        thickness = reader->getDouble();
        break;
    case 210:
        haveExtrusion = true;
        extPoint.x = reader->getDouble();
        break;
    case 220:
        extPoint.y = reader->getDouble();
        break;
    case 230:
        extPoint.z = reader->getDouble();
        break;
    default:
        DRW_Entity::parseCode(code, reader);
        break;
    }
}

bool DRW_Point::parseDwg(DRW::Version version, dwgBuffer *buf){
    bool ret = DRW_Entity::parseDwg(version, buf);
    if (!ret)
        return ret;
    DBG("\n***************************** parsing point *********************************************\n");

        basePoint.x = buf->getBitDouble();
        basePoint.y = buf->getBitDouble();
        basePoint.z = buf->getBitDouble();
    DBG("startX: "); DBG(basePoint.x);
    DBG(", startY: "); DBG(basePoint.y);
    DBG(", startZ: "); DBG(basePoint.z);
    bool readOpt = true;
    if (version > DRW::AC1014) {//2000+
        readOpt = !buf->getBit();
    }
    if (readOpt) {
        thickness = buf->getBitDouble();//BD
    }
    readOpt = true;
    if (version > DRW::AC1014) {//2000+
        readOpt = !buf->getBit();
    }
    if (readOpt) {
        extPoint.x = buf->getBitDouble();//BD
        extPoint.y = buf->getBitDouble();//BD
        extPoint.z = buf->getBitDouble();//BD
    }
    double x_axis = buf->getBitDouble();//BD
    DBG("  x_axis: ");DBG(x_axis);DBG("\n");
//    X handleAssoc;   //X
    ret = DRW_Entity::parseDwgEntHandle(version, buf);
    if (!ret)
        return ret;
    //    RS crc;   //RS */

    return buf->isGood();
}

void DRW_Line::parseCode(int code, dxfReader *reader){
    switch (code) {
    case 11:
        secPoint.x = reader->getDouble();
        break;
    case 21:
        secPoint.y = reader->getDouble();
        break;
    case 31:
        secPoint.z = reader->getDouble();
        break;
    default:
        DRW_Point::parseCode(code, reader);
        break;
    }
}

bool DRW_Line::parseDwg(DRW::Version version, dwgBuffer *buf){
    bool ret = DRW_Entity::parseDwg(version, buf);
    if (!ret)
        return ret;
    DBG("\n***************************** parsing line *********************************************\n");

    if (version < DRW::AC1015) {//14-
        basePoint.x = buf->getBitDouble();
        basePoint.y = buf->getBitDouble();
        basePoint.z = buf->getBitDouble();
        secPoint.x = buf->getBitDouble();
        secPoint.y = buf->getBitDouble();
        secPoint.z = buf->getBitDouble();
    }
    if (version > DRW::AC1014) {//2000+
        bool zIsZero = buf->getBit(); //B
        basePoint.x = buf->getRawDouble();//RD
        secPoint.x = buf->getDefaultDouble(basePoint.x);//DD
        basePoint.y = buf->getRawDouble();//RD
        secPoint.y = buf->getDefaultDouble(basePoint.y);//DD
        if (!zIsZero) {
            basePoint.z = buf->getRawDouble();//RD
            secPoint.z = buf->getDefaultDouble(basePoint.z);//DD
        }
    }
    DBG("startX: "); DBG(basePoint.x);
    DBG(", endX: "); DBG(secPoint.x);
    DBG(", startY: "); DBG(basePoint.y);
    DBG(", endY: "); DBG(secPoint.y);
    DBG(", startZ: "); DBG(basePoint.z);
    DBG(", endZ: "); DBG(secPoint.z); DBG("\n");
    bool readOpt = true;
    if (version > DRW::AC1014) {//2000+
        readOpt = !buf->getBit();
    }
    if (readOpt) {
        thickness = buf->getBitDouble();//BD
    }
    readOpt = true;
    if (version > DRW::AC1014) {//2000+
        readOpt = !buf->getBit();
    }
    if (readOpt) {
        extPoint.x = buf->getBitDouble();//BD
        extPoint.y = buf->getBitDouble();//BD
        extPoint.z = buf->getBitDouble();//BD
    }
//    X handleAssoc;   //X
    ret = DRW_Entity::parseDwgEntHandle(version, buf);
    if (!ret)
        return ret;
//    RS crc;   //RS */
    return buf->isGood();
}

void DRW_Circle::applyExtrusion(){
    if (haveExtrusion) {
        calculateAxis(extPoint);
        extrudePoint(extPoint, &basePoint);
    }
}

void DRW_Circle::parseCode(int code, dxfReader *reader){
    switch (code) {
    case 40:
        radious = reader->getDouble();
        break;
    default:
        DRW_Point::parseCode(code, reader);
        break;
    }
}

bool DRW_Circle::parseDwg(DRW::Version version, dwgBuffer *buf){
    bool ret = DRW_Entity::parseDwg(version, buf);
    if (!ret)
        return ret;
    DBG("\n***************************** parsing circle *********************************************\n");

    basePoint.x = buf->getBitDouble();
    basePoint.y = buf->getBitDouble();
    basePoint.z = buf->getBitDouble();
    DBG("startX: "); DBG(basePoint.x);
    DBG(", startY: "); DBG(basePoint.y);
    DBG(", startZ: "); DBG(basePoint.z);
    radious = buf->getBitDouble();
    DBG(" radius: "); DBG(radious);
    bool readOpt = true;
    if (version > DRW::AC1014) {//2000+
        readOpt = !buf->getBit();
    }
    if (readOpt) {
        thickness = buf->getBitDouble();//BD
    }
    readOpt = true;
    if (version > DRW::AC1014) {//2000+
        readOpt = !buf->getBit();
    }
    if (readOpt) {
        extPoint.x = buf->getBitDouble();//BD
        extPoint.y = buf->getBitDouble();//BD
        extPoint.z = buf->getBitDouble();//BD
    }
//    X handleAssoc;   //X
    ret = DRW_Entity::parseDwgEntHandle(version, buf);
    if (!ret)
        return ret;
//    RS crc;   //RS */
    return buf->isGood();
}

void DRW_Arc::parseCode(int code, dxfReader *reader){
    switch (code) {
    case 50:
        staangle = reader->getDouble()/ ARAD;
        break;
    case 51:
        endangle = reader->getDouble()/ ARAD;
        break;
    default:
        DRW_Circle::parseCode(code, reader);
        break;
    }
}

bool DRW_Arc::parseDwg(DRW::Version version, dwgBuffer *buf){
    bool ret = DRW_Entity::parseDwg(version, buf);
    if (!ret)
        return ret;
    DBG("\n***************************** parsing circle arc *********************************************\n");

    basePoint.x = buf->getBitDouble();
    basePoint.y = buf->getBitDouble();
    basePoint.z = buf->getBitDouble();
    DBG("startX: "); DBG(basePoint.x); DBG(", Y: "); DBG(basePoint.y); DBG(", : "); DBG(basePoint.z);
    radious = buf->getBitDouble();
    DBG(" radius: "); DBG(radious);
    bool readOpt = true;
    if (version > DRW::AC1014) {//2000+
        readOpt = !buf->getBit();
    }
    if (readOpt) {
        thickness = buf->getBitDouble();//BD
    }
    readOpt = true;
    if (version > DRW::AC1014) {//2000+
        readOpt = !buf->getBit();
    }
    if (readOpt) {
        extPoint.x = buf->getBitDouble();//BD
        extPoint.y = buf->getBitDouble();//BD
        extPoint.z = buf->getBitDouble();//BD
    }
    staangle = buf->getBitDouble();
    DBG(" start angle: "); DBG(staangle);
    endangle = buf->getBitDouble();
    DBG(" end angle: "); DBG(endangle);
//    X handleAssoc;   //X
//    RS crc;   //RS */
    ret = DRW_Entity::parseDwgEntHandle(version, buf);
    if (!ret)
        return ret;
    return buf->isGood();
}

void DRW_Ellipse::parseCode(int code, dxfReader *reader){
    switch (code) {
    case 40:
        ratio = reader->getDouble();
        break;
    case 41:
        staparam = reader->getDouble();
        break;
    case 42:
        endparam = reader->getDouble();
        break;
    default:
        DRW_Line::parseCode(code, reader);
        break;
    }
}

bool DRW_Ellipse::parseDwg(DRW::Version version, dwgBuffer *buf){
    bool ret = DRW_Entity::parseDwg(version, buf);
    if (!ret)
        return ret;
    DBG("\n***************************** parsing ellipse *********************************************\n");

    basePoint.x = buf->getBitDouble();
    basePoint.y = buf->getBitDouble();
    basePoint.z = buf->getBitDouble();
    secPoint.x = buf->getBitDouble();
    secPoint.y = buf->getBitDouble();
    secPoint.z = buf->getBitDouble();
    DBG("center X: "); DBG(basePoint.x); DBG(", Y: "); DBG(basePoint.y); DBG(", Z: "); DBG(basePoint.z);
    DBG(", axis X: "); DBG(secPoint.x); DBG(", Y: "); DBG(secPoint.y); DBG(", Z: "); DBG(secPoint.z); DBG("\n");
    extPoint.x = buf->getBitDouble();//BD
    extPoint.y = buf->getBitDouble();//BD
    extPoint.z = buf->getBitDouble();//BD
    ratio = buf->getBitDouble();//BD
    DBG("ratio: "); DBG(ratio);
    staparam = buf->getBitDouble();//BD
    DBG(" start param: "); DBG(staparam);
    endparam = buf->getBitDouble();//BD
    DBG(" end param: "); DBG(endparam); DBG("\n");
//    X handleAssoc;   //X
    ret = DRW_Entity::parseDwgEntHandle(version, buf);
    if (!ret)
        return ret;
//    RS crc;   //RS */
    return buf->isGood();
}

void DRW_Ellipse::toPolyline(DRW_Polyline *pol){
    double radMajor, radMinor, cosRot, sinRot, incAngle, curAngle;
    double cosCurr, sinCurr;
    radMajor = sqrt(secPoint.x*secPoint.x + secPoint.y*secPoint.y);
    radMinor = radMajor*ratio;
    incAngle = atan2(secPoint.y, secPoint.x);
    cosRot = cos(incAngle);
    sinRot = sin(incAngle);
    incAngle = M_PI/64;
    curAngle = staparam;
    int i = curAngle/incAngle;
    do {
        if (curAngle > endparam) {
            curAngle = endparam;
            i = 130;
        }
        cosCurr = cos(curAngle);
        sinCurr = sin(curAngle);
        double x = basePoint.x + (cosCurr*cosRot*radMajor) - (sinCurr*sinRot*radMinor);
        double y = basePoint.y + (cosCurr*sinRot*radMajor) + (sinCurr*cosRot*radMinor);
        pol->addVertex( DRW_Vertex(x, y, 0.0, 0.0));
        curAngle = (++i)*incAngle;
    } while (i<128);
    if ( fabs(endparam - 6.28318530718) < 1.0e-10){
        pol->flags = 1;
    }
    pol->layer = this->layer;
    pol->lineType = this->lineType;
    pol->color = this->color;
    pol->lWeight = this->lWeight;
}

void DRW_Trace::applyExtrusion(){
    if (haveExtrusion) {
        calculateAxis(extPoint);
        extrudePoint(extPoint, &basePoint);
        extrudePoint(extPoint, &secPoint);
        extrudePoint(extPoint, &thirdPoint);
        extrudePoint(extPoint, &fourPoint);
    }
}

void DRW_Trace::parseCode(int code, dxfReader *reader){
    switch (code) {
    case 12:
        thirdPoint.x = reader->getDouble();
        break;
    case 22:
        thirdPoint.y = reader->getDouble();
        break;
    case 32:
        thirdPoint.z = reader->getDouble();
        break;
    case 13:
        fourPoint.x = reader->getDouble();
        break;
    case 23:
        fourPoint.y = reader->getDouble();
        break;
    case 33:
        fourPoint.z = reader->getDouble();
        break;
    default:
        DRW_Line::parseCode(code, reader);
        break;
    }
}

void DRW_Solid::parseCode(int code, dxfReader *reader){
        DRW_Trace::parseCode(code, reader);
}

void DRW_3Dface::parseCode(int code, dxfReader *reader){
    switch (code) {
    case 70:
        invisibleflag = reader->getInt32();
        break;
    default:
        DRW_Trace::parseCode(code, reader);
        break;
    }
}

void DRW_Block::parseCode(int code, dxfReader *reader){
    switch (code) {
    case 2:
        name = reader->getUtf8String();
        break;
    case 70:
        flags = reader->getInt32();
        break;
    default:
        DRW_Point::parseCode(code, reader);
        break;
    }
}
bool DRW_Block::parseDwg(DRW::Version version, dwgBuffer *buf){
    bool ret = DRW_Entity::parseDwg(version, buf);
    if (!ret)
        return ret;
    if (!isEnd){
        DBG("\n***************************** parsing block *********************************************\n");
        if (version > DRW::AC1018) {//2007+
            name = buf->getVariableText();
        } else {//2004-
            name = buf->getVariableUtf8Text();
        }
        DBG("Block name: "); DBG(name.c_str()); DBG("\n");
    } else
    DBG("\n***************************** parsing end block *********************************************\n");

//    X handleAssoc;   //X
    ret = DRW_Entity::parseDwgEntHandle(version, buf);
    if (!ret)
        return ret;
//    RS crc;   //RS */
    return buf->isGood();
}

void DRW_Insert::parseCode(int code, dxfReader *reader){
    switch (code) {
    case 2:
        name = reader->getUtf8String();
        break;
    case 41:
        xscale = reader->getDouble();
        break;
    case 42:
        yscale = reader->getDouble();
        break;
    case 43:
        zscale = reader->getDouble();
        break;
    case 50:
        angle = reader->getDouble();
        break;
    case 70:
        colcount = reader->getInt32();
        break;
    case 71:
        rowcount = reader->getInt32();
        break;
    case 44:
        colspace = reader->getDouble();
        break;
    case 45:
        rowspace = reader->getDouble();
        break;
    default:
        DRW_Point::parseCode(code, reader);
        break;
    }
}

void DRW_LWPolyline::applyExtrusion(){
    if (haveExtrusion) {
        calculateAxis(extPoint);
        for (unsigned int i=0; i<vertlist.size(); i++) {
            DRW_Vertex2D *vert = vertlist.at(i);
            DRW_Coord v(vert->x, vert->y, elevation);
            extrudePoint(extPoint, &v);
            vert->x = v.x;
            vert->y = v.y;
        }
    }
}

void DRW_LWPolyline::parseCode(int code, dxfReader *reader){
    switch (code) {
    case 10: {
        vertex = new DRW_Vertex2D();
        vertlist.push_back(vertex);
        vertex->x = reader->getDouble();
        break; }
    case 20:
        if(vertex != NULL)
            vertex->y = reader->getDouble();
        break;
    case 40:
        if(vertex != NULL)
            vertex->stawidth = reader->getDouble();
        break;
    case 41:
        if(vertex != NULL)
            vertex->endwidth = reader->getDouble();
        break;
    case 42:
        if(vertex != NULL)
            vertex->bulge = reader->getDouble();
        break;
    case 38:
        elevation = reader->getDouble();
        break;
    case 43:
        width = reader->getDouble();
        break;
    case 70:
        flags = reader->getInt32();
        break;
    case 90:
        vertexnum = reader->getInt32();
        vertlist.reserve(vertexnum);
        break;
    case 210:
        haveExtrusion = true;
        extPoint.x = reader->getDouble();
        break;
    case 220:
        extPoint.y = reader->getDouble();
        break;
    case 230:
        extPoint.z = reader->getDouble();
        break;
    default:
        DRW_Entity::parseCode(code, reader);
        break;
    }
}

void DRW_Text::parseCode(int code, dxfReader *reader){
    switch (code) {
    case 40:
        height = reader->getDouble();
        break;
    case 41:
        widthscale = reader->getDouble();
        break;
    case 50:
        angle = reader->getDouble();
        break;
    case 51:
        oblique = reader->getDouble();
        break;
    case 71:
        textgen = reader->getInt32();
        break;
    case 72:
        alignH = (HAlign)reader->getInt32();
        break;
    case 73:
        alignV = (VAlign)reader->getInt32();
        break;
    case 1:
        text = reader->getUtf8String();
        break;
    case 7:
        style = reader->getUtf8String();
        break;
    default:
        DRW_Line::parseCode(code, reader);
        break;
    }
}

bool DRW_Text::parseDwg(DRW::Version version, dwgBuffer *buf){
    bool ret = DRW_Entity::parseDwg(version, buf);
    if (!ret)
        return ret;
    DBG("\n***************************** parsing text *********************************************\n");

    if (version < DRW::AC1015) {//14-
        basePoint.z = buf->getBitDouble(); /* Elevation BD --- */
        basePoint.x = buf->getRawDouble(); /* Insertion pt 2RD 10 */
        basePoint.y = buf->getRawDouble();
        secPoint.x = buf->getRawDouble();  /* Alignment pt 2RD 11 */
        secPoint.y = buf->getRawDouble();
        extPoint.x = buf->getBitDouble(); /* Extrusion 3BD 210 */
        extPoint.y = buf->getBitDouble();
        extPoint.z = buf->getBitDouble();
        thickness = buf->getBitDouble(); /* Thickness BD 39 */
        oblique = buf->getBitDouble(); /* Oblique ang BD 51 */
        angle = buf->getBitDouble(); /* Rotation ang BD 50 */
        height = buf->getBitDouble(); /* Height BD 40 */
        widthscale = buf->getBitDouble(); /* Width factor BD 41 */
        text = buf->getVariableUtf8Text(); /* Text value TV 1 */
        textgen = buf->getBitShort(); /* Generation BS 71 */
        alignH = (HAlign)buf->getBitShort(); /* Horiz align. BS 72 */
        alignV = (VAlign)buf->getBitShort(); /* Vert align. BS 73 */
    }
    if (version > DRW::AC1014) {//2000+
        duint8 data_flags = buf->getRawChar8(); /* DataFlags RC Used to determine presence of subsquent data */
        if ( data_flags & 0x01 )
        { /* Elevation RD --- present if !(DataFlags & 0x01) */
            basePoint.z = buf->getRawDouble();
        }
        basePoint.x = buf->getRawDouble(); /* Insertion pt 2RD 10 */
        basePoint.y = buf->getRawDouble();
        if ( data_flags & 0x02 )
        { /* Alignment pt 2DD 11 present if !(DataFlags & 0x02), use 10 & 20 values for 2 default values.*/
            extPoint.x = buf->getDefaultDouble(basePoint.x);
            extPoint.y = buf->getDefaultDouble(basePoint.y);
        }
        else
        {
            extPoint = basePoint;
        }
        buf->getExtrusion(&extPoint); /* Extrusion BE 210 */
        thickness = buf->getThickness(true); /* Thickness BT 39 */
        if ( data_flags & 0x04 )
        { /* Oblique ang RD 51 present if !(DataFlags & 0x04) */
            oblique = buf->getRawDouble();
        }
        if ( data_flags & 0x08 )
        { /* Rotation ang RD 50 present if !(DataFlags & 0x08) */
            angle = buf->getRawDouble();
        }
        height = buf->getRawDouble(); /* Height RD 40 */
        if ( data_flags & 0x10 )
        { /* Width factor RD 41 present if !(DataFlags & 0x10) */
            widthscale = buf->getRawDouble();
        }
        text = buf->getVariableUtf8Text(); /* Text value TV 1 */
        if ( data_flags & 0x20 )
        { /* Generation BS 71 present if !(DataFlags & 0x20) */
            textgen = buf->getBitShort();
        }
        if ( data_flags & 0x40 )
        { /* Horiz align. BS 72 present if !(DataFlags & 0x40) */
            alignH = (HAlign)buf->getBitShort();
        }
        if ( data_flags & 0x80 )
        { /* Vert align. BS 73 present if !(DataFlags & 0x80) */
            alignV = (VAlign)buf->getBitShort();
        }
    }
    
    /* Common Entity Handle Data */
    ret = DRW_Entity::parseDwgEntHandle(version, buf);
    if (!ret)
        return ret;
    
    styleH = buf->getHandle(); /* H 7 STYLE (hard pointer) */
    
    /* CRC X --- */
    return buf->isGood();
}

void DRW_MText::parseCode(int code, dxfReader *reader){
    switch (code) {
    case 1:
        text += reader->getString();
        text = reader->toUtf8String(text);
        break;
    case 11:
        haveXAxis = true;
        DRW_Text::parseCode(code, reader);
        break;
    case 3:
        text += reader->getString();
        break;
    case 44:
        interlin = reader->getDouble();
        break;
    default:
        DRW_Text::parseCode(code, reader);
        break;
    }
}

void DRW_MText::updateAngle(){
    if (haveXAxis) {
            angle = atan2(secPoint.y, secPoint.x)*180/M_PI;
    }
}

void DRW_Polyline::parseCode(int code, dxfReader *reader){
    switch (code) {
    case 70:
        flags = reader->getInt32();
        break;
    case 40:
        defstawidth = reader->getDouble();
        break;
    case 41:
        defendwidth = reader->getDouble();
        break;
    case 71:
        vertexcount = reader->getInt32();
        break;
    case 72:
        facecount = reader->getInt32();
        break;
    case 73:
        smoothM = reader->getInt32();
        break;
    case 74:
        smoothN = reader->getInt32();
        break;
    case 75:
        curvetype = reader->getInt32();
        break;
    default:
        DRW_Point::parseCode(code, reader);
        break;
    }
}

void DRW_Vertex::parseCode(int code, dxfReader *reader){
    switch (code) {
    case 70:
        flags = reader->getInt32();
        break;
    case 40:
        stawidth = reader->getDouble();
        break;
    case 41:
        endwidth = reader->getDouble();
        break;
    case 42:
        bulge = reader->getDouble();
        break;
    case 50:
        tgdir = reader->getDouble();
        break;
    case 71:
        vindex1 = reader->getInt32();
        break;
    case 72:
        vindex2 = reader->getInt32();
        break;
    case 73:
        vindex3 = reader->getInt32();
        break;
    case 74:
        vindex4 = reader->getInt32();
        break;
    case 91:
        identifier = reader->getInt32();
        break;
    default:
        DRW_Point::parseCode(code, reader);
        break;
    }
}

void DRW_Hatch::parseCode(int code, dxfReader *reader){
    switch (code) {
    case 2:
        name = reader->getUtf8String();
        break;
    case 70:
        solid = reader->getInt32();
        break;
    case 71:
        associative = reader->getInt32();
        break;
    case 72:        /*edge type*/
        if (ispol){ //if is polyline is a as_bulge flag
            break;
        } else if (reader->getInt32() == 1){ //line
            addLine();
        } else if (reader->getInt32() == 2){ //arc
            addArc();
        } else if (reader->getInt32() == 3){ //elliptic arc
            addEllipse();
        } else if (reader->getInt32() == 4){ //spline
            addSpline();
        }
        break;
    case 10:
        if (pt) pt->basePoint.x = reader->getDouble();
        else if (pline) {
            plvert = pline->addVertex();
            plvert->x = reader->getDouble();
        }
        break;
    case 20:
        if (pt) pt->basePoint.y = reader->getDouble();
        else if (plvert) plvert ->y = reader->getDouble();
        break;
    case 11:
        if (line) line->secPoint.x = reader->getDouble();
        else if (ellipse) ellipse->secPoint.x = reader->getDouble();
        break;
    case 21:
        if (line) line->secPoint.y = reader->getDouble();
        else if (ellipse) ellipse->secPoint.y = reader->getDouble();
        break;
    case 40:
        if (arc) arc->radious = reader->getDouble();
        else if (ellipse) ellipse->ratio = reader->getDouble();
        break;
    case 41:
        scale = reader->getDouble();
        break;
    case 42:
        if (plvert) plvert ->bulge = reader->getDouble();
        break;
    case 50:
        if (arc) arc->staangle = reader->getDouble();
        else if (ellipse) ellipse->staparam = reader->getDouble();
        break;
    case 51:
        if (arc) arc->endangle = reader->getDouble();
        else if (ellipse) ellipse->endparam = reader->getDouble();
        break;
    case 52:
        angle = reader->getDouble();
        break;
    case 73:
        if (arc) arc->isccw = reader->getInt32();
        else if (pline) pline->flags = reader->getInt32();
        break;
    case 75:
        hstyle = reader->getInt32();
        break;
    case 76:
        hpattern = reader->getInt32();
        break;
    case 77:
        doubleflag = reader->getInt32();
        break;
    case 78:
        deflines = reader->getInt32();
        break;
    case 91:
        loopsnum = reader->getInt32();
        looplist.reserve(loopsnum);
        break;
    case 92:
        loop = new DRW_HatchLoop(reader->getInt32());
        looplist.push_back(loop);
        if (reader->getInt32() & 2) {
            ispol = true;
            clearEntities();
            pline = new DRW_LWPolyline;
            loop->objlist.push_back(pline);
        } else ispol = false;
        break;
    case 93:
        if (pline) pline->vertexnum = reader->getInt32();
        else loop->numedges = reader->getInt32();//aqui reserve
        break;
    case 98: //seed points ??
        clearEntities();
        break;
    default:
        DRW_Point::parseCode(code, reader);
        break;
    }
}

void DRW_Spline::parseCode(int code, dxfReader *reader){
    switch (code) {
    case 210:
        ex = reader->getDouble();
        break;
    case 220:
        ey = reader->getDouble();
        break;
    case 230:
        ez = reader->getDouble();
        break;
    case 12:
        tgsx = reader->getDouble();
        break;
    case 22:
        tgsy = reader->getDouble();
        break;
    case 32:
        tgsz = reader->getDouble();
        break;
    case 13:
        tgex = reader->getDouble();
        break;
    case 23:
        tgey = reader->getDouble();
        break;
    case 33:
        tgez = reader->getDouble();
        break;
    case 70:
        flags = reader->getInt32();
        break;
    case 71:
        degree = reader->getInt32();
        break;
    case 72:
        nknots = reader->getInt32();
        break;
    case 73:
        ncontrol = reader->getInt32();
        break;
    case 74:
        nfit = reader->getInt32();
        break;
    case 42:
        tolknot = reader->getDouble();
        break;
    case 43:
        tolcontrol = reader->getDouble();
        break;
    case 44:
        tolfit = reader->getDouble();
        break;
    case 10: {
        controlpoint = new DRW_Coord();
        controllist.push_back(controlpoint);
        controlpoint->x = reader->getDouble();
        break; }
    case 20:
        if(controlpoint != NULL)
            controlpoint->y = reader->getDouble();
        break;
    case 30:
        if(controlpoint != NULL)
            controlpoint->z = reader->getDouble();
        break;
    case 11: {
        fitpoint = new DRW_Coord();
        fitlist.push_back(fitpoint);
        fitpoint->x = reader->getDouble();
        break; }
    case 21:
        if(fitpoint != NULL)
            fitpoint->y = reader->getDouble();
        break;
    case 31:
        if(fitpoint != NULL)
            fitpoint->z = reader->getDouble();
        break;
    case 40:
        knotslist.push_back(reader->getDouble());
        break;
//    case 41:
//        break;
    default:
        DRW_Entity::parseCode(code, reader);
        break;
    }
}

void DRW_Image::parseCode(int code, dxfReader *reader){
    switch (code) {
    case 12:
        vx = reader->getDouble();
        break;
    case 22:
        vy = reader->getDouble();
        break;
    case 32:
        vz = reader->getDouble();
        break;
    case 13:
        sizeu = reader->getDouble();
        break;
    case 23:
        sizev = reader->getDouble();
        break;
    case 340:
        ref = reader->getString();
        break;
    case 280:
        clip = reader->getInt32();
        break;
    case 281:
        brightness = reader->getInt32();
        break;
    case 282:
        contrast = reader->getInt32();
        break;
    case 283:
        fade = reader->getInt32();
        break;
    default:
        DRW_Line::parseCode(code, reader);
        break;
    }
}

void DRW_Dimension::parseCode(int code, dxfReader *reader){
    switch (code) {
    case 1:
        text = reader->getUtf8String();
        break;
    case 2:
        name = reader->getString();
        break;
    case 3:
        style = reader->getUtf8String();
        break;
    case 70:
        type = reader->getInt32();
        break;
    case 71:
        align = reader->getInt32();
        break;
    case 72:
        linesty = reader->getInt32();
        break;
    case 10:
        defPoint.x = reader->getDouble();
        break;
    case 20:
        defPoint.y = reader->getDouble();
        break;
    case 30:
        defPoint.z = reader->getDouble();
        break;
    case 11:
        textPoint.x = reader->getDouble();
        break;
    case 21:
        textPoint.y = reader->getDouble();
        break;
    case 31:
        textPoint.z = reader->getDouble();
        break;
    case 12:
        clonePoint.x = reader->getDouble();
        break;
    case 22:
        clonePoint.y = reader->getDouble();
        break;
    case 32:
        clonePoint.z = reader->getDouble();
        break;
    case 13:
        def1.x = reader->getDouble();
        break;
    case 23:
        def1.y = reader->getDouble();
        break;
    case 33:
        def1.z = reader->getDouble();
        break;
    case 14:
        def2.x = reader->getDouble();
        break;
    case 24:
        def2.y = reader->getDouble();
        break;
    case 34:
        def2.z = reader->getDouble();
        break;
    case 15:
        circlePoint.x = reader->getDouble();
        break;
    case 25:
        circlePoint.y = reader->getDouble();
        break;
    case 35:
        circlePoint.z = reader->getDouble();
        break;
    case 16:
        arcPoint.x = reader->getDouble();
        break;
    case 26:
        arcPoint.y = reader->getDouble();
        break;
    case 36:
        arcPoint.z = reader->getDouble();
        break;
    case 41:
        linefactor = reader->getDouble();
        break;
    case 53:
        rot = reader->getDouble();
        break;
    case 50:
        angle = reader->getDouble();
        break;
    case 52:
        oblique = reader->getDouble();
        break;
    case 40:
        length = reader->getDouble();
        break;
/*    case 51:
        hdir = reader->getDouble();
        break;*/
    default:
        DRW_Entity::parseCode(code, reader);
        break;
    }
}

void DRW_Leader::parseCode(int code, dxfReader *reader){
    switch (code) {
    case 3:
        style = reader->getUtf8String();
        break;
    case 71:
        arrow = reader->getInt32();
        break;
    case 72:
        leadertype = reader->getInt32();
        break;
    case 73:
        flag = reader->getInt32();
        break;
    case 74:
        hookline = reader->getInt32();
        break;
    case 75:
        hookflag = reader->getInt32();
        break;
    case 76:
        vertnum = reader->getInt32();
        break;
    case 77:
        coloruse = reader->getInt32();
        break;
    case 40:
        textheight = reader->getDouble();
        break;
    case 41:
        textwidth = reader->getDouble();
        break;
    case 10: {
        vertexpoint = new DRW_Coord();
        vertexlist.push_back(vertexpoint);
        vertexpoint->x = reader->getDouble();
        break; }
    case 20:
        if(vertexpoint != NULL)
            vertexpoint->y = reader->getDouble();
        break;
    case 30:
        if(vertexpoint != NULL)
            vertexpoint->z = reader->getDouble();
        break;
    case 340:
        handle = reader->getString();
        break;
    case 210:
        extrusionPoint.x = reader->getDouble();
        break;
    case 220:
        extrusionPoint.y = reader->getDouble();
        break;
    case 230:
        extrusionPoint.z = reader->getDouble();
        break;
    case 211:
        horizdir.x = reader->getDouble();
        break;
    case 221:
        horizdir.y = reader->getDouble();
        break;
    case 231:
        horizdir.z = reader->getDouble();
        break;
    case 212:
        offsetblock.x = reader->getDouble();
        break;
    case 222:
        offsetblock.y = reader->getDouble();
        break;
    case 232:
        offsetblock.z = reader->getDouble();
        break;
    case 213:
        offsettext.x = reader->getDouble();
        break;
    case 223:
        offsettext.y = reader->getDouble();
        break;
    case 233:
        offsettext.z = reader->getDouble();
        break;
    default:
        DRW_Entity::parseCode(code, reader);
        break;
    }
}

void DRW_Viewport::parseCode(int code, dxfReader *reader){
    switch (code) {
    case 40:
        pswidth = reader->getDouble();
        break;
    case 41:
        psheight = reader->getDouble();
        break;
    case 68:
        vpstatus = reader->getInt32();
        break;
    case 69:
        vpID = reader->getInt32();
        break;
    case 12: {
        centerPX = reader->getDouble();
        break; }
    case 22:
        centerPY = reader->getDouble();
        break;
    default:
        DRW_Point::parseCode(code, reader);
        break;
    }
}
