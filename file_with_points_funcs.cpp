#include <TXLib.h>

#include "file_with_points_funcs.h"

const char* points_file_name = "points.txt";

void FileWithPointsInit()
{
    FILE* points_file = fopen(points_file_name, "w");
    assert(points_file);
    fclose(points_file);
}

void AddPointInFileWithPoints(double x, double y)
{
    FILE* points_file = fopen(points_file_name, "a");
    assert(points_file);

    fprintf(points_file, "%lf %lf\n", x, y);

    fclose(points_file);
}
