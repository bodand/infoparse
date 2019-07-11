#!/usr/bin/perl
use v5.10;
use strict;
use warnings;

use File::Copy qw/copy/;


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
                local $_ = "$dirname/$_";
                $sub->($_);
            }
        }
    }
    closedir $dir
}

$_ = "./dox/html";
do_all {
    say "Copying $_ -> " . (my $dest = s{^./dox/html/(.*)$}{./docs/doxygen/$1}gr);
    say copy($_, $dest) ? "Success" : "Failure";
}
