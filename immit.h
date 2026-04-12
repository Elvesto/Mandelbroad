#ifndef IMMIT_H_
#define IMMIT_H_

typedef struct {
    float value[8];
} m256;

typedef struct {
    int value[8];
} m256i;

m256 mm_set1_ps(float f) {
    return (m256){f, f, f, f, f, f, f, f};
}

m256 mm_add_ps(m256 val1, m256 val2) {
    m256 temp = {};
    for (int i = 0; i < 8; i++) {
        temp.value[i] = val1.value[i] + val2.value[i];
    }

    return temp;
}

m256 mm_mul_ps(m256 val1, m256 val2) {
    m256 temp = {};
    for (int i = 0; i < 8; i++) {
        temp.value[i] = val1.value[i] * val2.value[i];
    }

    return temp;
}

m256i mm_cmple_ps(m256 val1, m256 val2) {
    m256i temp = {};
    for (int i = 0; i < 8; i++) {
        temp.value[i] = (val1.value[i] <= val2.value[i]) ? -1 : 0;
    }

    return temp;
}

int mm_movemask_ps(m256i val) {
    int res = 0;
    for (int i = 0; i < 8; i++) {
        if (val.value[i] != 0) {
            res = res | (1 << i);
        }
    }

    return res;
}

#endif // IMMIT_H_