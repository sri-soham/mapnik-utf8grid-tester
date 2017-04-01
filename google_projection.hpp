#ifndef GOOGLE_PROJECTION_H
#define GOOGLE_PROJECTION_H

#include "lat_lng.hpp"
class GoogleProjection {
    public:
        static const int SIZE = 18;
        GoogleProjection(void);
        LatLng fromLLToPixel(LatLng ll, int zoom);
        LatLng fromPixelToLL(LatLng px, int zoom);

    private:
        double m_Bc[SIZE];
        double m_Cc[SIZE];
        LatLng m_zc[SIZE];
        double m_Ac[SIZE];
        double minMax(double a, double b, double c);
        double degreesToRadians(double);
        double radiansToDegrees(double);
};

#endif
