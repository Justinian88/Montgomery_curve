#include <stdio.h>
#include <stdlib.h>
#include "tommath.h"
#include "calculus.h"

int main()
{
    struct m_point point, neutral, pp, neg;
    m_point_init(&point);
    m_point_init(&pp);
    m_point_init(&neg);
    m_point_init(&neutral);
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
    m_point_custom(&neutral, "0", "1", "0");
    binaryMethod(&pp, &point, &curve, &curve.q);
    if (point_cmp(&pp, &neutral)) {
            printf("Test passed\n");
        }
        else{
            printf("Test not passed\n");
        }

    /* Test 3 */

    printf("\nTest №3.1\n");
    printf("Condition: (q+1)*P = P\n");
    int res;
    mp_int temp;
    res = mp_init(&temp);
    mp_set_i32(&temp, 1);
    res = mp_add(&temp, &curve.q, &temp);
    binaryMethod(&pp, &point, &curve, &temp); //point = (q+1) * P
    if (point_cmp(&pp, &point)) {
        printf("Test passed\n");
    }
    else{
        printf("Test not passed\n");
    }
    printf("\nTest №3.2\n");
    printf("Condition: (q-1)*P = -P\n");

    point_negative(&neg, &point); // -P
    mp_set_i32(&temp, 1);
    res = mp_sub(&curve.q, &temp, &temp);
    binaryMethod(&pp, &point, &curve, &temp); //point = (q-1) * P
    if (point_cmp(&pp, &neg)) {
        printf("Test passed\n");
    }
    else{
        printf("Test not passed\n");
    }
    /*
    printf("Test passed\n");
    */

    /* Test 4 */
    printf("\nTest №4\n");
    distributivity(&point, &curve);

    /* Clear */
    m_point_clear(&point);
    m_curve_clear(&curve);
    /* THE END */
}


