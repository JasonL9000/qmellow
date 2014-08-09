#include "repl.h"

#include <cctype>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <utility>
#include "lexer.h"
#include "parser.h"

using namespace std;
using namespace qmellow;

repl_t::repl_t(const vector<string> &paths) {
  /* Load the files. */
  for (const auto &path: paths) {
    files.emplace_back(path);
  }
}

int repl_t::operator()() {
  /* Read-evaluate-print loop. */
  for (;;) {
    /* Get an expression from the user.  If we don't get one, it's time to
       exit. */
    auto expr = prompt_for_expr();
    if (!expr) {
      break;
    }
    /* Evaluate the expression against each file. */
    for (const auto &file: files) {
      cout << file.get_path() << ':' << endl;
      expr->eval(file).write(cout);
    }
  }
  return EXIT_SUCCESS;
}

unique_ptr<expr_t> repl_t::prompt_for_expr() {
  /* Loop until we get an expression from the user. */
  unique_ptr<expr_t> expr;
  for (;;) {
    try {
      /* Try to get some tokens.  If we get some, parse them into an
         expression and exit the loop. */
      auto tokens = prompt_for_tokens();
      if (!tokens.empty()) {
        expr = parser_t::parse(tokens.data());
        break;
      }
    } catch (exception &ex) {
      /* There was an error.  Show it to the user and let him try again. */
      cout << ex.what() << endl;
    }
  }  // for
  return move(expr);
}

vector<token_t> repl_t::prompt_for_tokens() {
  vector<token_t> tokens;
  pos_t pos;
  int paren_depth = 0;
  bool is_ext;
  do {
    /* Get a line from the user. */
    is_ext = false;
    cout << (tokens.empty() ? "qmellow: " : "...    : ");
    string line;
    getline(cin, line);
    /* Trim trailing whitespace. */
    while (!line.empty() && isspace(line.back())) {
      line.pop_back();
    }
    /* If the line's empty, the user wants to abort his script.  Drop any
       tokens we have so far and get out. */
    if (line.empty()) {
      tokens.clear();
      break;
    }
    /* If the line ends with a backslash, remove it, but remember that the
       user wants to extend the line. */
    if (line.back() == '\\') {
      line.pop_back();
      is_ext = true;
    }
    /* Lex the line into tokens.  Scan through these tokens and adjust our
       parenthesis depth appropriately. */
    auto more_tokens = lexer_t::lex(line, pos);
    for (const auto &token: more_tokens) {
      switch (token.get_kind()) {
        case token_t::open_paren: {
          ++paren_depth;
          break;
        }
        case token_t::close_paren: {
          if (paren_depth) {
            --paren_depth;
          }
          break;
        }
        default: {
          // do-nothing
        }
      }  // switch
    }  // for
    /* If we are inside parentheses, extend the line automatically. */
    if (paren_depth) {
      is_ext = true;
    }
    /* If we're extending the line, remember the position of the end token,
       then get rid of it.  We'll continue from this position with the next
       line of source. */
    if (is_ext) {
      pos = more_tokens.back().get_pos();
      pos.next_line();
      more_tokens.pop_back();
    }
    /* Append the tokens from this line to the collection of tokens we have
       so far. */
    tokens.insert(tokens.end(), more_tokens.begin(), more_tokens.end());
  }  while (is_ext);
  return move(tokens);
}
