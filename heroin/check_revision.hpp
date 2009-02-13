#ifndef CHECK_REVISION_HPP
#define CHECK_REVISION_HPP

#include <string>

#include <nil/types.hpp>

enum check_revision_result_type
{
	check_revision_result_success,
	check_revision_result_formula_error,
	check_revision_result_mpq_error,
	check_revision_result_file_error
};

check_revision_result_type check_revision(std::string const & formula, std::string const & mpq, std::string const & directory, ulong & output);

#endif
