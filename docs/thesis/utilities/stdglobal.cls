\ProvidesClass{stdglobal}[2019/07/09 v0.1.0 class with some minimal standard for global compilation by Markus Pawellek (markuspawellek@gmail.com)]
\NeedsTeXFormat{LaTeX2e}

\DeclareOption*{%
  \PassOptionsToClass{\CurrentOption}{article}
}
\ProcessOptions\relax
\LoadClass{article}

\RequirePackage{standard}

\endinput