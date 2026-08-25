<?php
// Configuration of translations status script

// Setup repository web access here, must point to the directory with .po files:
$git_url="https://cgit.git.savannah.gnu.org/cgit/gnubg.git/tree/po";

// branch used to generate the page:
$git_branch="master";

// Name of POT file (in git_url directory):
$potfile="gnubg.pot";

// Path to images (1x16 bars):
$img_translated="translated.png";
$img_fuzzy="fuzzy.png";
$img_untranslated="untranslated.png";

// Width of status bar:
$graph_width = 200;

// Uncomment this and define $author array in included file (key: ISO code,
// value: string with author(s) name(s)) if you want to show translator name
// alongside with the translation:

// $show_author = 1;
// include '__authors.php';
?>
