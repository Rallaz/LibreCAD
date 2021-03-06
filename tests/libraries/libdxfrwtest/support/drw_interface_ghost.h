/******************************************************************************
**  libDXFrw - Library to read/write DXF files (ascii & binary)              **
**                                                                           **
**  Copyright (C) 2013 TNick, nicu.tofan@gmail.com                           **
**                                                                           **
**  This library is free software, licensed under the terms of the GNU       **
**  General Public License as published by the Free Software Foundation,     **
**  either version 2 of the License, or (at your option) any later version.  **
**  You should have received a copy of the GNU General Public License        **
**  along with this program.  If not, see <http://www.gnu.org/licenses/>.    **
******************************************************************************/

#ifndef DRW_INTERFACE_GHOST_H
#define DRW_INTERFACE_GHOST_H

#include <drw_interface.h>

#	define APPLY_EXTRUSION	true


/**
 * Implementation for DRW_Interface that simply ignores all input
 *
 * Derive your class from this one and implement relevant methods.
 * 
 * @author TNick
 */
class DRW_InterfaceGhost : public DRW_Interface {
public:
    DRW_InterfaceGhost() : DRW_Interface() {  }
    virtual ~DRW_InterfaceGhost() {  }

    /** Called when header is parsed.  */
    virtual void addHeader(const DRW_Header* data) { Q_UNUSED(data); }

    /** Called for every line Type.  */
    virtual void addLType(const DRW_LType& data) { Q_UNUSED(data); }
    /** Called for every layer. */
    virtual void addLayer(const DRW_Layer& data) { Q_UNUSED(data); }
    /** Called for every dim style. */
    virtual void addDimStyle(const DRW_Dimstyle& data) { Q_UNUSED(data); }
    /** Called for every VPORT table. */
    virtual void addVport(const DRW_Vport& data) { Q_UNUSED(data); }
    /** Called for every text style. */
    virtual void addTextStyle(const DRW_Textstyle& data) { Q_UNUSED(data); }

    /**
     * Called for every block. Note: all entities added after this
     * command go into this block until endBlock() is called.
     *
     * @see endBlock()
     */
    virtual void addBlock(const DRW_Block& data) { Q_UNUSED(data); }

    /**
     * In DWG called when the following entities corresponding to a
     * block different from the current. Note: all entities added after this
     * command go into this block until setBlock() is called already.
     *
     * int handle are the value of DRW_Block::handleBlock added with addBlock()
     */
    virtual void setBlock(const int handle) { Q_UNUSED(handle); }

    /** Called to end the current block */
    virtual void endBlock() {  }

    /** Called for every point */
    virtual void addPoint(const DRW_Point& data) { Q_UNUSED(data); }

    /** Called for every line */
    virtual void addLine(const DRW_Line& data) { Q_UNUSED(data); }

    /** Called for every ray */
    virtual void addRay(const DRW_Ray& data) { Q_UNUSED(data); }

    /** Called for every xline */
    virtual void addXline(const DRW_Xline& data) { Q_UNUSED(data); }

    /** Called for every arc */
    virtual void addArc(const DRW_Arc& data) { Q_UNUSED(data); }

    /** Called for every circle */
    virtual void addCircle(const DRW_Circle& data) { Q_UNUSED(data); }

    /** Called for every ellipse */
    virtual void addEllipse(const DRW_Ellipse& data) { Q_UNUSED(data); }

    /** Called for every lwpolyline */
    virtual void addLWPolyline(const DRW_LWPolyline& data) { Q_UNUSED(data); }

    /** Called for every polyline start */
    virtual void addPolyline(const DRW_Polyline& data) { Q_UNUSED(data); }

    /** Called for every spline */
    virtual void addSpline(const DRW_Spline* data) { Q_UNUSED(data); }
	
	/** Called for every spline knot value */
    virtual void addKnot(const DRW_Entity& data) { Q_UNUSED(data); }

    /** Called for every insert. */
    virtual void addInsert(const DRW_Insert& data) { Q_UNUSED(data); }
    
    /** Called for every trace start */
    virtual void addTrace(const DRW_Trace& data) { Q_UNUSED(data); }
    
    /** Called for every 3dface start */
    virtual void add3dFace(const DRW_3Dface& data) { Q_UNUSED(data); }

    /** Called for every solid start */
    virtual void addSolid(const DRW_Solid& data) { Q_UNUSED(data); }


    /** Called for every Multi Text entity. */
    virtual void addMText(const DRW_MText& data) { Q_UNUSED(data); }

    /** Called for every Text entity. */
    virtual void addText(const DRW_Text& data) { Q_UNUSED(data); }

    /**
     * Called for every aligned dimension entity. 
     */
    virtual void addDimAlign(const DRW_DimAligned *data) { Q_UNUSED(data); }
    /**
     * Called for every linear or rotated dimension entity. 
     */
    virtual void addDimLinear(const DRW_DimLinear *data) { Q_UNUSED(data); }

	/**
     * Called for every radial dimension entity. 
     */
    virtual void addDimRadial(const DRW_DimRadial *data) { Q_UNUSED(data); }

	/**
     * Called for every diametric dimension entity. 
     */
    virtual void addDimDiametric(const DRW_DimDiametric *data) { Q_UNUSED(data); }

	/**
     * Called for every angular dimension (2 lines version) entity. 
     */
    virtual void addDimAngular(const DRW_DimAngular *data) { Q_UNUSED(data); }

	/**
     * Called for every angular dimension (3 points version) entity. 
     */
    virtual void addDimAngular3P(const DRW_DimAngular3p *data) { Q_UNUSED(data); }
	
    /**
     * Called for every ordinate dimension entity. 
     */
    virtual void addDimOrdinate(const DRW_DimOrdinate *data) { Q_UNUSED(data); }
    
    /** 
	 * Called for every leader start. 
	 */
    virtual void addLeader(const DRW_Leader *data) { Q_UNUSED(data); }
	
	/** 
	 * Called for every hatch entity. 
	 */
    virtual void addHatch(const DRW_Hatch *data) { Q_UNUSED(data); }
	
    /**
     * Called for every viewport entity.
     */
    virtual void addViewport(const DRW_Viewport& data) { Q_UNUSED(data); }

    /**
	 * Called for every image entity. 
	 */
    virtual void addImage(const DRW_Image *data) { Q_UNUSED(data); }

	/**
	 * Called for every image definition.
	 */
    virtual void linkImage(const DRW_ImageDef *data) { Q_UNUSED(data); }

    /**
     * Called for every comment in the DXF file (code 999).
     */
    virtual void addComment(const char* comment) { Q_UNUSED(comment); }


    virtual void writeHeader(DRW_Header& data) { Q_UNUSED(data); }
    virtual void writeBlocks() {  }
    virtual void writeBlockRecords() {  }
    virtual void writeEntities() {  }
    virtual void writeLTypes() {  }
    virtual void writeLayers() {  }
    virtual void writeTextstyles() {  }
    virtual void writeVports() {  }
    virtual void writeDimstyles() {  }

};

#endif // DRW_INTERFACE_GHOST_H
