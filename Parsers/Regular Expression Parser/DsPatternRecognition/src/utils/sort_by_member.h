#pragma once
/**
* author Yme Brugts (s4536622)
 * @file sort_by_member.h
 * @date 2024-12-01
 */

#include <algorithm>
template <typename Container, typename MemberPointerType>
/**
 * Sorts the elements of a container based on a specified member of the elements.
 *
 * Uses the provided member pointer to access the member of each contained element
 * for comparison. The function can sort the container in ascending or descending order.
 *
 * @param container The container holding the elements to be sorted. Must support
 *                  begin() and end() functions, and its elements must support the
 *                  dereferencing of the provided member pointer.
 * @param memberPtr Pointer to the member of the elements in the container which will
 *                  be used for sorting. The member must support comparison operators.
 * @param ascending Optional boolean flag indicating the sorting order. Defaults to
 *                  true for ascending order; if false, the container is sorted in
 *                  descending order.
 */
void sortByMember(Container& container, MemberPointerType memberPtr, bool ascending = true) {
    std::sort(container.begin(), container.end(),
        [&](const auto& a, const auto& b) -> bool {
            if (ascending) {
                return a.*memberPtr < b.*memberPtr;
            }
            else {
                return a.*memberPtr > b.*memberPtr;
            }
        });
}