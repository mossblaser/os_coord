/**
 * OS Coord: A Simple OS Coordinate Transformation Library for C
 *
 * This is a port of a the Javascript library produced by Chris Veness available
 * from http://www.movable-type.co.uk/scripts/latlong-gridref.html.
 *
 * Useful data/constants for the UK.
 */

#ifndef OS_COORD_DATA_H
#define OS_COORD_DATA_H

/******************************************************************************
 * Ellipsoid definitions. Values taken from "A guide to coordinate systems in
 * Great Britain"
 ******************************************************************************/

// Airy 1830
const os_ellipsoid_t OS_EL_AIRY_1830 = {.a=6377563.396, .b=6356256.910};

// Airy 1830 Modified
const os_ellipsoid_t OS_EL_AIRY_1830_MODIFIED = {.a=6377340.189, .b=6356034.447};

// Internaltional 1923 (aka Hayford 1909)
const os_ellipsoid_t OS_EL_INTERNATIONAL_1924 = {.a=6378388.000, .b=6356911.946};

// GRS80 (aka WGS84 ellipsoid)
const os_ellipsoid_t OS_EL_WGS84 = {.a=6378137.000, .b=6356752.3141};


/******************************************************************************
 * Helmert transformations
 ******************************************************************************/

// WGS84 -> OSGB36. Produces heights "similar to" ODN heights. From from "A
// guide to coordinate systems in Great Britain".
const os_helmert_t OS_HE_WGS84_TO_OSGB36 = {
	.tx= -446.448,  .ty=  125.157,   .tz= -542.060,
	.rx=   -0.1502, .ry=   -0.2470,  .rz=   -0.8421,
	 .s=   20.4894
};

// WGS84 -> ED50. Values taken from
// http://og.decc.gov.uk/en/olgs/cms/pons_and_cop/pons/pon4/pon4.aspx
const os_helmert_t OS_HE_WGS84_TO_ED50 = {
	.tx= 89.5, .ty= 93.8, .tz= 123.1,
	.rx=  0.0, .ry=  0.0, .rz=   0.156,
	 .s= -1.2 
};

// ETAF89 (similar to WGS84) -> IRL 1975. Values taken from
// http://www.osi.ie/OSI/media/OSI/Content/Publications/transformations_booklet.pdf
// and
// http://www.ordnancesurvey.co.uk/oswebsite/gps/information/coordinatesystemsinfo/guidecontents/guide6.html#6.5
const os_helmert_t OS_HE_ETRF89_TO_IRL1975 = {
	.tx= -482.530, .ty= 130.596, .tz= -564.557,
	.rx=   -1.042, .ry=  -0.214, .rz=   -0.631,
	 .s=   -8.150 
};


/******************************************************************************
 * Transverse Mercator Projections. Values taken from "A guide to coordinate
 * systems in Great Britain".
 ******************************************************************************/


// Ordenance Survey National Grid
const os_tm_projection_t OS_TM_NATIONAL_GRID = {
	.e0=400000.0, .n0=-100000.0,
	.f0=0.9996012717,
	.lat0=49.0, .lon0=-2.0,
	.ellipsoid={.a=6377563.396, .b=6356256.910} // Airy 1830
};

// Ordenance Survey Irish National Grid
const os_tm_projection_t OS_TM_IRISH_NATIONAL_GRID = {
	.e0=200000.0, .n0=250000.0,
	.f0=1.000035,
	.lat0=53.5, .lon0=-8.0,
	.ellipsoid={.a=6377340.189, .b=6356034.447} // Airy 1830 Modified
};

// Universal Transverse Mercator (UTM) Zone 29
const os_tm_projection_t OS_TM_UTM_ZOME_29 = {
	.e0=500000.0, .n0=0.0,
	.f0=0.9996,
	.lat0=0, .lon0=-9.0,
	.ellipsoid={.a=6378388.000, .b=6356911.946} // International 1924
};

// Universal Transverse Mercator (UTM) Zone 30
const os_tm_projection_t OS_TM_UTM_ZOME_30 = {
	.e0=500000.0, .n0=0.0,
	.f0=0.9996,
	.lat0=0, .lon0=-3.0,
	.ellipsoid={.a=6378388.000, .b=6356911.946} // International 1924
};

// Universal Transverse Mercator (UTM) Zone 31
const os_tm_projection_t OS_TM_UTM_ZOME_31 = {
	.e0=500000.0, .n0=0.0,
	.f0=0.9996,
	.lat0=0, .lon0=3.0,
	.ellipsoid={.a=6378388.000, .b=6356911.946} // International 1924
};


/******************************************************************************
 * National grid system grids.
 ******************************************************************************/

// Ordinance Survey National Grid over England, Scotland and Wales.
const os_grid_t OS_GR_NATIONAL_GRID = {
	.num_digits=2,
	.bottom_left_first_char='S',
	.width=7,
	.height=13
};

// Ordinance Survey National Grid over Ireland and Northern Ireland
const os_grid_t OS_GR_IRISH_NATIONAL_GRID = {
	.num_digits=1,
	.bottom_left_first_char='V',
	.width=5,
	.height=5
};

#endif

