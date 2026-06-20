from pygments.style import Style
from pygments.token import Token


class MDBStyle(Style):
    background_color = "#f8f8f8"

    # fmt:off
    styles = {
        Token:                           "#404040",
        Token.Escape:                    "#404040",
        Token.Other:                     "#404040",
        Token.Error:                     "border:#FF0000",

        Token.Text:                      "#404040",
        Token.Text.Whitespace:           "#bbbbbb",
        Token.Comment:                   "italic #3D7B7B",
        Token.Comment.Preproc:           "noitalic #9C6500",

        Token.Punctuation:               "#000000",

        Token.Operator:                  "#666666",
        Token.Operator.Word:             "bold #AA22FF",

        Token.Keyword:                   "bold #008000",
        Token.Keyword.Pseudo:            "nobold",
        Token.Keyword.Type:              "nobold #B00040",

        Token.Name:                      "#008000",
        Token.Name.Builtin:              "#008000",
        Token.Name.Function:             "#0000FF",
        Token.Name.Class:                "bold #0000FF",
        Token.Name.Namespace:            "bold #0000FF",
        Token.Name.Exception:            "bold #CB3F38",
        Token.Name.Variable:             "bold #aa0000",
        Token.Name.Constant:             "#880000",
        Token.Name.Label:                "#767600",
        Token.Name.Entity:               "bold #717171",
        Token.Name.Attribute:            "#687822",
        Token.Name.Tag:                  "bold #008000",
        Token.Name.Decorator:            "#AA22FF",

        Token.Literal.String:            "#BA2121",
        Token.Literal.String.Doc:        "italic",
        Token.Literal.String.Interpol:   "bold #A45A77",
        Token.Literal.String.Escape:     "#ff0000",
        Token.Literal.String.Regex:      "#A45A77",
        Token.Literal.String.Symbol:     "#19177C",
        Token.Literal.String.Other:      "#008000",
        Token.Literal.Number:            "#666666",

        Token.Generic:                   "#404040",
        Token.Generic.Heading:           "bold #000080",
        Token.Generic.Subheading:        "bold #800080",
        Token.Generic.Deleted:           "#A00000",
        Token.Generic.Inserted:          "#008400",
        Token.Generic.Error:             "#E40000",
        Token.Generic.Emph:              "italic",
        Token.Generic.Strong:            "bold",
        Token.Generic.EmphStrong:        "bold italic",
        Token.Generic.Prompt:            "bold #000080",
        Token.Generic.Output:            "#717171",
        Token.Generic.Traceback:         "#04D",
    }
    # fmt: on
