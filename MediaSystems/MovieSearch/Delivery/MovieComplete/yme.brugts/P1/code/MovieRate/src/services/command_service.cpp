/**
 * author Yme Brugts (s4536622)
 * @file command_service.cpp
 * @date 2025-09-24
 */


#include "command_service.h"

#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

#include "../models/Query.h"


namespace movie_rate::services
{
	std::optional<models::Query> parse_rating_distance_line(const std::vector<std::string>& arguments)
	{
		models::Query query;
		if (arguments.size() != 3)
		{
			return std::nullopt;
		}
		try
		{
			query.user_id = std::stoi(arguments[1]);
			query.movie_id = std::stoi(arguments[2]);
		}
		catch (const std::exception&)
		{
			return std::nullopt;
		}
		return query;
	}
}
