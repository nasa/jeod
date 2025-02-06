use strict;
use warnings;

sub get_dependencies($);

my $jeod_home = $ENV{JEOD_HOME};
$jeod_home =~ s/\/$//;
foreach my $f (@ARGV) {
    get_dependencies $f;
}

sub get_dependencies($) {
    my $f = shift;
    $f =~ m|^(.+)/dep([^\/]*)/([^/]+)\.d$| or die;
    my ($mdir, $suffix, $basename) = ($1, $2, $3);
    my $srcfile = "$mdir/src/$basename.cc";
    my $depfile = "$mdir/dep$suffix/$basename.d";
    my $objfile = "$mdir/obj$suffix/$basename.o";
    my @deps = ();
    open MM, "g++ -MM $srcfile $ENV{COMMON_CFLAGS} $ENV{CONFIG_CFLAGS} |"
       or die "Error open the compiler on $srcfile";
    my $line1 = <MM>;
    $line1 =~ s/^$basename\.o: */ / or die;
    # $line1 =~ m|^$srcfile\b| or die;
    push @deps, $line1;
    while (<MM>) {
        s/^ */ /;
        push @deps, $_;
    }
    close MM;
    foreach my $dep (@deps) {
        $dep =~ s| $jeod_home/| |g;
        $dep =~ s|/src/\.\./|/|g;
    }
    open DEP, ">$depfile" or die;
    print DEP "$depfile: | $mdir/dep$suffix\n";
    print DEP "$objfile: | $mdir/obj$suffix\n";
    print DEP "$objfile $depfile : \\\n";
    foreach my $dep (@deps) {
        print DEP $dep;
    }
    close DEP;
}
