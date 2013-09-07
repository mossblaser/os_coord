/**
 * OS Coord: A Simple OS Coordinate Transformation Library for C
 *
 * This is a port of a the Javascript library produced by Chris Veness available
 * from http://www.movable-type.co.uk/scripts/latlong-gridref.html.
 *
 * These functions are largely based on the guide in "A guide to coordinate
 * systems in Great Britain", Section 6 and Appendix C.
 */

#ifndef OS_COORD_TRANSFORM_H
#define OS_COORD_TRANSFORM_H

#include "os_coord.h"

/**
 * Conversion from cartesian to lat-lon coordinates is done via an iterative
 * algorithm. This constant defines the number of meters precision to achieve.
 */
#define OS_CART_TO_LAT_LON_PRECISION 4.0

/**
 * Conversion from cartesian to eastings and northings on a TM projection to
 * lat-lon is done via an iterative algorithm. This constant defines maximum
 * value that (N - N_0 - M) may hold (m). The value 0.1mm is suggested by "A
 * guide to coordinate systems in Great Britain".
 */
#define OS_EAS_NOR_TO_LAT_LON_PRECISION 0.00001

/**
 * Convert a lat/lon/eh point on an ellipsoid to the corresponding point in 3D
 * cartesian space.
 */
os_cartesian_t os_lat_lon_to_cartesian(os_lat_lon_t point, os_ellipsoid_t ellipsoid);

/**
 * Convert a 3D cartesian point into a lat/lon/eh point on an ellipsoid.
 */
os_lat_lon_t os_cartesian_to_lat_lon(os_cartesian_t point, os_ellipsoid_t ellipsoid);

/**
 * Transform a set of Helmert parameters to give the inverse transform.
 */
os_helmert_t os_helmert_invert(os_helmert_t helmert);

/**
 * Perform a Helmert transform on a point in cartesian space.
 */
os_cartesian_t os_helmert_transform(os_cartesian_t point, os_helmert_t helmert);

/**
 * Transform a lat/lon/eh into eastings and northings using a transverse
 * mercator projection. Ellipsoidal height is copied verbatim.
 */
os_eas_nor_t os_lat_lon_to_tm_eas_nor(os_lat_lon_t point, os_tm_projection_t projection);

/**
 * Transform a set of eastings and northings into lat/lon/eh from a transverse
 * mercator projection. Height is copied verbatim.
 */
os_lat_lon_t os_tm_eas_nor_to_lat_lon(os_eas_nor_t point, os_tm_projection_t projection);

#endif
