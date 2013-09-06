/**
 * OS Coord: A Simple OS Coordinate Transformation Library for C
 *
 * This is a port of a the Javascript library produced by Chris Veness available
 * from http://www.movable-type.co.uk/scripts/latlong-gridref.html.
 */

#ifndef OS_COORD_H
#define OS_COORD_H

/******************************************************************************
 * Coordinates
 ******************************************************************************/

/**
 * Latitude/Longitude with ellipsoidal height.
 */
typedef struct os_lat_lon {
	// Latitude (radians)
	double lat;
	
	// Longitude (radians)
	double lon;
	
	// Ellipsoidal height (m)
	double eh;
} os_lat_lon_t;

/**
 * Cartesian 3D coordinate.
 */
typedef struct os_cartesian {
	double x;
	double y;
	double z;
} os_cartesian_t;


/******************************************************************************
 * Ellipsoid definition and along with some comman ellipsoids.
 ******************************************************************************/

/**
 * Ellipsoid shape definition.
 */
typedef struct os_ellipsoid {
	// Semi-major axis (m)
	double a;
	
	// Semi-minor axis (m)
	double b;
} os_ellipsoid_t;

// Ellipsoid values taken from "A guide to coordinate systems in Great Britain"
const os_ellipsoid_t OS_EL_AIRY_1830          = {.a=6377563.396, .b=6356256.910};
const os_ellipsoid_t OS_EL_AIRY_1830_MODIFIED = {.a=6377340.189, .b=6356034.447};
const os_ellipsoid_t OS_EL_INTERNATIONAL_1924 = {.a=6378388.000, .b=6356911.946};
const os_ellipsoid_t OS_EL_GRS80              = {.a=6378137.000, .b=6356752.314140};
const os_ellipsoid_t OS_EL_WGS84              = {.a=6378137.000, .b=6356752.3142};


/******************************************************************************
 * Helmert transformations along with some useful transformations.
 ******************************************************************************/

/**
 * Helmert transformation parameters for approximately converting a set of
 * lat/lon between a pair of datum values.
 */
typedef struct os_helmert {
	// Translation (m)
	double tx;
	double ty;
	double tz;
	
	// Rotation (seconds)
	double rx;
	double ry;
	double rz;
	
	// Scale Factor (ppm)
	double s;
} os_helmert_t;

// Values taken from "A guide to coordinate systems in Great Britain"
const os_helmert_t OS_HE_WGS84_TO_OSGB36 = {
	.tx= -446.448,  .ty=  125.157,   .tz= -542.060,
	.rx=   -0.1502, .ry=   -0.2470,  .rz=   -0.8421,
	 .s=   20.4894
};

// Values taken from http://og.decc.gov.uk/en/olgs/cms/pons_and_cop/pons/pon4/pon4.aspx
const os_helmert_t OS_HE_WGS84_TO_ED50 = {
	.tx= 89.5, .ty= 93.8, .tz= 123.1,
	.rx=  0.0, .ry=  0.0, .rz=   0.156,
	 .s= -1.2 
};

// Values taken from http://www.osi.ie/OSI/media/OSI/Content/Publications/transformations_booklet.pdf
// and http://www.ordnancesurvey.co.uk/oswebsite/gps/information/coordinatesystemsinfo/guidecontents/guide6.html#6.5
const os_helmert_t OS_HE_ETRF89_TO_IRL1975 = {
	.tx= -482.530, .ty= 130.596, .tz= -564.557,
	.rx=   -1.042, .ry=  -0.214, .rz=   -0.631,
	 .s=   -8.150 
};

#endif
