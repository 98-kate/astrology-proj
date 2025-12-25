#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define PI 3.14159265358979323846
#define DEGRAD (PI/180.0)
#define RADEG (180.0/PI)
#define sind(x) sin((x) * DEGRAD)
#define cosd(x) cos((x) * DEGRAD)
#define tand(x) tan((x) * DEGRAD)
#define asind(x) (RADEG * asin(x))
#define acosd(x) (RADEG * acos(x))
#define atand(x) (RADEG * atan(x))
#define atan2d(y,x) (RADEG * atan2((y), (x)))

double julianDate(int y, int m, int d, int hour, int min) {
	/** This formula is only valid from Mar 1900 to Feb 2100:
		 d = 367*y - 7 * (y + (m+9)/12)/4 + 275*m/9 + D - 730530 **/
	
	int timescale = 367*y - 7 * (y + (m+9)/12)/4 - 3 * ((y + (m-9)/7)/100 + 1)/4 + 275 * m/9 + d - 730515;	
	double UT = (hour + min/60.0)/24.0;

	timescale += UT;
	return timescale;
}

/** Calculates the ecliptic longitude of the Sun to determine user's Sun sign **/
double sunPosition(double timescale) {
   
	// longitude of perihelion - point where the orbit crosses the ecliptic plane
	double w = 282.9404 + 4.70935E-5 * timescale;
   // eccentricity - defines how elliptical the Earth's orbit is
	double e = 0.016709 - 1.151E-9 * timescale;
   // mean anomaly - angular distance of the Earth along its orbit relative to perihelion
	double M = 356.0470 + 0.9856002585 * timescale;
   // M can go beyond 360 deg. so we normalize it to fall within range
	M = fmod(M, 360.0);

	double L = w + M; // calculate the sun's mean longitude (approx. position of Sun in orbit)
	// eccentric anomaly - angular parameter that describes position of the Earth in its orbit
	double E = M + (180.0/PI) * e * sin(M * DEGRAD) * (1 + e * cos(M * DEGRAD));
	double x = cos(E * DEGRAD) - e;					 // Earth-Sun distance towards perihelion
	double y = sin(E * DEGRAD) * sqrt(1 - e * e); // Earth-Sun distance perpendicular to perihelion
	double r = sqrt(x * x + y * y);
	double v = atan2(y,x) * RADEG;	// calc. true anomaly, Sun-Earth distance at a specific time
	
	double longitude = v + w;		   // final ecliptic longitude of the Sun
	longitude = fmod(longitude, 360.0);
	return longitude;
}

const char * sunSign(double longitude) {
	int signIndex = (int)(longitude/30.0);
	const char * signs[] = { "Aries", "Taurus", "Gemini", "Cancer", "Leo", "Virgo", "Libra", 
									 "Scorpio", "Sagittarius", "Capricorn", "Aquarius", "Pisces"    };
	return signs[signIndex];
}

int main(int argc, char * argv[]) {
	
	if (argc != 6) {
		printf("Please enter your birthdate in the format: MM/DD/YYYY HH:MM\n");
		return 1;
	}

	int month = atoi(argv[1]), day = atoi(argv[2]), year = atoi(argv[3]);
	int hour = atoi(argv[4]), minute = atoi(argv[5]);

	double jd = julianDate(year, month, day, hour, minute), sunLong = sunPosition(jd);
	const char * sun_sign = sunSign(sunLong);
	
	printf("Sun sign: %s\n", sun_sign);
	printf("Sun longitude: %.2f\n", sunLong);
	return 0;
}

