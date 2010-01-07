#ifndef SASL_PARSER_DETAIL_TOKEN_H
#define SASL_PARSER_DETAIL_TOKEN_H

#include "../grammars/token.h"
#include "../../parser_tree/literal.h"
#include "../../enums/token_types.h"
#include <string>

struct lex_context{
	lex_context(): line(0), column(0) {}
	size_t line;
	size_t column;
};

struct token_attribute_setter{
	token_attribute_setter( lex_context& ctxt ): lex_ctxt( ctxt ){
	}
	token_attribute_setter( const token_attribute_setter& rhs ): lex_ctxt( rhs.lex_ctxt ){
	}

	template <typename IteratorT, typename PassFlagT, typename IdtypeT, typename ContextT>
	void operator () (IteratorT& start, IteratorT& end, PassFlagT& matched, IdtypeT& id, ContextT& ctx){
		token_attr attr;
		attr.lit.assign(start, end);
		
		attr.column = lex_ctxt.column;
		attr.file_name = "undefined";
		attr.line = lex_ctxt.line;

		ctx.set_value( attr );

		lex_ctxt.column += attr.lit.length();
	}

	lex_context& lex_ctxt;
};

template <typename BaseLexerT>
sasl_tokens<BaseLexerT>::sasl_tokens(){
	ctxt.reset( new lex_context() );
	attr_setter.reset(new token_attribute_setter(*ctxt));

	this->self.add_pattern
		("SPACE", "[ \\t\\v\\f]+")
		("NEWLINE", "((\\r\\n?)|\\n)+")
		;

	(littok_int = "[0-9]+").id( token_types::_constant.to_value() );
	(optok_add = "[\\+]").id( token_types::_operator.to_value() );
	(whitetok_space = "{SPACE}").id( token_types::_whitespace.to_value() );
	(whitetok_newline = "{NEWLINE}").id(token_types::_newline.to_value());
	(whitetok_pp_line = "#line.*{NEWLINE}").id(token_types::_preprocessor.to_value());
	(whitetok_c_comment = "\\/\\*[^*]*\\*+([^/*][^*]*\\*+)*\\/").id(token_types::_comment.to_value());
	(whitetok_cpp_comment = "\\/\\/.*{NEWLINE}").id(token_types::_comment.to_value());

	this->self =
		littok_int					[*attr_setter]
		| optok_add					[*attr_setter]			
		;

	this->self("SKIPPED") =
		whitetok_space				[*attr_setter]
		| whitetok_newline			[*attr_setter]
		| whitetok_pp_line			[*attr_setter]
		| whitetok_c_comment		[*attr_setter]
		| whitetok_cpp_comment		[*attr_setter]							
		;
}



#endif //SASL_PARSER_TOKEN_H