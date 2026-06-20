from __future__ import annotations

from pygments.lexer import RegexLexer
from pygments.token import Token
from sphinx.application import Sphinx


def setup(sphinx: Sphinx):
    sphinx.add_lexer("mql", MQLLexer)


KEY_WORDS = [
    "ACYCLIC",
    "AND",
    "ANY",
    "AVG",
    "ALL",
    "ASC",
    "BY",
    "BOOL",
    "COUNT",
    "DESCRIBE",
    "DESC",
    "DISTINCT",
    "EDGE",
    "INCOMING",
    "INSERT",
    "INTEGER",
    "IS",
    "FLOAT",
    "GROUP",
    "LABELS",
    "LABEL",
    "LIMIT",
    "MAX",
    "MATCH",
    "MIN",
    "OPTIONAL",
    "ORDER",
    "OR",
    "OUTGOING",
    "PROPERTIES",
    "PROPERTY",
    "NOT",
    "NULL",
    "SHORTEST",
    "SIMPLE",
    "RETURN",
    "SET",
    "SUM",
    "STRING",
    "TRAILS",
    "WALKS",
    "WHERE",
]


def make_keyword_regex(words: list[str]):
    words.sort(key=lambda w: -len(w))
    join_words = "|".join(words)
    return "(?i)(" + join_words + ")"


class MQLLexer(RegexLexer):
    name = "MQL"
    aliases = ["mql"]
    filenames = ["*.mql"]

    tokens = {
        "root": [
            (make_keyword_regex(KEY_WORDS), Token.Keyword),
            ('"', Token.String, "string"),
            (r"\(|\)", Token.Punctuation),
            (r"\?[A-Za-z][A-Za-z0-9_]*", Token.Name.Variable),
            (r":\?[A-Za-z][A-Za-z0-9_]*", Token.Name.Variable),
            (".", Token.Text),
            ("\n", Token.Text),
        ],
        "string": [
            (r"\\.", Token.String.Escape),
            (r'"', Token.String, "#pop"),
            (r'[^\\"]+', Token.String),
        ],
    }
