#ifndef SASL_PARSER_GRAMMARS_TOKEN_H
#define SASL_PARSER_GRAMMARS_TOKEN_H

#include "../../syntax_tree/token.h"
#include <boost/shared_ptr.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/spirit/include/lex.hpp>
#include <boost/spirit/include/lex_lexertl.hpp>

#define SASL_PARSER_SKIPPER( TOKENIZER ) ( boost::spirit::qi::in_state("SKIPPED")[ TOKENIZER .self] )

struct lex_context;
struct token_attribute_setter;
struct token_attr;

typedef boost::mpl::vector< token_attr > sasl_token_attr_type;
typedef boost::spirit::lex::lexertl::token< const char*, sasl_token_attr_type > sasl_token_type;
typedef boost::spirit::lex::lexertl::actor_lexer< sasl_token_type > sasl_lexer_base;

template <typename BaseLexerT>
struct sasl_tokens : public boost::spirit::lex::lexer< BaseLexerT > {
	sasl_tokens();

	boost::spirit::lex::token_def<token_attr> 
		littok_int, 
		optok_add, 
		whitetok_space,
		whitetok_newline,
		whitetok_pp_line,
		whitetok_c_comment,
		whitetok_cpp_comment
		;

	boost::shared_ptr<lex_context> ctxt;
	boost::shared_ptr<token_attribute_setter> attr_setter;
};

typedef sasl_tokens<sasl_lexer_base> sasl_tokenizer;
typedef sasl_tokenizer::iterator_type sasl_token_iterator;
typedef sasl_tokenizer::lexer_def sasl_skipper;

#endif