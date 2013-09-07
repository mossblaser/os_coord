/**
 * OS Coord: A Simple OS Coordinate Transformation Library for C
 *
 * This is a port of a the Javascript library produced by Chris Veness available
 * from http://www.movable-type.co.uk/scripts/latlong-gridref.html.
 */

#ifndef OS_COORD_H
#define OS_COORD_H

/******************************************************************************
 * Coordinate Types
 ******************************************************************************/

/**
 * Eastings and Northings (and height).
 */
typedef struct os_eas_nor {
	// Eastings (m)
	double e;
	
	// Northings (m)
	double n;
	
	// Height (m)
	double h;
} os_eas_nor_t;


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
 * Ellipsoid definitions
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


/******************************************************************************
 * Helmert transformations
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


/******************************************************************************
 * Transverse Mercator projections
 ******************************************************************************/

typedef struct os_tm_projection {
	// Eastings and Northings of the true origin (m)
	double e0;
	double n0;
	
	// Central meridian scale factor
	double f0;
	
	// Latitude of true origin (degrees)
	double lat0;
	
	// Longitude of true origin and central meridian (degrees)
	double lon0;
	
	// The ellipsoid on which the lat/lon refer.
	os_ellipsoid_t ellipsoid;
} os_tm_projection_t;

#endif
