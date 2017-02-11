/* File: parser.y
 * --------------
 * Bison input file to generate the parser for the compiler.
 *
 * pp2: your job is to write a parser that will construct the parse tree
 *      and if no parse errors were found, print it.  The parser should
 *      accept the language as described in specification, and as augmented
 *      in the pp2 handout.
 */

%{

/* Just like lex, the text within this first region delimited by %{ and %}
 * is assumed to be C/C++ code and will be copied verbatim to the y.tab.c
 * file ahead of the definitions of the yyparse() function. Add other header
 * file inclusions or C++ variable declarations/prototypes that are needed
 * by your code here.
 */
#include "scanner.h" // for yylex
#include "parser.h"
#include "errors.h"

void yyerror(const char *msg); // standard error-handling routine

%}

/* The section before the first %% is the Definitions section of the yacc
 * input file. Here is where you declare tokens and types, add precedence
 * and associativity options, and so on.
 */

/* yylval
 * ------
 * Here we define the type of the yylval global variable that is used by
 * the scanner to store attibute information about the token just scanned
 * and thus communicate that information to the parser.
 *
 * pp2: You will need to add new fields to this union as you add different
 *      attributes to your non-terminal symbols.
 */
%union {
    int integerConstant;
    bool boolConstant;
    float floatConstant;
    char identifier[MaxIdentLen+1]; // +1 for terminating null
    Decl *decl;
    List<Decl*> *declList;

    //ast.h
    Node *node;
    Identifier *ident;
    Error *error;

    //ast_decl.h
    VarDecl *varDecl;
    List<VarDecl*> *varDeclList;
    VarDeclError *varDeclError;
    FnDecl *fnDecl;
    FormalsError *formalsError;

    //ast_expr.h
    List<Expr*> *exprList;
    Expr *expr;
    ExprError *exprError;
    EmptyExpr *emptyExpr;
    IntConstant *intConst;
    FloatConstant *floatConst;
    BoolConstant *boolConst;
    Operator *operators;
    CompoundExpr *compoundExpr;
    ArithmeticExpr *arithmeticExpr;
    RelationalExpr *relationalExpr;
    EqualityExpr *equalityExpr;
    LogicalExpr *logicalExpr;
    SelectionExpr *selectionExpr;    //new shit
    AssignExpr *assignExpr;
    PostfixExpr *postfixExpr;
    LValue *lValue;
    ArrayAccess *arrayAccess;
    FieldAccess *fieldAccess;
    Call *call;
    ActualsError *actualsError;
    VarExpr *varExpr;              //new shit


    //ast_stmt.h
    Program *program;
    List<Stmt*> *stmtList;
    Stmt *stmt;
    StmtBlock *stmtBlock;
    ConditionalStmt *conditionalStmt;
    LoopStmt *loopStmt;
    ForStmt *forStmt;
    WhileStmt *whileStmt;
    IfStmt *ifStmt;
    IfStmtExprError *ifStmtExprError;
    BreakStmt *breakStmt;
    ReturnStmt *returnStmt;
    SwitchLabel *switchLabel;
    Case *cases;
    Default *def;
    SwitchStmt *switchStmt;

    //ast_type.h
    Type *type;
    NamedType *namedType;
    ArrayType *arrayType;


    //functions

}


/* Tokens
 * ------
 * Here we tell yacc about all the token types that we are using.
 * Bison will assign unique numbers to these and export the #define
 * in the generated y.tab.h header file.
 */
%token   T_Void T_Bool T_Int T_Float
%token   T_LessEqual T_GreaterEqual T_EQ T_NE T_LeftAngle T_RightAngle
%token   T_And T_Or
%token   T_Equal T_MulAssign T_DivAssign T_AddAssign T_SubAssign
%token   T_While T_For T_If T_Else T_Return T_Break
%token   T_Const T_Uniform T_Layout T_Continue T_Do
%token   T_Inc T_Dec T_Switch T_Case T_Default
%token   T_In T_Out T_InOut
%token   T_Mat2 T_Mat3 T_Mat4 T_Vec2 T_Vec3 T_Vec4
%token   T_Ivec2 T_Ivec3 T_Ivec4 T_Bvec2 T_Bvec3 T_Bvec4
%token   T_Uint T_Uvec2 T_Uvec3 T_Uvec4 T_Struct
%token   T_Semicolon T_Dot T_Colon T_Question T_Comma
%token   T_Dash T_Plus T_Star T_Slash
%token   T_LeftParen T_RightParen T_LeftBracket T_RightBracket T_LeftBrace T_RightBrace

%token   <identifier> T_Identifier
%token   <integerConstant> T_IntConstant
%token   <floatConstant> T_FloatConstant
%token   <boolConstant> T_BoolConstant

/* Non-terminal types
 * ------------------
 * In order for yacc to assign/access the correct field of $$, $1, we
 * must to declare which field is appropriate for the non-terminal.
 * As an example, this first type declaration establishes that the DeclList
 * non-terminal uses the field named "declList" in the yylval union. This
 * means that when we are setting $$ for a reduction for DeclList ore reading
 * $n which corresponds to a DeclList nonterminal we are accessing the field
 * of the union named "declList" which is of type List<Decl*>.
 * pp2: You'll need to add many of these of your own.
 */
 /*
%type <declList>  DeclList

%type <decl>      decl*/

%type <program> Program
%type <decl>      declaration
%type <declList>  translation_unit
%type <decl> external_declaration
%type <fnDecl> function_definition


%type <ident> variable_identifier
                function_identifier
                function_call_header_no_parameters

%type <exprList> fn_call_parameters
%type <expr>    primary_expression
                postfix_expression
                expression
                unary_expression
                multiplicative_expression
                additive_expression
                shift_expression
                logical_xor_expression
                assignment_expression
                relational_expression
                equality_expression
                logical_or_expression
                conditional_expression
                exclusive_or_expression
                and_expression
                inclusive_or_expression
                logical_and_expression
                constant_expression
                initializer
                conditionopt
                condition
                expression_statement
                declaration_statement
                for_init_statement

%type <integerConstant> array_specifier
%type <type>                        fully_specified_type
%type <type>                        type_specifier
%type <type>                        type_specifier_nonarray
%type <fnDecl>                      function_prototype
%type <fnDecl>                      function_declarator
%type <varDeclList>                 fn_parameters

%type <varDecl>                     parameter_declaration
                                    single_declaration
                                    init_declarator_list

%type <operators>                   assignment_operator
%type <operators>                   unary_operator

%type <stmtList>                    statement_list

%type <stmt>                        statement
                                    statement_no_new_scope
                                    statement_with_scope
                                    simple_statement


                                    selection_statement
                                    switch_statement
                                    case_label
                                    iteration_statement
                                    jump_statement

                                    compound_statement

%type <call>                        function_call


%%
/* Rules
 * -----
 * All productions and actions should be placed between the start and stop
 * %% markers which delimit the Rules section.

 */
Program
    :    translation_unit {
            @1;
            /* pp2: The @1 is needed to convince
            * yacc to set up yylloc. You can remove
            * it once you have other uses of @n*/
            Program *program = new Program($1);
            // if no errors, advance to next phase
            if (ReportError::NumErrors() == 0)
            program->Print(0);
        }
    ;

/* replaced w/ translation_unit
DeclList  :    DeclList decl        { ($$=$1)->Append($2); }
          |    decl                 { ($$ = new List<Decl*>)->Append($1); }
          ;
decl        :   declaration                   { $$ = $1; }
            |   function_definition {}
            ;
*/

variable_identifier
    :	T_Identifier { $$ = new Identifier(@1,$1);};

primary_expression
    :	variable_identifier	{  $$ = new VarExpr(@1, $1); }
    |	T_IntConstant		{$$ = new IntConstant(@1,$1);}
    |	T_FloatConstant		{$$ = new IntConstant(@1,$1);}
    |	T_BoolConstant		{$$ = new BoolConstant(@1,$1);}
    |	T_LeftParen expression T_RightParen {$$=$2;}
    ;

postfix_expression
    :	primary_expression	{$$ = $1; }
	|	postfix_expression T_LeftBracket postfix_expression T_RightBracket { $$ = new ArrayAccess(@$, $1, $3);  }
	// | 	function_call {$$ = $1;}
    |	postfix_expression T_Dot T_Identifier {$$ = new FieldAccess($1, new Identifier(@3, $3));}
    |	postfix_expression T_Inc {$$ = new PostfixExpr($1, new Operator(@2, "++") );}
    |	postfix_expression T_Dec {$$ = new PostfixExpr($1, new Operator(@2, "--"));}
    ;

integer_expression
    :	expression {};

function_call
    :	function_call_header_no_parameters T_RightParen {
            Identifier *id = new Identifier(@1, $1->getName())
            $$ = new Call(@1, NULL, id, new List<Expr*>());
        }
    |	function_identifier T_LeftParen fn_call_parameters T_RightParen {
            Identifier *id = new Identifier(@1, $1->getName())
            $$ = new Call(@1, NULL, id, $3);
        }
    ;
    //
    //
    // | 	fully_specified_type T_Identifier T_LeftParen fn_parameters {
    //         $$ = new FnDecl(new Identifier(@2, $2), $1, $4);
    //     }
    // ;


fn_call_parameters
    :   assignment_expression {($$ = new List<Expr*>())->Append($1);}
    |   fn_call_parameters T_Comma assignment_expression {($$ = $1)->Append($3);}
    ;

// function_call_or_method
//     :	function_call_generic  {};
//
// function_call_generic
//     :	function_call_header_with_parameters T_RightParen {}
//     |	function_call_header_no_parameters T_RightParen {}
//     ;

function_call_header_no_parameters
    :	function_identifier T_LeftParen T_Void  {$$=$1;}
    |	function_identifier T_LeftParen  {$$=$1;}
    ;

// function_call_header_with_parameters
//     :	function_identifier T_LeftParen assignment_expression {
//             List<Expr*> *e = new List<Expr*>())->Append($2);
//             Identifier *id = new Identifier(@1, $1->getName())
//             $$ = new Call(@1, NULL, id, e);
//         }
//     | 	function_call_header_with_parameters ',' assignment_expression {
//         ($$ = $1)->Append($3)
//     }
//     ;

// function_call_header
//     : 	function_identifier T_LeftParen {};

function_identifier
    : 	type_specifier {$$=$1;}
    | 	variable_identifier {$$=$1;}
    ;

unary_expression
    : 	postfix_expression { $$ = $1; }
    | 	T_Inc unary_expression {
           $$ = new ArithmeticExpr(new Operator(@1, "++"), $2);
        }
    | 	T_Dec unary_expression {
           $$ = new ArithmeticExpr(new Operator(@1, "--"), $2);
        }
    | 	unary_operator unary_expression {
            $$ = new ArithmeticExpr($1, $2);
        }
    ;

unary_operator
    :	T_Plus {$$=new Operator(@1,"+");}
    |	T_Dash {$$=new Operator(@1,"+");}
    ;


multiplicative_expression
    : 	unary_expression {$$ = $1;}
    | 	multiplicative_expression T_Star unary_expression  { $$ = new ArithmeticExpr($1,new Operator(@2,"*"),$3); }
    | 	multiplicative_expression T_Slash unary_expression { $$ = new ArithmeticExpr($1,new Operator(@2,"/"),$3); }
    ;

additive_expression
    :	multiplicative_expression  {$$ = $1;}
    | 	additive_expression T_Plus multiplicative_expression {
            $$ = new ArithmeticExpr($1, new Operator(@2, "+"), $3);
        }
    | 	additive_expression T_Dash multiplicative_expression {
            $$ = new ArithmeticExpr($1, new Operator(@2, "-"), $3);
        }
    ;

shift_expression
    : 	additive_expression {$$=$1;} ;

relational_expression
    : 	shift_expression {$$=$1;}
    | 	relational_expression T_LeftAngle shift_expression {$$ = new RelationalExpr($1, new Operator(yylloc, "<"), $3);}
    | 	relational_expression T_RightAngle shift_expression {$$ = new RelationalExpr($1, new Operator(yylloc, ">"), $3);}
    | 	relational_expression T_LessEqual shift_expression {$$ = new RelationalExpr($1, new Operator(yylloc, "<="), $3);}
    | 	relational_expression T_GreaterEqual shift_expression {$$ = new RelationalExpr($1, new Operator(yylloc, ">="), $3);}
    ;

equality_expression
    : 	relational_expression {$$=$1;}
    | 	equality_expression T_EQ relational_expression {$$ = new EqualityExpr($1, new Operator(yylloc, "=="), $3);}
    | 	equality_expression T_NE relational_expression {$$ = new EqualityExpr($1, new Operator(yylloc, "!="), $3);}
    ;


and_expression
    :	equality_expression {$$=$1;};

exclusive_or_expression
    : 	and_expression {$$=$1;};

inclusive_or_expression
    : 	exclusive_or_expression {$$=$1;};

    /*
        and_expression
        exclusive_or_expression
        inclusive_or_expression
        are a redundant
    */

logical_and_expression
    : 	inclusive_or_expression {$$=$1;}
    | 	logical_and_expression T_And inclusive_or_expression {$$ = new LogicalExpr($1, new Operator(yylloc, "&&"), $3);}
    ;

logical_xor_expression
    :	logical_and_expression {$$=$1;};

logical_or_expression
    : 	logical_xor_expression {$$=$1;}
    | 	logical_or_expression T_Or logical_xor_expression {$$ = new LogicalExpr($1, new Operator(yylloc, "||"), $3);}
	;

conditional_expression
    :	logical_or_expression {$$=$1;}
	// |	logical_or_expression T_Question expression T_Colon assignment_expression {}
	;

assignment_expression
    : 	conditional_expression {$$=$1;}
    | 	unary_expression assignment_operator assignment_expression {$$ = new AssignExpr($1, $2, $3);}
    ;

assignment_operator
    : 	T_Equal     {$$ = new Operator(@1,"=");}
    | 	T_MulAssign {$$ = new Operator(@1,"*=");}
    | 	T_DivAssign {$$ = new Operator(@1,"/=");}
    | 	T_AddAssign {$$ = new Operator(@1,"+=");}
    | 	T_SubAssign {$$ = new Operator(@1,"-=");}
    ;

expression
    : 	assignment_expression {$$ = $1;};

constant_expression
    :	conditional_expression {$$ = $1;};


/*
Function declarations
Single declaration
*/
declaration
    : 	function_prototype T_Semicolon {}
    | 	init_declarator_list T_Semicolon {/* this is for test0 */}
    | 	type_qualifier T_Identifier T_Semicolon {}
    ;


/*
* function_prototype
* function_declarator
* function_header_with_parameters
* function_header
*/

function_prototype
    : 	function_declarator T_RightParen {}
    ;

function_declarator
    : 	fully_specified_type T_Identifier T_LeftParen {
            $$ = new FnDecl(new Identifier(@2, $2), $1, new List<VarDecl*>());
        }
    | 	fully_specified_type T_Identifier T_LeftParen fn_parameters {
            $$ = new FnDecl(new Identifier(@2, $2), $1, $4);
        }
    ;


fn_parameters
    :   parameter_declaration {($$ = new List<VarDecl*>())->Append($1);}
    |   fn_parameters T_Comma parameter_declaration {($$ = $1)->Append($3);}
    ;
/*
* function_header                     	: 	fully_specified_type T_Identifier T_LeftParen {};
*/

/*
* parameter_declarator			:	type_specifier T_Identifier {};
*/

parameter_declaration
    :	type_specifier T_Identifier { $$ = new VarDecl(new Identifier(@2,$2),$1); }
    | 	type_specifier {$$ = new VarDecl(new Identifier(@1, ""), $1);}
    ;

/*
* parameter_type_specifier		:	type_specifier {};
*/

init_declarator_list
    :	single_declaration {$$=$1;};

single_declaration
    :	fully_specified_type {} /* not tested i.e `int` or `const`*/
    |	fully_specified_type T_Identifier { $$ = new VarDecl(new Identifier(@2, $2), $1); }
    |	fully_specified_type T_Identifier array_specifier {
        $$ = new VarDecl(new Identifier(@2, $2), $1);
        // ArrayType *arr = new ArrayType($1);
        // $$ = new VarDecl(new Identifier(@2, $2), arr);
    }
    |	fully_specified_type T_Identifier T_Equal initializer { $$ = new VarDecl(new Identifier(@2, $2), $1); }
    ;

fully_specified_type
    :	type_specifier {}
    |	type_qualifier type_specifier {}
    ;


/* combined single_type_qualifier, storage_qualifier w/ type_qualifier */
type_qualifier
    :	T_Const {}
    |	T_In {}
    |	T_Out {}
    |	T_Uniform {}
    ;

type_specifier
    :	type_specifier_nonarray {$$ = $1;}
    |	type_specifier_nonarray array_specifier {}
    ;

array_specifier
    :	T_LeftBracket T_IntConstant T_RightBracket { $$ = $2; };

type_specifier_nonarray
    :	T_Void {$$ = Type::voidType;}
    |	T_Float {$$ = Type::floatType;}
    |	T_Int {$$ = Type::intType;}
    |	T_Bool {$$ = Type::boolType;}
    |	T_Vec2 {$$ = Type::vec2Type;}
    |	T_Vec3 {$$ = Type::vec3Type;}
    |	T_Vec4 {$$ = Type::vec4Type;}
    |	T_Bvec2 {$$ = Type::bvec2Type;}
    |	T_Bvec3 {$$ = Type::bvec3Type;}
    |	T_Bvec4 {$$ = Type::bvec4Type;}
    |	T_Ivec2 {$$ = Type::ivec2Type;}
    |	T_Ivec3 {$$ = Type::ivec3Type;}
    |	T_Ivec4 {$$ = Type::ivec4Type;}
    |	T_Uvec2 {$$ = Type::uvec2Type;}
    |	T_Uvec3 {$$ = Type::uvec3Type;}
    |	T_Uvec4 {$$ = Type::uvec4Type;}
    |	T_Mat2 {$$ = Type::mat2Type;}
    |	T_Mat3 {$$ = Type::mat3Type;}
    |	T_Mat4 {$$ = Type::mat4Type;}
    ;

initializer
    : 	assignment_expression {$$=$1;}
    ;



compound_statement
    : 	T_LeftBrace T_RightBrace {$$ = new StmtBlock(new List<VarDecl*>, new List<Stmt *>); }
    | 	T_LeftBrace statement_list T_RightBrace { $$ = new StmtBlock(new List<VarDecl*>, $2);}
    ;

statement
    : 	compound_statement {$$ = $1;}
    | 	simple_statement {$$ = $1;}
    ;

statement_no_new_scope
    : 	compound_statement {$$ = $1;}
    | 	simple_statement {$$ = $1;}
    ;

statement_with_scope
    : 	compound_statement {{$$ = $1;}}
    | 	simple_statement {{$$ = $1;}}
    ;

simple_statement
    : 	declaration_statement {$$ = $1;}
    | 	expression_statement {$$ = $1;}
    | 	selection_statement {$$ = $1;}
    | 	switch_statement {$$ = $1;}
    | 	case_label {$$ = $1;}
    | 	iteration_statement {$$ = $1;}
    |	jump_statement {$$ = $1;}
    ;


/*

compound_statement_no_new_scope     	: 	T_LeftBrace T_RightBrace { $$ = new StmtBlock(new List<VarDecl*>, new List<Stmt *>); }
                    					|	T_LeftBrace T_RightBrace statement_list T_RightBrace { $$ = new StmtBlock(new List<VarDecl*>, $2);}
                    					;
*/
statement_list
    : 	statement  { ($$ = new List<Stmt*>)->Append($1); }
    | 	statement_list statement  { ($$ = $1)->Append($2); }
    ;


declaration_statement
    :	declaration {};


expression_statement
    :	T_Semicolon {$$ = new EmptyExpr();}
    | 	expression T_Semicolon {$$ = $1;}
    ;
selection_statement
    : 	T_If T_LeftParen expression T_RightParen statement_with_scope T_Else statement_with_scope  {$$ = new IfStmt($3, $5, $7);}
    |   T_If T_LeftParen expression T_RightParen statement_with_scope {$$ = new IfStmt($3, $5, NULL);}
    ;

/* its easier to have this rule in selection_statement since we're passing multiple objects around
selection_rest_statement
    : 	statement_with_scope T_Else statement_with_scope {}
    | 	statement_with_scope {}
    ;
*/

condition
    : 	expression {$$=$1;}
    | 	fully_specified_type T_Identifier T_Equal initializer {
            $$ = new AssignExpr(
                new VarExpr(@2, new Identifier(@2, $2)),
                new Operator(@3, "=="), $4
            );
        }
    ;

switch_statement
    : 	T_Switch T_LeftParen expression T_RightParen T_LeftBrace statement_list T_RightBrace {
            $$ = new SwitchStmt($3, NULL, NULL);
    };


case_label
    : 	T_Case expression T_Colon {}
    | 	T_Default T_Colon {}
    ;

iteration_statement
    : 	T_While T_LeftParen condition T_RightParen statement_no_new_scope {
            $$ = new WhileStmt($3, $5);
        }
    |	T_Do statement_with_scope T_While T_LeftParen expression T_RightParen T_Semicolon {
            $$ = new DoWhileStmt($2, $5);
        }
    | 	T_For T_LeftParen for_init_statement conditionopt T_Semicolon T_RightParen statement_no_new_scope {
            $$ = new ForStmt($3, $4, NULL, $7);
        }
    |   T_For T_LeftParen for_init_statement conditionopt T_Semicolon expression T_RightParen statement_no_new_scope {
            $$ = new ForStmt($3, $4, $6, $8);
        }
    ;

for_init_statement
    : 	expression_statement {$$=$1;}
    | 	declaration_statement {$$=$1;}
    ;

conditionopt
    :	condition {}
    |	/* empty */ {}
    ;
/* simplified into iteration_statement
for_rest_statement
    : 	conditionopt T_Semicolon {}
    | 	conditionopt T_Semicolon expression {}
    ;
*/
jump_statement
    :	T_Continue T_Semicolon { $$ = new BreakStmt(@1);}
	|	T_Break T_Semicolon {$$ = new BreakStmt(@1);}
	|	T_Return T_Semicolon {$$ = new ReturnStmt(@1, new EmptyExpr());}
	|	T_Return expression T_Semicolon {$$ = new ReturnStmt(@1, $2);}
	;


translation_unit
    : 	external_declaration {($$ = new List<Decl*>)->Append($1);}
    | 	translation_unit external_declaration  {($$ = $1)->Append($2);}
    ;

external_declaration
    : 	function_definition {$$ = $1;}
    | 	declaration {$$ = $1;}
    ;

function_definition
    : 	function_prototype compound_statement {$1->SetFunctionBody($2); $$ = $1;};





%%

/* The closing %% above marks the end of the Rules section and the beginning
 * of the User Subroutines section. All text from here to the end of the
 * file is copied verbatim to the end of the generated y.tab.c file.
 * This section is where you put definitions of helper functions.
 */

/* Function: InitParser
 * --------------------
 * This function will be called before any calls to yyparse().  It is designed
 * to give you an opportunity to do anything that must be done to initialize
 * the parser (set global variables, configure starting state, etc.). One
 * thing it already does for you is assign the value of the global variable
 * yydebug that controls whether yacc prints debugging information about
 * parser actions (shift/reduce) and contents of state stack during parser.
 * If set to false, no information is printed. Setting it to true will give
 * you a running trail that might be helpful when debugging your parser.
 * Please be sure the variable is set to false when submitting your final
 * version.
 */
void InitParser()
{
   PrintDebug("parser", "Initializing parser");
   yydebug = false;
}
