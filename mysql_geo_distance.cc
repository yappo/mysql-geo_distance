#include <my_global.h>
extern "C" {
#include <my_sys.h>
#include <mysql.h>
#include <math.h>
}

#include <string>

extern "C" {
my_bool geo_distance_init(UDF_INIT* initid, UDF_ARGS* args, char* message);
void geo_distance_deinit(UDF_INIT* initid);
double geo_distance(UDF_INIT* initid, UDF_ARGS* args, char* result, unsigned long* length, char* is_null, char* error);
}


my_bool geo_distance_init(UDF_INIT* initid, UDF_ARGS* args, char* message)
{
  if (args->arg_count != 4)
  {
    strncpy(message, "geo_distance: required four argument", MYSQL_ERRMSG_SIZE);
    return 1;
  }
  args->arg_type[0] = REAL_RESULT;
  args->arg_type[1] = REAL_RESULT;
  args->arg_type[2] = REAL_RESULT;
  args->arg_type[3] = REAL_RESULT;
  return 0;
}


void geo_distance_deinit(UDF_INIT* initid)
{
}


// hubeny
double geo_distance(UDF_INIT* initid, UDF_ARGS* args, char* result, unsigned long* length, char* is_null, char* error)
{
  double lat1, lng1, lat2, lng2;

  lat1 = *((double *) args->args[0]) * (M_PI / 180);
  lng1 = *((double *) args->args[1]) * (M_PI / 180);
  lat2 = *((double *) args->args[2]) * (M_PI / 180);
  lng2 = *((double *) args->args[3]) * (M_PI / 180);

  double P = (lat1 + lat2) / 2;
  double dP = lat1 - lat2;
  if (dP < 0)
    dP = dP * -1;
  double dR = lng1 - lng2;
  if (dR < 0)
    dR = dR * -1;
  double _M = (1 - 0.006674 * sin(P) * sin(P));
  double M = 6334834 / sqrt(_M * _M * _M);
  double N = 6377397 / sqrt(_M);

  double distance = sqrt((M *dP) * (M * dP) + (N * cos(P) * dR) * (N * cos(P) * dR));
  return distance;
}
