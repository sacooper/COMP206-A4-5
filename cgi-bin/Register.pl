#!/usr/bin/perl

use strict;
use warnings;
use CGI;

my $form = new CGI;
my $name = $form->param( 'name' );
my $user = $form->param( 'user' );
my $password = $form->param( 'password' );
my $password_confirm = $form->param( 'password_confirm' );
my $canReg = 1;
my $errormessage = '';
my $file = "../data/Members.csv";

if ($password eq "" || $user eq "" || $name eq  "") {
  $errormessage = "Empty fields<br><br>\n";
  $canReg = 0;
}
elsif ($password ne $password_confirm) {
  $errormessage = "The passwords do not much<br><br>\n";
  $canReg = 0;
}
else{
  open(INFO, "<$file");
  my @lines = <INFO>;
  close(INFO);
  my @fileInfo = ("","","");
  my $fileFullUsername = "";
  my $fileUsername = "";

  foreach my $line (@lines)
  {
    @fileInfo = split(/,/, $line);
    $fileFullUsername = shift(@fileInfo);
    $fileUsername = shift(@fileInfo);
    if($fileUsername eq $user){
      $errormessage = "The user already exists<br><br>\n";
      $canReg = 0;
    }
  }
}

print "Content-type: text/plain\n\n";
print "testing...\n";
print "user: $user\n";
print "name: $name\n";
print "password; $password\n";

if ($canReg == 1){

} else {

}