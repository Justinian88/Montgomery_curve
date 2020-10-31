#include "tommath.h"
#include "calculus.h"
#include "parametrs.h"

/* Point */

void m_point_init(struct m_point *point){
    int res;
    res = mp_init(&point->X);
    res = mp_init(&point->Y);
    res = mp_init(&point->Z);
}
void m_point_clear(struct m_point *point){
    mp_clear_multi(&point->X, &point->Y, &point->Z, NULL);
}
void m_point_custom(struct m_point *point, char *X, char *Y, char *Z)
{
    int res;
    res = mp_read_radix(&point->X, X, 10);
    res = mp_read_radix(&point->Y, Y, 10);
    res = mp_read_radix(&point->Z, Z,10);
}

/* Curve */
void m_curve_init(struct m_curve *curve){
    int res;
    res = mp_init(&curve->X);
    res = mp_init(&curve->Y);
    res = mp_init(&curve->Z);
    res = mp_init(&curve->a);
    res = mp_init(&curve->b);
    res = mp_init(&curve->p);
    res = mp_init(&curve->q);
    res = mp_init(&curve->v);
    res = mp_init(&curve->u);
    res = mp_init(&curve->e);
    res = mp_init(&curve->d);
}
void m_curve_clear(struct m_curve *curve){
    mp_clear_multi(&curve->v, &curve->u, &curve->e, &curve->d, &curve->X, &curve->Y, &curve->Z, &curve->a, &curve->b, &curve->p, &curve->q, NULL);
}
void m_curve_create (struct m_curve *curve){
    int res;
    //printf("Object initialized");
    mp_int one, two, four;
    res = mp_init_multi(&one, &two, &four, NULL);
    mp_set(&one, 1);
    mp_set(&two, 2);
    mp_set(&four, 4);
    res = mp_read_radix(&curve->v, V, 16);
    res = mp_read_radix(&curve->u, U, 16);
    res = mp_read_radix(&curve->e, E, 16);
    res = mp_read_radix(&curve->d, D, 16);
    res = mp_read_radix(&curve->p, P, 16);
    res = mp_read_radix(&curve->q, Q, 16);
    res = mp_copy(&one, &curve->Z);

    res = mp_addmod(&curve->e, &curve->d, &curve->p, &curve->a); // (e+d) mod p -> a
    res = mp_mulmod(&curve->a, &two, &curve->p, &curve->a); // (a*2) mod p -> a
    res = mp_submod(&curve->e, &curve->d, &curve->p, &curve->b); // (e-d) mod p -> b
    res = mp_invmod(&curve->b, &curve->p, &curve->b); // b^(-1) mod p -> b
    res = mp_mulmod(&curve->a, &curve->b, &curve->p, &curve->a); // (a*b) mod p -> a
    res = mp_mulmod(&curve->b, &four, &curve->p, &curve->b); //(b*4) mod p -> b
    res = mp_addmod(&one, &curve->v, &curve->p, &curve->X); // (1+v) mod p -> X
    res = mp_submod(&one, &curve->v, &curve->p, &curve->Y); // (1-v) mod p -> Y
    res = mp_invmod(&curve->Y, &curve->p, &curve->Y); // Y^(-1) mod p -> Y
    res = mp_mulmod(&curve->X, &curve->Y, &curve->p, &curve->X); //(X*Y) mod p -> X
    res = mp_invmod(&curve->u, &curve->p, &curve->Y); // u^(-1) mod p -> Y
    res = mp_mulmod(&curve->X, &curve->Y, &curve->p, &curve->Y); //(X*Y) mod p -> Y
    mp_clear_multi(&one, &two, &four, NULL);
}
void default_point(struct m_point *point, const struct m_curve *curve){
    int res;
    res = mp_copy(&curve->X, &point->X);
    res = mp_copy(&curve->Y, &point->Y);
    res = mp_copy(&curve->Z, &point->Z);
}
void point_negative(struct m_point *point, struct m_point *point2){
    int res;
    res = mp_neg(&point2->X, &point->X);
    res = mp_copy(&point2->Y, &point->Y);
    res = mp_copy(&point2->Z, &point->Z);
}

/* View */
void print_coordinates(const struct m_point *point){
    char string1[60] = "";
    //char string2[60] = "";
    char string3[60] = "";
    int res;
    res = mp_to_radix(&point->X, string1, 60, NULL, 10);
    printf("\nX = %s", string1);
    //res = mp_to_radix(&point->Y, string2, 60, NULL, 10);
    //printf("\nY = %s", string2);
    res = mp_to_radix(&point->Z, string3, 60, NULL, 10);
    printf("\nZ = %s", string3);
}

/* Algorithms */
void xADD (struct m_point *point1, const struct m_point *point2, const struct m_point *default_, const struct m_curve *curve){
    int res;
    mp_int V0, V1, V2, V3, V4;
    res = mp_init_multi(&V0, &V1, &V2, &V3, &V4, NULL);
    res = mp_addmod(&point1->X, &point1->Z, &curve->p, &V0); // (p1.X+p1.Z) mod p -> V0
    res = mp_submod(&point2->X, &point2->Z, &curve->p, &V1); // (p2.X-p2.Z) mod p -> V1
    res = mp_mulmod(&V1, &V0, &curve->p, &V1); // (V1*V0) mod p -> V1
    res = mp_submod(&point1->X, &point1->Z, &curve->p, &V0); // (p1.X-p1.Z) mod p -> V0
    res = mp_addmod(&point2->X, &point2->Z, &curve->p, &V2); // (p2.X+p2.Z) mod p -> V2
    res = mp_mulmod(&V2, &V0, &curve->p, &V2); // (V2*V0) mod p -> V2
    res = mp_addmod(&V1, &V2, &curve->p, &V3); // (V1+V2) mod p -> V3
    res = mp_sqrmod(&V3, &curve->p, &V3); // (V1)^2 mod p -> V1
    res = mp_submod(&V1, &V2, &curve->p, &V4); // (V1-V2) mod p -> V4
    res = mp_sqrmod(&V4, &curve->p, &V4); // (V4)^2 mod p -> V4
    res = mp_mulmod(&default_->Z, &V3, &curve->p, &point1->X); // (V2*V0) mod p -> X_add
    res = mp_mulmod(&default_->X, &V4, &curve->p, &point1->Z); // (V2*V0) mod p -> Z_add
    mp_clear_multi(&V0, &V1, &V2, &V3, &V4, NULL);
}
void xDBL(struct m_point *point, const struct m_curve *curve){
    int res;
    mp_int a_, two, four, V1, V2, V3;
    res = mp_init_multi(&a_, &two, &four, &V1, &V2, &V3, NULL);
    mp_set(&two, 2);
    mp_set(&four, 4);
    res = mp_copy(&curve->a, &a_);
    res = mp_invmod(&four, &curve->p, &four);
    res = mp_addmod(&point->X, &point->Z, &curve->p, &V1); // (p.X+p.Z) mod p -> V1
    res = mp_sqrmod(&V1, &curve->p, &V1); // (V1)^2 mod p -> V1
    res = mp_submod(&point->X, &point->Z, &curve->p, &V2); // (p.X-p.Z) mod p -> V2
    res = mp_sqrmod(&V2, &curve->p, &V2); // (V2)^2 mod p -> V2
    res = mp_mulmod(&V1, &V2, &curve->p, &point->X); // (V1*V2) mod p -> X_new
    res = mp_submod(&V1, &V2, &curve->p, &V1); // (V1-V2) mod p -> V1
    res = mp_addmod(&a_, &two, &curve->p, &a_); // (a+2) mod p -> a
    res = mp_mulmod(&a_, &four, &curve->p, &a_); // (a/4) mod p -> a
    res = mp_mulmod(&a_, &V1, &curve->p, &V3); // (a*V1) mod p -> V3
    res = mp_addmod(&V3, &V2, &curve->p, &V3); // (V3+V2) mod p -> V3
    res = mp_mulmod(&V1, &V3, &curve->p, &point->Z); // (V1*V3) mod p -> Z_new
    mp_clear_multi(&a_, &two, &four, &V1, &V2, &V3, NULL);
}
void LADDER(struct m_point *point, const struct m_curve *curve, const mp_int *k){
    int res;
    mp_int invert;
    res = mp_init(&invert);
    int size = mp_count_bits(k);

    struct m_point zero_point, new_point;
    m_point_init(&zero_point);
    m_point_custom(&zero_point, "1", NULL, "0");
    m_point_init(&new_point);
    res = mp_copy(&point->X, &new_point.X);
    res = mp_copy(&point->Z, &new_point.Z);
    for(int i = size - 1; i >= 0; i--){
        if (mp_get_bit(k, i)){
            xADD(&zero_point, &new_point, point, curve);
            xDBL(&new_point, curve);
        }
        else{
            xADD(&new_point, &zero_point, point, curve);
            xDBL(&zero_point, curve);
        }
    }
    point->X = zero_point.X;
    point->Z = zero_point.Z;
    if(!(mp_iszero(&point->Z))){
        res = mp_invmod(&point->Z, &curve->p, &invert);
        res = mp_mulmod(&point->X, &invert, &curve->p, &point->X);
        res = mp_mulmod(&point->Z, &invert, &curve->p, &point->Z);
    }
    else{
        res = mp_invmod(&new_point.X, &curve->p, &invert);
        res = mp_mulmod(&new_point.X, &invert, &curve->p, &new_point.X);
    }
    m_point_clear(&new_point);
    m_point_clear(&zero_point);
}
void point_on_curve(const struct m_curve *curve, const struct m_point *point){
    mp_int V1, V2, V3, two, three;
    int res;
    res = mp_init_multi(&two, &three, &V1, &V2, &V3, NULL);
    mp_set(&two, 2);
    mp_set(&three, 3);
    res = mp_sqrmod(&point->Y, &curve->p, &V1); // (p.Y)^2 mod p -> V1
    res = mp_mulmod(&V1, &curve->b, &curve->p, &V1);
    res = mp_exptmod(&point->X, &three, &curve->p, &V2);
    res = mp_sqrmod(&point->X, &curve->p, &V3); // (p.Y)^2 mod p -> V1
    res = mp_mulmod(&V3, &curve->a, &curve->p, &V3);
    res = mp_addmod(&V2, &V3, &curve->p, &V2);
    res = mp_addmod(&V2, &point->X, &curve->p, &V2);
    printf("We are doing something here\n");
    if(mp_cmp(&V1, &V2)== MP_EQ)
    {
        printf("Point belongs to curve\n");
    }
    else
    {
        printf("Point doesn't belong to curve\n");
    }
    mp_clear_multi(&two, &three, &V1, &V2, &V3, NULL);
}
void binaryMethod(struct m_point *result, const struct m_point *point, const struct m_curve *curve, const mp_int *k) { // Лекции, слайд 53
    int res;
    struct m_point Q_point;
    m_point_init(&Q_point);
    m_point_custom(&Q_point, "0", "1", "0");
    int bits = mp_count_bits(k);

    for (int i = bits - 1; i >= 0; --i) {
        xDBL(&Q_point, curve);
        if (mp_get_bit(k, i))
            xADD(&Q_point, &Q_point, point, curve);
    }
    res = mp_copy(&Q_point.X, &result->X);
    res = mp_copy(&Q_point.Y, &result->Y);
    res = mp_copy(&Q_point.Z, &result->Z);
    m_point_clear(&Q_point);
}

/* Tests */
_Bool point_cmp(struct m_point *temp1, struct m_point *temp2){
    return  (mp_cmp(&temp1->X, &temp2->X) == MP_EQ) && (mp_cmp(&temp1->Z, &temp2->Z) == MP_EQ) ;
}
void distributivity(struct m_point *point, const struct m_curve *curve){
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

    struct m_point temp1, temp2, temp3;
    m_point_init(&temp1);
    m_point_init(&temp2);
    m_point_init(&temp3);

    binaryMethod(&temp1, point, curve, &k1); // temp1 = k1*P
    binaryMethod(&temp2, point, curve, &k2); // temp2 = k2*P
    xADD(&temp1, &temp2, &temp1, curve);     // temp1 = k1*P + k2*P
    binaryMethod(&temp3, point, curve, &k3); // temp3 = (k1+k2)*P
    if (point_cmp(&temp1, &temp3)) {
       printf("Test passed\n");
    }
    else
    {
        printf("Test not passed\n");
    }
    mp_clear_multi(&k1, &k2, &k3, NULL);
    m_point_clear(&temp1);
    m_point_clear(&temp2);
    m_point_clear(&temp3);
}
