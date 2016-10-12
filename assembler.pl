#this was for ealy testing/debugging purposes
#the code will have to be updated before it is really
#useable at all

#it will compile the assembly code at the end of the project
#which will enable me to determine if my interpeted code
#running in parallel is worth it over just compiling equivalent
#code

print '#include <iostream>

int registers[8];

struct Add {
  auto operator () (char loc, auto t, auto u) {
    registers[loc] = t + u;
  }
};

struct Sub {
  auto operator () (char loc, auto t, auto u) {
    registers[loc] = t - u;
  }
};

struct Mul {
  auto operator () (char loc, auto t, auto u) {
    registers[loc] = t * u;
  }
};

struct Div {
  auto operator () (char loc, auto t, auto u) {
    registers[loc] = t / u;
  }
};

struct Mov {
  auto operator () (char loc, auto t) {
    registers[loc] = registers[t];
  }
};

struct Pnt {
  auto operator () (char loc) {
    std::cout << registers[loc];
  }
};

int main (void) {
  Add add;
  Sub sub;
  Mul mul;
  Div div;

  Mov mov;
  Pnt pnt; '."\n";
while (<>) {
    chomp;
    my @originalLine = split / /;
	my $newLine = shift(@originalLine).'(';
	foreach (@originalLine) {
	    $newLine = $newLine.$_;
	}
	$newLine = $newLine.')';
    print "  ".$newLine.";\n";
}
print '  return 0;
}
';
