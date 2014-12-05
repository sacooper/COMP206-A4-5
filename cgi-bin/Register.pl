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

if ($canReg == 1){
    open(CATALOGUE, "<../catalgue.html");
    my @cat = <CATALOGUE>;
    close(CATALOGUE);
    foreach my $line (@cat)
    {
        if (index($line, "{user}") != -1) {
            print "$line\n";
        }
        else {
            print "<input type=\"hidden\" name=\"user\" value=\"$user\"/>\n"
        }
    }
} else {
    open(ERROR, "<../error.html");
    my @errorpage = <ERROR>;
    close(CATALOGUE);
    foreach my $line (@errorpage)
    {
        if (index($line, "{errormessage}") != -1) {
            print $line;
        }
        else {
            print "<h3 style=\"color:red\">$errormessage</h3>\n"
        }
    }
}
