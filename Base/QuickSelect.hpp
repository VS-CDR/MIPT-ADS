#pragma once

static constexpr int kBlockSz = 5;

template <std::random_access_iterator It>
void Merge(It left, It mid, It right) {
  std::array<int, kBlockSz> merged{};
  std::random_access_iterator auto left_ptr = left;
  std::random_access_iterator auto right_ptr = mid + 1;

  auto *merged_ptr = merged.begin();
  while (left_ptr <= mid && right_ptr <= right) {
    if (*left_ptr < *right_ptr) {
      *merged_ptr++ = *left_ptr++;
    } else {
      *merged_ptr++ = *right_ptr++;
    }
  }
  while (left_ptr <= mid) {
    *merged_ptr++ = *left_ptr++;
  }
  while (right_ptr <= right) {
    *merged_ptr++ = *right_ptr++;
  }
  std::copy(merged.begin(), merged_ptr, left);
}

template <std::random_access_iterator It>
void MergeSort(It left, It right) {
  if (right != left) {
    const auto kMid = left + (right - left) / 2;
    MergeSort(left, kMid);
    MergeSort(kMid + 1, right);
    Merge(left, kMid, right);
  }
}

template <std::random_access_iterator It>
std::pair<int, int> CountLessEqual(int pivot, It left, It right) {
  int count_less = 0;
  int count_eq = 0;
  for (; left <= right; ++left) {
    count_less += static_cast<int>(*left < pivot);
    count_eq += static_cast<int>(*left == pivot);
  }
  return {count_less, count_eq};
}

template <std::random_access_iterator It, std::integral Int>
void Partition(Int pivot, It left_ptr, It right_ptr){
  while (left_ptr <= right_ptr){
    while (*left_ptr < pivot){ ++left_ptr; }
    while (*right_ptr > pivot){ --right_ptr; }
    if (left_ptr <= right_ptr){
      std::swap(*left_ptr++, *right_ptr--);
    }
  }
}

template <std::random_access_iterator It>
int DQSelect(ssize_t kth, It begin, It end) {
  if (end - begin < kBlockSz) {
    MergeSort(begin, end);
    return *(begin + kth);
  }

  bool extra_block = (end - begin + 1) % kBlockSz != 0;
  std::vector medians((end - begin + 1) / kBlockSz + (extra_block ? 1 : 0), 0);
  for (auto i = begin, j = medians.begin(); i <= end; i += kBlockSz, ++j) {
    if (i + kBlockSz - 1 > end) {
      *j = DQSelect((end - i) / 2, i, end);
    } else {
      *j = DQSelect(kBlockSz / 2, i, i + kBlockSz - 1);
    }
  }

  int median = DQSelect(medians.size() / 2, medians.begin(), medians.end() - 1);
  auto [sz_less, sz_eq] = CountLessEqual(median, begin, end);
  Partition(median, begin, end);
  if (kth <= sz_less - 1) {
    return DQSelect(kth, begin, begin + sz_less - 1);
  }
  if (kth <= sz_less + sz_eq - 1) {
    return median;
  }
  return DQSelect(kth - (sz_less + sz_eq), begin + sz_less + sz_eq, end);
}