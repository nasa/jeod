use strict;
use warnings;

my $cxx = shift;
$cxx = 'c++' unless defined $cxx;

my $test_program = <<'EOF';
#include <type_traits>
#include <iostream>
int main()
{
    for (auto c:"abc\n") { std::cout<<c; }
}
EOF

sub main();
main;

sub test_compile ($$)
{
    my ($cxx, $std) = @_;
    open SAVERR, '>&STDERR' or die "Can't dup STDERR: $!";
    open STDERR, '>/dev/null' or die "Can't redirect STDERR: $!";
    open CXX, '|-', "$cxx -c -o /dev/null -xc++ -std=$std -";
    print CXX $test_program;
    my $status = close CXX;
    open STDERR, '>&', *SAVERR or die;
    close SAVERR;
    return $status;
}

sub main ()
{
    my $std_to_use = undef;
    foreach my $std (qw(c++11))
    {
        if (test_compile $cxx, $std)
        {
            $std_to_use = $std;
            last;
        }
    }

    if (defined $std_to_use)
    {
        print "-std=$std_to_use\n";
    }
}
