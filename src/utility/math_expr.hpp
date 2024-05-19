#pragma once

#include <exprtk.hpp>

#include <vector>

namespace Utils {

    static std::pair<std::vector<double>, std::vector<double>> 
    do_math(const std::string& expression_str, const float min, const float max, const float precision,
			const float time, const float a = FLT_MAX, const float b = FLT_MAX)
    {
        using symbol_table_t = exprtk::symbol_table<float>;
        using expression_t = exprtk::expression<float>;
        using parser_t = exprtk::parser<float>;

        float i;

        symbol_table_t symbol_table;
        symbol_table.add_variable("x", i);
        symbol_table.add_constants();
        symbol_table.add_constant("time", time);

		if (a < FLT_MAX) {
			symbol_table.add_constant("a", a);
		}

		if (b < FLT_MAX) {
			symbol_table.add_constant("b", b);
		}
		
        expression_t expression;
        expression.register_symbol_table(symbol_table);

        parser_t parser;
        parser.compile(expression_str, expression);

        std::pair<std::vector<double>, std::vector<double>> xys;

        for (i = min; i < max; i += precision) {
            xys.first.push_back(i);
            xys.second.push_back(expression.value());
        }

        return xys;
    }

}
