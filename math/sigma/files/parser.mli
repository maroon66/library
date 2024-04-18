type token =
  | LPAREN
  | RPAREN
  | LBRACKET
  | RBRACKET
  | SUM
  | PLUS
  | MINUS
  | MUL
  | DIV
  | POW
  | LESS_EQUAL
  | GREATER_EQUAL
  | LESS
  | GREATER
  | EQUAL
  | BAR
  | COMMA
  | AND
  | OR
  | AT
  | IDENT of (string)
  | CONST of (int)
  | EOF

val prog :
  (Lexing.lexbuf  -> token) -> Lexing.lexbuf -> Syntax.t * Syntax.t list
