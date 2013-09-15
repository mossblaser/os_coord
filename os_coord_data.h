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
const os_ellipsoid_t OS_EL_AIRY_1830 = {.a=OS_NUM_LIT(6377563.396), .b=OS_NUM_LIT(6356256.910)};

// Airy 1830 Modified
const os_ellipsoid_t OS_EL_AIRY_1830_MODIFIED = {.a=OS_NUM_LIT(6377340.189), .b=OS_NUM_LIT(6356034.447)};

// Internaltional 1923 (aka Hayford 1909)
const os_ellipsoid_t OS_EL_INTERNATIONAL_1924 = {.a=OS_NUM_LIT(6378388.000), .b=OS_NUM_LIT(6356911.946)};

// GRS80 (aka WGS84 ellipsoid)
const os_ellipsoid_t OS_EL_WGS84 = {.a=OS_NUM_LIT(6378137.000), .b=OS_NUM_LIT(6356752.3141)};


/******************************************************************************
 * Helmert transformations
 ******************************************************************************/

// WGS84 -> OSGB36. Produces heights "similar to" ODN heights. From from "A
// guide to coordinate systems in Great Britain".
const os_helmert_t OS_HE_WGS84_TO_OSGB36 = {
	.tx= OS_NUM_LIT(-446.448),  .ty=  OS_NUM_LIT(125.157),   .tz= OS_NUM_LIT(-542.060),
	.rx=   OS_NUM_LIT(-0.1502), .ry=   OS_NUM_LIT(-0.2470),  .rz=   OS_NUM_LIT(-0.8421),
	 .s=   OS_NUM_LIT(20.4894)
};

// WGS84 -> ED50. Values taken from
// http://og.decc.gov.uk/en/olgs/cms/pons_and_cop/pons/pon4/pon4.aspx
const os_helmert_t OS_HE_WGS84_TO_ED50 = {
	.tx= OS_NUM_LIT(89.5), .ty= OS_NUM_LIT(93.8), .tz= OS_NUM_LIT(123.1),
	.rx=  OS_NUM_LIT(0.0), .ry=  OS_NUM_LIT(0.0), .rz=   OS_NUM_LIT(0.156),
	 .s= OS_NUM_LIT(-1.2) 
};

// ETAF89 (similar to WGS84) -> IRL 1975. Values taken from
// http://www.osi.ie/OSI/media/OSI/Content/Publications/transformations_booklet.pdf
// and
// http://www.ordnancesurvey.co.uk/oswebsite/gps/information/coordinatesystemsinfo/guidecontents/guide6.html#OS_NUM_LIT(6.5)
const os_helmert_t OS_HE_ETRF89_TO_IRL1975 = {
	.tx= OS_NUM_LIT(-482.530), .ty= OS_NUM_LIT(130.596), .tz= OS_NUM_LIT(-564.557),
	.rx=   OS_NUM_LIT(-1.042), .ry=  OS_NUM_LIT(-0.214), .rz=   OS_NUM_LIT(-0.631),
	 .s=   OS_NUM_LIT(-8.150) 
};


/******************************************************************************
 * Transverse Mercator Projections. Values taken from "A guide to coordinate
 * systems in Great Britain".
 ******************************************************************************/


// Ordenance Survey National Grid
const os_tm_projection_t OS_TM_NATIONAL_GRID = {
	.e0=OS_NUM_LIT(400000.0), .n0=OS_NUM_LIT(-100000.0),
	.f0=OS_NUM_LIT(0.9996012717),
	.lat0=OS_NUM_LIT(49.0), .lon0=OS_NUM_LIT(-2.0),
	.ellipsoid={.a=OS_NUM_LIT(6377563.396), .b=OS_NUM_LIT(6356256.910)} // Airy 1830
};

// Ordenance Survey Irish National Grid
const os_tm_projection_t OS_TM_IRISH_NATIONAL_GRID = {
	.e0=OS_NUM_LIT(200000.0), .n0=OS_NUM_LIT(250000.0),
	.f0=OS_NUM_LIT(1.000035),
	.lat0=OS_NUM_LIT(53.5), .lon0=OS_NUM_LIT(-8.0),
	.ellipsoid={.a=OS_NUM_LIT(6377340.189), .b=OS_NUM_LIT(6356034.447)} // Airy 1830 Modified
};

// Universal Transverse Mercator (UTM) Zone 29
const os_tm_projection_t OS_TM_UTM_ZOME_29 = {
	.e0=OS_NUM_LIT(500000.0), .n0=OS_NUM_LIT(0.0),
	.f0=OS_NUM_LIT(0.9996),
	.lat0=OS_NUM_LIT(0.0), .lon0=OS_NUM_LIT(-9.0),
	.ellipsoid={.a=OS_NUM_LIT(6378388.000), .b=OS_NUM_LIT(6356911.946)} // International 1924
};

// Universal Transverse Mercator (UTM) Zone 30
const os_tm_projection_t OS_TM_UTM_ZOME_30 = {
	.e0=OS_NUM_LIT(500000.0), .n0=OS_NUM_LIT(0.0),
	.f0=OS_NUM_LIT(0.9996),
	.lat0=OS_NUM_LIT(0.0), .lon0=OS_NUM_LIT(-3.0),
	.ellipsoid={.a=OS_NUM_LIT(6378388.000), .b=OS_NUM_LIT(6356911.946)} // International 1924
};

// Universal Transverse Mercator (UTM) Zone 31
const os_tm_projection_t OS_TM_UTM_ZOME_31 = {
	.e0=OS_NUM_LIT(500000.0), .n0=OS_NUM_LIT(0.0),
	.f0=OS_NUM_LIT(0.9996),
	.lat0=OS_NUM_LIT(0.0), .lon0=OS_NUM_LIT(3.0),
	.ellipsoid={.a=OS_NUM_LIT(6378388.000), .b=OS_NUM_LIT(6356911.946)} // International 1924
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

