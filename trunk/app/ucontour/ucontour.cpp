#include <windows.h>
#include <tchar.h>

typedef struct tagXYZ
{
    double x;
    double y;
    double z;
} XYZ;

int sign(double pa)
{
    if (pa > 0)
        return 1;
    else
        return -1;
}

/*-------------------------------------------------------------------------
   Create a contour slice through a 3 vertex facet pa,pb,pc
   The contour "level" is a horizontal plane perpendicular to the z axis,
        ie: The equation of the contour plane Ax + By + Cz + D = 0
                 has A = 0, B = 0, C = 1, D = -level
   Return
         0 if the contour plane doesn't cut the facet
         2 if it does cut the facet
        -1 for an unexpected occurrence
   If a vertex touches the contour plane nothing need to be drawn!?
*/
int ContourFace(XYZ pa, XYZ pb, XYZ pc, double level, XYZ *p1, XYZ *p2)
{
    double sidea,sideb,sidec;

    /*
      Evaluate the equation of the plane for each vertex
        sidea = A * pa.x + B * pa.y + C * pa.z + D;
        sideb = A * pb.x + B * pb.y + C * pb.z + D;
        sidec = A * pc.x + B * pc.y + C * pc.z + D;
    */
    sidea = pa.z - level;
    sideb = pb.z - level;
    sidec = pc.z - level;

    /* Are all the vertices on one side */
    if (sidea >= 0 && sideb >= 0 && sidec >= 0)
        return 0;
    if (sidea <= 0 && sideb <= 0 && sidec <= 0)
        return 0;

    /* Is p0 the only point on a side by itself */
    if ((sign(sidea) != sign(sideb)) && (sign(sidea) != sign(sidec)))
    {
        p1->x = pa.x - sidea * (pc.x - pa.x) / (sidec - sidea);
        p1->y = pa.y - sidea * (pc.y - pa.y) / (sidec - sidea);
        p1->z = pa.z - sidea * (pc.z - pa.z) / (sidec - sidea);
        p2->x = pa.x - sidea * (pb.x - pa.x) / (sideb - sidea);
        p2->y = pa.y - sidea * (pb.y - pa.y) / (sideb - sidea);
        p2->z = pa.z - sidea * (pb.z - pa.z) / (sideb - sidea);
        return 2;
    }

    /* Is p1 the only point on a side by itself */
    if ((sign(sideb) != sign(sidea)) && (sign(sideb) != sign(sidec)))
    {
        p1->x = pb.x - sideb * (pc.x - pb.x) / (sidec - sideb);
        p1->y = pb.y - sideb * (pc.y - pb.y) / (sidec - sideb);
        p1->z = pb.z - sideb * (pc.z - pb.z) / (sidec - sideb);
        p2->x = pb.x - sideb * (pa.x - pb.x) / (sidea - sideb);
        p2->y = pb.y - sideb * (pa.y - pb.y) / (sidea - sideb);
        p2->z = pb.z - sideb * (pa.z - pb.z) / (sidea - sideb);
        return 2;
    }

    /* Is p2 the only point on a side by itself */
    if ((sign(sidec) != sign(sidea)) && (sign(sidec) != sign(sideb)))
    {
        p1->x = pc.x - sidec * (pa.x - pc.x) / (sidea - sidec);
        p1->y = pc.y - sidec * (pa.y - pc.y) / (sidea - sidec);
        p1->z = pc.z - sidec * (pa.z - pc.z) / (sidea - sidec);
        p2->x = pc.x - sidec * (pb.x - pc.x) / (sideb - sidec);
        p2->y = pc.y - sidec * (pb.y - pc.y) / (sideb - sidec);
        p2->z = pc.z - sidec * (pb.z - pc.z) / (sideb - sidec);
        return 2;
    }

    /* Shouldn't get here */
    return -1;
}

