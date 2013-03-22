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

#ifndef BITSBUILD_H
#define BITSBUILD_H

#include <stdio.h>
#include <stdarg.h>
#include <qglobal.h>

#define BITS_STOP_MARKER	0
#define _	(quint64)

#define BITS_D64(nr) \
    (char)(( nr >> 63 ) & 1 ), \
    (char)(( nr >> 62 ) & 1 ), \
    (char)(( nr >> 61 ) & 1 ), \
    (char)(( nr >> 60 ) & 1 ), \
    (char)(( nr >> 59 ) & 1 ), \
    (char)(( nr >> 58 ) & 1 ), \
    (char)(( nr >> 57 ) & 1 ), \
    (char)(( nr >> 56 ) & 1 ), \
    (char)(( nr >> 55 ) & 1 ), \
    (char)(( nr >> 54 ) & 1 ), \
    (char)(( nr >> 53 ) & 1 ), \
    (char)(( nr >> 52 ) & 1 ), \
    (char)(( nr >> 51 ) & 1 ), \
    (char)(( nr >> 50 ) & 1 ), \
    (char)(( nr >> 49 ) & 1 ), \
    (char)(( nr >> 48 ) & 1 ), \
    (char)(( nr >> 47 ) & 1 ), \
    (char)(( nr >> 46 ) & 1 ), \
    (char)(( nr >> 45 ) & 1 ), \
    (char)(( nr >> 44 ) & 1 ), \
    (char)(( nr >> 43 ) & 1 ), \
    (char)(( nr >> 42 ) & 1 ), \
    (char)(( nr >> 41 ) & 1 ), \
    (char)(( nr >> 40 ) & 1 ), \
    (char)(( nr >> 39 ) & 1 ), \
    (char)(( nr >> 38 ) & 1 ), \
    (char)(( nr >> 37 ) & 1 ), \
    (char)(( nr >> 36 ) & 1 ), \
    (char)(( nr >> 35 ) & 1 ), \
    (char)(( nr >> 34 ) & 1 ), \
    (char)(( nr >> 33 ) & 1 ), \
    (char)(( nr >> 32 ) & 1 ), \
    BITS_D32(nr)
    
#define BITS_D32(nr) \
    (char)(( nr >> 31 ) & 1 ), \
    (char)(( nr >> 30 ) & 1 ), \
    (char)(( nr >> 29 ) & 1 ), \
    (char)(( nr >> 28 ) & 1 ), \
    (char)(( nr >> 27 ) & 1 ), \
    (char)(( nr >> 26 ) & 1 ), \
    (char)(( nr >> 25 ) & 1 ), \
    (char)(( nr >> 24 ) & 1 ), \
    (char)(( nr >> 23 ) & 1 ), \
    (char)(( nr >> 22 ) & 1 ), \
    (char)(( nr >> 21 ) & 1 ), \
    (char)(( nr >> 20 ) & 1 ), \
    (char)(( nr >> 19 ) & 1 ), \
    (char)(( nr >> 18 ) & 1 ), \
    (char)(( nr >> 17 ) & 1 ), \
    (char)(( nr >> 16 ) & 1 ), \
    BITS_D16(nr)
    
#define BITS_D16(nr) \
    (char)(( nr >> 15 ) & 1 ), \
    (char)(( nr >> 14 ) & 1 ), \
    (char)(( nr >> 13 ) & 1 ), \
    (char)(( nr >> 12 ) & 1 ), \
    (char)(( nr >> 11 ) & 1 ), \
    (char)(( nr >> 10 ) & 1 ), \
    (char)(( nr >> 9 ) & 1 ), \
    (char)(( nr >> 8 ) & 1 ), \
    BITS_D8(nr)
    
#define BITS_D8(nr) \
    (char)(( nr >> 7 ) & 1 ), \
    (char)(( nr >> 6 ) & 1 ), \
    (char)(( nr >> 5 ) & 1 ), \
    (char)(( nr >> 4 ) & 1 ), \
    BITS_D4(nr)
    
#define BITS_D4(nr) \
    (char)(( nr >> 3 ) & 1 ), \
    (char)(( nr >> 2 ) & 1 ), \
    (char)(( nr >> 1 ) & 1 ), \
    (char)(( nr >> 0 ) & 1 ) \

inline void addABit(int off, char * p_buff, bool is_set)
{
    int byte_off = off / 8;
    int bit_off = off % 8;
    unsigned char tmp = (unsigned char)p_buff[byte_off];
    unsigned char mask = 1 << bit_off;
    if ( is_set )
        tmp |= mask;
    else
        tmp &= ~mask;
    p_buff[byte_off] = tmp;
}
    
inline int addBits(int off, char * p_buff, ...)
{
    quint64 i_len; /* number of bits to copy from source */
    quint64 val;
    va_list argp;
    va_start(argp, p_buff);
    for (;;)
    {
        i_len = va_arg(argp, quint64);
        if ( i_len == BITS_STOP_MARKER )
            break;
        val = va_arg(argp, quint64);
        char loc_exp[64] = { BITS_D64(val) }; 
        for ( int i = 64 - i_len; i < 64; i++ )
        {
            addABit(off,p_buff,loc_exp[i]);
            off++;
        }
    }
    va_end(argp);
    return off;
}

inline int addRawBytes(int off, char * p_buff, char * p_inp, int cnt )
{
	if ( cnt <= 0 )
		return off;
	for ( int j = 0; j < cnt; j++ )
	{
		char loc_exp[8] = { BITS_D8(p_inp[j]) };
        for ( int i = 7; i >= 0; i-- )
        {
            addABit(off,p_buff,loc_exp[i]);
            off++;
        }
    }
    return off;
}


#endif	//	BITSBUILD_H