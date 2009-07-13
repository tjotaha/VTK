/*=========================================================================

  Program:   Visualization Toolkit
  Module:    $RCSfile$

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
/*-------------------------------------------------------------------------
  Copyright 2008 Sandia Corporation.
  Under the terms of Contract DE-AC04-94AL85000 with Sandia Corporation,
  the U.S. Government retains certain rights in this software.
-------------------------------------------------------------------------*/

#include <vtkTextAnalysisUtility.h>

const char* vtkTextAnalysisUtility::DefaultStopWords()
{
  return
    "a\n"
    "about\n"
    "above\n"
    "accordingly\n"
    "across\n"
    "after\n"
    "afterwards\n"
    "again\n"
    "against\n"
    "all\n"
    "allows\n"
    "almost\n"
    "alone\n"
    "along\n"
    "already\n"
    "also\n"
    "although\n"
    "always\n"
    "am\n"
    "among\n"
    "amongst\n"
    "an\n"
    "and\n"
    "another\n"
    "any\n"
    "anybody\n"
    "anyhow\n"
    "anyone\n"
    "anything\n"
    "anywhere\n"
    "apart\n"
    "appear\n"
    "appropriate\n"
    "are\n"
    "around\n"
    "as\n"
    "aside\n"
    "associated\n"
    "at\n"
    "available\n"
    "away\n"
    "awfully\n"
    "b\n"
    "back\n"
    "be\n"
    "became\n"
    "because\n"
    "become\n"
    "becomes\n"
    "becoming\n"
    "been\n"
    "before\n"
    "beforehand\n"
    "behind\n"
    "being\n"
    "below\n"
    "beside\n"
    "besides\n"
    "best\n"
    "better\n"
    "between\n"
    "beyond\n"
    "both\n"
    "brief\n"
    "but\n"
    "by\n"
    "c\n"
    "came\n"
    "can\n"
    "cannot\n"
    "cant\n"
    "cause\n"
    "causes\n"
    "certain\n"
    "changes\n"
    "co\n"
    "come\n"
    "consequently\n"
    "contain\n"
    "containing\n"
    "contains\n"
    "corresponding\n"
    "could\n"
    "currently\n"
    "d\n"
    "day\n"
    "described\n"
    "did\n"
    "different\n"
    "do\n"
    "does\n"
    "doing\n"
    "done\n"
    "down\n"
    "downwards\n"
    "during\n"
    "e\n"
    "each\n"
    "eg\n"
    "eight\n"
    "either\n"
    "else\n"
    "elsewhere\n"
    "enough\n"
    "et\n"
    "etc\n"
    "even\n"
    "ever\n"
    "every\n"
    "everybody\n"
    "everyone\n"
    "everything\n"
    "everywhere\n"
    "ex\n"
    "example\n"
    "except\n"
    "f\n"
    "far\n"
    "few\n"
    "fifth\n"
    "first\n"
    "five\n"
    "followed\n"
    "following\n"
    "for\n"
    "former\n"
    "formerly\n"
    "forth\n"
    "four\n"
    "from\n"
    "further\n"
    "furthermore\n"
    "g\n"
    "get\n"
    "gets\n"
    "given\n"
    "gives\n"
    "go\n"
    "gone\n"
    "good\n"
    "got\n"
    "great\n"
    "h\n"
    "had\n"
    "hardly\n"
    "has\n"
    "have\n"
    "having\n"
    "he\n"
    "hence\n"
    "her\n"
    "here\n"
    "hereafter\n"
    "hereby\n"
    "herein\n"
    "hereupon\n"
    "hers\n"
    "herself\n"
    "him\n"
    "himself\n"
    "his\n"
    "hither\n"
    "how\n"
    "howbeit\n"
    "however\n"
    "i\n"
    "ie\n"
    "if\n"
    "ignored\n"
    "immediate\n"
    "in\n"
    "inasmuch\n"
    "inc\n"
    "indeed\n"
    "indicate\n"
    "indicated\n"
    "indicates\n"
    "inner\n"
    "insofar\n"
    "instead\n"
    "into\n"
    "inward\n"
    "is\n"
    "it\n"
    "its\n"
    "itself\n"
    "j\n"
    "just\n"
    "k\n"
    "keep\n"
    "kept\n"
    "know\n"
    "l\n"
    "last\n"
    "latter\n"
    "latterly\n"
    "least\n"
    "less\n"
    "lest\n"
    "life\n"
    "like\n"
    "little\n"
    "long\n"
    "ltd\n"
    "m\n"
    "made\n"
    "make\n"
    "man\n"
    "many\n"
    "may\n"
    "me\n"
    "meanwhile\n"
    "men\n"
    "might\n"
    "more\n"
    "moreover\n"
    "most\n"
    "mostly\n"
    "mr\n"
    "much\n"
    "must\n"
    "my\n"
    "myself\n"
    "n\n"
    "name\n"
    "namely\n"
    "near\n"
    "necessary\n"
    "neither\n"
    "never\n"
    "nevertheless\n"
    "new\n"
    "next\n"
    "nine\n"
    "no\n"
    "nobody\n"
    "none\n"
    "noone\n"
    "nor\n"
    "normally\n"
    "not\n"
    "nothing\n"
    "novel\n"
    "now\n"
    "nowhere\n"
    "o\n"
    "of\n"
    "off\n"
    "often\n"
    "oh\n"
    "old\n"
    "on\n"
    "once\n"
    "one\n"
    "ones\n"
    "only\n"
    "onto\n"
    "or\n"
    "other\n"
    "others\n"
    "otherwise\n"
    "ought\n"
    "our\n"
    "ours\n"
    "ourselves\n"
    "out\n"
    "outside\n"
    "over\n"
    "overall\n"
    "own\n"
    "p\n"
    "particular\n"
    "particularly\n"
    "people\n"
    "per\n"
    "perhaps\n"
    "placed\n"
    "please\n"
    "plus\n"
    "possible\n"
    "probably\n"
    "provides\n"
    "q\n"
    "que\n"
    "quite\n"
    "r\n"
    "rather\n"
    "really\n"
    "relatively\n"
    "respectively\n"
    "right\n"
    "s\n"
    "said\n"
    "same\n"
    "second\n"
    "secondly\n"
    "see\n"
    "seem\n"
    "seemed\n"
    "seeming\n"
    "seems\n"
    "self\n"
    "selves\n"
    "sensible\n"
    "sent\n"
    "serious\n"
    "seven\n"
    "several\n"
    "shall\n"
    "she\n"
    "should\n"
    "since\n"
    "six\n"
    "so\n"
    "some\n"
    "somebody\n"
    "somehow\n"
    "someone\n"
    "something\n"
    "sometime\n"
    "sometimes\n"
    "somewhat\n"
    "somewhere\n"
    "specified\n"
    "specify\n"
    "specifying\n"
    "state\n"
    "still\n"
    "sub\n"
    "such\n"
    "sup\n"
    "t\n"
    "take\n"
    "taken\n"
    "than\n"
    "that\n"
    "the\n"
    "their\n"
    "theirs\n"
    "them\n"
    "themselves\n"
    "then\n"
    "thence\n"
    "there\n"
    "thereafter\n"
    "thereby\n"
    "therefore\n"
    "therein\n"
    "thereupon\n"
    "these\n"
    "they\n"
    "third\n"
    "this\n"
    "thorough\n"
    "thoroughly\n"
    "those\n"
    "though\n"
    "three\n"
    "through\n"
    "throughout\n"
    "thru\n"
    "thus\n"
    "time\n"
    "to\n"
    "together\n"
    "too\n"
    "toward\n"
    "towards\n"
    "twice\n"
    "two\n"
    "u\n"
    "under\n"
    "unless\n"
    "until\n"
    "unto\n"
    "up\n"
    "upon\n"
    "us\n"
    "use\n"
    "used\n"
    "useful\n"
    "uses\n"
    "using\n"
    "usually\n"
    "v\n"
    "value\n"
    "various\n"
    "very\n"
    "via\n"
    "viz\n"
    "vs\n"
    "w\n"
    "was\n"
    "way\n"
    "we\n"
    "well\n"
    "went\n"
    "were\n"
    "what\n"
    "whatever\n"
    "when\n"
    "whence\n"
    "whenever\n"
    "where\n"
    "whereafter\n"
    "whereas\n"
    "whereby\n"
    "wherein\n"
    "whereupon\n"
    "wherever\n"
    "whether\n"
    "which\n"
    "while\n"
    "whither\n"
    "who\n"
    "whoever\n"
    "whole\n"
    "whom\n"
    "whose\n"
    "why\n"
    "will\n"
    "with\n"
    "within\n"
    "without\n"
    "work\n"
    "world\n"
    "would\n"
    "x\n"
    "y\n"
    "year\n"
    "years\n"
    "yet\n"
    "you\n"
    "your\n"
    "yours\n"
    "yourself\n"
    "yourselves\n"
    "z\n"
    "zero\n";
}

