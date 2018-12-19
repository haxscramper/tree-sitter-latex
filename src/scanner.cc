#include <bitset>
#include <string>
#include <vector>

#include "tree_sitter/parser.h"

#include "serialization.hh"
#include "catcode.hh"

namespace LaTeX {

using std::bitset;
using std::string;
using std::vector;

enum SymbolType {
  __ccc_alltt,
  __ccc_at_letter,
  __ccc_at_other,
  __ccc_expl_begin,
  __ccc_expl_end,
  __ccc_l3doc,
  __ccc_luacode,
  __ccc_luadirect,
  __ccc_luaexec,
  __ccc_pipe_verb_delim,
  _cs_begin,
  _cs_end,
  _escaped_begin,
  _escaped_end,
  _scope_begin,
  _scope_end,
  _space,
  active_char,
  alignment_tab,
  arara_comment,
  bib_comment,
  BVerbatim_body,
  BVerbatimstar_body,
  comment_body,
  comment,
  delete_verb_delim,
  eol,
  exit,
  filecontents_body,
  filecontentsstar_body,
  l,
  lstlisting_body,
  luacodestar_body,
  LVerbatim_body,
  LVerbatimstar_body,
  magic_comment,
  make_verb_delim,
  math_shift,
  minted_body,
  parameter_char,
  r,
  short_verb_delim,
  subscript,
  superscript,
  tag_comment,
  verb_body,
  verb_delim,
  verbatim_body,
  Verbatim_body,
  verbatimstar_body,
  Verbatimstar_body,
};

struct CatCodeCommand {
  SymbolType symbol;
  bool global;
  vector<CatCodeInterval> intervals;
};

struct VerbatimEnv {
  SymbolType symbol;
  string terminator;

  VerbatimEnv(SymbolType sym, string name) {
    symbol = sym;
    terminator = " \\end {" + name + "}";
  }
};

enum ScannerMode: uint8_t {
  CS_MODE,
  ESCAPED_MODE,
  NORMAL_MODE,
  VERB_MODE
};

struct Scanner {
  vector<CatCodeCommand> catcode_commands = {
    { // alltt catcode table
      __ccc_alltt,
      false,
      {
        {
          {'\t',   '\t',   OTHER_CATEGORY},
          {' ',    ' ',    OTHER_CATEGORY},
          {'#',   '#',     OTHER_CATEGORY},
          {'$',   '$',     OTHER_CATEGORY},
          {'%',   '%',     OTHER_CATEGORY},
          {'&',   '&',     OTHER_CATEGORY},
          {'A',   'Z',     LETTER_CATEGORY},
          {'\\',  '\\',    ESCAPE_CATEGORY},
          {'^',   '^',     OTHER_CATEGORY},
          {'_',   '_',     OTHER_CATEGORY},
          {'a',   'z',     LETTER_CATEGORY},
          {'{',   '{',     BEGIN_CATEGORY},
          {'}',   '}',     END_CATEGORY},
          {'~',   '~',     OTHER_CATEGORY}
        }
      }
    },
    {
      __ccc_at_letter,
      false,
      {
        {
          {'@',   '@',   LETTER_CATEGORY}
        }
      }
    },
    {
      __ccc_at_other,
      false,
      {
        {
          {'@',   '@',   OTHER_CATEGORY}
        }
      }
    },
    {
      __ccc_expl_begin,
      false,
      {
        {
          {'\t',   '\t',   IGNORED_CATEGORY},
          {' ',    ' ',    IGNORED_CATEGORY},
          {'"',    '"',    OTHER_CATEGORY},
          {'&',    '&',    ALIGNMENT_TAB_CATEGORY},
          {':',    ':',    LETTER_CATEGORY},
          {'^',    '^',    SUPERSCRIPT_CATEGORY},
          {'_',    '_',    LETTER_CATEGORY},
          {'|',    '|',    OTHER_CATEGORY},
          {'~',    '~',    SPACE_CATEGORY}
        }
      }
    },
    { // This the default action for \ExplSyntaxOff. It will be overridden by the call to \ExplSyntaxOn.
      __ccc_expl_end,
      false,
      {
        {
          {'\t',   '\t',   SPACE_CATEGORY},
          {' ',    ' ',    SPACE_CATEGORY},
          {'"',    '"',    OTHER_CATEGORY},
          {'&',    '&',    ALIGNMENT_TAB_CATEGORY},
          {':',    ':',    OTHER_CATEGORY},
          {'^',    '^',    SUPERSCRIPT_CATEGORY},
          {'_',    '_',    SUBSCRIPT_CATEGORY},
          {'|',    '|',    OTHER_CATEGORY},
          {'~',    '~',    ACTIVE_CHAR_CATEGORY}
        }
      }
    },
    { // Catcodes for the l3doc class
      __ccc_l3doc,
      true,
      {
        {
          {'"',   '"',     VERB_DELIM_EXT_CATEGORY},
          {'|',   '|',     VERB_DELIM_EXT_CATEGORY}
        }
      }
    },
    { // luacode catcode table
      __ccc_luacode,
      false,
      {
        {
          {1,      '@',    OTHER_CATEGORY},
          {'A',    'Z',    LETTER_CATEGORY},
          {'[',    '[',    OTHER_CATEGORY},
          {'\\',   '\\',   ESCAPE_CATEGORY},
          {']',    '`',    OTHER_CATEGORY},
          {'a',    'z',    LETTER_CATEGORY},
          {'{',    '{',    BEGIN_CATEGORY},
          {'|',    '|',    OTHER_CATEGORY},
          {'}',    '}',    END_CATEGORY},
          {'~',    '~',    OTHER_CATEGORY},
          {'\x7f', '\x7f', INVALID_CATEGORY}
        }
      }
    },
    { // \luadirect catcode table
      __ccc_luadirect,
      false,
      {
        {
          {1,      9,      EOL_CATEGORY},
          {'\n',   '\n',   EOL_CATEGORY},
          {11,     '$',    OTHER_CATEGORY},
          {'%',    '%',    COMMENT_CATEGORY},
          {'&',    '@',    OTHER_CATEGORY},
          {'A',    'Z',    LETTER_CATEGORY},
          {'[',    '[',    OTHER_CATEGORY},
          {'\\',   '\\',   ESCAPE_CATEGORY},
          {']',    '`',    OTHER_CATEGORY},
          {'a',    'z',    LETTER_CATEGORY},
          {'{',    '{',    BEGIN_CATEGORY},
          {'|',    '|',    OTHER_CATEGORY},
          {'}',    '}',    END_CATEGORY},
          {'~',    '~',    ACTIVE_CHAR_CATEGORY},
          {'\x7f', '\x7f', INVALID_CATEGORY}
        }
      }
    },
    { // luaexec catcode table
      __ccc_luaexec,
      false,
      {
        {
          {1,      9,      EOL_CATEGORY},
          {'\n',   '\n',   EOL_CATEGORY},
          {11,     '$',    OTHER_CATEGORY},
          {'%',    '%',    COMMENT_CATEGORY},
          {'&',    '@',    OTHER_CATEGORY},
          {'A',    'Z',    LETTER_CATEGORY},
          {'[',    '[',    OTHER_CATEGORY},
          {'\\',   '\\',   ESCAPE_CATEGORY},
          {']',    '`',    OTHER_CATEGORY},
          {'a',    'z',    LETTER_CATEGORY},
          {'{',    '{',    BEGIN_CATEGORY},
          {'|',    '|',    OTHER_CATEGORY},
          {'}',    '}',    END_CATEGORY},
          {'~',    '~',    OTHER_CATEGORY},
          {'\x7f', '\x7f', INVALID_CATEGORY}
        }
      }
    },
    {
      __ccc_pipe_verb_delim,
      true,
      {
        {
          {'|',   '|',     VERB_DELIM_EXT_CATEGORY}
        }
      }
    },
  };

  ScannerMode mode = NORMAL_MODE;
  int32_t start_delim = 0;
  CatCodeTable catcode_table = {
    {' ',    ' ',    SPACE_CATEGORY},
    {'_',    '_',    SUBSCRIPT_CATEGORY},
    {'{',    '{',    BEGIN_CATEGORY},
    {'}',    '}',    END_CATEGORY},
    {'\\',   '\\',   ESCAPE_CATEGORY},
    // NUL is technically ignored, but tree sitter seems to use it to indicate
    // EOF.
    // {'\0',   '\0',   IGNORED_CATEGORY},
    {'\n',   '\n',   EOL_CATEGORY},
    {'\t',   '\t',   SPACE_CATEGORY},
    {'\x7f', '\x7f', INVALID_CATEGORY},
    {'&',    '&',    ALIGNMENT_TAB_CATEGORY},
    {'#',    '#',    PARAMETER_CATEGORY},
    {'%',    '%',    COMMENT_CATEGORY},
    {'^',    '^',    SUPERSCRIPT_CATEGORY},
    {'~',    '~',    ACTIVE_CHAR_CATEGORY},
    {'$',    '$',    MATH_SHIFT_CATEGORY},
    {'a',    'z',    LETTER_CATEGORY},
    {'A',    'Z',    LETTER_CATEGORY},
  };

  vector<VerbatimEnv> verbatims = {
    {BVerbatim_body, "BVerbatim"},
    {BVerbatimstar_body, "BVerbatim*"},
    {comment_body, "comment"},
    {filecontents_body, "filecontents"},
    {filecontentsstar_body, "filecontents*"},
    {lstlisting_body, "lstlisting"},
    {luacodestar_body, "luacode*"},
    {LVerbatim_body, "LVerbatim"},
    {LVerbatimstar_body, "LVerbatim*"},
    {minted_body, "minted"},
    {verbatim_body, "verbatim"},
    {Verbatim_body, "Verbatim"},
    {verbatimstar_body, "verbatim*"},
    {Verbatimstar_body, "Verbatim*"}
  };

  Scanner() {}

  void reset () {
    mode = NORMAL_MODE;
    start_delim = 0;
    catcode_table.reset();
  }

  unsigned serialize(char *buffer) const {
    SerializationBuffer buf(buffer);

    buf << mode << start_delim << catcode_table;

    return buf.length;
  }

  void deserialize(const char *buffer, unsigned length) {
    reset();

    if (length == 0) {
      return;
    }

    DeserializationBuffer buf(buffer, length);

    buf >> mode >> start_delim >> catcode_table;
  }

  bool scan_start_verb_delim(TSLexer *lexer, SymbolType symbol) {
    // NOTE: ' ' (space) is a perfectly valid delim, as is %
    // Also: The first * (if present) is gobbled by the main grammar, but the second is a valid delim
    if (lexer->lookahead && catcode_table[lexer->lookahead] != EOL_CATEGORY) {
      mode = VERB_MODE;
      start_delim = lexer->lookahead;
      lexer->advance(lexer, false);
      lexer->result_symbol = symbol;
      lexer->mark_end(lexer);
      return true;
    }

    return false;
  }

  bool scan_end_verb_delim(TSLexer *lexer) {
    if (lexer->lookahead == start_delim) {
      mode = NORMAL_MODE;
      lexer->advance(lexer, false);
      lexer->result_symbol = verb_delim;
      lexer->mark_end(lexer);
      return true;
    }

    if (catcode_table[lexer->lookahead] == EOL_CATEGORY) {
      mode = NORMAL_MODE;
      lexer->result_symbol = verb_delim; // don't eat the newline (for consistency)
      lexer->mark_end(lexer);
      return true;
    }

    return false;
  }

  bool scan_verb_body(TSLexer *lexer) {
    while (lexer->lookahead && lexer->lookahead != start_delim && catcode_table[lexer->lookahead] != EOL_CATEGORY) {
      lexer->advance(lexer, false);
    }

    lexer->result_symbol = verb_body;
    lexer->mark_end(lexer);

    return true;
  }

  int match_length(TSLexer *lexer, string value, bitset<16> terminator = ~0) {
    size_t length = 0;

    for (char ch: value) {
      if (std::tolower(lexer->lookahead) == ch) {
        length++;
        lexer->advance(lexer, false);
      } else {
        return length;
      }
    }

    return (terminator[catcode_table[lexer->lookahead]]) ? -1 : length;
  }

  bool matches_string(TSLexer *lexer, string value) {
    for (char ch: value) {
      switch (ch) {
        case '\\':
          if (catcode_table[lexer->lookahead] != ESCAPE_CATEGORY) {
            return false;
          }
          lexer->advance(lexer, false);
          break;
        case '{':
          if (catcode_table[lexer->lookahead] != BEGIN_CATEGORY) {
            return false;
          }
          lexer->advance(lexer, false);
          break;
        case '}':
          if (catcode_table[lexer->lookahead] != END_CATEGORY) {
            return false;
          }
          lexer->advance(lexer, false);
          break;
        case ' ':
          while (lexer->lookahead && catcode_table[lexer->lookahead] == SPACE_CATEGORY) {
            lexer->advance(lexer, false);
          }
          break;
        default:
          if (lexer->lookahead != ch) {
            return false;
          }
          lexer->advance(lexer, false);
          break;
      }
    }

    return true;
  }

  bool scan_verbatim_body(TSLexer *lexer, const bool *valid_symbols) {
    for (const VerbatimEnv& env: verbatims) {
      if (valid_symbols[env.symbol]) {

        lexer->mark_end(lexer);
        lexer->result_symbol = env.symbol;

        do {
          if (matches_string(lexer, env.terminator)) {
            return true;
          }

          while (lexer->lookahead && catcode_table[lexer->lookahead] != EOL_CATEGORY) {
            lexer->advance(lexer, false);
          }

          if (catcode_table[lexer->lookahead] == EOL_CATEGORY) {
            lexer->advance(lexer, false);
          }

          lexer->mark_end(lexer);
        } while (lexer->lookahead);

        return true;
      }
    }

    return false;
  }

  bool scan_comment(TSLexer *lexer) {
    // bitset<16> comment_categories = ~(EOL_FLAG | IGNORED_FLAG);
    string comment_type;

    // Skip the comment char
    lexer->advance(lexer, false);
    lexer->result_symbol = comment;

    if (match_length(lexer, ":") == -1) {
      lexer->advance(lexer, false);
      lexer->result_symbol = tag_comment;
    } else {
      // Skip any leading spaces
      while (lexer->lookahead && catcode_table[lexer->lookahead] == SPACE_CATEGORY) {
        lexer->advance(lexer, false);
      }

      int len = match_length(lexer, "arara:");

      if (len == -1) {
        lexer->result_symbol = arara_comment;
      } else if (len == 0) {
        len = match_length(lexer, "!tex", EOL_FLAG | SPACE_FLAG | IGNORED_FLAG);
        if (len == -1) {
          lexer->result_symbol = magic_comment;
        } else if (len == 1 && match_length(lexer, "bib", EOL_FLAG | SPACE_FLAG | IGNORED_FLAG) == -1) {
          lexer->result_symbol = bib_comment;
        }
      }
    }

    // Gobble the reset of the comment
    while (lexer->lookahead && catcode_table[lexer->lookahead] != EOL_CATEGORY) {
      lexer->advance(lexer, false);
    }

    // Eat any EOL
    if (catcode_table[lexer->lookahead] == EOL_CATEGORY) {
      lexer->advance(lexer, false);
    }

    lexer->mark_end(lexer);

    return true;
  }

  bool scan_catcode_commands(TSLexer *lexer, const bool *valid_symbols) {
    // Loop through the command list.
    for (const CatCodeCommand& cmd: catcode_commands) {
      if (valid_symbols[cmd.symbol]) {
        lexer->result_symbol = cmd.symbol;
        lexer->mark_end(lexer);

        catcode_table.assign(cmd.intervals, cmd.global);

        return true;
      }
    }

    return false;
  }

  bool scan_cs_mode(TSLexer *lexer, const bool *valid_symbols) {
    if (valid_symbols[_cs_end] && catcode_table[lexer->lookahead] != LETTER_CATEGORY) {
      mode = NORMAL_MODE;
      lexer->result_symbol = _cs_end;
      lexer->mark_end(lexer);
      return true;
    }

    return false;
  }

  bool scan_escaped_mode(TSLexer *lexer, const bool *valid_symbols) {
    if (valid_symbols[_escaped_end]) {
      mode = NORMAL_MODE;
      lexer->result_symbol = _escaped_end;
      lexer->mark_end(lexer);
      return true;
    }

    return false;
  }

  bool scan_escape(TSLexer *lexer) {
    lexer->advance(lexer, false);

    if (catcode_table[lexer->lookahead] == LETTER_CATEGORY) {
      mode = CS_MODE;
      lexer->result_symbol = _cs_begin;
    } else {
      mode = ESCAPED_MODE;
      lexer->result_symbol = _escaped_begin;
    }
    lexer->mark_end(lexer);

    return true;
  }

  bool scan_verb_mode(TSLexer *lexer, const bool *valid_symbols) {
    // Look for an inline verbatim delimiter and end the verbatim.
    if (valid_symbols[verb_delim]) {
      return scan_end_verb_delim(lexer);
    }

    // Scan an inline verbatim body.
    if (valid_symbols[verb_body]) {
      return scan_verb_body(lexer);
    }

    return false;
  }

  inline bool scan_empty_symbol(TSLexer *lexer, SymbolType symbol) {
    lexer->result_symbol = symbol;
    lexer->mark_end(lexer);

    return true;
  }

  inline bool scan_single_char_symbol(TSLexer *lexer, SymbolType symbol) {
    lexer->advance(lexer, false);

    lexer->result_symbol = symbol;
    lexer->mark_end(lexer);

    return true;
  }

  inline bool scan_multi_char_symbol(TSLexer *lexer, SymbolType symbol, Category code) {
    do {
      lexer->advance(lexer, false);
    } while (lexer->lookahead && catcode_table[lexer->lookahead] == code);

    lexer->result_symbol = symbol;
    lexer->mark_end(lexer);

    return true;
  }

  bool scan_space(TSLexer *lexer) {
    Category code = catcode_table[lexer->lookahead];
    bool eol = false;

    do {
      if (code == EOL_CATEGORY) {
        // If we are ready have an EOL then do not scan as space since this is
        // a paragraph break.
        if (eol) {
          return false;
        }
        eol = true;
      }

      lexer->advance(lexer, false);
      code = catcode_table[lexer->lookahead];
    } while (lexer->lookahead && (code == SPACE_CATEGORY || code == EOL_CATEGORY));

    lexer->result_symbol = _space;
    lexer->mark_end(lexer);

    return true;
  }

  bool scan_make_verb_delim(TSLexer *lexer) {
    lexer->advance(lexer, false);

    catcode_table.assign(lexer->lookahead, VERB_DELIM_EXT_CATEGORY, true);
    lexer->advance(lexer, false);

    lexer->result_symbol = make_verb_delim;
    lexer->mark_end(lexer);

    return true;
  }

  bool scan_delete_verb_delim(TSLexer *lexer) {
    lexer->advance(lexer, false);

    catcode_table.erase(lexer->lookahead, true);
    lexer->advance(lexer, false);

    lexer->result_symbol = delete_verb_delim;
    lexer->mark_end(lexer);

    return true;
  }

  bool scan_normal_mode(TSLexer *lexer, const bool *valid_symbols) {
    Category code = catcode_table[lexer->lookahead];

    bool res = scan_catcode_commands(lexer, valid_symbols);
    if (res) return true;

    // Look for an inline verbatim delimiter and start VERB_MODE.
    if (valid_symbols[verb_delim]) {
      return scan_start_verb_delim(lexer, verb_delim);
    }

    res = scan_verbatim_body(lexer, valid_symbols);
    if (res) return true;

    if (valid_symbols[_scope_begin]) {
      catcode_table.push();
      lexer->mark_end(lexer);
      lexer->result_symbol = _scope_begin;
      return true;
    }

    if (valid_symbols[_scope_end]) {
      catcode_table.pop();
      lexer->mark_end(lexer);
      lexer->result_symbol = _scope_end;
      return true;
    }

    if (lexer->lookahead == 0 && valid_symbols[exit]) {
      return scan_empty_symbol(lexer, exit);
    }

    switch (code) {
      case ESCAPE_CATEGORY:
        if (valid_symbols[make_verb_delim]) {
          return scan_make_verb_delim(lexer);
        }
        if (valid_symbols[delete_verb_delim]) {
          return scan_delete_verb_delim(lexer);
        }
        if (valid_symbols[_cs_begin] || valid_symbols[_escaped_begin]) {
          return scan_escape(lexer);
        }
        break;
      case BEGIN_CATEGORY:
        if (valid_symbols[l]) {
          return scan_single_char_symbol(lexer, l);
        }
        break;
      case END_CATEGORY:
        if (valid_symbols[exit]) {
          return scan_empty_symbol(lexer, exit);
        } else if (valid_symbols[r]) {
          return scan_single_char_symbol(lexer, r);
        }
        break;
      case MATH_SHIFT_CATEGORY:
        if (valid_symbols[math_shift]) {
          return scan_single_char_symbol(lexer, math_shift);
        }
        break;
      case ALIGNMENT_TAB_CATEGORY:
        if (valid_symbols[alignment_tab]) {
          return scan_single_char_symbol(lexer, alignment_tab);
        }
        break;
      case EOL_CATEGORY:
        if (valid_symbols[eol]) {
          return scan_single_char_symbol(lexer, eol);
        }
        if (valid_symbols[_space]) {
          return scan_space(lexer);
        }
        break;
      case PARAMETER_CATEGORY:
        if (valid_symbols[parameter_char]) {
          return scan_multi_char_symbol(lexer, parameter_char, PARAMETER_CATEGORY);
        }
        break;
      case SUPERSCRIPT_CATEGORY:
        if (valid_symbols[superscript]) {
          return scan_single_char_symbol(lexer, superscript);
        }
        break;
      case SUBSCRIPT_CATEGORY:
        if (valid_symbols[subscript]) {
          return scan_single_char_symbol(lexer, subscript);
        }
        break;
      // case IGNORED_CATEGORY:
      //   do {
      //     lexer->advance(lexer, true);
      //   } while (lexer->lookahead && catcode_table[lexer->lookahead] == IGNORED_CATEGORY);
      //   return false;
      case SPACE_CATEGORY:
        if (valid_symbols[_space]) {
          return scan_space(lexer);
        }
        break;
      case ACTIVE_CHAR_CATEGORY:
        if (valid_symbols[active_char]) {
          return scan_single_char_symbol(lexer, active_char);
        }
        break;
      case COMMENT_CATEGORY:
        if (valid_symbols[comment]) {
          return scan_comment(lexer);
        }
        break;
      case VERB_DELIM_EXT_CATEGORY:
        if (valid_symbols[short_verb_delim]) {
          return scan_start_verb_delim(lexer, short_verb_delim);
        }
        break;
      default:
        break;
    }

    // Look for catcode commands.
    return false;
  }

  bool scan(TSLexer *lexer, const bool *valid_symbols) {
    switch (mode) {
      case CS_MODE:
        return scan_cs_mode(lexer, valid_symbols);
      case ESCAPED_MODE:
        return scan_escaped_mode(lexer, valid_symbols);
      case VERB_MODE:
        return scan_verb_mode(lexer, valid_symbols);
      default:
        return scan_normal_mode(lexer, valid_symbols);
    }
  }

};

}


// Source for required functions:
// https://github.com/tree-sitter/tree-sitter/blob/78b54810a62580537b0a2df7eb781e03667d63ba/src/compiler/generate_code/c_code.cc#L546-L552

extern "C" {

void *tree_sitter_latex_external_scanner_create() {
  return new LaTeX::Scanner();
}

bool tree_sitter_latex_external_scanner_scan(void *payload, TSLexer *lexer, const bool *valid_symbols) {
  LaTeX::Scanner *scanner = static_cast<LaTeX::Scanner *>(payload);
  return scanner->scan(lexer, valid_symbols);
}

unsigned tree_sitter_latex_external_scanner_serialize(void *payload, char *buffer) {
  LaTeX::Scanner *scanner = static_cast<LaTeX::Scanner *>(payload);
  return scanner->serialize(buffer);
}

void tree_sitter_latex_external_scanner_deserialize(void *payload, const char *buffer, unsigned length) {
  LaTeX::Scanner *scanner = static_cast<LaTeX::Scanner *>(payload);
  scanner->deserialize(buffer, length);
}

void tree_sitter_latex_external_scanner_destroy(void *payload) {
  LaTeX::Scanner *scanner = static_cast<LaTeX::Scanner *>(payload);
  delete scanner;
}

}
