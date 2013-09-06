/**
 * OS Coord: A Simple OS Coordinate Transformation Library for C
 *
 * This is a port of a the Javascript library produced by Chris Veness available
 * from http://www.movable-type.co.uk/scripts/latlong-gridref.html.
 */

#include <stdio.h>
#include <math.h>

#include "os_coord.h"
#include "os_coord_transform.h"
#include "os_coord_math.h"

os_cartesian_t
os_lat_lon_to_cartesian( os_lat_lon_t   point 
                       , os_ellipsoid_t ellipsoid
                       )
{
	double sinPhi = sin(point.lat);
	double cosPhi = cos(point.lat);
	double sinLambda = sin(point.lon);
	double cosLambda = cos(point.lon);
	
	double eSq = ((ellipsoid.a*ellipsoid.a) - (ellipsoid.b*ellipsoid.b))
	             / (ellipsoid.a*ellipsoid.a);
	double nu = ellipsoid.a / sqrt(1.0 - (eSq*(sinPhi*sinPhi)));
	
	os_cartesian_t cart_point;
	cart_point.x = (nu+point.eh) * cosPhi * cosLambda;
	cart_point.y = (nu+point.eh) * cosPhi * sinLambda;
	cart_point.z = ((1.0-eSq)*nu + point.eh) * sinPhi;
	
	return cart_point;
}


os_lat_lon_t
os_cartesian_to_lat_lon( os_cartesian_t point
                       , os_ellipsoid_t ellipsoid
                       )
{
	// results accurate to around the given number of metres
	double precision = OS_CART_TO_LAT_LON_PRECISION / ellipsoid.a;
	
	double eSq = ((ellipsoid.a*ellipsoid.a) - (ellipsoid.b*ellipsoid.b))
	             / (ellipsoid.a*ellipsoid.a);
	double p = sqrt((point.x*point.x) + (point.y*point.y));
	double phi  = atan2(point.z, p*(1.0-eSq));
	double phiP = 2.0*PI;
	double nu;
	while (fabs(phi-phiP) > precision) {
	  nu   = ellipsoid.a / sqrt(1.0 - eSq*(sin(phi)*sin(phi)));
	  phiP = phi;
	  phi  = atan2(point.z + eSq*nu*sin(phi), p);
	}
	
	os_lat_lon_t lat_lon;
	lat_lon.lat = phi;
	lat_lon.lon = atan2(point.y, point.x);
	lat_lon.eh  = p/cos(phi) - nu;
	
	return lat_lon;
}


os_helmert_t
os_helmert_invert(os_helmert_t helmert)
{
	os_helmert_t new_helmert;
	
	new_helmert.tx = -helmert.tx;
	new_helmert.ty = -helmert.ty;
	new_helmert.tz = -helmert.tz;
	
	new_helmert.rx = -helmert.rx;
	new_helmert.ry = -helmert.ry;
	new_helmert.rz = -helmert.rz;
	
	new_helmert.s = -helmert.s;
	
	return new_helmert;
}


os_cartesian_t
os_helmert_transform( os_cartesian_t point 
                    , os_helmert_t   helmert
                    )
{
	// Normalise seconds to radians
	double rx = DEG_2_RAD(helmert.rx/3600.0);
	double ry = DEG_2_RAD(helmert.ry/3600.0);
	double rz = DEG_2_RAD(helmert.rz/3600.0);
	// Normalise ppm to (1+s)
	double s1 = 1+ (helmert.s/1000000.0);
	
	// Apply transform
	os_cartesian_t new_point;
	new_point.x = helmert.tx + point.x*s1 - point.y*rz + point.z*ry;
	new_point.y = helmert.ty + point.x*rz + point.y*s1 - point.z*rx;
	new_point.z = helmert.tz - point.x*ry + point.y*rx + point.z*s1;
	
	return new_point;
}


int
main(int argc, char *argv[])
{
	// Quick tests
	os_lat_lon_t ll = {.lat=DEG_2_RAD(52.65757), .lon=DEG_2_RAD(1.7179216), .eh=24.7};
	
	os_cartesian_t c  = os_lat_lon_to_cartesian(ll, OS_EL_AIRY_1830);
	os_cartesian_t c2 = os_helmert_transform(c,OS_HE_WGS84_TO_OSGB36);
	os_cartesian_t c3 = os_helmert_transform(c2,os_helmert_invert(OS_HE_WGS84_TO_OSGB36));
	os_lat_lon_t ll2 = os_cartesian_to_lat_lon(c3, OS_EL_AIRY_1830);
	
	printf("Lat/lon/eh before and after ll->cart->helmert->inv-helmert->ll\n");
	
	printf("Lat %f - %f = %0.10f\n", ll.lat, ll2.lat, ll.lat - ll2.lat);
	printf("Lon %f - %f = %0.10f\n", ll.lon, ll2.lon, ll.lon - ll2.lon);
	printf("Eh  %f - %f = %0.10f\n", ll.eh , ll2.eh , ll.eh  - ll2.eh);
	return 0;
}
