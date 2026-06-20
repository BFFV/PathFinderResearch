from __future__ import annotations

from pygments.lexer import RegexLexer
from pygments.token import Token
from sphinx.application import Sphinx


def setup(sphinx: Sphinx):
    sphinx.add_lexer("gql", GQLLexer)


KEY_WORDS = [
    "ACYCLIC",
    "AND",
    "ANY",
    "AVG",
    "ALL",
    "ASC",
    "AS",
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
    "IN",
    "FILTER",
    "FLOAT",
    "FOR",
    "GROUP",
    "LABELS",
    "LABEL",
    "LET",
    "LIMIT",
    "MAX",
    "MATCH",
    "MIN",
    "OFFSET",
    "OPTIONAL",
    "ORDER",
    "OR",
    "OUTGOING",
    "PATH_LENGTH",
    "PROPERTIES",
    "PROPERTY",
    "NOT",
    "NULL",
    "NULLS LAST",
    "NULLS FIRST",
    "SHORTEST",
    "SIMPLE",
    "RETURN",
    "SET",
    "SUM",
    "SKIP",
    "STRING",
    "TRAIL",
    "TRAILS",
    "WALKS",
    "WHERE",
]


def make_keyword_regex(words: list[str]):
    words.sort(key=lambda w: -len(w))
    join_words = "\\b|\\b".join(words)
    return "(?i)(\\b" + join_words + "\\b)"


class GQLLexer(RegexLexer):
    name = "GQL"
    aliases = ["gql"]
    filenames = ["*.gql"]

    tokens = {
        "root": [
            (make_keyword_regex(KEY_WORDS), Token.Keyword),
            ('"', Token.String, "string"),
            (r"\(|\)", Token.Punctuation),
            (r"[A-Za-z][A-Za-z0-9_]*", Token.Name.Variable),
            (r"\{\d*,?\d*\}", Token.Constant),
            (r"\{", Token.Text, "property-key"),
            (":", Token.Punctuation, "label-expr"),
            (r"\.", Token.Text, "property"),
            (".", Token.Text),
            ("\n", Token.Text),
        ],
        "string": [
            (r"\\.", Token.String.Escape),
            (r'"', Token.String, "#pop"),
            (r'[^\\"]+', Token.String),
        ],
        "property": [
            (r"[A-Za-z][A-Za-z0-9_]*", Token.Name.Function),
            (r'.', Token.Punctuation, "#pop"),
        ],
        "property-key": [
            (r"[A-Za-z][A-Za-z0-9_]*", Token.Name.Entity),
            (r": ?", Token.Punctuation, "property-value"),
        ],
        "property-value": [
            (r'"', Token.String, "string"),
            (r"\d+", Token.Constant),
            (r"[A-Za-z][A-Za-z0-9_]*", Token.Name.Variable),
            (r", ?", Token.Text, "property-key"),
            (r"\}", Token.Text, "root"),
        ],
        "label-expr": [
            (r"\s+", Token.Text.Whitespace),
            ("WHERE", Token.Keyword, "#pop"),
            (r"[A-Za-z][A-Za-z0-9_]*", Token.Name.Label),
            (r"%", Token.Name.Label),
            (r"\||\&|\!", Token.Operator),
            (r"\(", Token.Punctuation, "#push"),
            (r"\)", Token.Punctuation, "#pop"),
            (r"\{", Token.Text, "property-key"),
            (r".", Token.Punctuation, "#pop"),
        ],
    }
