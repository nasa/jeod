open HFILES, "find . -name '*.hh' | " .
             "grep -v -E '(/delete|/junk|/class_decl|inline.hh)' | " .
             "sort |";
@hfiles = <HFILES>;
close HFILES;
map chomp, @hfiles;

$dir = '';

print "Directory\tClass name\tInherits from\n";
open CLASSES, "grep '^class' @hfiles |";
while (<CLASSES>) {
   /class +\w+ *;/ and next;
   s/^\.\/// or die;
   s/ *\/[\*\/].*//;
   s/ *{ *$//;
   s/ *$//;

   /^(.*)\/include\/[^\/]+\.hh:class (.*)/ or die $_;
   if ($1 ne $dir) {
      $dir = $1;
      print "\n$dir\n";
   }
   $class_spec = $2;
   $class_spec =~ s/ *, *(public)? */\t/g;
   $class_spec =~ s/ *: *(public)? */\t/g;
   print "\t$class_spec\n";
}
