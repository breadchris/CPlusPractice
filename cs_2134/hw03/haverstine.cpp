//This code is from Prof. Hellerstein and Prof. Frankl
#include <iostream>
#include <math.h>
#include <vector>
#include <functional>


double degrad(double d) {
   return d * M_PI / 180;
}


// Code below is adapted from http://www.movable-type.co.uk/scripts/latlong.html
// FYI: That website has an applet that computes the haversine distance.
// It also has a link that will show the locations on a map,
// with a line between them.
double haverdist(double lat1, double longit1, double lat2, double longit2)
{
     double r = 6371;
     double dlat = degrad(lat2 - lat1);
     double dlongit = degrad(longit2 - longit1);
     double a = sin(dlat/2)*sin(dlat/2)+
        cos(degrad(lat1))*cos(degrad(lat2))*sin(dlongit/2)*sin(dlongit/2);
     double c = 2*atan2(sqrt(a),sqrt(1-a));
     return r*c;
}
