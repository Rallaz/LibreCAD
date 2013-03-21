/****************************************************************************
**
** This file is part of the LibreCAD project, a 2D CAD program
**
**  Copyright (C) 2011 Rallaz, rallazz@gmail.com
** Copyright (C) 2010 R. van Twisk (librecad@rvt.dds.nl)
**
**
** This file may be distributed and/or modified under the terms of the
** GNU General Public License as published by the Free Software
** Foundation either version 2 of the License, or (at your option)
**  any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
**
**********************************************************************/


#include "filterdxfrw.h"

#include <stdio.h>
//#include <map>

/*#include "rs_dimaligned.h"
#include "rs_dimangular.h"
#include "rs_dimdiametric.h"
#include "rs_dimlinear.h"
#include "rs_dimradial.h"
#include "rs_hatch.h"
#include "rs_image.h"
#include "rs_leader.h"
#include "rs_spline.h"
#include "rs_system.h"
#include "rs_graphicview.h"
#include "rs_grid.h"*/

#include <QStringList>
#include <QApplication>
#include <QFile>
#include <QFileInfo>

//#include <qtextcodec.h>
#include <QMessageBox>


/**
 * Default constructor.
 *
 */
FilterDXFRW::FilterDXFRW()
    :DRW_Interface() {

    DBG("FilterDXFRW::RS_FilterDXFRW()\n");

// Init hash to change the QCAD "normal" style to the more correct ISO-3059
// or draftsight symbol (AR*.shx) to sy*.lff
    fontList["normal"] = "iso";
    fontList["normallatin1"] = "iso";
    fontList["normallatin2"] = "iso";
    fontList["arastro"] = "syastro";
    fontList["armap"] = "symap";
    fontList["math"] = "symath";
    fontList["armeteo"] = "symeteo";
    fontList["armusic"] = "symusic";


    DBG("FilterDXFRW::RS_FilterDXFRW(): OK\n");
}

/**
 * Destructor.
 */
FilterDXFRW::~FilterDXFRW() {
    DBG("FilterDXFRW::~RS_FilterDXFRW(): OK");
}



/**
 * Implementation of the method used for RS_Import to communicate
 * with this filter.
 *
 * @param g The graphic in which the entities from the file
 * will be created or the graphics from which the entities are
 * taken to be stored in a file.
 */
bool FilterDXFRW::fileImport(const QString& file) {
    DBG("FilterDXFRW::fileImport\n");

    DBG(QString("DXFRW Filter: importing file '%1'\n").arg(file) );
    this->file = file;

        dwg = new dwgR(QFile::encodeName(file));
        bool success = dwg->read(this, true);
        QApplication *ap = qobject_cast<QApplication *>(qApp);
        QWidget *parentWin = NULL;
        if (ap)
            parentWin = ap->activeWindow();

        switch (dwg->getVersion()) {
        case DRW::AC1006:
            versionStr = "dwg version 10";
            break;
        case DRW::AC1009:
            versionStr = "dwg version 11 or 12";
            break;
        case DRW::AC1012:
            versionStr = "dwg version 13";
            break;
        case DRW::AC1014:
            versionStr = "dwg version 14";
            break;
        case DRW::AC1015:
            versionStr = "dwg version 2000";
            break;
        case DRW::AC1018:
            versionStr = "dwg version 2004";
            break;
        case DRW::AC1021:
            versionStr = "dwg version 2007";
            break;
        case DRW::AC1024:
            versionStr = "dwg version 2010";
            break;
        default:
            versionStr = "unknown dwg version";
        }
        switch (dwg->getError()) {
        case DRW::BAD_UNKNOWN:
            QMessageBox::warning(parentWin, "LibreCAD message", QObject::tr("unknown error opening dwg file")
                                + "\n" + versionStr);
            DBG(QString("DRW::BAD_UNKNOWN\n"));
            break;
        case DRW::BAD_OPEN:
            QMessageBox::warning(parentWin, "LibreCAD message", QObject::tr("can't open this dwg file")
                                + "\n" + versionStr);
            DBG(QString("DRW::BAD_OPEN\n"));
            break;
        case DRW::BAD_VERSION:
            QMessageBox::warning(parentWin, "LibreCAD message", QObject::tr("unsupported dwg version")
                                + "\n" + versionStr);
            DBG(QString("DRW::BAD_VERSION\n"));
            break;
        case DRW::BAD_READ_FILE_HEADER:
            QMessageBox::warning(parentWin, "LibreCAD message", QObject::tr("error reading file header in dwg file")
                                + "\n" + versionStr);
            DBG(QString("DRW::BAD_READ\n"));
            break;
        case DRW::BAD_READ_OFFSETS:
            QMessageBox::warning(parentWin, "LibreCAD message", QObject::tr("error reading offsets in dwg file")
                                + "\n" + versionStr);
            DBG(QString("DRW::BAD_READ\n"));
            break;
        case DRW::BAD_READ_TABLES:
            QMessageBox::warning(parentWin, "LibreCAD message", QObject::tr("error reading tables in dwg file")
                                + "\n" + versionStr);
            DBG(QString("DRW::BAD_READ\n"));
            break;
        case DRW::BAD_READ_ENTITIES:
            QMessageBox::warning(parentWin, "LibreCAD message", QObject::tr("error reading entities in dwg file")
                                + "\n" + versionStr);
            DBG(QString("DRW::BAD_READ\n"));
            break;
        default:
            QMessageBox::warning(parentWin, "LibreCAD message", QObject::tr("opening dwg file")
                                + "\n" + versionStr);
        }

        if (success==false) {
            DBG(QString("Cannot open DWG file '%1'\n.").arg(file) );
            delete dwg;
            return false;
        }
        delete dwg;
//        return true;

    // add some variables that need to be there for DXF drawings:
//    graphic->addVariable("$DIMSTYLE", "Standard", 2);
    dimStyle = "Standard";
    codePage = "ANSI_1252";
    textStyle = "Standard";

    return true;
}

/**
 * Implementation of the method which handles layers.
 */
void FilterDXFRW::addLayer(const DRW_Layer &data) {
    DBG("--Layer----------------------------------------------------------------------------\n");
    DBG("RS_FilterDXF::addLayer,");
//    DBG(QString("  adding layer: %1\n").arg(data.name.c_str()));


    QString name = QString::fromUtf8(data.name.c_str());
    DBG(QString("  adding layer: %1\n").arg(name));

    DBG("RS_FilterDXF::EntityAttributes:\n");
    DBG(" Color: "); DBG(data.color);
    DBG(" Linetype: "); DBG( QString::fromUtf8(data.lineType.c_str()) );
    DBG(" Width: "); DBG(data.lWeight);
    DBG("\n");

    DBG("RS_FilterDXF::addLayer: flags\n");
    if (data.flags&0x01) {
        DBG(" Layer is frozen,");
    } else DBG(" Layer is NOT frozen,");
    if (data.flags&0x04) {
        DBG(" Layer is locked,");
    } else DBG(" Layer is NOT locked,");
    if (data.plotF) {
        DBG(" Layer is NOT a help layer, plot it.\n");
    } else DBG(" This is a help layer. do not plot.\n");

    DBG("RS_FilterDXF::addLayer: OK\n");
    DBG("--end layer-------------------------------------------------------------------------\n");
}

/**
 * Implementation of the method which handles vports.
 */
void FilterDXFRW::addVport(const DRW_Vport &data) {
    QString name = QString::fromStdString(data.name);
    if (name.toLower() == "*active") {
//        data.grid == 1? graphic->setGridOn(true):graphic->setGridOn(false);
//        graphic->setIsometricGrid(data.snapStyle);
//        graphic->setCrosshairType( (RS2::CrosshairType)data.snapIsopair);
/*        RS_GraphicView *gv = graphic->getGraphicView();
        if (gv != NULL) {
            double width = data.height * data.ratio;
            double factorX= gv->getWidth() / width;
            double factorY= gv->getHeight() / data.height;
            if (factorX > factorY)
                factorX = factorY;
            int ox = gv->getWidth() -data.center.x*2*factorX;
            int oy = gv->getHeight() -data.center.y*2*factorX;
            gv->setOffset(ox, oy);
            gv->setFactor(factorX);
        }*/
    }
}

/**
 * Implementation of the method which handles blocks.
 *
 * @todo Adding blocks to blocks (stack for currentContainer)
 */
void FilterDXFRW::addBlock(const DRW_Block& data) {

    DBG("RS_FilterDXF::addBlock");

    DBG(QString("  adding block: %1").arg(data.name.c_str()));
/*TODO correct handle of model-space*/

    QString name = QString::fromUtf8(data.name.c_str());
    QString mid = name.mid(1,11);
// Prevent special blocks (paper_space, model_space) from being added:
    if (mid.toLower() != "paper_space" && mid.toLower() != "model_space") {

//            RS_Vector bp(data.basePoint.x, data.basePoint.y);
/*            RS_Block* block =
                new RS_Block(graphic, RS_BlockData(name, bp, false ));
            //block->setFlags(flags);

            if (graphic->addBlock(block)) {
                currentContainer = block;
            }*/
    }
}

void FilterDXFRW::setBlock(const int /*handle*/) {
}


/**
 * Implementation of the method which closes blocks.
 */
void FilterDXFRW::endBlock() {
/*    if (currentContainer->rtti() == RS2::EntityBlock) {
        RS_Block *bk = (RS_Block *)currentContainer;
        //remove unnamed blocks *D only if version != R12
        if (version!=1009) {
            if (bk->getName().startsWith("*D") )
                graphic->removeBlock(bk);
        }
    }
    currentContainer = graphic;*/
}



/**
 * Implementation of the method which handles point entities.
 */
void FilterDXFRW::addPoint(const DRW_Point& data) {
    Q_UNUSED(data);
/*    RS_Vector v(data.basePoint.x, data.basePoint.y);

    RS_Point* entity = new RS_Point(currentContainer,
                                    RS_PointData(v));
    setEntityAttributes(entity, &data);

    currentContainer->addEntity(entity);*/
}



/**
 * Implementation of the method which handles line entities.
 */
void FilterDXFRW::addLine(const DRW_Line& data) {
    Q_UNUSED(data);
    DBG("RS_FilterDXF::addLine:\n");
    DBG("start point: "); DBG(data.basePoint.x); DBG(" , "); DBG(data.basePoint.y); DBG("\n");
    DBG("end point: "); DBG(data.secPoint.x); DBG(" , "); DBG(data.secPoint.y); DBG("\n");

/*    RS_Vector v1(data.basePoint.x, data.basePoint.y);
    RS_Vector v2(data.secPoint.x, data.secPoint.y);

    DBG("RS_FilterDXF::addLine: create line");

    if (currentContainer==NULL) {
        DBG("RS_FilterDXF::addLine: currentContainer is NULL");
    }

    RS_Line* entity = new RS_Line(currentContainer,
                                  RS_LineData(v1, v2));
    DBG("RS_FilterDXF::addLine: set attributes");*/
    setEntityAttributes(&data);

/*    DBG("RS_FilterDXF::addLine: add entity");

    currentContainer->addEntity(entity);

    DBG("RS_FilterDXF::addLine: OK");*/
}


/**
 * Implementation of the method which handles ray entities.
 */
void FilterDXFRW::addRay(const DRW_Ray& data) {
    Q_UNUSED(data);
    DBG("RS_FilterDXF::addRay");

/*    RS_Vector v1(data.basePoint.x, data.basePoint.y);
    RS_Vector v2(data.basePoint.x+data.secPoint.x, data.basePoint.y+data.secPoint.y);

    DBG("RS_FilterDXF::addRay: create line");

    if (currentContainer==NULL) {
        DBG("RS_FilterDXF::addRay: currentContainer is NULL");
    }

    RS_Line* entity = new RS_Line(currentContainer,
                                  RS_LineData(v1, v2));
    DBG("RS_FilterDXF::addRay: set attributes");
    setEntityAttributes(entity, &data);

    DBG("RS_FilterDXF::addRay: add entity");

    currentContainer->addEntity(entity);*/

    DBG("RS_FilterDXF::addRay: OK");
}


/**
 * Implementation of the method which handles line entities.
 */
void FilterDXFRW::addXline(const DRW_Xline& data) {
    Q_UNUSED(data);
    DBG("RS_FilterDXF::addXline");

/*    RS_Vector v1(data.basePoint.x, data.basePoint.y);
    RS_Vector v2(data.basePoint.x+data.secPoint.x, data.basePoint.y+data.secPoint.y);

    DBG("RS_FilterDXF::addXline: create line");

    if (currentContainer==NULL) {
        DBG("RS_FilterDXF::addXline: currentContainer is NULL");
    }

    RS_Line* entity = new RS_Line(currentContainer,
                                  RS_LineData(v1, v2));
    DBG("RS_FilterDXF::addXline: set attributes");
    setEntityAttributes(entity, &data);

    DBG("RS_FilterDXF::addXline: add entity");

    currentContainer->addEntity(entity);*/

    DBG("RS_FilterDXF::addXline: OK");
}



/**
 * Implementation of the method which handles circle entities.
 */
void FilterDXFRW::addCircle(const DRW_Circle& data) {
    Q_UNUSED(data);
    DBG("RS_FilterDXF::addCircle");

/*    RS_Vector v(data.basePoint.x, data.basePoint.y);
    RS_CircleData d(v, data.radious);
    RS_Circle* entity = new RS_Circle(currentContainer, d);
    setEntityAttributes(entity, &data);

    currentContainer->addEntity(entity);*/
}



/**
 * Implementation of the method which handles arc entities.
 *
 * @param angle1 Start angle in deg (!)
 * @param angle2 End angle in deg (!)
 */
void FilterDXFRW::addArc(const DRW_Arc& data) {
    Q_UNUSED(data);
    DBG("RS_FilterDXF::addArc");
/*    RS_Vector v(data.basePoint.x, data.basePoint.y);
    RS_ArcData d(v, data.radious,
                 data.staangle/ARAD,
                 data.endangle/ARAD,
                 false);
    RS_Arc* entity = new RS_Arc(currentContainer, d);
    setEntityAttributes(entity, &data);

    currentContainer->addEntity(entity);*/
}



/**
 * Implementation of the method which handles ellipse entities.
 *
 * @param angle1 Start angle in rad (!)
 * @param angle2 End angle in rad (!)
 */
void FilterDXFRW::addEllipse(const DRW_Ellipse& data) {
    Q_UNUSED(data);
    DBG("FilterDXFRW::addEllipse");

/*    RS_Vector v1(data.basePoint.x, data.basePoint.y);
    RS_Vector v2(data.secPoint.x, data.secPoint.y);
    double ang2 = data.endparam;
    if ( fabs(ang2- 6.28318530718) < 1.0e-10 && fabs(data.staparam) < 1.0e-10 )
        ang2 = 0.0;
    RS_EllipseData ed(v1, v2, data.ratio, data.staparam,
                                    ang2, false);
    RS_Ellipse* entity = new RS_Ellipse(currentContainer, ed);
    setEntityAttributes(entity, &data);

    currentContainer->addEntity(entity);*/
}


/**
 * Implementation of the method which handles trace entities.
 */
void FilterDXFRW::addTrace(const DRW_Trace& data) {
    Q_UNUSED(data);
/*    RS_Solid* entity;
    RS_Vector v1(data.basePoint.x, data.basePoint.y);
    RS_Vector v2(data.secPoint.x, data.secPoint.y);
    RS_Vector v3(data.thirdPoint.x, data.thirdPoint.y);
    RS_Vector v4(data.fourPoint.x, data.fourPoint.y);
    if (v3 == v4)
        entity = new RS_Solid(currentContainer, RS_SolidData(v1, v2, v3));
    else
        entity = new RS_Solid(currentContainer, RS_SolidData(v1, v2, v3,v4));

    setEntityAttributes(entity, &data);
    currentContainer->addEntity(entity);*/
}

/**
 * Implementation of the method which handles solid entities.
 */
void FilterDXFRW::addSolid(const DRW_Solid& data) {
    addTrace(data);
}

/**
 * Implementation of the method which handles lightweight polyline entities.
 */
void FilterDXFRW::addLWPolyline(const DRW_LWPolyline& data) {
    Q_UNUSED(data);
    DBG("FilterDXFRW::addLWPolyline");
/*    RS_PolylineData d(RS_Vector(false),
                      RS_Vector(false),
                      data.flags&0x1);
    RS_Polyline *polyline = new RS_Polyline(currentContainer, d);
    setEntityAttributes(polyline, &data);

    for (unsigned int i=0; i<data.vertlist.size(); i++) {
        DRW_Vertex2D *vert = data.vertlist.at(i);
        RS_Vector v(vert->x, vert->y);
        polyline->addVertex(v, vert->bulge);
    }

    currentContainer->addEntity(polyline);*/
}


/**
 * Implementation of the method which handles polyline entities.
 */
void FilterDXFRW::addPolyline(const DRW_Polyline& data) {
    Q_UNUSED(data);
    DBG("FilterDXFRW::addPolyline");
    if ( data.flags&0x10)
        return; //the polyline is a polygon mesh, not handled

    if ( data.flags&0x40)
        return; //the polyline is a poliface mesh, TODO convert

/*    RS_PolylineData d(RS_Vector(false),
                      RS_Vector(false),
                      data.flags&0x1);
    RS_Polyline *polyline = new RS_Polyline(currentContainer, d);
    setEntityAttributes(polyline, &data);
    for (unsigned int i=0; i<data.vertlist.size(); i++) {
        DRW_Vertex *vert = data.vertlist.at(i);
        RS_Vector v(vert->basePoint.x, vert->basePoint.y);
        polyline->addVertex(v, vert->bulge);
    }

    currentContainer->addEntity(polyline);*/
}


/**
 * Implementation of the method which handles splines.
 */
void FilterDXFRW::addSpline(const DRW_Spline* data) {
    DBG(QString("FilterDXFRW::addSpline: degree: %1").arg(data->degree));

/*        RS_Spline* spline;
        if (data->degree>=1 && data->degree<=3) {
        RS_SplineData d(data->degree, ((data->flags&0x1)==0x1));
        spline = new RS_Spline(currentContainer, d);
        setEntityAttributes(spline, data);

        currentContainer->addEntity(spline);
    } else {
        DBG(QString("RS_FilterDXF::addSpline: Invalid degree for spline: %1. "
                        "Accepted values are 1..3.").arg(data->degree).toStdString());
        return;
    }
    for (unsigned int i=0; i<data->controllist.size(); i++) {
        DRW_Coord *vert = data->controllist.at(i);
        RS_Vector v(vert->x, vert->y);
        spline->addControlPoint(v);
    }
    spline->update();*/
}


/**
 * Implementation of the method which handles inserts.
 */
void FilterDXFRW::addInsert(const DRW_Insert& data) {
    Q_UNUSED(data);

    DBG("RS_FilterDXF::addInsert");

/*    RS_Vector ip(data.basePoint.x, data.basePoint.y);
    RS_Vector sc(data.xscale, data.yscale);
    RS_Vector sp(data.colspace, data.rowspace);

    //cout << "Insert: " << name << " " << ip << " " << cols << "/" << rows << endl;

    RS_InsertData d( QString::fromUtf8(data.name.c_str()),
                    ip, sc, data.angle/ARAD,
                    data.colcount, data.rowcount,
                    sp, NULL, RS2::NoUpdate);
    RS_Insert* entity = new RS_Insert(currentContainer, d);
    setEntityAttributes(entity, &data);
    DBG(QString("  id: %1").arg(entity->getId()));
//    entity->update();
    currentContainer->addEntity(entity);*/
}



/**
 * Implementation of the method which handles
 * multi texts (MTEXT).
 */
void FilterDXFRW::addMText(const DRW_MText& data) {
    DBG(QString("RS_FilterDXF::addMText: %1").arg(data.text.c_str()));

/*    RS_MTextData::VAlign valign;
    RS_MTextData::HAlign halign;
    RS_MTextData::MTextDrawingDirection dir;
    RS_MTextData::MTextLineSpacingStyle lss;
    QString sty = QString::fromUtf8(data.style.c_str());
    sty=sty.toLower();

    if (data.textgen<=3) {
        valign=RS_MTextData::VATop;
    } else if (data.textgen<=6) {
        valign=RS_MTextData::VAMiddle;
    } else {
        valign=RS_MTextData::VABottom;
    }

    if (data.textgen%3==1) {
        halign=RS_MTextData::HALeft;
    } else if (data.textgen%3==2) {
        halign=RS_MTextData::HACenter;
    } else {
        halign=RS_MTextData::HARight;
    }

    if (data.alignH==1) {
        dir = RS_MTextData::LeftToRight;
    } else if (data.alignH==3) {
        dir = RS_MTextData::TopToBottom;
    } else {
        dir = RS_MTextData::ByStyle;
    }

    if (data.alignV==1) {
        lss = RS_MTextData::AtLeast;
    } else {
        lss = RS_MTextData::Exact;
    }
*/
    QString mtext = toNativeString(QString::fromUtf8(data.text.c_str()));
    // use default style for the drawing:
/*    if (sty.isEmpty()) {
        // japanese, cyrillic:
        if (codePage=="ANSI_932" || codePage=="ANSI_1251") {
            sty = "Unicode";
        } else {
            sty = textStyle;
        }
    } else {
        sty = fontList.value(sty, sty);
    }*/

    DBG("Text as unicode:");
//    RS_DEBUG->printUnicode(mtext);
    double interlin = data.interlin;
//    double angle = data.angle*M_PI/180.;
//    RS_Vector ip = RS_Vector(data.basePoint.x, data.basePoint.y);

//Correct bad alignment of older dxflib or libdxfrw < 0.5.4
    if (oldMText) {
        interlin = data.interlin*0.96;
        /*if (valign == RS_MTextData::VABottom) {
            QStringList tl = mtext.split('\n', QString::SkipEmptyParts);
            if (!tl.isEmpty()) {
                QString txt = tl.at(tl.size()-1);
#ifdef  RS_VECTOR2D
                RS_TextData d(RS_Vector(0.,0.), RS_Vector(0.,0.),
#else
                RS_TextData d(RS_Vector(0.,0.,0.), RS_Vector(0.,0.,0.),
#endif

                              data.height, 1, RS_TextData::VABaseline, RS_TextData::HALeft,
                              RS_TextData::None, txt, sty, 0,
                              RS2::Update);
                RS_Text* entity = new RS_Text(NULL, d);
                double textTail = entity->getMin().y;
                delete entity;
                RS_Vector ot = RS_Vector(0.0,textTail).rotate(angle);
                ip.move(ot);
            }
        }*/
    }
    DBG(interlin);

/*    RS_MTextData d(ip, data.height, data.widthscale,
                  valign, halign,
                  dir, lss,
                  interlin,
                  mtext, sty, angle,
                  RS2::NoUpdate);
    RS_MText* entity = new RS_MText(currentContainer, d);

    setEntityAttributes(entity, &data);
    entity->update();
    currentContainer->addEntity(entity);*/
}



/**
 * Implementation of the method which handles
 * texts (TEXT).
 */
void FilterDXFRW::addText(const DRW_Text& data) {
    Q_UNUSED(data);
    DBG("FilterDXFRW::addText\n");
/*    RS_Vector refPoint = RS_Vector(data.basePoint.x, data.basePoint.y);;
    RS_Vector secPoint = RS_Vector(data.secPoint.x, data.secPoint.y);;
    double angle = data.angle;

    if (data.alignV !=0 || data.alignH !=0 ||data.alignH ==DRW_Text::HMiddle){
        if (data.alignH !=DRW_Text::HAligned && data.alignH !=DRW_Text::HFit){
            secPoint = RS_Vector(data.basePoint.x, data.basePoint.y);
            refPoint = RS_Vector(data.secPoint.x, data.secPoint.y);
        }
    }

    RS_TextData::VAlign valign = (RS_TextData::VAlign)data.alignV;
    RS_TextData::HAlign halign = (RS_TextData::HAlign)data.alignH;
    RS_TextData::TextGeneration dir;
    QString sty = QString::fromUtf8(data.style.c_str());
    sty=sty.toLower();

    if (data.textgen==2) {
        dir = RS_TextData::Backward;
    } else if (data.textgen==4) {
        dir = RS_TextData::UpsideDown;
    } else {
        dir = RS_TextData::None;
    }

    QString mtext = toNativeString(QString::fromUtf8(data.text.c_str()));
    // use default style for the drawing:
    if (sty.isEmpty()) {
        // japanese, cyrillic:
        if (codePage=="ANSI_932" || codePage=="ANSI_1251") {
            sty = "Unicode";
        } else {
            sty = textStyle;
        }
    } else {
        sty = fontList.value(sty, sty);
    }
*/
//    DBG("Text as unicode:");
//    RS_DEBUG->printUnicode(mtext);

/*    RS_TextData d(refPoint, secPoint, data.height, data.widthscale,
                  valign, halign, dir,
                  mtext, sty, angle*M_PI/180,
                  RS2::NoUpdate);
    RS_Text* entity = new RS_Text(currentContainer, d);

    setEntityAttributes(entity, &data);
    entity->update();
    currentContainer->addEntity(entity);*/
}


/**
 * Implementation of the method which handles
 * aligned dimensions (DIMENSION).
 */
void FilterDXFRW::addDimAlign(const DRW_DimAligned *data) {
    Q_UNUSED(data);
    DBG("FilterDXFRW::addDimAligned");

/*    RS_DimensionData dimensionData = convDimensionData((DRW_Dimension*)data);

    RS_Vector ext1(data->getDef1Point().x, data->getDef1Point().y);
    RS_Vector ext2(data->getDef2Point().x, data->getDef2Point().y);

    RS_DimAlignedData d(ext1, ext2);

    RS_DimAligned* entity = new RS_DimAligned(currentContainer,
                            dimensionData, d);
    setEntityAttributes(entity, data);
    entity->update();
    currentContainer->addEntity(entity);*/
}



/**
 * Implementation of the method which handles
 * linear dimensions (DIMENSION).
 */
void FilterDXFRW::addDimLinear(const DRW_DimLinear *data) {
    Q_UNUSED(data);
    DBG("FilterDXFRW::addDimLinear");

/*    RS_DimensionData dimensionData = convDimensionData((DRW_Dimension*)data);

    RS_Vector dxt1(data->getDef1Point().x, data->getDef1Point().y);
    RS_Vector dxt2(data->getDef2Point().x, data->getDef2Point().y);

    RS_DimLinearData d(dxt1, dxt2, RS_Math::deg2rad(data->getAngle()),
                       RS_Math::deg2rad(data->getOblique()));

    RS_DimLinear* entity = new RS_DimLinear(currentContainer,
                                            dimensionData, d);
    setEntityAttributes(entity, data);
    entity->update();
    currentContainer->addEntity(entity);*/
}



/**
 * Implementation of the method which handles
 * radial dimensions (DIMENSION).
 */
void FilterDXFRW::addDimRadial(const DRW_DimRadial* data) {
    Q_UNUSED(data);
    DBG("FilterDXFRW::addDimRadial");

/*    RS_DimensionData dimensionData = convDimensionData((DRW_Dimension*)data);
    RS_Vector dp(data->getDiameterPoint().x, data->getDiameterPoint().y);

    RS_DimRadialData d(dp, data->getLeaderLength());

    RS_DimRadial* entity = new RS_DimRadial(currentContainer,
                                            dimensionData, d);

    setEntityAttributes(entity, data);
    entity->update();
    currentContainer->addEntity(entity);*/
}



/**
 * Implementation of the method which handles
 * diametric dimensions (DIMENSION).
 */
void FilterDXFRW::addDimDiametric(const DRW_DimDiametric* data) {
    Q_UNUSED(data);
    DBG("FilterDXFRW::addDimDiametric");

/*    RS_DimensionData dimensionData = convDimensionData((DRW_Dimension*)data);
    RS_Vector dp(data->getDiameter1Point().x, data->getDiameter1Point().y);

    RS_DimDiametricData d(dp, data->getLeaderLength());

    RS_DimDiametric* entity = new RS_DimDiametric(currentContainer,
                              dimensionData, d);

    setEntityAttributes(entity, data);
    entity->update();
    currentContainer->addEntity(entity);*/
}



/**
 * Implementation of the method which handles
 * angular dimensions (DIMENSION).
 */
void FilterDXFRW::addDimAngular(const DRW_DimAngular* data) {
    Q_UNUSED(data);
    DBG("FilterDXFRW::addDimAngular");

/*    RS_DimensionData dimensionData = convDimensionData(data);
    RS_Vector dp1(data->getFirstLine1().x, data->getFirstLine1().y);
    RS_Vector dp2(data->getFirstLine2().x, data->getFirstLine2().y);
    RS_Vector dp3(data->getSecondLine1().x, data->getSecondLine1().y);
    RS_Vector dp4(data->getDimPoint().x, data->getDimPoint().y);

    RS_DimAngularData d(dp1, dp2, dp3, dp4);

    RS_DimAngular* entity = new RS_DimAngular(currentContainer,
                            dimensionData, d);

    setEntityAttributes(entity, data);
    entity->update();
    currentContainer->addEntity(entity);*/
}



/**
 * Implementation of the method which handles
 * angular dimensions (DIMENSION).
 */
void FilterDXFRW::addDimAngular3P(const DRW_DimAngular3p* data) {
    Q_UNUSED(data);
    DBG("FilterDXFRW::addDimAngular3P");

/*    RS_DimensionData dimensionData = convDimensionData(data);
    RS_Vector dp1(data->getFirstLine().x, data->getFirstLine().y);
    RS_Vector dp2(data->getSecondLine().x, data->getSecondLine().y);
    RS_Vector dp3(data->getVertexPoint().x, data->getVertexPoint().y);
    RS_Vector dp4 = dimensionData.definitionPoint;
    dimensionData.definitionPoint = RS_Vector(data->getVertexPoint().x, data->getVertexPoint().y);

    RS_DimAngularData d(dp1, dp2, dp3, dp4);

    RS_DimAngular* entity = new RS_DimAngular(currentContainer,
                            dimensionData, d);

    setEntityAttributes(entity, data);
    entity->update();
    currentContainer->addEntity(entity);*/
}



void FilterDXFRW::addDimOrdinate(const DRW_DimOrdinate* /*data*/) {
    DBG("FilterDXFRW::addDimOrdinate(const DL_DimensionData&, const DL_DimOrdinateData&) not yet implemented");
}


/**
 * Implementation of the method which handles leader entities.
 */
void FilterDXFRW::addLeader(const DRW_Leader *data) {
    Q_UNUSED(data);
    DBG("FilterDXFRW::addDimLeader");
/*    RS_LeaderData d(data->arrow!=0);
    RS_Leader* leader = new RS_Leader(currentContainer, d);
    setEntityAttributes(leader, data);

    currentContainer->addEntity(leader);

    for (unsigned int i=0; i<data->vertexlist.size(); i++) {
        DRW_Coord *vert = data->vertexlist.at(i);
        RS_Vector v(vert->x, vert->y);
        leader->addVertex(v);
    }*/
}



/**
 * Implementation of the method which handles hatch entities.
 */
void FilterDXFRW::addHatch(const DRW_Hatch *data) {
    Q_UNUSED(data);
    DBG("RS_FilterDXF::addHatch()");
/*    RS_Hatch* hatch;
    RS_EntityContainer* hatchLoop;

    hatch = new RS_Hatch(currentContainer,
                         RS_HatchData(data->solid, data->scale, data->angle,
                                      QString::fromUtf8(data->name.c_str())));
    setEntityAttributes(hatch, data);
    currentContainer->appendEntity(hatch);

    for (unsigned int i=0; i < data->looplist.size(); i++) {
        DRW_HatchLoop *loop = data->looplist.at(i);
        if ((loop->type & 32) == 32) continue;
        hatchLoop = new RS_EntityContainer(hatch);
        hatchLoop->setLayer(NULL);
        hatch->addEntity(hatchLoop);

        RS_Entity* e = NULL;
        if ((loop->type & 2) == 2){   //polyline, convert to lines & arcs
            DRW_LWPolyline *pline = (DRW_LWPolyline *)loop->objlist.at(0);
            RS_Polyline *polyline = new RS_Polyline(NULL,
                    RS_PolylineData(RS_Vector(false), RS_Vector(false), pline->flags) );
            for (unsigned int j=0; j < pline->vertlist.size(); j++) {
                    DRW_Vertex2D *vert = pline->vertlist.at(j);
                    polyline->addVertex(RS_Vector(vert->x, vert->y), vert->bulge);
            }
            for (RS_Entity* e=polyline->firstEntity(); e!=NULL;
                    e=polyline->nextEntity()) {
                RS_Entity* tmp = e->clone();
                tmp->reparent(hatchLoop);
                tmp->setLayer(NULL);
                hatchLoop->addEntity(tmp);
            }
            delete polyline;

        } else {
            for (unsigned int j=0; j<loop->objlist.size(); j++) {
                e = NULL;
                DRW_Entity *ent = loop->objlist.at(j);
                switch (ent->eType) {
                case DRW::LINE: {
                    DRW_Line *e2 = (DRW_Line *)ent;
                    e = new RS_Line(hatchLoop,
                                    RS_LineData(RS_Vector(e2->basePoint.x, e2->basePoint.y),
                                                RS_Vector(e2->secPoint.x, e2->secPoint.y)));
                    break;
                }
                case DRW::ARC: {
                    DRW_Arc *e2 = (DRW_Arc *)ent;
                    if (e2->isccw && e2->staangle<1.0e-6 && e2->endangle>360-1.0e-6) {
                        e = new RS_Circle(hatchLoop,
                                          RS_CircleData(RS_Vector(e2->basePoint.x, e2->basePoint.y),
                                                        e2->radious));
                    } else {

                        if (e2->isccw) {
                            e = new RS_Arc(hatchLoop,
                                        RS_ArcData(RS_Vector(e2->basePoint.x, e2->basePoint.y), e2->radious,
                                                   RS_Math::correctAngle(RS_Math::deg2rad(e2->staangle)),
                                                   RS_Math::correctAngle(RS_Math::deg2rad(e2->endangle)),
                                                   false));
                        } else {
                            e = new RS_Arc(hatchLoop,
                                        RS_ArcData(RS_Vector(e2->basePoint.x, e2->basePoint.y), e2->radious,
                                                   RS_Math::correctAngle(2*M_PI-RS_Math::deg2rad(e2->staangle)),
                                                   RS_Math::correctAngle(2*M_PI-RS_Math::deg2rad(e2->endangle)),
                                                   true));
                        }
                    }
                    break;
                }
                case DRW::ELLIPSE: {
                    DRW_Ellipse *e2 = (DRW_Ellipse *)ent;
                    double ang1 = RS_Math::deg2rad(e2->staparam);
                    double ang2 = RS_Math::deg2rad(e2->endparam);
                    if ( fabs(ang2 - 6.28318530718) < 1.0e-10 && fabs(ang1) < 1.0e-10 )
                        ang2 = 0.0;
                    e = new RS_Ellipse(hatchLoop,
                                       RS_EllipseData(RS_Vector(e2->basePoint.x, e2->basePoint.y),
                                                      RS_Vector(e2->secPoint.x, e2->secPoint.y),
                                                      e2->ratio, ang1, ang2, e2->isccw));
                    break;
                }
                default:
                    break;
                }
                if (e!=NULL) {
                    e->setLayer(NULL);
                    hatchLoop->addEntity(e);
                }
            }
        }

    }

    DBG("hatch->update()");
    if (hatch->validate()) {
        hatch->update();
    } else {
        graphic->removeEntity(hatch);
        DBG("FilterDXFRW::endEntity(): updating hatch failed: invalid hatch area");
    }*/
}


/**
 * Implementation of the method which handles image entities.
 */
void FilterDXFRW::addImage(const DRW_Image *data) {
    Q_UNUSED(data);
    DBG("RS_FilterDXF::addImage");

/*    RS_Vector ip(data->basePoint.x, data->basePoint.y);
    RS_Vector uv(data->secPoint.x, data->secPoint.y);
    RS_Vector vv(data->vx, data->vy);
    RS_Vector size(data->sizeu, data->sizev);

    RS_Image* image = new RS_Image( currentContainer,
            RS_ImageData(QString(data->ref.c_str()).toInt(NULL, 16),
                         ip, uv, vv, size,
                         QString(""), data->brightness,
                         data->contrast, data->fade));

    setEntityAttributes(image, data);
    currentContainer->appendEntity(image);*/
}



/**
 * Implementation of the method which links image entities to image files.
 */
void FilterDXFRW::linkImage(const DRW_ImageDef *data) {
    DBG("FilterDXFRW::linkImage");

//    int handle = QString(data->handle.c_str()).toInt(NULL, 16);
    QString sfile(QString::fromUtf8(data->name.c_str()));
    QFileInfo fiDxf(file);
    QFileInfo fiBitmap(sfile);

    // try to find the image file:

    // first: absolute path:
    if (!fiBitmap.exists()) {
        DBG(QString("File %1 doesn't exist.").arg(
                        (const char*)QFile::encodeName(sfile)) );
        // try relative path:
        QString f1 = fiDxf.absolutePath() + "/" + sfile;
        if (QFileInfo(f1).exists()) {
            sfile = f1;
        } else {
            DBG(QString("File %1 doesn't exist.").arg((const char*)QFile::encodeName(f1)) );
            // try drawing path:
            QString f2 = fiDxf.absolutePath() + "/" + fiBitmap.fileName();
            if (QFileInfo(f2).exists()) {
                sfile = f2;
            } else {
                DBG(QString("File %1 doesn't exist.").arg((const char*)QFile::encodeName(f2)) );
            }
        }
    }

    // Also link images in subcontainers (e.g. inserts):
/*    for (RS_Entity* e=graphic->firstEntity(RS2::ResolveNone);
            e!=NULL; e=graphic->nextEntity(RS2::ResolveNone)) {
        if (e->rtti()==RS2::EntityImage) {
            RS_Image* img = (RS_Image*)e;
            if (img->getHandle()==handle) {
                img->setFile(sfile);
                DBG(QString("image found: %1".arg((const char*)QFile::encodeName(img->getFile()))) );
                img->update();
            }
        }
    }*/

    // update images in blocks:
/*    for (uint i=0; i<graphic->countBlocks(); ++i) {
        RS_Block* b = graphic->blockAt(i);
        for (RS_Entity* e=b->firstEntity(RS2::ResolveNone);
                e!=NULL; e=b->nextEntity(RS2::ResolveNone)) {
            if (e->rtti()==RS2::EntityImage) {
                RS_Image* img = (RS_Image*)e;
                if (img->getHandle()==handle) {
                    img->setFile(sfile);
                    DBG(QString("image in block found: %1").arg(
                                    (const char*)QFile::encodeName(img->getFile())));
                    img->update();
                }
            }
        }
    }*/
    DBG("linking image: OK");
}

using std::map;
/**
 * Sets the header variables from the DXF file.
 */
void FilterDXFRW::addHeader(const DRW_Header* data){
/*    RS_Graphic* container = NULL;
    if (currentContainer->rtti()==RS2::EntityGraphic) {
        container = (RS_Graphic*)currentContainer;
    } else return;
*/
    map<std::string,DRW_Variant *>::const_iterator it;
    for ( it=data->vars.begin() ; it != data->vars.end(); it++ ){
        QString key = QString::fromStdString((*it).first);
//        DRW_Variant *var = (*it).second;
/*        switch (var->type) {
        case DRW_Variant::COORD:
            container->addVariable(key,
#ifdef  RS_VECTOR2D
            RS_Vector(var->content.v->x, var->content.v->y), var->code);
#else
            RS_Vector(var->content.v->x, var->content.v->y, var->content.v->z), var->code);
#endif
            break;
        case DRW_Variant::STRING:
            container->addVariable(key, QString::fromUtf8(var->content.s->c_str()), var->code);
            break;
        case DRW_Variant::INTEGER:
            container->addVariable(key, var->content.i, var->code);
            break;
        case DRW_Variant::DOUBLE:
            container->addVariable(key, var->content.d, var->code);
            break;
        default:
            break;
        }*/

    }
/*    codePage = graphic->getVariableString("$DWGCODEPAGE", "ANSI_1252");
    textStyle = graphic->getVariableString("$TEXTSTYLE", "Standard");
    dimStyle = graphic->getVariableString("$DIMSTYLE", "Standard");

    QString acadver = versionStr = graphic->getVariableString("$ACADVER", "");
    acadver.replace(QRegExp("[a-zA-Z]"), "");*/
//    bool ok;
//    version=acadver.toInt(&ok);
//    if (!ok) { version = 1021;}

    //detect if dxf lib are a old dxflib or libdxfrw<0.5.4 (used to correct mtext alignment)
    oldMText = false;
    QStringList comm = QString::fromStdString(data->getComments()).split('\n',QString::SkipEmptyParts);
    for (int i = 0; i < comm.size(); ++i) {
        QStringList comstr = comm.at(i).split(' ',QString::SkipEmptyParts);
        if (!comstr.isEmpty() && comstr.at(0) == "dxflib") {
            oldMText = true;
            break;
        } else if (comstr.size()>1 && comstr.at(0) == "dxfrw"){
            QStringList libversionstr = comstr.at(1).split('.',QString::SkipEmptyParts);
            if (libversionstr.size()<3) break;
            int libRelease = (libversionstr.at(1)+ libversionstr.at(2)).toInt();
            if (libversionstr.at(0)=="0" && libRelease < 54){
                oldMText = true;
                break;
            }
        }
    }
}

void FilterDXFRW::add3dFace(const DRW_3Dface& data) {
    Q_UNUSED(data);
    DBG("FilterDXFRW::add3dFace");
/*    RS_PolylineData d(RS_Vector(false),
                      RS_Vector(false),
                      !data.invisibleflag);
    RS_Polyline *polyline = new RS_Polyline(currentContainer, d);
    setEntityAttributes(polyline, &data);
    RS_Vector v1(data.basePoint.x, data.basePoint.y);
    RS_Vector v2(data.secPoint.x, data.secPoint.y);
    RS_Vector v3(data.thirdPoint.x, data.thirdPoint.y);
    RS_Vector v4(data.fourPoint.x, data.fourPoint.y);

    polyline->addVertex(v1, 0.0);
    polyline->addVertex(v2, 0.0);
    polyline->addVertex(v3, 0.0);
    polyline->addVertex(v4, 0.0);

    currentContainer->addEntity(polyline);*/
}

void FilterDXFRW::addComment(const char*) {
    DBG("RS_FilterDXF::addComment(const char*) not yet implemented.");
}

void FilterDXFRW::setEntityAttributes( const DRW_Entity* attrib) {
    DBG("RS_FilterDXF::EntityAttributes\n");
    DBG("Layer: "); DBG( toNativeString(QString::fromUtf8(attrib->layer.c_str())) );
    DBG(" Color: "); DBG(attrib->color);
    DBG(" Linetype: "); DBG( QString::fromUtf8(attrib->lineType.c_str()) );
    DBG(" Width: "); DBG(attrib->lWeight);
    DBG("\n");

/*    RS_Pen pen;
    pen.setColor(Qt::black);
    pen.setLineType(RS2::SolidLine);
    QString layName = toNativeString(QString::fromUtf8(attrib->layer.c_str()));

    // Layer: add layer in case it doesn't exist:
    if (graphic->findLayer(layName)==NULL) {
        DRW_Layer lay;
        lay.name = attrib->layer;
        addLayer(lay);
    }
    entity->setLayer(layName);

    // Color:
    pen.setColor(numberToColor(attrib->color));

    // Linetype:
    pen.setLineType(nameToLineType( QString::fromUtf8(attrib->lineType.c_str()) ));

    // Width:
    pen.setWidth(numberToWidth(attrib->lWeight));

    entity->setPen(pen);
    RS_DEBUG->print("RS_FilterDXF::setEntityAttributes: OK");*/
}

/**
 * Converts a native unicode string into a DXF encoded string.
 *
 * DXF endoding includes the following special sequences:
 * - %%%c for a diameter sign
 * - %%%d for a degree sign
 * - %%%p for a plus/minus sign
 */
QString FilterDXFRW::toDxfString(const QString& str) {
    QString res = "";
    int j=0;
    for (int i=0; i<str.length(); ++i) {
        int c = str.at(i).unicode();
        if (c>175 || c<11){
            res.append(str.mid(j,i-j));
            j=i;

            switch (c) {
            case 0x0A:
                res+="\\P";
                break;
                // diameter:
            case 0x2205://RLZ: Empty_set, diameter is 0x2300 need to add in all fonts
                res+="%%C";
                break;
                // degree:
            case 0x00B0:
                res+="%%D";
                break;
                // plus/minus
            case 0x00B1:
                res+="%%P";
                break;
            default:
                j--;
                break;
            }
            j++;
        }

    }
    res.append(str.mid(j));
    return res;
}



/**
 * Converts a DXF encoded string into a native Unicode string.
 */
QString FilterDXFRW::toNativeString(const QString& data) {
    QString res;

    // Ignore font tags:
    int j = 0;
    for (int i=0; i<data.length(); ++i) {
        if (data.at(i).unicode() == 0x7B){ //is '{' ?
            if (data.at(i+1).unicode() == 0x5c && data.at(i+2).unicode() == 0x66){ //is "\f" ?
                //found font tag, append parsed part
                res.append(data.mid(j,i-j));
                //skip to ';'
                for (int k=i+3; k<data.length(); ++k) {
                    if (data.at(k).unicode() == 0x3B) {
                        i = j = ++k;
                        break;
                    }
                }
                //add to '}'
                for (int k=i; k<data.length(); ++k) {
                    if (data.at(k).unicode() == 0x7D) {
                        res.append(data.mid(i,k-i));
                        i = j = ++k;
                        break;
                    }
                }

            }
        }
    }
    res.append(data.mid(j));

    // Line feed:
    res = res.replace(QRegExp("\\\\P"), "\n");
    // Space:
    res = res.replace(QRegExp("\\\\~"), " ");
    // diameter:
    res = res.replace(QRegExp("%%[cC]"), QChar(0x2205));//RLZ: Empty_set, diameter is 0x2300 need to add in all fonts
    // degree:
    res = res.replace(QRegExp("%%[dD]"), QChar(0x00B0));
    // plus/minus
    res = res.replace(QRegExp("%%[pP]"), QChar(0x00B1));

    return res;
}


/**
 * Checks if the given variable is two-dimensional (e.g. $LIMMIN).
 */
bool FilterDXFRW::isVariableTwoDimensional(const QString& var) {
    if (var=="$LIMMIN" ||
            var=="$LIMMAX" ||
            var=="$PLIMMIN" ||
            var=="$PLIMMAX" ||
            var=="$GRIDUNIT" ||
            var=="$VIEWCTR") {

        return true;
    } else {
        return false;
    }
}


// EOF


