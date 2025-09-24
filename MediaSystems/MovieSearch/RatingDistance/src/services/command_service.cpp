/**
 * author Yme Brugts (s4536622)
 * @file command_service.cpp
 * @date 2025-09-23
 */


#include "command_service.h"

#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

#include "../models/Query.h"


namespace rating_distance::services
{
	std::optional<models::Query> parse_rating_distance_line(const std::vector<std::string>& arguments)
	{
		rating_distance::models::Query query;
		if (arguments.size() != 3)
		{
			return std::nullopt;
		}
		try
		{
			query.user_id_one = std::stoi(arguments[1]);
			query.user_id_two = std::stoi(arguments[2]);
		}
		catch (const std::exception&)
		{
			return std::nullopt;
		}
		return query;
	}
}
