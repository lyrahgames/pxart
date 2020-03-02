\NeedsTeXFormat{LaTeX2e}
\ProvidesClass{standaloneexercise}[standalone exercise class]

\LoadClass[crop=false]{standalone}

% do not require more than this package
\RequirePackage{exercises}

\DeclareOption*{\PassOptionsToClass{\CurrentOption}{standalone}}
\ProcessOptions\relax

\endinput