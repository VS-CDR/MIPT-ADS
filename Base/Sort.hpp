#pragma once

template <std::random_access_iterator It, std::integral Int>
void Partition(Int pivot, It& left_ptr, It& right_ptr){
    while (left_ptr <= right_ptr){
        while (*left_ptr < pivot){ ++left_ptr; }
        while (*right_ptr > pivot){ --right_ptr; }
        if (left_ptr <= right_ptr){
            std::swap(*left_ptr++, *right_ptr--);
        }
    }
}

template <std::random_access_iterator It>
void QuickSort(It left, It right){
    if (left < right){
        auto left_ptr(left);
        auto right_ptr(right);
        Partition(
            *(left + (std::distance(left, right) / 2)),
            left_ptr,
            right_ptr
        );
        QuickSort(left, right_ptr);
        QuickSort(left_ptr, right);
    }
}

template <std::integral T>
std::vector<T> Merge(const std::vector<T> left,
                     const std::vector<T> right,
                     std::vector<T>& sorted){
    T i = 0, j = 0, p = 0;
    while (i < left.size() && j < right.size()){
        if (left[i] <= right[j]){
            sorted[p++] = left[i++];
        } else{
            sorted[p++] = right[j++];
        }
    }
    while (i < left.size()){
        sorted[p++] = left[i++];
    }
    while (j < right.size()){
        sorted[p++] = right[j++];
    }
    return sorted;
}

template <std::integral T>
std::vector<T> MergeSort(std::vector<T> a){
    if (a.size() == 1){
        return a;
    }
    const auto mid = a.begin() + a.size() / 2;
    return Merge(
        MergeSort(std::vector(a.begin(), mid)),
        MergeSort(std::vector(mid, a.end())),
        a
    );
}
