#ifndef _CONV_H_
#define _CONV_H_

#include <math.h>
#include <Eigen/Dense>

namespace Converter {
    static double EccentricitySquared = 6.69437999014 * 0.001;
    static double SemimajorAxis = 6378137;

    class Conv {
        public:
        Conv() { isRef = false; }

        ~Conv() {}

        void initialize(const double latitude, const double longitude, const double altitude) {
            initial_latitude_ = deg2Rad(latitude);
            initial_longitude_ = deg2Rad(longitude);
            initial_altitude_ = altitude;

            geodetic2Ecef(latitude, longitude, altitude, &initial_ecef_x_, &initial_ecef_y_, &initial_ecef_z_);

            double phiP = atan2(initial_ecef_z_, sqrt(pow(initial_ecef_x_, 2) + pow(initial_ecef_y_, 2)));

            ecef_to_ned_matrix_ = nRe(phiP, initial_longitude_);
            ned_to_ecef_matrix_ = nRe(initial_latitude_, initial_longitude_).transpose();

            isRef = true;
        }

        void geodetic2Ecef(const double latitude, const double longitude, const double altitude,
                            double *x, double *y, double *z) {
            double lati_rad = deg2Rad(latitude);
            double long_rad = deg2Rad(longitude);
            double xi = sqrt(1 - EccentricitySquared * sin(lati_rad) * sin(lati_rad));

            *x = (SemimajorAxis / xi + altitude) * cos(lati_rad) * cos(long_rad);
            *y = (SemimajorAxis / xi + altitude) * cos(lati_rad) * sin(long_rad);
            *z = (SemimajorAxis / xi * (1 - EccentricitySquared) + altitude) * sin(lati_rad);
        }

        void ecef2Ned(const double x, const double y, const double z,
                        double *north, double *east, double *down) {
            Eigen::Vector3d vect, ret;


            if(isRef == true) {
                vect(0) = x - initial_ecef_x_;
                vect(1) = y - initial_ecef_y_;
                vect(2) = z - initial_ecef_z_;

                ret = ecef_to_ned_matrix_ * vect;

                *north = ret(0);
                *east = ret(1);
                *down = -ret(2);
            } else {
                std::cout<<"Not Found References.\n";
            }
        }

        void geodetic2Ned(const double latitude, const double longitude, const double altitude,
                            double *north, double *east, double *down) {
            double x, y, z;

            geodetic2Ecef(latitude, longitude, altitude, &x, &y, &z);
            ecef2Ned(x, y, z, north, east, down);
        }

        private:
        inline Eigen::Matrix3d nRe(const double lati_radians, const double long_radians) {
            const double sLati = sin(lati_radians);
            const double sLong = sin(long_radians);
            const double cLati = cos(lati_radians);
            const double cLong = cos(long_radians);

            Eigen::Matrix3d ret;

            ret(0, 0) = -sLati * cLong;
            ret(0, 1) = -sLati * sLong;
            ret(0, 2) = cLati;
            ret(1, 0) = -sLong;
            ret(1, 1) = cLong;
            ret(1, 2) = 0.0;
            ret(2, 0) = cLati * cLong;
            ret(2, 1) = cLati * sLong;
            ret(2, 2) = sLati;

            return ret;
        }

        inline double deg2Rad(const double degrees) {
            return (degrees / 180.0) * M_PI;
        }

        double initial_latitude_;
        double initial_longitude_;
        double initial_altitude_;

        double initial_ecef_x_;
        double initial_ecef_y_;
        double initial_ecef_z_;

        Eigen::Matrix3d ecef_to_ned_matrix_;
        Eigen::Matrix3d ned_to_ecef_matrix_;

        bool isRef;
    };
};

#endif
