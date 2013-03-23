global i_bytes

def parse_header(s_raw,lst_out):
	# the name
	i = s_raw.find( ":" )+1
	j = s_raw.find( "(" )
	s = s_raw[i:j].strip()
	s = s.replace(" ","_")
	lst_out.append( s )

	# the id (hex)
	i = s_raw.find( "H" )
	s = "0x" + s_raw[j+1:i] 
	lst_out.append( s )
	
	# the lenght (decimal)
	i = s_raw.find( "(",i )+1
	j = s_raw.find( ")", i )
	s = s_raw[i:j].strip()
	lst_out.append( s )
	
	# the handle
	i = s_raw.find( ":", j )
	s = s_raw[i+1:].strip()
	lst_bytes = s.split()
	s = ""
	for by in lst_bytes:
		s = s + by
	s = "0x" + s
	lst_out.append( s )
	
	if ( lst_out[0] == "proxy" ):
		lst_out[0] = "proxy_" + lst_out[3]
	elif ( lst_out[0] == "spline" ):
		lst_out[0] = "spline_" + lst_out[3]
	
def bytesLine ( s_raw ):
	global i_bytes
	s_ret = ""
	bylst = s_raw.split(" ",8)
	for s in bylst:
		s = s.strip()
		if len(s) == 0:
			break
		i_bytes = i_bytes + 1
		s_ret = s_ret + "0x" + s + ", "
	while len(s_ret) < 50:
		s_ret = s_ret + " "
	return s_ret
# ====================================================================



b_first = True;
line_nr = 0
ins = open( "OpenDesignSamples.txt", "r" )
f_cpp = open('odsample.cpp','w')
f_hpp = open('odsample.h','w')


f_cpp.write('''
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

#include "odsample.h"


''')

f_hpp.write('''
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

#ifndef OPEN_DESIGN_DWG_SAMPLE_H
#define OPEN_DESIGN_DWG_SAMPLE_H

''')

hd_list = []
i_bytes = 0

for line in ins:
	line_nr = line_nr + 1
	line = line.strip()
	if len(line) == 0:
		continue
	#print line
	if line.startswith("OBJECT"):
		print hd_list
		if ( b_first ):
			b_first = False
		else:
			f_cpp.write( "            0x00\n/* ENDOBJECT */\n};\n\n" )
			f_hpp.write( "extern unsigned char od_" + hd_list[0] + "[" + str(i_bytes+1) + "];\n" )
			f_hpp.write( "enum od_" + hd_list[0] + "_defs {\n" )
			f_hpp.write( "\tod_" + hd_list[0] + "_id = " +  hd_list[1] + ",\n" )
			f_hpp.write( "\tod_" + hd_list[0] + "_len = " +  hd_list[2] + ",\n" )
			f_hpp.write( "\tod_" + hd_list[0] + "_hdl = " +  hd_list[3] + "\n" )
			f_hpp.write( "};\n\n" )
			hd_list = []
			
		parse_header(line,hd_list)
		
		f_cpp.write( "unsigned char od_" + hd_list[0] + "[] = {\n" )
		f_cpp.write( "/* " + line + " */\n" )
		i_bytes = 0
	else:
		if line[5] != " ":
			print line_nr
		elif line[29] != " ":
			print line_nr
		elif len(line) > 38:
			if line[38] != " ":
				print line_nr
		s_addr = line[0:5]
		s_bytes = line[6:29]
		s_tx = line[30:38]
		if len(line) > 39:
			s_bits = line[39:]
		else:
			s_bits = ""
		s_bytes = bytesLine(s_bytes)
		f_cpp.write( "/* " + s_addr + " */ " )
		f_cpp.write( s_bytes )		
		f_cpp.write( " /* " + s_tx + " " + s_bits + " */\n" )
		
		#print s_bits
	
f_hpp.write('''
#endif // OPEN_DESIGN_DWG_SAMPLE_H
''')

f_cpp.write('''
};
''')
f_hpp.write( "extern unsigned char od_" + hd_list[0] + "[];\n" )
f_hpp.write( "enum od_" + hd_list[0] + "_defs {\n" )
f_hpp.write( "\tod_" + hd_list[0] + "_id = " +  hd_list[1] + ",\n" )
f_hpp.write( "\tod_" + hd_list[0] + "_len = " +  hd_list[2] + ",\n" )
f_hpp.write( "\tod_" + hd_list[0] + "_hdl = " +  hd_list[3] + "\n" )
f_hpp.write( "};\n\n" )

