/**
 The One Programming Language

 File: ast/ast.h
  _        _
 / \ |\ | |_    Max Base
 \_/ | \| |_    Copyright 2021; One Language Contributors

 **/

#ifndef _ONE_AST_AST_H_
#define _ONE_AST_AST_H_

typedef enum _token_type TokenType;

/*
package main
package test
package firstName.subName
*/
typedef struct
{
	char* name;
} AstModule;

typedef struct
{
	AstModule module;
	Array imports; // AstImportDeclaration
	Array blocks; // AstBlockDeclaration

	char* path;
	char* path_base;
} AstFile;

/*
import math // math.sin(), math.cos()
import math { sin } // sin()
import math { sin, cos } // sin(), cos()
import parentLib.childLib // childLib.functionName()
import parentLib.childLib as pc // pc.functionName()
import parentLib {childLib.functionName as a_fn} // a_fn()
import file // file.create()
import file { create } // create()
import file { create as cr } // cr()
*/
typedef struct
{
	// IMPORT ****.****.*** { *** } AS ***
	// ^ pos
	//        ^ pos_names
	//                      ^ pos_symbols
	//                              ^ pos_alias
	Location pos;
	Location pos_names;
	Location pos_symbols;
	Location pos_alias;

	Array names; // AstImportName
	Array symbols; // AstImportSymbol
	char* alias;
} AstImportDeclaration;

typedef struct
{
	Location pos;

	char* name;
} AstImportName;

typedef struct
{
	Location pos_names;
	Location pos_alias;

	bool has_alias;

	Array names;
	char* alias;
} AstImportSymbol;

typedef struct
{
	Location pos;

	AstBlockType type;

	union
	{
		AstFunctionDeclaration function;
		AstStructDeclaration structure;
		AstEnumDeclaration enumerate;
		AstTypeDeclaration type;
		AstStatementDeclaration statement;
	} value;
} AstBlockDeclaration;

typedef enum
{
	AST_BLOCK_FUNCTION,
	AST_BLOCK_STRUCT,
	AST_BLOCK_ENUM,
	AST_BLOCK_TYPE,
	AST_BLOCK_STATEMENT,
} AstBlockType;

typedef struct
{
	// [main] FN ( mut name type ) fn_name () type { }
	// ^ pos_attribute
	//        ^ pos_function
	//           ^ pos_reciver
	//                             ^ pos_name
	//                                      ^ pos_param
	//                                        ^ pos_return
	//                                             ^ pos_body
	Location pos;
	Location pos_attribute;
	Location pos_function;
	Location pos_reciver;
	Location pos_name;
	Location pos_param;
	Location pos_return;
	Location pos_body;

	bool is_main;
	bool is_noreturn;
	bool is_public;
	bool is_depracted;

	bool has_return;

	AstParam reciver;
	Array params; // AstParam
	Array statements; // AstBlockDeclaration
	Array attributes; //AstAttributeDeclaration
} AstFunctionDeclaration;

typedef struct
{
	Location pos;
	Location pos_name;
	Location pos_value;

	AstAttributeType type;

	bool has_value;

	char* name;
	char* value;
} AstAttributeDeclaration;

typedef enum
{
	AST_ATTRIBUTE_IDENTIFIER,
	AST_ATTRIBUTE_KEY,
} AstAttributeType;
typedef struct
{
	// fn (mut t MyTime) century() int {}
	//	   mut t MyTime
	//	   ^ pos_mut
	//	       ^ pos_name
	//	         ^ pos_type
	Location pos;
	Location pos_mut;
	Location pos_name;
	Location pos_type;

	bool is_mut;

	char* name;
	AstData type;
} AstParam;

/*
i8, u8
i16, u16
i32, u32
i64, u64
i128, u128
f32, f64
bool
char, string
pubName.publicTypeName
*/
typedef struct
{
	// &Node<T>
	// ^ pos_address
	//  ^ pos_name
	//      ^ pos_generic

	// Node<T>
	// ^ pos_address or maybe nil
	// ^ pos_name
	//     ^ pos_generic
	Location pos;
	Location pos_address;
	Location pos_name;
	Location pos_generic;

	bool has_address;
	bool has_generic;

	Array names; // AstDataItem
	Array generics; // char*
} AstData;

typedef struct
{
	Location pos;

	char* name; // char*
} AstDataItem;

typedef struct
{
	Location pos;
	Location pos_attribute;
	Location pos_name;
	Location pos_body;

	char* name;

	// Array global; // AstStructField
	// Array pub_mut; // AstStructField
	// Array pub; // AstStructField
	// Array mut; // AstStructField
	// Array imut; // AstStructField

	Array fields; // AstStructField
	Array attributes; // AstAttributeDeclaration
} AstStructDeclaration;

typedef struct
{
	Location pos;
	Location pos_attribute;
	Location pos_type;
	Location pos_name;
	Location pos_value;

	bool is_mut;
	bool is_public;
	bool is_global;

	bool has_default;
	AstExprDeclaration value;

	AstData type;
	char* name;
	Array attributes; // AstAttributeDeclaration
} AstStructField;

typedef struct
{
	Location pos;
	Location pos_attribute;
	Location pos_name;
	Location pos_body;

	char* name;
	Array fields; // AstEnumField
	Array attributes; // AstAttributeDeclaration
} AstEnumDeclaration;

typedef struct
{
	Location pos;
	Location pos_attribute;
	Location pos_name;
	Location pos_value;

	bool has_default;
	AstExprDeclaration value;

	char* name;
	Array attributes; // AstAttributeDeclaration
} AstEnumField;

typedef struct
{
	Location pos;
	Location pos_op;
	Location pos_left;
	Location pos_right;
	Location pos_value;

	AstExprType type;
	AstOperatorType op;

	AstExprDeclaration left;
	AstExprDeclaration right;

	enum
	{
		char vi8;
		unsigned char vu8;

		short vi16;
		unsigned short vu16;

		int vi32;
		unsigned int vu32;

		int64_t vi64;
		uint64_t vu64;

		float vf32;
		double vf64;

		bool vbool;

		char * vstring;
		char * vchar;
	}
	value;
} AstExprDeclaration;

typedef enum
{
	AST_OPERATOR_NONE, // emit data

	AST_OPERATOR_PLUS, // -
	AST_OPERATOR_PLUS_PLUS, // --

	AST_OPERATOR_MINUS, // -
	AST_OPERATOR_MINUS_MINUS, // --

	AST_OPERATOR_STAR, // *
	AST_OPERATOR_STAR_STAR, // **

	AST_OPERATOR_MOD, // %

	AST_OPERATOR_SLASH, // /
	AST_OPERATOR_SLASH_INT, // //

	AST_OPERATOR_SHIFT_LEFT, // >>
	AST_OPERATOR_SHIFT_RIGHT, // <<

	AST_OPERATOR_BIT_AND, // &
	AST_OPERATOR_BIT_OR, // |
	AST_OPERATOR_BIT_XOR, // ^
	AST_OPERATOR_BIT_NOT, // ~

	AST_OPERATOR_AND, // &&
	AST_OPERATOR_OR, // ||

	AST_OPERATOR_NOT, // !
	AST_OPERATOR_NOT_EQUAL, // !=
	// AST_OPERATOR_NOT_EQUAL_EQUAL, // !==

	AST_OPERATOR_EQUAL, // =
	AST_OPERATOR_EQUAL_ASSIGN, // :=
	AST_OPERATOR_EQUAL_EQUAL, // ==
	// AST_OPERATOR_EQUAL_EQUAL, // ===

	// Relational Operators
	AST_OPERATOR_GREATER, // >
	AST_OPERATOR_GREATER_EQUAL, // >=
	AST_OPERATOR_LESS, // <
	AST_OPERATOR_LESS_EQUAL, // <=

	AST_OPERATOR_DOT, // parent.sub
	AST_OPERATOR_DOT_DOT, // [1..4]
	AST_OPERATOR_DOT_DOT_DOT, // {...objects}

	// Assignment Operators
	AST_OPERATOR_EQUAL_PLUS, // +=
	AST_OPERATOR_EQUAL_MINUS, // -=
	AST_OPERATOR_EQUAL_MINUS, // *=
	AST_OPERATOR_EQUAL_SLASH, // /=
	AST_OPERATOR_EQUAL_MOD, // %=
	AST_OPERATOR_EQUAL_BIT_AND, // &=
	AST_OPERATOR_EQUAL_BIT_OR, // |=
	AST_OPERATOR_EQUAL_BIT_NOT, // ^=
	AST_OPERATOR_EQUAL_SHIFT_LEFT, // >>=
	AST_OPERATOR_EQUAL_SHIFT_RIGHT, // <<=

} AstOperatorType;

typedef enum
{
	AST_EXPRESSION_TYPE_I8,
	AST_EXPRESSION_TYPE_U8,

	AST_EXPRESSION_TYPE_I16,
	AST_EXPRESSION_TYPE_U16,

	AST_EXPRESSION_TYPE_I32,
	AST_EXPRESSION_TYPE_U32,

	AST_EXPRESSION_TYPE_I64,
	AST_EXPRESSION_TYPE_U64,

	AST_EXPRESSION_TYPE_I128,
	AST_EXPRESSION_TYPE_U128,

	AST_EXPRESSION_TYPE_F32,
	AST_EXPRESSION_TYPE_F64,

	AST_EXPRESSION_TYPE_BOOL,
	AST_EXPRESSION_TYPE_STRING,
	AST_EXPRESSION_TYPE_CHAR,
} AstExprType;

/*
struct Point {
	x int
	y int
}
struct Line {
	p1 Point
	p2 Point
}
type ObjectSumType = Line | Point

struct Moon {}
struct Mars {}
struct Venus {}
type World = Mars | Moon | Venus

type Filter = fn (string) string
*/
typedef struct
{
	Location pos;
	Location pos_public;
	Location pos_name;

	AstTypeDeclarationType type;

	bool is_public;

	char* name;

	union
	{
		AstTypeFunctionDeclaration function;
		AstTypeAliasDeclaration alias;
		AstTypeSumDeclaration sum;
	} value;
} AstTypeDeclaration;

typedef enum
{
	AST_TYPE_SUM,
	AST_TYPE_ALIAS,
	AST_TYPE_FUNCTION,
} AstTypeDeclarationType;

typedef struct
{
	Location pos;
	Location pos_public;
	Location pos_name;
	Location pos_type;

	bool is_public;

	char* name;
	AstData data; // TODO: AstData can store a function type?
} AstTypeFunctionDeclaration;

typedef struct
{
	Location pos;
	Location pos_public;
	Location pos_name;
	Location pos_type;

	bool is_public;

	char* name;
	AstData data;
} AstTypeAliasDeclaration;

typedef struct
{
	Location pos;
	Location pos_public;
	Location pos_name;
	Location pos_name;

	bool is_public;

	char* name;
	Array data; // AstTypeSumItem
} AstTypeSumDeclaration;

typedef struct
{
	Location pos;
	AstData data;
} AstTypeSumItem;

// typedef enum {
// 	TYPE_STRUCT,
// 	TYPE_SUMTYPE,
// } AstParamDataType;

typedef struct
{
	Location pos;

	AstStatementType type;

	union
	{
		AstStatementIf clauses;
		AstStatementFor foreach;
		AstStatementMatch match;
		AstStatementRet ret;
		AstStatementAssign assign;
		AstStatementAssignConst assign_const;
		AstStatementVariable variable;
		AstStatementExpr expr;
	} value;
} AstStatementDeclaration;

typedef enum 
{
	AST_STATEMENT_IF,
	AST_STATEMENT_FOR,
	AST_STATEMENT_MATCH,
	AST_STATEMENT_RET,
	AST_STATEMENT_ASSIGN,
	AST_STATEMENT_ASSIGN_CONST,
	AST_STATEMENT_VARIABLE,
	AST_STATEMENT_EXPRESSION,
} AstStatementType;

typedef struct {
	// if <expr> {} else ...
	Location pos;
	Location pos_expr;
	Location pos_body;
	Location pos_else;

	bool has_else;

	AstExprDeclaration expr;
	Array body; // AstBlockDeclaration

	AstStatementIf otherwise;
} AstStatementIf;
typedef struct {

} AstStatementFor;
typedef struct {

} AstStatementMatch;
typedef struct {

} AstStatementRet;
typedef struct {

} AstStatementAssign;
typedef struct {

} AstStatementAssignConst;
typedef struct {

} AstStatementVariable;
typedef struct {

} AstStatementExpr;


// char* ast_statement_name(AstStatementType type);

// char* ast_value_name(AstValueType type);

#endif // _ONE_AST_AST_H_
