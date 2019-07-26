#!/usr/bin/perl
use v5.10;
use warnings;

use Crypt::Digest::SHA256 qw/sha256_file_hex/;
use LWP::Simple;

my $win = $^O =~ /win/i;
my $url;
my $file;
my $hash;
my $uncompress;

if ($win) {
    $url = "https://dl.bintray.com/boostorg/release/1.70.0/source/boost_1_70_0.zip";
    $file = "../boost.zip";
    $hash = "48f379b2e90dd1084429aae87d6bdbde9670139fa7569ee856c8c86dd366039d";

    # The followings until the end of the if-block
    # are copied from https://gist.github.com/eqhmcow/5389877
    # It is licensed either under the
    # 1) GPL 2(https://www.gnu.org/licenses/old-licenses/gpl-2.0.html); and/or
    # 2) Artistic License(https://web.archive.org/web/20121016115951/http://www.perlfoundation.org/artistic_license_1_0);
    # licenses
    use File::Spec::Functions qw(splitpath);
    use IO::File;
    use IO::Uncompress::Unzip qw($UnzipError);
    use File::Path qw(mkpath);
    $uncompress = sub {
        my ($file, $dest) = @_;

        die 'Need a file argument' unless defined $file;
        $dest = "." unless defined $dest;

        my $u = IO::Uncompress::Unzip->new($file)
            or die "Cannot open $file: $UnzipError";

        my $status;
        for ($status = 1; $status > 0; $status = $u->nextStream()) {
            my $header = $u->getHeaderInfo();
            my (undef, $path, $name) = splitpath($header->{Name});
            my $destdir = "$dest/$path";

            unless (-d $destdir) {
                mkpath($destdir) or die "Couldn't mkdir $destdir: $!";
            }

            if ($name =~ m!/$! || $name eq "") {
                last if $status < 0;
                next;
            }

            my $destfile = "$dest/$path/$name";
            # https://cwe.mitre.org/data/definitions/37.html
            # CWE-37: Path Traversal
            die "unsafe $destfile" if $destfile =~ m!\Q..\E(/|\\)!;

            my $buff;
            my $fh = IO::File->new($destfile, "w")
                or die "Couldn't write to $destfile: $!";
            while (($status = $u->read($buff)) > 0) {
                $fh->write($buff);
            }
            $fh->close();
            my $stored_time = $header->{'Time'};
            utime($stored_time, $stored_time, $destfile)
                or die "Couldn't touch $destfile: $!";
        }

        die "Error processing $file: $!\n"
            if $status < 0;

        return;
    }
}
else {
    $url = "https://dl.bintray.com/boostorg/release/1.70.0/source/boost_1_70_0.tar.bz2";
    $file = "../boost.tar.bz2";
    $hash = "430ae8354789de4fd19ee52f3b1f739e1fba576f0aded0897c3c2bc00fb38778";

    use Archive::Tar;
    $uncompress = sub {
        my $file = shift;
        my $tar = Archive::Tar->new;
        $tar->read($file);
        $tar->extract();
    }
}

say "Downloading Boost";
getstore($url, $file) unless -f $file && !-z $file;
say "Done";

say "Checking checksum";
sha256_file_hex($file) eq $hash
    or die "Checksums differ!";
say "Done";

say "Uncompressing Boost";
$uncompress->($file);
say "Done";

say "Deleting Boost archive";
unlink $file;
say "Done"
