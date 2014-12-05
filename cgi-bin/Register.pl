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
my $file = "~/public_html/data/Members.csv";

if ($password eq "" || $user eq "" || $name eq  "" || $password_confirm eq "") {
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

print "Content-type: text/html\n\n";

if ($canReg == 1){
    open(CATALOGUE, "<~/public_html/catalgue.html");
    my @cat = <CATALOGUE>;
    close(CATALOGUE);
    foreach my $line (@cat)
    {
        if (index($line, "{user}") != -1) {
            print "<input type=\"hidden\" name=\"user\" value=\"$user\"/>\n";
        }
        else {
            print "$line\n";
        }
    }
} else {
    open(ERROR, "<~/public_html/error.html");
    my @errorpage = <ERROR>;
    close(CATALOGUE);
    foreach my $line (@errorpage)
    {
        if (index($line, "{errormessage}") != -1) {
            print "<center><h3 style=\"color:red\">$errormessage</h3></center>\n";
        }
        else {
            print $line;
        }
    }
}
