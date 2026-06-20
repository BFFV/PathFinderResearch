# Configuration file for the Sphinx documentation builder.
#
# For the full list of built-in configuration values, see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

import os
import sys

# Path to search for extensions
sys.path.append(os.path.abspath("./extensions"))

# -- Project information -----------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#project-information

project = "MillenniumDB"
copyright = "2025, IMFD"
author = "IMFD"
release = "0.2"

# -- General configuration ---------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#general-configuration

templates_path = ["templates"]
exclude_patterns = ["Thumbs.db", ".DS_Store"]

# Extensions
pygments_style = "mdb_style.MDBStyle"
# fmt: off
extensions = [
    "mdb_lexer",                 # Custom lexer for MQL
    "gql_lexer",                 # Custom lexer for GQL
    "sphinx.ext.todo",           # Adds Todo directive
    "sphinx_tabs.tabs",          # Adds Tabs directive
    "sphinx_copybutton",         # Copy button for code blocks
    # "sphinx_design",           # Extension providing things like buttons, cards, etc
    # "sphinxemoji.sphinxemoji", # Adds emoji roles
]
# fmt: on

# Settings for todo extension
todo_include_todos = True
todo_emit_warnings = False
todo_link_only = True

# Do not copy .rst source files and show links
html_copy_source = False

# Remove Python default domain and language
primary_domain = None
highlight_language = "none"

# Make `interpreted role` the same as ``literal role``
default_role = "literal"

# Show warnings in html output
keep_warnings = True

# Define custom roles included in all documents
rst_prolog = """
.. role:: python(code)
   :language: python
.. role:: mql(code)
   :language: mql
.. role:: sparql(code)
   :language: sparql
.. role:: gql(code)
   :language: gql
"""

# -- Options for HTML output -------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#options-for-html-output

html_static_path = ["static"]
html_css_files = ["style.css"]
html_favicon = "static/favicon.ico"
html_theme = "sphinx_rtd_theme"
# fmt: off
html_theme_options = {
    "navigation_depth": 2,        # Depth of the sidebars contents
    "style_external_links": True, # Add a symbol to links to other sites
}
# fmt: on
