/**
 * OS Coord: A Simple OS Coordinate Transformation Library for C
 *
 * This is a port of a the Javascript library produced by Chris Veness available
 * from http://www.movable-type.co.uk/scripts/latlong-gridref.html.
 *
 * These functions are largely based on the guide in "A guide to coordinate
 * systems in Great Britain", Section 6.
 */

#include <stdio.h>

#include "os_coord.h"
#include "os_coord_transform.h"
#include "os_coord_math.h"

os_cartesian_t
os_lat_lon_to_cartesian( os_lat_lon_t   point 
                       , os_ellipsoid_t ellipsoid
                       )
{
	OS_NUM_T sinPhi = OS_SIN(point.lat);
	OS_NUM_T cosPhi = OS_COS(point.lat);
	OS_NUM_T sinLambda = OS_SIN(point.lon);
	OS_NUM_T cosLambda = OS_COS(point.lon);
	
	OS_NUM_T eSq = OS_DIV(
		OS_SUB(
			OS_MUL(ellipsoid.a,ellipsoid.a),
			OS_MUL(ellipsoid.b,ellipsoid.b)
		),
		OS_MUL(ellipsoid.a,ellipsoid.a)
	);
	OS_NUM_T nu = OS_DIV(
		ellipsoid.a,
		OS_SQRT(OS_SUB(
			OS_NUM_LIT(1.0),
			OS_MUL(eSq,OS_MUL(sinPhi,sinPhi))
		))
	);
	
	os_cartesian_t cart_point;
	cart_point.x = OS_MUL(OS_MUL(
		OS_ADD(nu, point.eh),
		cosPhi),
		cosLambda
	);
	cart_point.y = OS_MUL(OS_MUL(
		OS_ADD(nu, point.eh),
		cosPhi),
		sinLambda
	);
	cart_point.z = OS_MUL(
		OS_ADD(
			OS_MUL(
				OS_SUB(OS_NUM_LIT(1.0), eSq),
				nu
			),
			point.eh
		),
		sinPhi
	);
	
	return cart_point;
}


os_lat_lon_t
os_cartesian_to_lat_lon( os_cartesian_t point
                       , os_ellipsoid_t ellipsoid
                       )
{
	// results accurate to around the given number of metres
	OS_NUM_T precision = OS_DIV(OS_CART_TO_LAT_LON_PRECISION, ellipsoid.a);
	
	OS_NUM_T eSq = OS_DIV(
		OS_SUB(
			OS_MUL(ellipsoid.a,ellipsoid.a),
			OS_MUL(ellipsoid.b,ellipsoid.b)
		),
		OS_MUL(ellipsoid.a,ellipsoid.a)
	);
	OS_NUM_T p = OS_SQRT(
		OS_ADD(
			OS_MUL(point.x,point.x),
			OS_MUL(point.y,point.y)
		)
	);
	OS_NUM_T phi = OS_ATAN2(
		point.z,
		OS_MUL(
			p,
			OS_SUB(OS_NUM_LIT(1.0), eSq)
		)
	);
	OS_NUM_T phiP = OS_MUL(OS_NUM_LIT(2.0), PI);
	OS_NUM_T nu;
	while (OS_CMP(OS_ABS(OS_SUB(phi,phiP)), precision) > 0) {
	  nu = OS_DIV(
			ellipsoid.a,
			OS_SQRT(
				OS_SUB(
					OS_NUM_LIT(1.0),
					OS_MUL(
						eSq,
						OS_MUL(OS_SIN(phi),OS_SIN(phi))
					)
				)
			)
		);
	  phiP = phi;
	  phi = OS_ATAN2(
			OS_ADD(
				point.z,
				OS_MUL(
					eSq,
					OS_MUL(nu, OS_SIN(phi))
				)
			),
			p
		);
	}
	
	os_lat_lon_t lat_lon;
	lat_lon.lat = phi;
	lat_lon.lon = OS_ATAN2(point.y, point.x);
	lat_lon.eh  = OS_SUB(
		OS_DIV(p,OS_COS(phi)),
		nu
	);
	
	return lat_lon;
}


os_helmert_t
os_helmert_invert(os_helmert_t helmert)
{
	os_helmert_t new_helmert;
	
	new_helmert.tx = OS_SUB(OS_NUM_LIT(0.0),helmert.tx);
	new_helmert.ty = OS_SUB(OS_NUM_LIT(0.0),helmert.ty);
	new_helmert.tz = OS_SUB(OS_NUM_LIT(0.0),helmert.tz);
	
	new_helmert.rx = OS_SUB(OS_NUM_LIT(0.0),helmert.rx);
	new_helmert.ry = OS_SUB(OS_NUM_LIT(0.0),helmert.ry);
	new_helmert.rz = OS_SUB(OS_NUM_LIT(0.0),helmert.rz);
	
	new_helmert.s = OS_SUB(OS_NUM_LIT(0.0),helmert.s);
	
	return new_helmert;
}


os_cartesian_t
os_helmert_transform( os_cartesian_t point 
                    , os_helmert_t   helmert
                    )
{
	// Normalise seconds to radians
	OS_NUM_T rx = DEG_2_RAD(OS_DIV(helmert.rx, OS_NUM_LIT(3600.0)));
	OS_NUM_T ry = DEG_2_RAD(OS_DIV(helmert.ry, OS_NUM_LIT(3600.0)));
	OS_NUM_T rz = DEG_2_RAD(OS_DIV(helmert.rz, OS_NUM_LIT(3600.0)));
	// Normalise ppm to (1+s)
	OS_NUM_T s1 = OS_ADD(
		OS_NUM_LIT(1.0),
		OS_DIV(helmert.s, OS_NUM_LIT(1000000.0))
	);
	
	// Apply transform
	os_cartesian_t new_point;
	new_point.x = OS_ADD(OS_SUB(OS_ADD(helmert.tx, OS_MUL(point.x, s1)), OS_MUL(point.y, rz)), OS_MUL(point.z, ry));
	new_point.y = OS_SUB(OS_ADD(OS_ADD(helmert.ty, OS_MUL(point.x, rz)), OS_MUL(point.y, s1)), OS_MUL(point.z, rx));
	new_point.z = OS_ADD(OS_ADD(OS_SUB(helmert.tz, OS_MUL(point.x, ry)), OS_MUL(point.y, rx)), OS_MUL(point.z, s1));
	
	return new_point;
}


os_eas_nor_t
os_lat_lon_to_tm_eas_nor( os_lat_lon_t       point
                        , os_tm_projection_t projection
                        )
{
	// Convert to radians
	OS_NUM_T lat0 = DEG_2_RAD(projection.lat0);
	OS_NUM_T lon0 = DEG_2_RAD(projection.lon0);
	
	// Shorter-named alias
	OS_NUM_T lat = point.lat;
	OS_NUM_T lon = point.lon;
	OS_NUM_T a = projection.ellipsoid.a;
	OS_NUM_T b = projection.ellipsoid.b;
	
	OS_NUM_T e2 = OS_SUB(OS_NUM_LIT(1.0), OS_DIV(OS_MUL(b,b),OS_MUL(a,a)));
	
	OS_NUM_T n = OS_DIV(OS_SUB(a,b),OS_ADD(a,b));
	OS_NUM_T n2 = OS_MUL(n,n);
	OS_NUM_T n3 = OS_MUL(OS_MUL(n,n),n);
	
	OS_NUM_T cosLat = OS_COS(lat);
	OS_NUM_T sinLat = OS_SIN(lat);
	
	// Transverse radius of curvature
	OS_NUM_T nu = OS_MUL(
		a,
		OS_DIV(
			projection.f0,
			OS_SQRT(
				OS_SUB(
					OS_NUM_LIT(1.0),
					OS_MUL(e2, OS_MUL(sinLat,sinLat))
				)
			)
		)
	);
	// Meridional radius of curvature
	OS_NUM_T rho = OS_MUL(
		a,
		OS_MUL(
			projection.f0,
			OS_DIV(
				OS_SUB(OS_NUM_LIT(1.0),e2),
				OS_POW(
					OS_SUB(
						OS_NUM_LIT(1.0),
						OS_MUL(OS_MUL(
							e2,
							sinLat),
							sinLat
						)
					),
					OS_NUM_LIT(1.5)
				)
			)
		)
	);
	OS_NUM_T eta2 = OS_SUB(OS_DIV(nu,rho), OS_NUM_LIT(1.0));
	
	OS_NUM_T Ma = OS_MUL(
		OS_ADD(
			OS_ADD(
				OS_ADD(OS_NUM_LIT(1.0), n),
				OS_MUL(
					OS_NUM_LIT(1.25),
					n2
				)
			),
			OS_MUL(
				OS_NUM_LIT(1.25),
				n3
			)
		),
		OS_SUB(lat,lat0)
	);
	
	OS_NUM_T Mb = OS_MUL(
		OS_MUL(
			OS_ADD(
				OS_ADD(
					OS_MUL(OS_NUM_LIT(3.0),n),
					OS_MUL(OS_MUL(OS_NUM_LIT(3.0),n),n)
				),
				OS_MUL(OS_NUM_LIT(2.625),n3)
			),
			OS_SIN(OS_SUB(lat,lat0))
		),
		OS_COS(OS_ADD(lat,lat0))
	);
	
	OS_NUM_T Mc = OS_MUL(
		OS_MUL(
			OS_ADD(
				OS_MUL(OS_NUM_LIT(1.875),n2),
				OS_MUL(OS_NUM_LIT(1.875),n3)
			),
			OS_SIN(OS_MUL(
				OS_NUM_LIT(2.0),
				OS_SUB(lat,lat0)
			))
		),
		OS_COS(OS_MUL(
			OS_NUM_LIT(2.0),
			OS_ADD(lat,lat0)
		))
	);
	
	OS_NUM_T Md = OS_MUL(
		OS_MUL(
			OS_MUL(OS_NUM_LIT(1.458333333333333333333),n3),
			OS_SIN(OS_MUL(
				OS_NUM_LIT(3.0),
				OS_SUB(lat,lat0)
			))
		),
		OS_COS(OS_MUL(
			OS_NUM_LIT(3.0),
			OS_ADD(lat,lat0)
		))
	);
	
	// Meridional arc
	OS_NUM_T M = OS_MUL(
		OS_MUL(b, projection.f0),
		OS_ADD(
			OS_SUB(Ma, Mb),
			OS_SUB(Mc, Md)
		)
	);
	
	OS_NUM_T cos3lat = OS_MUL(OS_MUL(cosLat,cosLat),cosLat);
	OS_NUM_T cos5lat = OS_MUL(OS_MUL(cos3lat,cosLat),cosLat);
	OS_NUM_T tan2lat = OS_MUL(OS_TAN(lat),OS_TAN(lat));
	OS_NUM_T tan4lat = OS_MUL(tan2lat,tan2lat);
	
	OS_NUM_T I = OS_ADD(M, projection.n0);
	
	OS_NUM_T II = OS_MUL(
		OS_MUL(
			OS_DIV(nu,OS_NUM_LIT(2.0)),
			sinLat
		),
		cosLat
	);
	
	OS_NUM_T III = OS_MUL(
		OS_MUL(
			OS_MUL(
				OS_DIV(nu,OS_NUM_LIT(24.0)),
				sinLat
			),
			cos3lat
		),
		OS_ADD(
			OS_SUB(
				OS_NUM_LIT(5.0),
				tan2lat
			),
			OS_MUL(OS_NUM_LIT(9.0), eta2)
		)
	);
	
	OS_NUM_T IIIA = OS_MUL(
		OS_MUL(
			OS_MUL(
				OS_DIV(nu,OS_NUM_LIT(720.0)),
				sinLat
			),
			cos5lat
		),
		OS_ADD(
			OS_SUB(
				OS_NUM_LIT(61.0),
				OS_MUL(OS_NUM_LIT(58.0),tan2lat)
			),
			tan4lat
		)
	);
	
	OS_NUM_T IV = OS_MUL(nu,cosLat);
	
	OS_NUM_T V = OS_MUL(
		OS_MUL(
			OS_DIV(nu,OS_NUM_LIT(6.0)),
			cos3lat
		),
		OS_SUB(
			OS_DIV(nu,rho),
			tan2lat
		)
	);
	
	OS_NUM_T VI = OS_MUL(
		OS_MUL(
			OS_DIV(nu,OS_NUM_LIT(120.0)),
			cos5lat
		),
		OS_ADD(
			OS_ADD(
				OS_SUB(
					OS_NUM_LIT(5.0),
					OS_MUL(OS_NUM_LIT(18.0),tan2lat)
				),
				tan4lat
			),
			OS_SUB(
				OS_MUL(OS_NUM_LIT(14.0),eta2),
				OS_MUL(OS_MUL(OS_NUM_LIT(58.0),tan2lat),eta2)
			)
		)
	);
	
	OS_NUM_T dLon = OS_SUB(lon,lon0);
	OS_NUM_T dLon2 = OS_MUL(dLon,dLon);
	OS_NUM_T dLon3 = OS_MUL(dLon2,dLon);
	OS_NUM_T dLon4 = OS_MUL(dLon3,dLon);
	OS_NUM_T dLon5 = OS_MUL(dLon4,dLon);
	OS_NUM_T dLon6 = OS_MUL(dLon5,dLon);
	
	os_eas_nor_t eas_nor;
	eas_nor.n = OS_ADD(OS_ADD(OS_ADD(
		I,
		OS_MUL(II,dLon2)),
		OS_MUL(III,dLon4)),
		OS_MUL(IIIA,dLon6)
	);
	eas_nor.e = OS_ADD(OS_ADD(OS_ADD(
		projection.e0,
		OS_MUL(IV,dLon)),
		OS_MUL(V,dLon3)),
		OS_MUL(VI,dLon5)
	);
	eas_nor.h = point.eh;
	
	return eas_nor;
}


os_lat_lon_t
os_tm_eas_nor_to_lat_lon( os_eas_nor_t       point
                        , os_tm_projection_t projection
                        )
{
	// Convert to radians
	OS_NUM_T lat0 = DEG_2_RAD(projection.lat0);
	OS_NUM_T lon0 = DEG_2_RAD(projection.lon0);
	
	// Shorter-named alias
	OS_NUM_T a = projection.ellipsoid.a;
	OS_NUM_T b = projection.ellipsoid.b;
	
	// Eccentricity squared
	OS_NUM_T e2 = OS_SUB(OS_NUM_LIT(1.0) , OS_DIV(OS_MUL(b,b),OS_MUL(a,a)));
	OS_NUM_T n = OS_DIV(OS_SUB(a,b),OS_ADD(a,b));
	OS_NUM_T n2 = OS_MUL(n,n);
	OS_NUM_T n3 = OS_MUL(OS_MUL(n,n),n);
	
	OS_NUM_T lat=lat0;
	OS_NUM_T M=OS_NUM_LIT(0.0);
	
	do {
		lat = OS_ADD(
			OS_DIV(
				OS_SUB(
					OS_SUB(
						point.n,
						projection.n0
					),
					M
				),
				OS_MUL(a,projection.f0)
			),
			lat
		);
	
	  OS_NUM_T Ma = OS_MUL(
			OS_ADD(OS_ADD(OS_ADD(
				OS_NUM_LIT(1.0),
				n),
				OS_MUL(OS_NUM_LIT(1.25),n2)),
				OS_MUL(OS_NUM_LIT(1.25),n3)
			),
			OS_SUB(lat,lat0)
		);
	  
	  OS_NUM_T Mb = OS_MUL(OS_MUL(
			OS_ADD(OS_ADD(
				OS_MUL(OS_NUM_LIT(3.0),n),
				OS_MUL(OS_MUL(OS_NUM_LIT(3.0),n),n)),
				OS_MUL(OS_NUM_LIT(2.625),n3)
			),
			OS_SIN(OS_SUB(lat,lat0))),
			OS_COS(OS_ADD(lat,lat0))
		);
	  
	  OS_NUM_T Mc = OS_MUL(
			OS_MUL(
				OS_ADD(
					OS_MUL(OS_NUM_LIT(1.875),n2),
					OS_MUL(OS_NUM_LIT(1.875),n3)
				),
				OS_SIN(OS_MUL(
					OS_NUM_LIT(2.0),
					OS_SUB(lat,lat0)
				))
			),
			OS_COS(OS_MUL(
				OS_NUM_LIT(2.0),
				OS_ADD(lat,lat0)
			))
		);
		
		OS_NUM_T Md = OS_MUL(
			OS_MUL(
				OS_MUL(OS_NUM_LIT(1.458333333333333333333),n3),
				OS_SIN(OS_MUL(
					OS_NUM_LIT(3.0),
					OS_SUB(lat,lat0)
				))
			),
			OS_COS(OS_MUL(
				OS_NUM_LIT(3.0),
				OS_ADD(lat,lat0)
			))
		);
		
		// Meridional arc
		M = OS_MUL(OS_MUL(
			b,
			projection.f0),
			OS_ADD(
				OS_SUB(Ma, Mb),
				OS_SUB(Mc , Md)
			)
		);
		
	} while (
		OS_CMP(
			OS_SUB(OS_SUB(
				point.n,
				projection.n0),
				M
			),
			OS_EAS_NOR_TO_LAT_LON_PRECISION
		)
		>= 0
	);
	
	OS_NUM_T cosLat = OS_COS(lat);
	OS_NUM_T sinLat = OS_SIN(lat);
	
	// Transverse radius of curvature
	OS_NUM_T nu = OS_MUL(
		a,OS_DIV(
			projection.f0,
			OS_SQRT(OS_SUB(
				OS_NUM_LIT(1.0),
				OS_MUL(OS_MUL(e2,sinLat),sinLat)
			))
		)
	);
	
	// Meridional radius of curvature
	OS_NUM_T rho = OS_MUL(
		OS_MUL(a,projection.f0),
		OS_DIV(
			OS_SUB(
				OS_NUM_LIT(1.0),
				e2
			),
			OS_POW(
				OS_SUB(
					OS_NUM_LIT(1.0),
					OS_MUL(OS_MUL(e2,sinLat),sinLat)
				),
				OS_NUM_LIT(1.5)
			)
		)
	);
	
	OS_NUM_T eta2 = OS_SUB(OS_DIV(nu,rho) , OS_NUM_LIT(1.0));
	OS_NUM_T tanLat = OS_TAN(lat);
	OS_NUM_T tan2lat = OS_MUL(tanLat,tanLat);
	OS_NUM_T tan4lat = OS_MUL(tan2lat,tan2lat);
	OS_NUM_T tan6lat = OS_MUL(tan4lat,tan2lat);
	OS_NUM_T secLat = OS_DIV(OS_NUM_LIT(1.0),cosLat);
	OS_NUM_T nu3 = OS_MUL(OS_MUL(nu,nu),nu);
	OS_NUM_T nu5 = OS_MUL(OS_MUL(nu3,nu),nu);
	OS_NUM_T nu7 = OS_MUL(OS_MUL(nu5,nu),nu);
	OS_NUM_T VII = OS_DIV(tanLat,OS_MUL(OS_MUL(OS_NUM_LIT(2.0),rho),nu));
	
	OS_NUM_T VIII = OS_MUL(
		OS_DIV(
			tanLat,
			OS_MUL(OS_MUL(OS_NUM_LIT(24.0),rho),nu3)
		),
		OS_SUB(
			OS_ADD(
				OS_ADD(
					OS_NUM_LIT(5.0),
					OS_MUL(OS_NUM_LIT(3.0),tan2lat)
				),
				eta2
			),
			OS_MUL(OS_MUL(OS_NUM_LIT(9.0),tan2lat),eta2)
		)
	);
	
	OS_NUM_T IX = OS_MUL(
		OS_DIV(
			tanLat,
			OS_MUL(OS_MUL(OS_NUM_LIT(720.0),rho),nu5)
		),
		OS_ADD(OS_ADD(
			OS_NUM_LIT(61.0),
			OS_MUL(OS_NUM_LIT(90.0),tan2lat)),
			OS_MUL(OS_NUM_LIT(45.0),tan4lat)
		)
	);
	OS_NUM_T X = OS_DIV(secLat,nu);
	OS_NUM_T XI = OS_MUL(
		OS_DIV(
			secLat,
			OS_MUL(OS_NUM_LIT(6.0),nu3)
		),
		OS_ADD(
			OS_DIV(nu,rho),
			OS_MUL(OS_NUM_LIT(2.0),tan2lat)
		)
	);
	OS_NUM_T XII = OS_MUL(
		OS_DIV(
			secLat,
			OS_MUL(OS_NUM_LIT(120.0),nu5)
		),
		OS_ADD(OS_ADD(
			OS_NUM_LIT(5.0),
			OS_MUL(OS_NUM_LIT(28.0),tan2lat)),
			OS_MUL(OS_NUM_LIT(24.0),tan4lat)
		)
	);
	
	OS_NUM_T XIIA = OS_MUL(
		OS_DIV(
			secLat,
			OS_MUL(OS_NUM_LIT(5040.0),nu7)
		),
		OS_ADD(OS_ADD(OS_ADD(
			OS_NUM_LIT(61.0),
			OS_MUL(OS_NUM_LIT(662.0),tan2lat)),
			OS_MUL(OS_NUM_LIT(1320.0),tan4lat)),
			OS_MUL(OS_NUM_LIT(720.0),tan6lat)
		)
	);
	
	OS_NUM_T dE = OS_SUB(point.e,projection.e0);
	OS_NUM_T dE2 = OS_MUL(dE,dE);
	OS_NUM_T dE3 = OS_MUL(dE2,dE);
	OS_NUM_T dE4 = OS_MUL(dE2,dE2);
	OS_NUM_T dE5 = OS_MUL(dE3,dE2);
	OS_NUM_T dE6 = OS_MUL(dE4,dE2);
	OS_NUM_T dE7 = OS_MUL(dE5,dE2);
	
	os_lat_lon_t lat_lon;
	
	lat_lon.lat = OS_SUB(OS_ADD(OS_SUB(
		lat,
		OS_MUL(VII,dE2)),
		OS_MUL(VIII,dE4)),
		OS_MUL(IX,dE6)
	);
	lat_lon.lon = OS_SUB(OS_ADD(OS_SUB(OS_ADD(
		lon0,
		OS_MUL(X,dE)),
		OS_MUL(XI,dE3)),
		OS_MUL(XII,dE5)),
		OS_MUL(XIIA,dE7)
	);
	
	lat_lon.eh = point.h;
	
	return lat_lon;
}



#include "os_coord_data.h"
#include "os_coord_ordinance_survey.h"

int
main(int argc, char *argv[])
{
	// Quick tests
	os_lat_lon_t ll = {.lat=DEG_2_RAD(OS_NUM_LIT(52.65757)), .lon=DEG_2_RAD(OS_NUM_LIT(1.7179216)), .eh=OS_NUM_LIT(24.7)};
	
	os_cartesian_t c  = os_lat_lon_to_cartesian(ll, OS_EL_AIRY_1830);
	os_cartesian_t c2 = os_helmert_transform(c,OS_HE_WGS84_TO_OSGB36);
	os_cartesian_t c3 = os_helmert_transform(c2,os_helmert_invert(OS_HE_WGS84_TO_OSGB36));
	os_lat_lon_t ll2 = os_cartesian_to_lat_lon(c3, OS_EL_AIRY_1830);
	
	printf("Lat/lon/eh before and after ll->cart->helmert->inv-helmert->ll\n");
	
	printf("Lat %f - %f = %0.10f\n", ll.lat, ll2.lat, ll.lat - ll2.lat);
	printf("Lon %f - %f = %0.10f\n", ll.lon, ll2.lon, ll.lon - ll2.lon);
	printf("Eh  %f - %f = %0.10f\n", ll.eh , ll2.eh , ll.eh  - ll2.eh);
	
	os_lat_lon_t   home_ll_wgs84   = {.lat=DEG_2_RAD(OS_NUM_LIT(53.467097)), .lon=DEG_2_RAD(OS_NUM_LIT(-2.220490)), .eh=OS_NUM_LIT(24.7)};
	os_cartesian_t home_c_wgs84    = os_lat_lon_to_cartesian(home_ll_wgs84, OS_EL_WGS84);
	os_cartesian_t home_c_airy30   = os_helmert_transform(home_c_wgs84, OS_HE_WGS84_TO_OSGB36);
	os_lat_lon_t   home_ll_airy30  = os_cartesian_to_lat_lon(home_c_airy30, OS_EL_AIRY_1830);
	os_eas_nor_t   home_en_airy30  = os_lat_lon_to_tm_eas_nor(home_ll_airy30, OS_TM_NATIONAL_GRID);
	os_lat_lon_t   home_ll_airy30_ = os_tm_eas_nor_to_lat_lon(home_en_airy30, OS_TM_NATIONAL_GRID);
	os_grid_ref_t  home_grid_ref   = os_eas_nor_to_grid_ref(home_en_airy30, OS_GR_NATIONAL_GRID);
	os_eas_nor_t   home_en_airy30_ = os_grid_ref_to_eas_nor(home_grid_ref, OS_GR_NATIONAL_GRID);
	
	printf("\n");
	printf("OS Eastings/Northings Height\n");
	printf("Eas %f\n", home_en_airy30.e);
	printf("Nor %f\n", home_en_airy30.n);
	printf("Hei %f\n", home_en_airy30.h);
	
	printf("\n");
	printf("Lat/lon/eh before and after ll->eastings/northings->ll\n");
	printf("Lat %f - %f = %0.10f\n", home_ll_airy30.lat, home_ll_airy30_.lat, home_ll_airy30.lat - home_ll_airy30_.lat);
	printf("Lon %f - %f = %0.10f\n", home_ll_airy30.lon, home_ll_airy30_.lon, home_ll_airy30.lon - home_ll_airy30_.lon);
	printf("Eh  %f - %f = %0.10f\n", home_ll_airy30.eh,  home_ll_airy30_.eh, home_ll_airy30.eh   - home_ll_airy30_.eh);
	
	printf("\n");
	printf("National Grid Coordinates\n");
	printf("%s %05.0f %05.0f (%0.1fm)\n", home_grid_ref.code, home_grid_ref.e, home_grid_ref.n, home_grid_ref.h);
	printf("Lossless conversion back: %s\n",
		(home_en_airy30_.e == home_en_airy30.e &&
		home_en_airy30_.n == home_en_airy30.n &&
		home_en_airy30_.h == home_en_airy30.h) ? "True" : "False"
	);
	
	return 0;
}
