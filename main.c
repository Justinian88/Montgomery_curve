#include <stdio.h>
#include <stdlib.h>
#include "tommath.h"
#include "pointCurveStructures.h"

int main()
{
    struct m_point point;
    m_point_init(&point);
    //m_point_custom(&neutral, "1", "0", "1");
    struct m_curve curve;
    m_curve_init(&curve);
    m_curve_create(&curve);
    default_point(&point, &curve);
    printf("Default point coordinates:");
    print_coordinates(&point);

    /* Test 1 */
    printf("\n\nTest №1\n");
    point_on_curve(&curve, &point);

    /* Test 2 */
    printf("\nTest №2\n");
    printf("Condition: q*P = 0\n");
    /*
    printf("Test passed\n");
    */

    /* Test 3.1 */
    printf("\nTest №3.1\n");
    printf("Condition: (q+1)*P = P\n");
    /*
    printf("Test passed\n");
    */

    /* Test 3.2 */
    printf("\nTest №3.2\n");
    printf("Condition: (q-1)*P = -P\n");
    /*
    printf("Test passed\n");
    */


    printf("\nTest №4\n");
    int res;
    mp_int k1, k2, k3;
    char stroka[50] = "";
    res = mp_init_multi(&k1, &k2, &k3, NULL);

    res = mp_rand(&k1, 5);
    res = mp_to_radix(&k1, stroka, 50, NULL, 10);
    printf("random k1 = %s", stroka);

    res = mp_rand(&k2, 5);
    res = mp_to_radix(&k2, stroka, 50, NULL, 10);
    printf("\nrandom k2 = %s", stroka);

    res = mp_add(&k1, &k2, &k3);
    res = mp_to_radix(&k3, stroka, 50, NULL, 10);
    printf("\nk1 + k2   = %s", stroka);
    printf("\nCondition: k1*P + k2*P = (k1+k2)*P\n");

    /*
    printf("Test passed\n\n");
    */
}


