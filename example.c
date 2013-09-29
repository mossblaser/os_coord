/**
 * An example program which converts WGS-84 (i.e. GPS) coordinates to an
 * Ordnance Survey grid references.
 *
 * Compilation:
 *   gcc -std=c99 -lm *.c
 *
 * Usage:
 *   ./a.out [lat] [lon] [height]
 *
 * Example:
 *   $ ./a.out 52.65757 1.7179216 24.7
 *   TG 51539 13138 (Altitude: -20.0m)
 */

#include <stdio.h>
#include <stdlib.h>

#include "os_coord.h"
#include "os_coord_math.h"
#include "os_coord_data.h"
#include "os_coord_transform.h"
#include "os_coord_ordinance_survey.h"

int
main(int argc, char *argv[])
{
	if (argc != 4) {
		fprintf(stderr, "%s: Usage %s [latitude] [longitude] [ellipsoidal height]\n"
		              , argv[0]
		              , argv[0]
		              );
		return -1;
	}
	
	// Parse arguments
	double gps_latitude           = strtod(argv[1], NULL);
	double gps_longitude          = strtod(argv[2], NULL);
	double gps_ellipsoidal_height = strtod(argv[3], NULL);
	
	// Convert to radians
	os_lat_lon_t   home_ll_wgs84 = { .lat=DEG_2_RAD(gps_latitude)
	                               , .lon=DEG_2_RAD(gps_longitude)
	                               , .eh=gps_ellipsoidal_height
	                               };
	
	// Convert from the WGS-84 ellipsoid to 3D Cartesian coordinates
	os_cartesian_t home_c_wgs84 = os_lat_lon_to_cartesian(home_ll_wgs84, OS_EL_WGS84);
	
	// Perform the Helmert transform to (approximately) correct for differences in
	// datum
	os_cartesian_t home_c_airy30 = os_helmert_transform(home_c_wgs84, OS_HE_WGS84_TO_OSGB36);
	
	// Convert back to latitude, longitude and ellipsoidal height to give
	// coordinates on the Airy 1830 ellipsoid.
	os_lat_lon_t home_ll_airy30 = os_cartesian_to_lat_lon(home_c_airy30, OS_EL_AIRY_1830);
	
	// Project the coordinates to the National Grid's 2D projection
	os_eas_nor_t home_en_airy30 = os_lat_lon_to_tm_eas_nor(home_ll_airy30, OS_TM_NATIONAL_GRID);
	
	// Turn these Eastings and Northings into a grid references
	os_grid_ref_t home_grid_ref = os_eas_nor_to_grid_ref(home_en_airy30, OS_GR_NATIONAL_GRID);
	
	// Print the result
	if (home_grid_ref.code[0] != '\0') {
		printf("%s %05.0f %05.0f (Altitude: %0.1fm)\n", home_grid_ref.code, home_grid_ref.e, home_grid_ref.n, home_grid_ref.h);
		return 0;
	} else {
		fprintf(stderr, "%s: Coordinate not covered by National Grid\n", argv[0]);
		return -1;
	}
}
