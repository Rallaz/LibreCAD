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

#ifndef DRW_BASE_H
#define DRW_BASE_H

#define DRW_VERSION "0.5.7"

#include <string>
#include <list>
#include <cmath>

using std::string;

#ifdef DRW_ASSERTS
# define drw_assert(a) assert(a)
#else
# define drw_assert(a)
#endif

#define drw_unused(x) (void)x;


#define UTF8STRING std::string
#define DRW_UNUSED(x) (void)x

#if defined(WIN64) || defined(_WIN64) || defined(__WIN64__)
#  define DRW_WIN
#elif defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#    define DRW_WIN
#elif defined(__MWERKS__) && defined(__INTEL__)
#  define DRW_WIN
#else
#  define DRW_POSIX
#endif

#ifndef M_PI
 #define M_PI       3.141592653589793238462643
#endif
#ifndef M_PI_2
 #define M_PI_2       1.57079632679489661923
#endif
#define M_PIx2      6.283185307179586 // 2*PI
#define ARAD 57.29577951308232

typedef signed char dint8;              /* 8 bit signed */
typedef signed short dint16;            /* 16 bit signed */
typedef signed int dint32;              /* 32 bit signed */
typedef long long int dint64;           /* 64 bit signed */

typedef unsigned char duint8;           /* 8 bit unsigned */
typedef unsigned short duint16;         /* 16 bit unsigned */
typedef unsigned int duint32;           /* 32 bit unsigned */
typedef unsigned long long int duint64; /* 64 bit unsigned */

typedef float dfloat32;                 /* 32 bit floating point */
typedef double ddouble64;               /* 64 bit floating point */
typedef long double ddouble80;          /* 80 bit floating point */

namespace DRW {
class Amorph;

//! Version numbers for the DXF Format.
enum Version {
    UNKNOWNV,     /*!< UNKNOWN VERSION. */
    AC1006,       /*!< R10. */
    AC1009,       /*!< R11 & R12. */
    AC1012,       /*!< R13. */
    AC1014,       /*!< R14. */
    AC1015,       /*!< ACAD 2000. */
    AC1018,       /*!< ACAD 2004. */
    AC1021,       /*!< ACAD 2007. */
    AC1024        /*!< ACAD 2010. */
};

enum error {
BAD_NONE,             /*!< No error. */
BAD_UNKNOWN,          /*!< UNKNOWN. */
BAD_OPEN,             /*!< error opening file. */
BAD_VERSION,          /*!< unsupported version. */
BAD_READ_FILE_HEADER, /*!< error in file header read process. */
BAD_READ_HEADER,      /*!< error in header vars read process. */
BAD_READ_OFFSETS,     /*!< error in object map read process. */
BAD_READ_CLASSES,     /*!< error in classes read process. */
BAD_READ_TABLES,      /*!< error in tables read process. */
BAD_READ_ENTITIES     /*!< error in entities read process. */
};

//! Special codes for colors
enum ColorCodes        {
    ColorByLayer = 256,
    ColorByBlock = 0
};

//! Spaces
enum Space        {
    ModelSpace = 0,
    PaperSpace = 1
};

//! Special kinds of handles
enum HandleCodes        {
    NoHandle = -1
};

//! A group in dxf file
struct Group {
    std::string name;
    std::list<DRW::Amorph> content;
};

//! Shadow mode
enum ShadowMode        {
    CastAndReceieveShadows = 0,
    CastShadows = 1,
    ReceiveShadows = 2,
    IgnoreShadows = 3
};

//! Special kinds of materials
enum MaterialCodes        {
    MaterialByLayer = 0
};

//! Special kinds of plot styles
enum PlotStyleCodes        {
    DefaultPlotStyle = 0
};

//! Special kinds of transparencies
enum TransparencyCodes        {
    Opaque = 0,
    Transparent = -1
};

//! Class capable of storing values of various kind
/*!
 * The class stores the actual value and may also store a string representation
 * of that value, depending on the way the value was obtained.
 *
 * Internal string has two roles: to store the value for the string type 
 * (Amorph::tyString) and to store a string representation for the other types.
 * hasStringRepr() tells us if the string representation has been stored or not
 * and stringRepr() will generate a string representation of the value in 
 * internal string.
 */
class Amorph        {
public:

    enum Type {
        tyInt8,     /**< 8 bit signed */
        tyInt16,    /**< 16 bit signed */
        tyInt32,    /**< 32 bit signed */
        tyInt64,    /**< 64 bit signed */
        
        tyUint8,    /**< 8 bit unsigned */
        tyUint16,   /**< 16 bit unsigned */
        tyUint32,   /**< 32 bit unsigned */
        tyUint64,   /**< 64 bit unsigned */
        
        tyFloat32,  /**< 32 bit floating point */
        tyDouble64, /**< 64 bit floating point */
        tyDouble80, /**< 80 bit floating point */
        
        tyBool,     /**< a bool value */
        
        tyString    /**< a string value */
    };

    //! tell if the instance has a string representation for the value
    bool hasStringRepr()
    { return s_.length() > 0; }
    
    //!@{
    //! set the internal value and update the type
    void setInt8 (dint8 val)
    { int8_ = val; ty = tyInt8; s_.clear(); }
    void setInt16 (dint16 val)
    { int16_ = val; ty = tyInt16 ; s_.clear(); }
    void setInt32 (dint32 val)
    { int32_ = val; ty = tyInt32; s_.clear(); }
    void setInt64 (dint64 val)
    { int64_ = val; ty = tyInt64; s_.clear(); }
    void setUint8 (duint8 val)
    { uint8_ = val; ty = tyUint8; s_.clear(); }
    void setUint16 (duint16 val)
    { uint16_ = val; ty = tyUint16; s_.clear(); }
    void setUint32 (duint32 val)
    { uint32_ = val; ty = tyUint32; s_.clear(); }
    void setuint64 (duint64 val)
    { uint64_ = val; ty = tyUint64; s_.clear(); }
    void setFloat32 (dfloat32 val)
    { float32_ = val; ty = tyFloat32; s_.clear(); }
    void setDouble64 (ddouble64 val)
    { double64_ = val; ty = tyDouble64; s_.clear(); }
    void setDouble80 (ddouble80 val)
    { double80_ = val; ty = tyDouble80; s_.clear(); }
    void setBool (bool val)
    { b_ = val; ty = tyBool; s_.clear(); }
    void setString (const std::string & val)
    { s_ = val; ty = tyString; }
    //!@}
    
    //!@{
    //! get current value; assert corect type
    dint8 getInt8() const
    { drw_assert(ty==tyInt8); return int8_; }
    dint16 getInt16() const
    { drw_assert(ty==tyInt16); return int16_; }
    dint32 getInt32() const
    { drw_assert(ty==tyInt32); return int32_; }
    dint64 getInt64() const
    { drw_assert(ty==tyInt64); return int64_; }
    duint8 getUint8() const
    { drw_assert(ty==tyUint8); return uint8_; }
    duint16 getUint16() const
    { drw_assert(ty==tyUint16); return uint16_; }
    duint32 getUint32() const
    { drw_assert(ty==tyUint32); return uint32_; }
    duint64 getUint64() const
    { drw_assert(ty==tyUint64); return uint64_; }
    dfloat32 getFloat32() const
    { drw_assert(ty==tyFloat32); return float32_; }
    ddouble64 getDouble64() const
    { drw_assert(ty==tyDouble64); return double64_; }
    ddouble80 getDouble80() const
    { drw_assert(ty==tyDouble80); return double80_; }
    bool getBool() const
    { drw_assert(ty==tyBool); return b_; }
    const std::string & getString() const
    { drw_assert(ty==tyString); return s_; }
    //!@}

    //! get the string representation of current value
    const std::string & stringRepr()
    {
        char buff[128];
        switch (ty)    {
        case tyInt8:
            sprintf(buff,"%d",int8_); 
        case tyInt16:
            sprintf(buff,"%d",int16_); 
        case tyInt32:
            sprintf(buff,"%d",int32_); 
        case tyInt64:
            sprintf(buff,"%lli",int64_); 
        case tyUint8:
            sprintf(buff,"%d",uint8_); 
        case tyUint16:
            sprintf(buff,"%d",uint16_); 
        case tyUint32:
            sprintf(buff,"%d",uint32_); 
        case tyUint64:
            sprintf(buff,"%llu",uint64_); 
        case tyFloat32:
            sprintf(buff,"%f",float32_); 
        case tyDouble64:
            sprintf(buff,"%f",double64_); 
        case tyDouble80:
            sprintf(buff,"%Lf",double80_); 
        case tyBool:
            sprintf(buff,"%d",b_); 
        case tyString:
            return s_;
        default:
            drw_assert(false);
            s_.clear();
            return s_;
        }
        s_ = buff;
        return s_;
    }
    
    Type ty;                        /**< the type for current value */
    union {
        dint8       int8_;          /**< 8 bit signed */
        dint16      int16_;         /**< 16 bit signed */
        dint32      int32_;         /**< 32 bit signed */
        dint64      int64_;         /**< 64 bit signed */
        
        duint8        uint8_;         /**< 8 bit unsigned */
        duint16     uint16_;        /**< 16 bit unsigned */
        duint32     uint32_;        /**< 32 bit unsigned */
        duint64     uint64_;        /**< 64 bit unsigned */
        
        dfloat32    float32_;       /**< 32 bit floating point */
        ddouble64   double64_;      /**< 64 bit floating point */
        ddouble80   double80_;      /**< 80 bit floating point */
        
        bool        b_;             /**< a bool value */
    };
    std::string     s_;             /**< a string value or representation of the value */
}; // class Amorph

} // namespace DRW

//! Class to handle 3D coordinate point
/*!
*  Class to handle 3D coordinate point
*  @author Rallaz
*/
class DRW_Coord {
public:
    DRW_Coord() { x = 0; y = 0; z = 0; }
    DRW_Coord(double ix, double iy, double iz) {
        x = ix; y = iy; z = iz;
    }

     DRW_Coord operator = (const DRW_Coord& data) {
        x = data.x;  y = data.y;  z = data.z;
        return *this;
    }
/*!< convert to unitary vector */
    void unitize(){
        double dist;
        dist = sqrt(x*x + y*y + z*z);
        if (dist > 0.0) {
            x= x/dist;
            y= y/dist;
            z= z/dist;
        }
    }

public:
    double x;
    double y;
    double z;
};


//! Class to handle vertex
/*!
*  Class to handle vertex for lwpolyline entity
*  @author Rallaz
*/
class DRW_Vertex2D {
public:
    DRW_Vertex2D() {
//        eType = DRW::LWPOLYLINE;
        stawidth = endwidth = bulge = 0;
    }
    DRW_Vertex2D(double sx, double sy, double b) {
        stawidth = endwidth = 0;
        x = sx;
        y =sy;
        bulge = b;
    }

public:
    double x;                 /*!< x coordinate, code 10 */
    double y;                 /*!< y coordinate, code 20 */
    double stawidth;          /*!< Start width, code 40 */
    double endwidth;          /*!< End width, code 41 */
    double bulge;             /*!< bulge, code 42 */
};


//! Class to handle header vars
/*!
*  Class to handle header vars
*  @author Rallaz
*/
class DRW_Variant {
public:
    enum TYPE {
        STRING,
        INTEGER,
        DOUBLE,
        COORD,
        INVALID
    };

    DRW_Variant() {
        type = INVALID;
    }
    ~DRW_Variant() {
        if (type == COORD)
            delete content.v;
    }
    enum TYPE type;

    void addString(UTF8STRING s) {data = s; content.s = &data; setType(STRING);}
    void addInt(int i) {content.i = i; setType(INTEGER);}
    void addDouble(double d) {content.d = d; setType(DOUBLE);}
    void addCoord(DRW_Coord *v) {content.v = v; setType(COORD);}
    void setType(enum TYPE t) { if (type == COORD) delete content.v; type = t;}
    void setCoordX(double d) { if (type == COORD) content.v->x = d;}
    void setCoordY(double d) { if (type == COORD) content.v->y = d;}
    void setCoordZ(double d) { if (type == COORD) content.v->z = d;}

private:
    typedef union {
        UTF8STRING *s;
        int i;
        double d;
        DRW_Coord *v;
    } DRW_VarContent;

public:
    DRW_VarContent content;

public:
    int code;
//    string version;
//    string codepage;
private:
//    DRW_VarContent content;
    string data;
};


//! Class to convert between line width and integer
/*!
*  Class to convert between line width and integer
*  verifing valid values, if value is not valid
*  returns widthDefault.
*  @author Rallaz
*/
class DRW_LW_Conv{
public:
    enum lineWidth {
        width00 = 0,       /*!< 0.00mm */
        width01 = 1,       /*!< 0.05mm */
        width02 = 2,       /*!< 0.09mm */
        width03 = 3,       /*!< 0.13mm */
        width04 = 4,       /*!< 0.15mm */
        width05 = 5,       /*!< 0.18mm */
        width06 = 6,       /*!< 0.20mm */
        width07 = 7,       /*!< 0.25mm */
        width08 = 8,       /*!< 0.30mm */
        width09 = 9,       /*!< 0.35mm */
        width10 = 10,      /*!< 0.40mm */
        width11 = 11,      /*!< 0.50mm */
        width12 = 12,      /*!< 0.53mm */
        width13 = 13,      /*!< 0.60mm */
        width14 = 14,      /*!< 0.70mm */
        width15 = 15,      /*!< 0.80mm */
        width16 = 16,      /*!< 0.90mm */
        width17 = 17,      /*!< 1.00mm */
        width18 = 18,      /*!< 1.06mm */
        width19 = 19,      /*!< 1.20mm */
        width20 = 20,      /*!< 1.40mm */
        width21 = 21,      /*!< 1.58mm */
        width22 = 22,      /*!< 2.00mm */
        width23 = 23,      /*!< 2.11mm */
        widthByLayer = 29, /*!< by layer (dxf -1) */
        widthByBlock = 30, /*!< by block (dxf -2) */
        widthDefault = 31  /*!< by default (dxf -3) */
    };

    static int lineWidth2int(enum lineWidth lw){
        return static_cast<int> (lw);
    }

    static enum lineWidth dxfInt2lineWidth(int i){
        if (i<0) {
            if (i==-1)
                return widthByLayer;
            else if (i==-2)
                return widthByBlock;
            else if (i==-3)
                return widthDefault;
        } else if (i<3) {
            return width00;
        } else if (i<7) {
            return width01;
        } else if (i<11) {
            return width02;
        } else if (i<14) {
            return width03;
        } else if (i<16) {
            return width04;
        } else if (i<19) {
            return width05;
        } else if (i<22) {
            return width06;
        } else if (i<27) {
            return width07;
        } else if (i<32) {
            return width08;
        } else if (i<37) {
            return width09;
        } else if (i<45) {
            return width10;
        } else if (i<52) {
            return width11;
        } else if (i<57) {
            return width12;
        } else if (i<65) {
            return width13;
        } else if (i<75) {
            return width14;
        } else if (i<85) {
            return width15;
        } else if (i<95) {
            return width16;
        } else if (i<103) {
            return width17;
        } else if (i<112) {
            return width18;
        } else if (i<130) {
            return width19;
        } else if (i<149) {
            return width20;
        } else if (i<180) {
            return width21;
        } else if (i<205) {
            return width22;
        } else {
            return width23;
        }
        //default by default
        return widthDefault;
    }

    static enum lineWidth dwgInt2lineWidth(int i){
        if ( (i>-1 && i<24) || (i>28 && i<32) ) {
            return static_cast<lineWidth> (i);
        }
        //default by default
        return widthDefault;
    }
};

#endif

// EOF

