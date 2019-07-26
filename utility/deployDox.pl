use v5.10;
use warnings;

use Digest::MD5 qw/md5_hex md5_base64/;
use MIME::Base64;
use JSON::MaybeXS qw/encode_json to_json/;

my $deployer = "https://deployer.isdevnet.com";

sub read_base64 {
    open(my $file, "<:raw", shift)
        or die "Unusable file supplied";
    local $/ = undef;
    encode_base64(<$file>)
}

sub read_md5 {
    open(my $file, "<:raw", shift)
        or die "Unusable file supplied";
    local $/ = undef;
    md5_hex(<$file>)
}

my $data = {
    pwd      => $ENV{DEPLOYER_PASSWORD} // die "Undefined environment variable of password",
    usr      => $ENV{DEPLOYER_USERNAME} // die "Undefined environment variable of username",
    compflag => 'J',
    name     => 'docs.tar.xz',
    file     => read_base64('docs.tar.xz'),
    md5      => read_md5('docs.tar.xz'),
};

open my $curlFile, '>', 'deploy.curl'
    or die "Can't open curl configuration file.";
print $curlFile "-d " . to_json($data);
close $curlFile;

system 'curl', $deployer, '-K', 'deploy.curl';

unlink "deploy.curl";

