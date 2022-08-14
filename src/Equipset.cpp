#include "Equipset.h"

void CycleEquipset::push_back(Equipset* _data) {
    if (!last) {
        last = _data;
        last->next = _data;
    } else {
        _data->next = last->next;
        last->next = _data;
        last = _data;
    }
}

void CycleEquipset::pop_back() {
    if (last->next == last) {
        last->next = nullptr;
        last = nullptr;
    } else {
        Equipset* p;
        p = last;
        while (p->next != last) {
            p = p->next;
        }
        p->next = last->next;
        last->next = nullptr;
        last = p;
    }
}