#!/usr/bin/perl
use strict;
use warnings;
use Path::Class;
use autodie;

my $dir = dir("/data");
my $file = $dir->file("Members.csv");
my $file_handle = $file->openw();

