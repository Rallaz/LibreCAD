
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

extern unsigned char od_text[78];
enum od_text_defs {
	od_text_id = 0x1,
	od_text_len = 73,
	od_text_hdl = 0x4C
};

extern unsigned char od_attrib[93];
enum od_attrib_defs {
	od_attrib_id = 0x2,
	od_attrib_len = 88,
	od_attrib_hdl = 0x52
};

extern unsigned char od_attdef[85];
enum od_attdef_defs {
	od_attdef_id = 0x3,
	od_attdef_len = 80,
	od_attdef_hdl = 0x4C
};

extern unsigned char od_block[27];
enum od_block_defs {
	od_block_id = 0x4,
	od_block_len = 22,
	od_block_hdl = 0x4E
};

extern unsigned char od_endblk[20];
enum od_endblk_defs {
	od_endblk_id = 0x5,
	od_endblk_len = 15,
	od_endblk_hdl = 0x1B
};

extern unsigned char od_seqend[22];
enum od_seqend_defs {
	od_seqend_id = 0x6,
	od_seqend_len = 17,
	od_seqend_hdl = 0x53
};

extern unsigned char od_insert[46];
enum od_insert_defs {
	od_insert_id = 0x7,
	od_insert_len = 41,
	od_insert_hdl = 0x51
};

extern unsigned char od_minsert[59];
enum od_minsert_defs {
	od_minsert_id = 0x8,
	od_minsert_len = 54,
	od_minsert_hdl = 0x59
};

extern unsigned char od_pline_vert[39];
enum od_pline_vert_defs {
	od_pline_vert_id = 0xA,
	od_pline_vert_len = 34,
	od_pline_vert_hdl = 0x4D
};

extern unsigned char od_3d_pline_vert[31];
enum od_3d_pline_vert_defs {
	od_3d_pline_vert_id = 0xB,
	od_3d_pline_vert_len = 26,
	od_3d_pline_vert_hdl = 0x62
};

extern unsigned char od_pface_pt[38];
enum od_pface_pt_defs {
	od_pface_pt_id = 0xD,
	od_pface_pt_len = 33,
	od_pface_pt_hdl = 0x56
};

extern unsigned char od_pface_face_def[24];
enum od_pface_face_def_defs {
	od_pface_face_def_id = 0xE,
	od_pface_face_def_len = 19,
	od_pface_face_def_hdl = 0x5A
};

extern unsigned char od_pline_st[29];
enum od_pline_st_defs {
	od_pline_st_id = 0xF,
	od_pline_st_len = 24,
	od_pline_st_hdl = 0x4C
};

extern unsigned char od_3d_poly_start[30];
enum od_3d_poly_start_defs {
	od_3d_poly_start_id = 0x10,
	od_3d_poly_start_len = 25,
	od_3d_poly_start_hdl = 0x5E
};

extern unsigned char od_arc[63];
enum od_arc_defs {
	od_arc_id = 0x11,
	od_arc_len = 58,
	od_arc_hdl = 0x64
};

extern unsigned char od_circle[48];
enum od_circle_defs {
	od_circle_id = 0x12,
	od_circle_len = 43,
	od_circle_hdl = 0x92
};

extern unsigned char od_line[58];
enum od_line_defs {
	od_line_id = 0x13,
	od_line_len = 53,
	od_line_hdl = 0xCC
};

extern unsigned char od_dim_ordinate[97];
enum od_dim_ordinate_defs {
	od_dim_ordinate_id = 0x14,
	od_dim_ordinate_len = 92,
	od_dim_ordinate_hdl = 0x9E
};

extern unsigned char od_dim_linear[112];
enum od_dim_linear_defs {
	od_dim_linear_id = 0x15,
	od_dim_linear_len = 107,
	od_dim_linear_hdl = 0xAC
};

extern unsigned char od_dim_aligned[112];
enum od_dim_aligned_defs {
	od_dim_aligned_id = 0x16,
	od_dim_aligned_len = 107,
	od_dim_aligned_hdl = 0xBA
};

extern unsigned char od_dim_angular[128];
enum od_dim_angular_defs {
	od_dim_angular_id = 0x17,
	od_dim_angular_len = 123,
	od_dim_angular_hdl = 0xC9
};

extern unsigned char od_dim_radial[118];
enum od_dim_radial_defs {
	od_dim_radial_id = 0x19,
	od_dim_radial_len = 113,
	od_dim_radial_hdl = 0xD5
};

extern unsigned char od_dim_diameter[117];
enum od_dim_diameter_defs {
	od_dim_diameter_id = 0x1A,
	od_dim_diameter_len = 112,
	od_dim_diameter_hdl = 0xE1
};

extern unsigned char od_point[40];
enum od_point_defs {
	od_point_id = 0x1B,
	od_point_len = 35,
	od_point_hdl = 0xD2
};

extern unsigned char od_3d_face[85];
enum od_3d_face_defs {
	od_3d_face_id = 0x1C,
	od_3d_face_len = 80,
	od_3d_face_hdl = 0xE3
};

extern unsigned char od_pface_start[30];
enum od_pface_start_defs {
	od_pface_start_id = 0x1D,
	od_pface_start_len = 25,
	od_pface_start_hdl = 0x55
};

extern unsigned char od_3d_surf_sol_st[31];
enum od_3d_surf_sol_st_defs {
	od_3d_surf_sol_st_id = 0x1E,
	od_3d_surf_sol_st_len = 26,
	od_3d_surf_sol_st_hdl = 0x66
};

extern unsigned char od_solid[87];
enum od_solid_defs {
	od_solid_id = 0x1F,
	od_solid_len = 82,
	od_solid_hdl = 0xCF
};

extern unsigned char od_trace[86];
enum od_trace_defs {
	od_trace_id = 0x20,
	od_trace_len = 81,
	od_trace_hdl = 0xE7
};

extern unsigned char od_shape[43];
enum od_shape_defs {
	od_shape_id = 0x21,
	od_shape_len = 38,
	od_shape_hdl = 0xF5
};

extern unsigned char od_vpent[284];
enum od_vpent_defs {
	od_vpent_id = 0x22,
	od_vpent_len = 279,
	od_vpent_hdl = 0x0126
};

extern unsigned char od_ellipse[81];
enum od_ellipse_defs {
	od_ellipse_id = 0x23,
	od_ellipse_len = 76,
	od_ellipse_hdl = 0x0122
};

extern unsigned char od_spline_0x0101[102];
enum od_spline_0x0101_defs {
	od_spline_0x0101_id = 0x24,
	od_spline_0x0101_len = 97,
	od_spline_0x0101_hdl = 0x0101
};

extern unsigned char od_spline_0x0102[192];
enum od_spline_0x0102_defs {
	od_spline_0x0102_id = 0x24,
	od_spline_0x0102_len = 187,
	od_spline_0x0102_hdl = 0x0102
};

extern unsigned char od_region[562];
enum od_region_defs {
	od_region_id = 0x25,
	od_region_len = 557,
	od_region_hdl = 0x0103
};

extern unsigned char od_3d_solid[825];
enum od_3d_solid_defs {
	od_3d_solid_id = 0x26,
	od_3d_solid_len = 820,
	od_3d_solid_hdl = 0x0104
};

extern unsigned char od_ray[52];
enum od_ray_defs {
	od_ray_id = 0x28,
	od_ray_len = 47,
	od_ray_hdl = 0x0106
};

extern unsigned char od_const_line[52];
enum od_const_line_defs {
	od_const_line_id = 0x29,
	od_const_line_len = 47,
	od_const_line_hdl = 0x0105
};

extern unsigned char od_dictionary[49];
enum od_dictionary_defs {
	od_dictionary_id = 0x2A,
	od_dictionary_len = 44,
	od_dictionary_hdl = 0x0C
};

extern unsigned char od_mtext[82];
enum od_mtext_defs {
	od_mtext_id = 0x2C,
	od_mtext_len = 77,
	od_mtext_hdl = 0xCE
};

extern unsigned char od_leader[133];
enum od_leader_defs {
	od_leader_id = 0x2D,
	od_leader_len = 128,
	od_leader_hdl = 0x0109
};

extern unsigned char od_tolerance[106];
enum od_tolerance_defs {
	od_tolerance_id = 0x2E,
	od_tolerance_len = 101,
	od_tolerance_hdl = 0x010C
};

extern unsigned char od_mline[233];
enum od_mline_defs {
	od_mline_id = 0x2F,
	od_mline_len = 228,
	od_mline_hdl = 0x010D
};

extern unsigned char od_blk_ctrl[37];
enum od_blk_ctrl_defs {
	od_blk_ctrl_id = 0x30,
	od_blk_ctrl_len = 32,
	od_blk_ctrl_hdl = 0x01
};

extern unsigned char od_blk_hdr[30];
enum od_blk_hdr_defs {
	od_blk_hdr_id = 0x31,
	od_blk_hdr_len = 25,
	od_blk_hdr_hdl = 0xCA
};

extern unsigned char od_layer_ctrl[20];
enum od_layer_ctrl_defs {
	od_layer_ctrl_id = 0x32,
	od_layer_ctrl_len = 15,
	od_layer_ctrl_hdl = 0x02
};

extern unsigned char od_layer[32];
enum od_layer_defs {
	od_layer_id = 0x33,
	od_layer_len = 27,
	od_layer_hdl = 0x99
};

extern unsigned char od_shpfile_ctrl[20];
enum od_shpfile_ctrl_defs {
	od_shpfile_ctrl_id = 0x34,
	od_shpfile_ctrl_len = 15,
	od_shpfile_ctrl_hdl = 0x03
};

extern unsigned char od_shpfile[42];
enum od_shpfile_defs {
	od_shpfile_id = 0x35,
	od_shpfile_len = 37,
	od_shpfile_hdl = 0x10
};

extern unsigned char od_ltype_ctrl[22];
enum od_ltype_ctrl_defs {
	od_ltype_ctrl_id = 0x38,
	od_ltype_ctrl_len = 17,
	od_ltype_ctrl_hdl = 0x05
};

extern unsigned char od_view_ctrl[18];
enum od_view_ctrl_defs {
	od_view_ctrl_id = 0x3C,
	od_view_ctrl_len = 13,
	od_view_ctrl_hdl = 0x06
};

extern unsigned char od_view[69];
enum od_view_defs {
	od_view_id = 0x3D,
	od_view_len = 64,
	od_view_hdl = 0x3F
};

extern unsigned char od_ucs_ctrl[18];
enum od_ucs_ctrl_defs {
	od_ucs_ctrl_id = 0x3E,
	od_ucs_ctrl_len = 13,
	od_ucs_ctrl_hdl = 0x07
};

extern unsigned char od_ucs[74];
enum od_ucs_defs {
	od_ucs_id = 0x3F,
	od_ucs_len = 69,
	od_ucs_hdl = 0x4C
};

extern unsigned char od_vport_ctrl[23];
enum od_vport_ctrl_defs {
	od_vport_ctrl_id = 0x40,
	od_vport_ctrl_len = 18,
	od_vport_ctrl_hdl = 0x08
};

extern unsigned char od_vport[152];
enum od_vport_defs {
	od_vport_id = 0x41,
	od_vport_len = 147,
	od_vport_hdl = 0x4E
};

extern unsigned char od_regapp_ctrl[20];
enum od_regapp_ctrl_defs {
	od_regapp_ctrl_id = 0x42,
	od_regapp_ctrl_len = 15,
	od_regapp_ctrl_hdl = 0x09
};

extern unsigned char od_regapp[24];
enum od_regapp_defs {
	od_regapp_id = 0x43,
	od_regapp_len = 19,
	od_regapp_hdl = 0x11
};

extern unsigned char od_dimstyle_ctrl[21];
enum od_dimstyle_ctrl_defs {
	od_dimstyle_ctrl_id = 0x44,
	od_dimstyle_ctrl_len = 16,
	od_dimstyle_ctrl_hdl = 0x0A
};

extern unsigned char od_dimstyle[117];
enum od_dimstyle_defs {
	od_dimstyle_id = 0x45,
	od_dimstyle_len = 112,
	od_dimstyle_hdl = 0x1D
};

extern unsigned char od_vpent_ctrl[28];
enum od_vpent_ctrl_defs {
	od_vpent_ctrl_id = 0x46,
	od_vpent_ctrl_len = 23,
	od_vpent_ctrl_hdl = 0x0B
};

extern unsigned char od_vpent_hdr[22];
enum od_vpent_hdr_defs {
	od_vpent_hdr_id = 0x47,
	od_vpent_hdr_len = 17,
	od_vpent_hdr_hdl = 0x58
};

extern unsigned char od_group[44];
enum od_group_defs {
	od_group_id = 0x48,
	od_group_len = 39,
	od_group_hdl = 0x7B
};

extern unsigned char od_mlstyle[90];
enum od_mlstyle_defs {
	od_mlstyle_id = 0x49,
	od_mlstyle_len = 85,
	od_mlstyle_hdl = 0x74
};

extern unsigned char od_proxy_0x01EA[23];
enum od_proxy_0x01EA_defs {
	od_proxy_0x01EA_id = 0x1F9,
	od_proxy_0x01EA_len = 18,
	od_proxy_0x01EA_hdl = 0x01EA
};

extern unsigned char od_proxy_0x68[231];
enum od_proxy_0x68_defs {
	od_proxy_0x68_id = 0x1F5,
	od_proxy_0x68_len = 226,
	od_proxy_0x68_hdl = 0x68
};

extern unsigned char od_proxy_0x8B[23];
enum od_proxy_0x8B_defs {
	od_proxy_0x8B_id = 0x1FA,
	od_proxy_0x8B_len = 18,
	od_proxy_0x8B_hdl = 0x8B
};

extern unsigned char od_proxy_0x6D[270];
enum od_proxy_0x6D_defs {
	od_proxy_0x6D_id = 0x1F9,
	od_proxy_0x6D_len = 265,
	od_proxy_0x6D_hdl = 0x6D
};

extern unsigned char od_proxy_0x6B[83];
enum od_proxy_0x6B_defs {
	od_proxy_0x6B_id = 0x1F7,
	od_proxy_0x6B_len = 78,
	od_proxy_0x6B_hdl = 0x6B
};

extern unsigned char od_proxy_0x6C[17];
enum od_proxy_0x6C_defs {
	od_proxy_0x6C_id = 0x1F8,
	od_proxy_0x6C_len = 12,
	od_proxy_0x6C_hdl = 0x6C
};

extern unsigned char od_proxy_0x01F8[94];
enum od_proxy_0x01F8_defs {
	od_proxy_0x01F8_id = 0x1FF,
	od_proxy_0x01F8_len = 89,
	od_proxy_0x01F8_hdl = 0x01F8
};

extern unsigned char od_proxy_0x010F[205];
enum od_proxy_0x010F_defs {
	od_proxy_0x010F_id = 0x1FB,
	od_proxy_0x010F_len = 200,
	od_proxy_0x010F_hdl = 0x010F
};

extern unsigned char od_proxy_0x5A[22];
enum od_proxy_0x5A_defs {
	od_proxy_0x5A_id = 0x1F5,
	od_proxy_0x5A_len = 17,
	od_proxy_0x5A_hdl = 0x5A
};

extern unsigned char od_proxy_0xA5[94];
enum od_proxy_0xA5_defs {
	od_proxy_0xA5_id = 0x1FA,
	od_proxy_0xA5_len = 89,
	od_proxy_0xA5_hdl = 0xA5
};

extern unsigned char od_proxy_0x0215[128];
enum od_proxy_0x0215_defs {
	od_proxy_0x0215_id = 0x1FD,
	od_proxy_0x0215_len = 123,
	od_proxy_0x0215_hdl = 0x0215
};

extern unsigned char od_proxy_0x01F9[1035];
enum od_proxy_0x01F9_defs {
	od_proxy_0x01F9_id = 0x200,
	od_proxy_0x01F9_len = 1030,
	od_proxy_0x01F9_hdl = 0x01F9
};


#endif // OPEN_DESIGN_DWG_SAMPLE_H
extern unsigned char od_proxy_0x28[];
enum od_proxy_0x28_defs {
	od_proxy_0x28_id = 0x1F4,
	od_proxy_0x28_len = 101,
	od_proxy_0x28_hdl = 0x28
};

