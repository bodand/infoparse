#!/usr/bin/perl
use v5.10;
use strict;
use warnings;

use File::Copy qw/cp/;
use File::Path qw/make_path/;


sub do_all(&_) {
    my ($sub, $dirname) = @_;
    say "[DBG]" . $dirname;
    opendir my $dir, "$dirname";
    while (local $_ = readdir $dir) {
        if (/^\.{1,2}$/) {
            next;
        }
        if (-d "$dirname/$_") {
            &do_all($sub, "$dirname/$_");
        }
        else {
            eval {
                $sub->($_, $dirname);
            }
        }
    }
    closedir $dir
}

$_ = "./dox/html";
do_all {
    my ($file, $dir) = @_;
    my $newdir = "./dox_site/doxygen" . substr $dir, length "./dox/html";
    say "Copying $dir/$file -> $newdir/$file";
    make_path($newdir);
    cp("$dir/$file", "$newdir/$file")
}
