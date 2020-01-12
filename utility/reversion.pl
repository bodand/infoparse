#!/usr/bin/env perl
use v5.10;
use warnings;

my %files = (
    cmake  => {
        file  => "CMakeLists.txt",
        match => {
            major => qr/(set\(InfoParse_VERSION_MAJOR )(\d+)/,
            minor => qr/(set\(InfoParse_VERSION_MINOR )(\d+)/,
            patch => qr/(set\(InfoParse_VERSION_PATCH )(\d+)/,
        },
        dot   => "",
    },
    readme => {
        file  => "readme.md",
        match => {
            major => qr{(InfoParse/)(\d+)\.},
            minor => qr{(InfoParse/\d+\.)(\d+)\.},
            patch => qr{(InfoParse/\d+\.\d+\.)(\d+)},
        },
        dot   => '.',
    },
);

die "Give version" if !$ARGV[0];

my @version = split '\.', $ARGV[0];
my %version = (
    major => $version[0],
    minor => $version[1],
    patch => $version[2],
);

foreach (keys %files) {
    say "Reversioning $_";
    open my $file, "<", $files{$_}{file}
        or die "Fuckening";

    my @contents = <$file>;
    my $contents = join '', @contents;
    foreach my $version_t (qw/major minor patch/) {
        $contents =~ s/$files{$_}{match}{$version_t}/
                "$1$version{$version_t}" .
                    ($version_t ne "patch" ?
                        "$files{$_}{dot}"
                        : "") /eg;
    }
    close $file;
    open $file, '>', $files{$_}{file}
        or die "Fuckening the unforgiving world";

    say $file $contents;
}
