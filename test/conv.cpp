#include <iostream>
#include "conv.hpp"

using namespace std;

int main() {
    double latitude=44.544, longitude=-72.814, altitude=1340;
    double north, east, down;

    Converter::Conv conv;

    conv.initialize(44.532, -72.782, 1699);

    conv.geodetic2Ned(latitude, longitude, altitude, &north, &east, &down);

    cout<<north<<' '<<east<<' '<<down<<'\n';

	return 0;
}
