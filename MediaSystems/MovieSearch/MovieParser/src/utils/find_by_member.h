#pragma once
/**
* author Yme Brugts (s4536622)
 * @file find_by_member.h
 * @date 2024-11-20
 */

#include <algorithm>

template <typename Container, typename MemberType, typename MemberPointerType>
/**
 * Finds an element in a container where a specific member matches a target value.
 *
 * @tparam Container The type of the container.
 * @tparam MemberType The type of the member.
 * @tparam MemberPointerType The type of the member pointer.
 * @param container The container to search within.
 * @param target The target value to match the member against.
 * @param MemberPointer The pointer to the member of the elements in the container.
 * @return An iterator to the first element whose specified member matches the target value,
 *         or the end of the container if no such element is found.
 */
auto findByMember(const Container& container, const MemberType& target, MemberPointerType MemberPointer) {
    return std::find_if(container.begin(), container.end(), [&](const auto& element) {
        return element.*MemberPointer == target;
        });
}