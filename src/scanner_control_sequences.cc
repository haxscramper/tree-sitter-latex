#include "scanner.hh"

namespace LaTeX {

using std::string;
using std::unordered_map;

unordered_map<string, CatCodeCommand> Scanner::control_sequences = {
    // initex
    {"begingroup", cs_begingroup},
    {"bgroup", cs_bgroup},
    {"catcode", cs_code},
    {"def", cs_def},
    {"delcode", cs_code},
    {"edef", cs_def},
    {"egroup", cs_egroup},
    {"endgroup", cs_endgroup},
    {"expandafter", cs_expandafter},
    {"gdef", cs_def},
    {"input", cs_input},
    {"kcatcode", cs_code},
    {"lccode", cs_code},
    {"left", cs_left},
    {"let", cs_let},
    {"mathcode", cs_code},
    {"par", cs_par},
    {"right", cs_right},
    {"sfcode", cs_code},
    {"string", cs_string},
    {"uccode", cs_code},
    {"xdef", cs_def},
    // latex amsmath amsmath-sty
    {"eqref", cs_ref},
    {"tag", cs_tag},
    {"text", cs_text},
    // latex base doc-sty
    {"DoNotIndex", cs_DoNotIndex},
    // latex base latex-ltx
    {"@ifpackagelater", cs_at_ifpackagelater},
    {"(", cs_inline_math_begin},
    {")", cs_inline_math_end},
    {"[", cs_display_math_begin},
    {"]", cs_display_math_end},
    // {"\\", cs_newline},
    {"acute", cs_mathaccent},
    {"AtBeginDocument", cs_At},
    {"AtBeginDvi", cs_At},
    {"AtEndDocument", cs_At},
    {"AtEndOfClass", cs_At},
    {"AtEndOfPackage", cs_At},
    {"author", cs_author},
    {"bar", cs_mathaccent},
    {"begin", cs_begin},
    {"bibitem", cs_bibitem},
    {"breve", cs_mathaccent},
    {"chapter", cs_section},
    {"check", cs_mathaccent},
    {"CheckCommand", cs_CheckCommand},
    {"ClassError", cs_Error},
    {"ClassInfo", cs_WarningInfo},
    {"ClassInfoNoLine", cs_WarningInfo},
    {"ClassWarning", cs_WarningInfo},
    {"ClassWarningNoLine", cs_WarningInfo},
    {"cline", cs_cline},
    {"date", cs_date},
    {"ddot", cs_mathaccent},
    {"DeclareOption", cs_DeclareOption},
    {"DeclareRobustCommand", cs_newcommand},
    {"discretionary", cs_discretionary},
    {"documentclass", cs_use},
    {"documentstyle", cs_use_209},
    {"dot", cs_mathaccent},
    {"emph", cs_emph},
    {"end", cs_end},
    {"endinput", cs_endinput},
    {"enlargethispage", cs_enlargethispage},
    {"ensuremath", cs_ensuremath},
    {"ExecuteOptions", cs_ExecuteOptions},
    {"fbox", cs_mbox},
    {"footnote", cs_footnote},
    {"footnotemark", cs_footnotemark},
    {"footnotetext", cs_footnote},
    {"frac", cs_frac},
    {"framebox", cs_makebox},
    {"grave", cs_mathaccent},
    {"hat", cs_mathaccent},
    {"hyphenation", cs_hyphenation},
    {"IfFileExists", cs_IfFileExists},
    {"include", cs_input},
    {"InputIfFileExists", cs_IfFileExists},
    {"item", cs_item},
    {"label", cs_label},
    {"linebreak", cs_linebreak},
    {"LoadClass", cs_use},
    {"LoadClassWithOptions", cs_use},
    {"makeatletter", {cs, false, {{'@', '@', LETTER_CATEGORY}}}},
    {"makeatother", {cs, false, {{'@', '@', OTHER_CATEGORY}}}},
    {"makebox", cs_makebox},
    {"marginpar", cs_marginpar},
    {"mathbf", cs_mathstyle},
    {"mathcal", cs_mathstyle},
    {"mathit", cs_mathstyle},
    {"mathnormal", cs_mathstyle},
    {"mathring", cs_mathaccent},
    {"mathrm", cs_mathstyle},
    {"mathsf", cs_mathstyle},
    {"mathtt", cs_mathstyle},
    {"mbox", cs_mbox},
    {"mit", cs_mathstyle},
    {"multicolumn", cs_multicolumn},
    {"NeedsTeXFormat", cs_NeedsTeXFormat},
    {"newcommand", cs_newcommand},
    {"newcounter", cs_newcounter},
    {"newenvironment", cs_newenvironment},
    {"newfont", cs_newfont},
    {"newlength", cs_newlength},
    {"newline", cs_newline},
    {"newsavebox", cs_newsavebox},
    {"newtheorem", cs_newtheorem},
    {"nolinebreak", cs_linebreak},
    {"nopagebreak", cs_pagebreak},
    {"obeycr", {cs_obeycr, false, {{'\n', '\n', ACTIVE_CHAR_CATEGORY}}}},
    {"PackageError", cs_Error},
    {"PackageInfo", cs_WarningInfo},
    {"PackageInfoNoLine", cs_WarningInfo},
    {"PackageWarning", cs_WarningInfo},
    {"PackageWarningNoLine", cs_WarningInfo},
    {"pagebreak", cs_pagebreak},
    {"paragraph", cs_section},
    {"parbox", cs_parbox},
    {"part", cs_section},
    {"PassOptionsToClass", cs_PassOptionsTo},
    {"PassOptionsToPackage", cs_PassOptionsTo},
    {"ProcessOptions", cs_ProcessOptions},
    {"providecommand", cs_newcommand},
    {"ProvidesClass", {cs_Provides, false, {{'@', '@', LETTER_CATEGORY}}}},
    {"ProvidesFile", {cs_Provides, false, {{'@', '@', LETTER_CATEGORY}}}},
    {"ProvidesPackage", {cs_Provides, false, {{'@', '@', LETTER_CATEGORY}}}},
    {"ref", cs_ref},
    {"relax", cs_relax},
    {"renewcommand", cs_newcommand},
    {"renewenvironment", cs_newenvironment},
    {"RequirePackage", cs_use},
    {"RequirePackageWithOptions", cs_use},
    {"restorecr", {cs_obeycr, false, {{'\n', '\n', EOL_CATEGORY}}}},
    {"section", cs_section},
    {"setlength", cs_setlength},
    {"sqrt", cs_sqrt},
    {"stackrel", cs_stackrel},
    {"subparagraph", cs_section},
    {"subsection", cs_section},
    {"subsubsection", cs_section},
    {"textbf", cs_textstyle},
    {"textit", cs_textstyle},
    {"textmd", cs_textstyle},
    {"textnormal", cs_textstyle},
    {"textrm", cs_textstyle},
    {"textsc", cs_textstyle},
    {"textsf", cs_textstyle},
    {"textsl", cs_textstyle},
    {"texttt", cs_textstyle},
    {"textup", cs_textstyle},
    {"thanks", cs_thanks},
    {"tilde", cs_mathaccent},
    {"title", cs_title},
    {"usepackage", cs_use},
    {"vec", cs_mathaccent},
    {"verb", cs_verb},
    {"widehat", cs_mathaccent},
    {"widetilde", cs_mathaccent},
    // latex base shortvrb-sty
    {"DeleteShortVerb", cs_DeleteShortVerb},
    {"MakeShortVerb", cs_MakeShortVerb},
    // latex biblatex biblatex-sty
    {"autocite", cs_cite},
    {"Autocite", cs_cite},
    {"autocites", cs_cites},
    {"Autocites", cs_cites},
    {"avolcite", cs_volcite},
    {"Avolcite", cs_volcite},
    {"avolcites", cs_volcites},
    {"Avolcites", cs_volcites},
    {"cite", cs_cite},
    {"Cite", cs_cite},
    {"citeauthor", cs_cite},
    {"Citeauthor", cs_cite},
    {"citedate", cs_cite},
    {"cites", cs_cites},
    {"Cites", cs_cites},
    {"citetitle", cs_cite},
    {"citeurl", cs_cite},
    {"citeyear", cs_cite},
    {"Citeyear", cs_cite},
    {"fnotecite", cs_cite},
    {"footcite", cs_cite},
    {"footcites", cs_cites},
    {"footcitetext", cs_cite},
    {"footcitetexts", cs_cites},
    {"footfullcite", cs_cite},
    {"fref", cs_fref},
    {"Fref", cs_fref},
    {"ftvolcite", cs_volcite},
    {"Ftvolcite", cs_volcite},
    {"ftvolcites", cs_volcites},
    {"Ftvolcites", cs_volcites},
    {"fullcite", cs_cite},
    {"fvolcite", cs_volcite},
    {"Fvolcite", cs_volcite},
    {"fvolcites", cs_volcites},
    {"Fvolcites", cs_volcites},
    {"nocite", cs_nocite},
    {"nolinkurl", cs_url},
    {"notecite", cs_cite},
    {"Notecite", cs_cite},
    {"nptextcite", cs_cite},
    {"pageref", cs_ref},
    {"parencite", cs_cite},
    {"Parencite", cs_cite},
    {"parencites", cs_cites},
    {"Parencites", cs_cites},
    {"pnotecite", cs_cite},
    {"Pnotecite", cs_cite},
    {"pvolcite", cs_volcite},
    {"Pvolcite", cs_volcite},
    {"pvolcites", cs_volcites},
    {"Pvolcites", cs_volcites},
    {"regexp", cs_regexp},
    {"svolcite", cs_volcite},
    {"Svolcite", cs_volcite},
    {"svolcites", cs_volcites},
    {"Svolcites", cs_volcites},
    {"tvolcite", cs_volcite},
    {"Tvolcite", cs_volcite},
    {"tvolcites", cs_volcites},
    {"Tvolcites", cs_volcites},
    {"smartcite", cs_cite},
    {"Smartcite", cs_cite},
    {"smartcites", cs_cites},
    {"Smartcites", cs_cites},
    {"shortcite", cs_cite},
    {"volcite", cs_volcite},
    {"Volcite", cs_volcite},
    {"volcites", cs_volcites},
    {"Volcites", cs_volcites},
    {"supercite", cs_cite},
    {"supercites", cs_cites},
    // latex biblatex-chicago biblatex-chicago-sty
    {"citejournal", cs_cite},
    {"Citetitle", cs_cite},
    {"citetitles", cs_cites},
    {"gentextcite", cs_cite},
    {"Gentextcite", cs_cite},
    {"gentextcites", cs_cites},
    {"Gentextcites", cs_cites},
    {"headlesscite", cs_cite},
    {"headlesscites", cs_cites},
    {"headlessfullcite", cs_cite},
    {"shorthandcite", cs_cite},
    {"surnamecite", cs_cite},
    {"surnamecites", cs_cites},
    {"textcite", cs_cite},
    {"Textcite", cs_cite},
    {"textcites", cs_cites},
    {"Textcites", cs_cites},
    // latex cleveref cleveref-sty
    {"cpageref", cs_ref},
    {"Cpageref", cs_ref},
    {"cpagerefrange", cs_refrange},
    {"Cpagerefrange", cs_refrange},
    {"cref", cs_ref},
    {"Cref", cs_ref},
    {"crefrange", cs_refrange},
    {"Crefrange", cs_refrange},
    {"labelcref", cs_ref},
    {"labelcpageref", cs_ref},
    {"lcnamecref", cs_ref},
    {"lcnamecrefs", cs_ref},
    {"namecref", cs_ref},
    {"nameCref", cs_ref},
    {"namecrefs", cs_ref},
    {"nameCrefs", cs_ref},
    // latex fancyvrb fancyvrb-sty
    {"DefineShortVerb", cs_MakeShortVerb},
    {"UndefineShortVerb", cs_DeleteShortVerb},
    // latex glossaries base glossaries-sty
    {"ac", cs_gls_acr},
    {"glsentryname", cs_glsentry},
    {"Glsentryname", cs_glsentry},
    {"glossentryname", cs_glsentry},
    {"Glossentryname", cs_glsentry},
    {"glsentrytext", cs_glsentry},
    {"Glsentrytext", cs_glsentry},
    {"glsentryplural", cs_glsentry},
    {"Glsentryplural", cs_glsentry},
    {"glsentryfirst", cs_glsentry},
    {"Glsentryfirst", cs_glsentry},
    {"glsentryfirstplural", cs_glsentry},
    {"Glsentryfirstplural", cs_glsentry},
    {"glsentrydesc", cs_glsentry},
    {"Glsentrydesc", cs_glsentry},
    {"glossentrydesc", cs_glsentry},
    {"Glossentrydesc", cs_glsentry},
    {"glsentrydescplural", cs_glsentry},
    {"Glsentrydescplural", cs_glsentry},
    {"glsentrysymbol", cs_glsentry},
    {"Glsentrysymbol", cs_glsentry},
    {"glsentryfield", cs_glsentry},
    {"Glsentryfield", cs_glsentry},
    {"glossentrysymbol", cs_glsentry},
    {"Glossentrysymbol", cs_glsentry},
    {"glsentrysymbolplural", cs_glsentry},
    {"Glsentrysymbolplural", cs_glsentry},
    {"glsentryuseri", cs_glsentry},
    {"Glsentryuseri", cs_glsentry},
    {"glsentryuserii", cs_glsentry},
    {"Glsentryuserii", cs_glsentry},
    {"glsentryuseriii", cs_glsentry},
    {"Glsentryuseriii", cs_glsentry},
    {"glsentryuseriv", cs_glsentry},
    {"Glsentryuseriv", cs_glsentry},
    {"glsentryuserv", cs_glsentry},
    {"Glsentryuserv", cs_glsentry},
    {"glsentryuservi", cs_glsentry},
    {"Glsentryuservi", cs_glsentry},
    {"glsentrynumberlist", cs_glsentry},
    {"Glsentrynumberlist", cs_glsentry},
    {"Ac", cs_gls_acr},
    {"acf", cs_gls_acr},
    {"Acf", cs_gls_acr},
    {"acfp", cs_gls_acr},
    {"Acfp", cs_gls_acr},
    {"acl", cs_gls_acr},
    {"Acl", cs_gls_acr},
    {"aclp", cs_gls_acr},
    {"Aclp", cs_gls_acr},
    {"acp", cs_gls_acr},
    {"Acp", cs_gls_acr},
    {"acrfull", cs_gls_acr},
    {"Acrfull", cs_gls_acr},
    {"ACRfull", cs_gls_acr},
    {"acrfullfmt", cs_gls_acr},
    {"acrfullformat", cs_gls_acr},
    {"acrfullpl", cs_gls_acr},
    {"Acrfullpl", cs_gls_acr},
    {"ACRfullpl", cs_gls_acr},
    {"acrlong", cs_gls_acr},
    {"Acrlong", cs_gls_acr},
    {"ACRlong", cs_gls_acr},
    {"acrlongpl", cs_gls_acr},
    {"Acrlongpl", cs_gls_acr},
    {"ACRlongpl", cs_gls_acr},
    {"acrshort", cs_gls_acr},
    {"Acrshort", cs_gls_acr},
    {"ACRshort", cs_gls_acr},
    {"acrshortpl", cs_gls_acr},
    {"Acrshortpl", cs_gls_acr},
    {"ACRshortpl", cs_gls_acr},
    {"acs", cs_gls_acr},
    {"Acs", cs_gls_acr},
    {"acsp", cs_gls_acr},
    {"Acsp", cs_gls_acr},
    {"gls", cs_gls_acr},
    {"Gls", cs_gls_acr},
    {"GLS", cs_gls_acr},
    {"glsdesc", cs_gls_acr},
    {"Glsdesc", cs_gls_acr},
    {"GLSdesc", cs_gls_acr},
    {"glsdisp", cs_glsdisp},
    {"glsfirst", cs_gls_acr},
    {"Glsfirst", cs_gls_acr},
    {"GLSfirst", cs_gls_acr},
    {"glsfirstplural", cs_gls_acr},
    {"Glsfirstplural", cs_gls_acr},
    {"GLSfirstplural", cs_gls_acr},
    {"glslink", cs_glsdisp},
    {"glsname", cs_gls_acr},
    {"Glsname", cs_gls_acr},
    {"GLSname", cs_gls_acr},
    {"glspl", cs_gls_acr},
    {"Glspl", cs_gls_acr},
    {"GLSpl", cs_gls_acr},
    {"glsplural", cs_gls_acr},
    {"Glsplural", cs_gls_acr},
    {"GLSplural", cs_gls_acr},
    {"glssee", cs_glssee},
    {"glssymbol", cs_gls_acr},
    {"Glssymbol", cs_gls_acr},
    {"GLSsymbol", cs_gls_acr},
    {"glstext", cs_gls_acr},
    {"Glstext", cs_gls_acr},
    {"GLStext", cs_gls_acr},
    {"glsuseri", cs_gls_acr},
    {"Glsuseri", cs_gls_acr},
    {"GLSuseri", cs_gls_acr},
    {"glsuserii", cs_gls_acr},
    {"Glsuserii", cs_gls_acr},
    {"GLSuserii", cs_gls_acr},
    {"glsuseriii", cs_gls_acr},
    {"Glsuseriii", cs_gls_acr},
    {"GLSuseriii", cs_gls_acr},
    {"glsuseriv", cs_gls_acr},
    {"Glsuseriv", cs_gls_acr},
    {"GLSuseriv", cs_gls_acr},
    {"glsuserv", cs_gls_acr},
    {"Glsuserv", cs_gls_acr},
    {"GLSuserv", cs_gls_acr},
    {"glsuservi", cs_gls_acr},
    {"Glsuservi", cs_gls_acr},
    {"GLSuservi", cs_gls_acr},
    {"longnewglossaryentry", cs_longnewglossaryentry},
    {"longnewglossaryentry", cs_longnewglossaryentry},
    {"longprovideglossaryentry", cs_longnewglossaryentry},
    {"newacronym", cs_newacronym},
    {"newglossaryentry", cs_newglossaryentry},
    {"provideglossaryentry", cs_newglossaryentry},
    // latex hyoerref hyperref-sty
    {"autopageref", cs_ref},
    {"autoref", cs_ref},
    {"href", cs_href},
    {"hyperbaseurl", cs_hyperbaseurl},
    {"hyperimage", cs_hyperimage},
    {"hyperref", cs_hyperref},
    {"nameref", cs_ref},
    {"url", cs_url},
    // latex koma-script scr-cls
    {"addchap", cs_section},
    {"addpart", cs_section},
    {"addsec", cs_section},
    // latex listings listings-sty
    {"lstinline", cs_lstinline},
    // latex l3kernel expl3-sty
    {"ExplSyntaxOff",
     {cs,
      false,
      {{'\t', '\t', SPACE_CATEGORY},
       {' ', ' ', SPACE_CATEGORY},
       {'"', '"', OTHER_CATEGORY},
       {'&', '&', ALIGNMENT_TAB_CATEGORY},
       {':', ':', OTHER_CATEGORY},
       {'^', '^', SUPERSCRIPT_CATEGORY},
       {'_', '_', SUBSCRIPT_CATEGORY},
       {'|', '|', OTHER_CATEGORY},
       {'~', '~', ACTIVE_CHAR_CATEGORY}}}},
    {"ExplSyntaxOn",
     {cs,
      false,
      {{'\t', '\t', IGNORED_CATEGORY},
       {' ', ' ', IGNORED_CATEGORY},
       {'"', '"', OTHER_CATEGORY},
       {'&', '&', ALIGNMENT_TAB_CATEGORY},
       {':', ':', LETTER_CATEGORY},
       {'^', '^', SUPERSCRIPT_CATEGORY},
       {'_', '_', LETTER_CATEGORY},
       {'|', '|', OTHER_CATEGORY},
       {'~', '~', SPACE_CATEGORY}}}},
    {"ProvidesExplClass",
     {cs,
      false,
      {{'\t', '\t', IGNORED_CATEGORY},
       {' ', ' ', IGNORED_CATEGORY},
       {'"', '"', OTHER_CATEGORY},
       {'&', '&', ALIGNMENT_TAB_CATEGORY},
       {':', ':', LETTER_CATEGORY},
       {'^', '^', SUPERSCRIPT_CATEGORY},
       {'_', '_', LETTER_CATEGORY},
       {'|', '|', OTHER_CATEGORY},
       {'~', '~', SPACE_CATEGORY}}}},
    {"ProvidesExplFile",
     {cs,
      false,
      {{'\t', '\t', IGNORED_CATEGORY},
       {' ', ' ', IGNORED_CATEGORY},
       {'"', '"', OTHER_CATEGORY},
       {'&', '&', ALIGNMENT_TAB_CATEGORY},
       {':', ':', LETTER_CATEGORY},
       {'^', '^', SUPERSCRIPT_CATEGORY},
       {'_', '_', LETTER_CATEGORY},
       {'|', '|', OTHER_CATEGORY},
       {'~', '~', SPACE_CATEGORY}}}},
    {"ProvidesExplPackage",
     {cs,
      false,
      {{'\t', '\t', IGNORED_CATEGORY},
       {' ', ' ', IGNORED_CATEGORY},
       {'"', '"', OTHER_CATEGORY},
       {'&', '&', ALIGNMENT_TAB_CATEGORY},
       {':', ':', LETTER_CATEGORY},
       {'^', '^', SUPERSCRIPT_CATEGORY},
       {'_', '_', LETTER_CATEGORY},
       {'|', '|', OTHER_CATEGORY},
       {'~', '~', SPACE_CATEGORY}}}},
    // latex minted minted-sty
    {"mint", cs_mint},
    {"mintinline", cs_mintinline},
    // latex tools varioref-sty
    {"Ref", cs_ref},
    {"vpageref", cs_ref},
    {"vpagerefrange", cs_refrange},
    {"vref", cs_ref},
    {"Vref", cs_ref},
    {"vrefrange", cs_refrange},
    // lualatex luacode luacode-sty
    {"luadirect",
     {cs_luacode,
      false,
      {{1, 9, EOL_CATEGORY},
       {'\n', '\n', EOL_CATEGORY},
       {11, '$', OTHER_CATEGORY},
       {'%', '%', COMMENT_CATEGORY},
       {'&', '@', OTHER_CATEGORY},
       {'A', 'Z', LETTER_CATEGORY},
       {'[', '[', OTHER_CATEGORY},
       {'\\', '\\', ESCAPE_CATEGORY},
       {']', '`', OTHER_CATEGORY},
       {'a', 'z', LETTER_CATEGORY},
       {'{', '{', BEGIN_CATEGORY},
       {'|', '|', OTHER_CATEGORY},
       {'}', '}', END_CATEGORY},
       {'~', '~', ACTIVE_CHAR_CATEGORY},
       {'\x7f', '\x7f', INVALID_CATEGORY}}}},
    {"luaexec",
     {cs_luacode,
      false,
      {{1, 9, EOL_CATEGORY},
       {'\n', '\n', EOL_CATEGORY},
       {11, '$', OTHER_CATEGORY},
       {'%', '%', COMMENT_CATEGORY},
       {'&', '@', OTHER_CATEGORY},
       {'A', 'Z', LETTER_CATEGORY},
       {'[', '[', OTHER_CATEGORY},
       {'\\', '\\', ESCAPE_CATEGORY},
       {']', '`', OTHER_CATEGORY},
       {'a', 'z', LETTER_CATEGORY},
       {'{', '{', BEGIN_CATEGORY},
       {'|', '|', OTHER_CATEGORY},
       {'}', '}', END_CATEGORY},
       {'~', '~', OTHER_CATEGORY},
       {'\x7f', '\x7f', INVALID_CATEGORY}}}},
    // luatex luatexbase luatexbase-sty
    {"directlua",
     {cs_lua,
      false,
      {{1, 9, EOL_CATEGORY},
       {'\n', '\n', EOL_CATEGORY},
       {11, '$', OTHER_CATEGORY},
       {'%', '%', COMMENT_CATEGORY},
       {'&', '@', OTHER_CATEGORY},
       {'A', 'Z', LETTER_CATEGORY},
       {'[', '[', OTHER_CATEGORY},
       {'\\', '\\', ESCAPE_CATEGORY},
       {']', '`', OTHER_CATEGORY},
       {'a', 'z', LETTER_CATEGORY},
       {'{', '{', BEGIN_CATEGORY},
       {'|', '|', OTHER_CATEGORY},
       {'}', '}', END_CATEGORY},
       {'~', '~', ACTIVE_CHAR_CATEGORY},
       {'\x7f', '\x7f', INVALID_CATEGORY}}}},
    {"latelua",
     {cs_lua,
      false,
      {{1, 9, EOL_CATEGORY},
       {'\n', '\n', EOL_CATEGORY},
       {11, '$', OTHER_CATEGORY},
       {'%', '%', COMMENT_CATEGORY},
       {'&', '@', OTHER_CATEGORY},
       {'A', 'Z', LETTER_CATEGORY},
       {'[', '[', OTHER_CATEGORY},
       {'\\', '\\', ESCAPE_CATEGORY},
       {']', '`', OTHER_CATEGORY},
       {'a', 'z', LETTER_CATEGORY},
       {'{', '{', BEGIN_CATEGORY},
       {'|', '|', OTHER_CATEGORY},
       {'}', '}', END_CATEGORY},
       {'~', '~', ACTIVE_CHAR_CATEGORY},
       {'\x7f', '\x7f', INVALID_CATEGORY}}}},
};

}; // namespace LaTeX
