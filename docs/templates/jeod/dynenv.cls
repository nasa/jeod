%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% File: dynenv.cls
% This class provides a simple means for authoring JEOD model documents.
%
% All options to dynenv.cls are passed directly to dynenv.sty.
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

\NeedsTeXFormat{LaTeX2e}% LaTeX 2.09 can't be used (nor non-LaTeX)
[1994/12/01]% LaTeX date must be December 1994 or later
\ProvidesClass{dynenv}[2011/06/06 v2.2 JEOD document class]

\LoadClass[twoside,11pt,titlepage]{report}

\PassOptionsToPackage{complete}{dynenv}
\DeclareOption*{\PassOptionsToPackage{\CurrentOption}{dynenv}}
\ProcessOptions\relax

\RequirePackage{dynenv}
