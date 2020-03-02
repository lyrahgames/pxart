\ProvidesClass{stdlocal}[2019/07/09 v0.1.0 class with some minimal standard for standalone compilation by Markus Pawellek (markuspawellek@gmail.com)]
\NeedsTeXFormat{LaTeX2e}

\DeclareOption*{%
  \PassOptionsToClass{\CurrentOption}{standalone}
}
\ProcessOptions\relax
\LoadClass[crop=false]{standalone}

\RequirePackage{standard}

\endinput