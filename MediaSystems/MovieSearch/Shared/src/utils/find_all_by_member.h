#pragma once
/**
 * author Yme Brugts (s4536622)
 * @file find_all_by_member.h
 * @date 2025-09-21
 */

#include <ranges>

namespace shared::utils {

    template <std::ranges::range Container, typename MemberType, typename MemberPointerType>

	auto find_all_by_member(Container&& container, const MemberType& target, MemberPointerType MemberPointer) {
        return std::forward<Container>(container) | std::views::filter([&](const auto& element) {
            return element.*MemberPointer == target;
            });
    }
}
