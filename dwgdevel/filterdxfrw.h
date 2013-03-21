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


#ifndef RS_FILTERDXFRW_H
#define RS_FILTERDXFRW_H

/*#include "rs_filterinterface.h"

#include "rs_block.h"
#include "rs_color.h"
#include "rs_dimension.h"
#include "rs_insert.h"
#include "rs_layer.h"
#include "rs_leader.h"
#include "rs_polyline.h"
#include "rs_solid.h"
#include "rs_mtext.h"
#include "rs_text.h"
#include "rs_image.h"*/

#include "drw_interface.h"
//#include "libdxfrw.h"
#include "libdwgr.h"

/*class RS_Spline;
class RS_Hatch;
class DL_WriterA;*/
#include <QString>
#include <QHash>

/**
 * This format filter class can import and export DXF files.
 * It depends on the dxflib library.
 *
 * @author Rallaz
 */
class FilterDXFRW : public DRW_Interface {
public:
    FilterDXFRW();
    ~FilterDXFRW();
	
    // Import:
    virtual bool fileImport(const QString& file);

    // Methods from DRW_CreationInterface:
    virtual void addHeader(const DRW_Header* data);
    virtual void addLType(const DRW_LType& /*data*/){}
    virtual void addLayer(const DRW_Layer& data);
    virtual void addDimStyle(const DRW_Dimstyle& /*data*/){}
    virtual void addVport(const DRW_Vport& data);
    virtual void addTextStyle(const DRW_Textstyle& /*data*/){}
    virtual void addBlock(const DRW_Block& data);
    virtual void setBlock(const int handle);
    virtual void endBlock();
    virtual void addPoint(const DRW_Point& data);
    virtual void addLine(const DRW_Line& data);
    virtual void addRay(const DRW_Ray& data);
    virtual void addXline(const DRW_Xline& data);
    virtual void addCircle(const DRW_Circle& data);
    virtual void addArc(const DRW_Arc& data);
    virtual void addEllipse(const DRW_Ellipse& data);
    virtual void addLWPolyline(const DRW_LWPolyline& data);
    virtual void addText(const DRW_Text& data);
    virtual void addPolyline(const DRW_Polyline& data);
    virtual void addSpline(const DRW_Spline* data);
    virtual void addKnot(const DRW_Entity&) {}
    virtual void addInsert(const DRW_Insert& data);
    virtual void addTrace(const DRW_Trace& data);
    virtual void addSolid(const DRW_Solid& data);
    virtual void addMText(const DRW_MText& data);
//    RS_DimensionData convDimensionData(const DRW_Dimension* data);
    virtual void addDimAlign(const DRW_DimAligned *data);
    virtual void addDimLinear(const DRW_DimLinear *data);
    virtual void addDimRadial(const DRW_DimRadial *data);
    virtual void addDimDiametric(const DRW_DimDiametric *data);
    virtual void addDimAngular(const DRW_DimAngular *data);
    virtual void addDimAngular3P(const DRW_DimAngular3p *data);
    virtual void addDimOrdinate(const DRW_DimOrdinate *data);
    virtual void addLeader(const DRW_Leader *data);
    virtual void addHatch(const DRW_Hatch* data);
    virtual void addViewport(const DRW_Viewport& /*data*/){}
    virtual void addImage(const DRW_Image* data);
    virtual void linkImage(const DRW_ImageDef* data);

    virtual void add3dFace(const DRW_3Dface& data);
    virtual void addComment(const char*);

    virtual void writeHeader(DRW_Header& data){Q_UNUSED(data);}
    virtual void writeEntities(){}
    virtual void writeLTypes(){}
    virtual void writeLayers(){}
    virtual void writeTextstyles(){}
    virtual void writeVports(){}
    virtual void writeBlockRecords(){}
    virtual void writeBlocks(){}
    virtual void writeDimstyles(){}


    void setEntityAttributes( const DRW_Entity* attrib);

        static QString toDxfString(const QString& str);
    static QString toNativeString(const QString& data);

public:

        static bool isVariableTwoDimensional(const QString& var);


private:
    /** File name. Used to find out the full path of images. */
    QString file;
    /** File codePage. Used to find the text coder. */
    QString codePage;
    /** File version. */
    QString versionStr;
    int version;
    /** dimension style. */
    QString dimStyle;
    /** text style. */
    QString textStyle;
    QHash <QString, QString> fontList;
    bool oldMText;
    dwgR *dwg;
};

#endif
